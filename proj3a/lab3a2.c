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
int n_directory;
int * inode_directory;

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

bool checkValidBit(char byte, int index) {
    return !(byte & (1 << index));
}

// WE ONLY HAVE TO DEAL WITH SINGLE GROUP SYSTEMS (PIAZZA)
void analyzeBitmap() {
    char byte;
    int currentIndex = 0;
    
    for (int i = 0; i < block_size; i++) {
        pread(fd_image, &byte, 1, group.bg_block_bitmap * block_size + i);
        
        for (int j = 0; j < 8; j++) {
            currentIndex++;
            if (checkValidBit(byte, j)) {
                fprintf(stdout, "BFREE,%d\n", currentIndex);
            }
        }
    }
}

void analyzeInode() {
    char byte;
    int currentIndex = 0;
    
    for (int i = 0; i < block_size; i++) {
        pread(fd_image, &byte, 1, group.bg_inode_bitmap * block_size + i);
        
        for (int j = 0; j < 8; j++) {
            currentIndex++;
            
            if (checkValidBit(byte, j))
                if (currentIndex <= super.s_inodes_per_group)
                    fprintf(stdout, "IFREE,%d\n", currentIndex);
        }
    }
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
        } while (entry_offset < block_size);
        
    }
}

void indirect_block_reference(struct ext2_inode * curr_inode_ptr, __u32 inode_number)
{
 
    //single indirect
    if(curr_inode_ptr->i_block[12] > 0)
    {
        int * firstLevel_blocks = malloc(block_size);
        int first_level_offset = curr_inode_ptr->i_block[12] * block_size; //where the first level block locate.
        pread(fd_image, firstLevel_blocks, block_size, first_level_offset);  //the first_level_block is an array of block numbers, get that array ("firstLevel_blocks")
        
        int j=0;
        for( j = 0; j < block_size/4; j++){  //for every data blocks
            if(firstLevel_blocks[j] == 0) //break if the block number is 0
                break;
            
            
            __u32 level_indirection = 1;
            __u32 logical_block_offset = j +12;
            __u32 indirect_block_num = curr_inode_ptr->i_block[12];
            __u32 curr_block_num=firstLevel_blocks[j];
            
            fprintf(stdout, "INDIRECT,%u,%u,%u,%u,%u\n", inode_number,level_indirection,logical_block_offset,indirect_block_num,curr_block_num);
            
            
            /*
             __u16 entry_offset=0;
             do
             {
             pread(fd_image, &curr_entry, sizeof( struct ext2_dir_entry), firstLevel_blocks[j] * block_size + entry_offset); //locate this specific data block
             
             indirectory_entry(&curr_entry, logical_offset);
             
             entry_offset += curr_entry.rec_len;
             
             }while (entry_offset < block_size);
             
             */
            
        }
        
        free(firstLevel_blocks);
    }
    
    
    //double indirect
    if(curr_inode_ptr->i_block[13] > 0)
    {
        int * firstLevel_blocks = malloc(block_size);
        int first_level_offset = curr_inode_ptr->i_block[13] * block_size; //where the first level block locate.
        pread(fd_image, firstLevel_blocks, block_size, first_level_offset);  //the first_level_block is an array of block numbers, get that array ("firstLevel_blocks")
        
        
        int * secondLevel_blocks = malloc(block_size);
        int j=0;
        for( j = 0; j < block_size/4; j++)
        {  //for every indirect blocks
	  if (firstLevel_blocks[j]==0)
	    break;

	
            __u32 level_indirection = 1;
            __u32 logical_block_offset = 888888888;
            __u32 indirect_block_num = curr_inode_ptr->i_block[13];
            __u32 curr_block_num=firstLevel_blocks[j];
            
            fprintf(stdout, "INDIRECT,%u,%u,%u,%u,%u\n", inode_number,level_indirection,logical_block_offset,indirect_block_num,curr_block_num);
            
            
            pread(fd_image, secondLevel_blocks, block_size, firstLevel_blocks[j]*block_size); //get the second level block array.
            
            int x=0;
            for( x = 0; x < block_size/4; x++)
            {  //for every data blocks
                
                if(secondLevel_blocks[x] == 0) //break if the block number is 0
                    break;
		  fprintf(stdout,"double direct\n");
                __u32 level_indirection = 2;
                __u32 logical_block_offset = 888888888;
                __u32 indirect_block_num = firstLevel_blocks[j];
                __u32 curr_block_num=secondLevel_blocks[x];
                
                fprintf(stdout, "INDIRECT,%u,%u,%u,%u,%u\n", inode_number,level_indirection,logical_block_offset,indirect_block_num,curr_block_num);
                
            }
            
        }
        
        free(firstLevel_blocks);
        free(secondLevel_blocks);
        
    }
    
    
    //triple indirect
    if(curr_inode_ptr->i_block[14] > 0)
    {
        int * firstLevel_blocks = malloc(block_size);
        int first_level_offset = curr_inode_ptr->i_block[14] * block_size; //where the first level block locate.
        pread(fd_image, firstLevel_blocks, block_size, first_level_offset);  //the first_level_block is an array of block numbers, get that array ("firstLevel_blocks")
        
        
        int * secondLevel_blocks = malloc(block_size);
        int * thirdLevel_blocks = malloc(block_size);
        int j=0;
        for( j = 0; j < block_size/4; j++)
        {  //for every indirect blocks
            if(firstLevel_blocks[j] == 0) //break if the block number is 0
                break;
            
            
            __u32 level_indirection = 1;
            __u32 logical_block_offset = 888888888;
            __u32 indirect_block_num = curr_inode_ptr->i_block[14];
            __u32 curr_block_num=firstLevel_blocks[j];
            
            fprintf(stdout, "INDIRECT,%u,%u,%u,%u,%u\n", inode_number,level_indirection,logical_block_offset,indirect_block_num,curr_block_num);
            
            
            pread(fd_image, secondLevel_blocks, block_size, firstLevel_blocks[j]*block_size); //get the second level block array.
            
            int x=0;
            for( x = 0; x < block_size/4; x++)
            {  //for every data blocks
                
                if(secondLevel_blocks[x] == 0) //break if the block number is 0
                    break;
               
                __u32 level_indirection = 2;
                __u32 logical_block_offset = 888888888;
                __u32 indirect_block_num = firstLevel_blocks[j];
                __u32 curr_block_num=secondLevel_blocks[x];
                
                fprintf(stdout, "INDIRECT,%u,%u,%u,%u,%u\n", inode_number,level_indirection,logical_block_offset,indirect_block_num,curr_block_num);
                
                pread(fd_image, thirdLevel_blocks, block_size, secondLevel_blocks[x]*block_size); //get the second level block array.
                int k=0;
                for( k = 0; k < block_size/4; k++)
                {  //for every data blocks
                    if(thirdLevel_blocks[k] == 0) //break if the block number is 0
                        break;
		     fprintf(stdout,"tri-direct\n");
                    __u32 level_indirection = 3;
                    __u32 logical_block_offset = 888888888;
                    __u32 indirect_block_num = secondLevel_blocks[x];
                    __u32 curr_block_num=thirdLevel_blocks[k];
                    
                    fprintf(stdout, "INDIRECT,%u,%u,%u,%u,%u\n", inode_number,level_indirection,logical_block_offset,indirect_block_num,curr_block_num);
                }
                
            }
            
        }
        
        free(firstLevel_blocks);
        free(secondLevel_blocks);
        free(thirdLevel_blocks);
        
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
            if ( fileType == 'd')  // what if 'f'
            {
                directory_entry(&current_inode,inode_number);
            }
            
            if (fileType == 'd' || fileType == 'f')
            {
                indirect_block_reference(&current_inode,inode_number);
            }
            
        }
        
        
    }
}


void analyzeIndirect() {
    // no idea atm :/
}

int main(int argc, char* argv[]) {
    inode_directory = malloc(1);
    
    processArguments(argc, argv);
    analyzeSuper();
    analyzeGroup();
    analyzeBitmap();
    analyzeInode();
    inode_summary();
    analyzeIndirect();
}
