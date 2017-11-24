#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

#include "ext2_fs.h"

// NECESSARY INFORMATION
int fd_image, fd_csv;
struct ext2_super_block super;
struct ext2_group_desc group;

// HELPFUL CONSTANTS (THAT ARE DECLARED LATER)
int block_size;

void handleError(char loc[256], int err) {
    fprintf(stderr, "Error encountered in ");
    fprintf(stderr, "%s", loc);
    fprintf(stderr, ": ");
    fprintf(stderr, "%s\n", strerror(err));
    
    exit(1);
}

void processArguments(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Requires an image file.\n");
        exit(1);
    }
    
    fd_image = open(argv[1], O_RDONLY);
    if (fd_image < 0) {
        handleError("processArguments (open)", errno);
    }
}

void analyzeSuper() {
    pread(fd_image, &super, sizeof(struct ext2_super_block), 1024);
    block_size = 1024 << super.s_log_block_size;
    
    fprintf(stdout, "SUPERBLOCK,");
    fprintf(stdout, "%d,", super.s_blocks_count);
    fprintf(stdout, "%d,", super.s_inodes_count);
    fprintf(stdout, "%d,", block_size);  // SPEC
    fprintf(stdout, "%d,", super.s_inode_size);
    fprintf(stdout, "%d,", super.s_blocks_per_group);
    fprintf(stdout, "%d,", super.s_inodes_per_group);
    fprintf(stdout, "%d\n", super.s_first_ino);
}

// WE ONLY HAVE TO DEAL WITH SINGLE GROUP SYSTEMS (PIAZZA)
void analyzeGroup() {
    int offset = super.s_first_data_block ? block_size * 2 : block_size;
    pread(fd_image, &group, sizeof(struct ext2_group_desc), offset);
    
    fprintf(stdout, "GROUP,0,");  // SINGLE GROUP SYSTEM
    fprintf(stdout, "%d,", super.s_blocks_count);  // DUE TO SINGLE GRP
    fprintf(stdout, "%d,", super.s_inodes_per_group);
    fprintf(stdout, "%d,", group.bg_free_blocks_count);
    fprintf(stdout, "%d,", group.bg_free_inodes_count);
    fprintf(stdout, "%d,", group.bg_block_bitmap);
    fprintf(stdout, "%d,", group.bg_inode_bitmap);
    fprintf(stdout, "%d\n", group.bg_inode_table);
}


/*
 DIRENT
 parent inode number (decimal) ... the I-node number of the directory that contains this entry
 logical byte offset (decimal) of this entry within the directory
 inode number of the referenced file (decimal)
 entry length (decimal)
 name length (decimal)
 name (string, surrounded by single-quotes). Don't worry about escaping, we promise there will be no single-quotes or commas in any of the file names.
 */
void directory_entry(struct ext2_inode * curr_inode_ptr, __u32 inode_number )
{
    int i =0;
    for (i=0;i<EXT2_NDIR_BLOCKS;i++)  //for all the directory blocks
    {
      if (! (curr_inode_ptr->i_block[i]))  //if this is 0, there's no more blocks
            return;
        
        struct ext2_dir_entry curr_entry;
        int entry_offset=0;
        __u32 directory_offset = curr_inode_ptr->i_block[i] * block_size;
        
        do
	  {
	  
            pread(fd_image, &curr_entry, sizeof(struct ext2_dir_entry), entry_offset + directory_offset);
	    
	    if(curr_entry.inode !=0)
	      {
	      
	    __u32 parent_inode_number =  inode_number;
            __u32 logical_byte_offset = entry_offset;
            __u32 inode_number = curr_entry.inode;
            __u16 entry_length = curr_entry.rec_len;
            __u8 name_length = curr_entry.name_len;
            
            fprintf(stdout, "DIRENT,%u,%u,%u,%u,%u,'%s'\n",parent_inode_number,logical_byte_offset,inode_number,entry_length,name_length,curr_entry.name);
	      }
            entry_offset+=curr_entry.rec_len;  //go to the next entry
        }while (entry_offset < block_size);

    }
}

void inode_summary()
{
    int inode_table_offset = group.bg_inode_table * block_size;
    
    struct ext2_inode current_inode;
    
    __u32 i = 0;
    for (i = 0; i < super.s_inodes_count; i++)   //for every inodes
    {
        
        //store infos to current_inode
        pread(fd_image, &current_inode, sizeof(struct ext2_inode), inode_table_offset + i * sizeof(struct ext2_inode));
        
        //process current_inode
        if ( (current_inode.i_mode) != 0 && (current_inode.i_links_count) != 0) {
            __u32 inode_number = i+1;
            
            char fileType = '?';
            if (current_inode.i_mode & 0xA000)
                fileType = 's';
            if (current_inode.i_mode & 0x8000)
                fileType = 'f';
            if (current_inode.i_mode & 0x4000)
                fileType = 'd';
            
            __u16 mode = current_inode.i_mode & 0xFFF;
            __u16 owner = current_inode.i_uid;
            __u16 group = current_inode.i_gid;
            __u16 link_count = current_inode.i_links_count;
            
            struct tm timestamp;
            
            char creation_time[19];  //??time of last I-node change
            time_t c_timestamp = current_inode.i_ctime;
            timestamp = *gmtime(&c_timestamp);
            strftime(creation_time, 19, "%D %H:%M:%S", &timestamp);
            
            char modification_time[19];
            time_t m_timestamp = current_inode.i_mtime;
            timestamp = *gmtime(&m_timestamp);
            strftime(modification_time, 19, "%D %H:%M:%S", &timestamp);
            
            char access_time[19];
            time_t a_timestamp = current_inode.i_atime;
            timestamp = *gmtime(&a_timestamp);
            strftime(access_time, 19, "%D %H:%M:%S", &timestamp);
            
            __u32 file_size = current_inode.i_size;
            __u32 num_blocks = current_inode.i_blocks;
            
            fprintf(stdout, "INODE,%u,%c,%o,%u,%u,%u,%s,%s,%s,%u,%u",inode_number,fileType,mode,owner,group,link_count,creation_time,modification_time,access_time,file_size,num_blocks);
            
            //print out addresses of blocks
            int j=0;
            for (j=0; j < 15 ; j++ )
            {
                fprintf(stdout, ",%u",current_inode.i_block[j]);
            }
            fprintf(stdout,"\n");
            
            //if this is a directory, process it.
            if ( fileType == 'd')
            {
                directory_entry(&current_inode,inode_number);
            }
        }
        
        
    }
}


int main(int argc, char* argv[]) {
    processArguments(argc, argv);
    analyzeSuper();
    analyzeGroup();
    inode_summary();
}
