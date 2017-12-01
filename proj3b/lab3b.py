import csv
import sys

# ------------ FILE SYSTEM INFORMATION ---------------#
superblock = None
group = None
free_inodes = []
free_blocks = []
inodes = []
dirents = []
indirects = []

# ---------------- AUDIT VARIABLES --------------------#
reader = None
writer = None
errors = 0
used_blocks = []                 # used to check for unref and alloc
blocks = {}                      # used to check for duplicates
nonzero_inodes = []
allocated_inode_num = {}


# -------------- CSV FILE CLASSES ---------------------#
class Superblock:
    def __init__(self, entry):
        self.n_blocks = int(entry[1])
        self.n_inodes = int(entry[2])
        self.block_size = int(entry[3])
        self.inode_size = int(entry[4])
        self.blocks_per_group = int(entry[5])
        self.inodes_per_group = int(entry[6])
        self.first_nonreserved_inode = int(entry[7])

class Group:
    def __init__(self, entry):
        self.group_num = int(entry[1])
        self.n_blocks = int(entry[2])
        self.n_inodes = int(entry[3])
        self.n_free_blocks = int(entry[4])
        self.n_free_inodes = int(entry[5])
        self.block_num_free_block_bitmap = int(entry[6])
        self.block_num_free_inode_bitmap = int(entry[7])
        self.block_num_first_inode  = int(entry[8])

class Inode:
    def __init__(self, entry):
        self.inode_num = int(entry[1])
        self.inode_file = entry[2]
        self.mode = int(entry[3], 8)
        self.owner = int(entry[4])
        self.group = int(entry[5])
        self.link_count = int(entry[6])
        self.last_change = entry[7]
        self.last_modified = entry[8]
        self.last_accessed = entry[9]
        self.file_size = int(entry[10])
        self.n_blocks = int(entry[11])
        self.blocks = []

        for i in range(0, 15):
            self.blocks.append(int(entry[i + 12]))

        if (self.inode_file != '0'):
            #these two is basically the same thing, but one is for inode search and another is for inode_num search
            nonzero_inodes.append(self)
            allocated_inode_num[self.inode_num] = self.inode_num
            

class Dirent:
    def __init__(self, entry):
        self.parent_inode_num = int(entry[1])
        self.byte_offset = int(entry[2])
        self.ref_inode_num = int(entry[3])
        self.entry_len = int(entry[4])
        self.name_len = int(entry[5])
        self.name = entry[6]

class Indirect:
    def __init__(self, entry):
        self.own_inode_num = int(entry[1])
        self.indirect_level = int(entry[2])
        self.byte_offset = int(entry[3])
        self.scan_block_num = int(entry[4])
        self.ref_block_number = int(entry[5])

class Reader:
    def __init__(self, csv_file):
        self.csv_file = csv_file

    def read_entry(self, entry):
        global superblock, group, free_inodes, free_blocks, inodes
        global dirents, indirects

        if (entry[0] == 'SUPERBLOCK'):
            superblock = Superblock(entry)
        elif (entry[0] == 'GROUP'):
            group = Group(entry)
        elif (entry[0] == 'BFREE'):
            free_blocks.append(int(entry[1]))
        elif (entry[0] == 'IFREE'):
            free_inodes.append(int(entry[1]))
        elif (entry[0] == 'INODE'):
            inodes.append(Inode(entry))
        elif (entry[0] == 'DIRENT'):
            dirents.append(Dirent(entry))
        elif (entry[0] == 'INDIRECT'):
            indirects.append(Indirect(entry))
        else:
            print("INVALID CSV FILE")
            exit(1)



    def read_csv(self):
        reader = csv.reader(self.csv_file, delimiter=',')

        for r in reader:
            self.read_entry(r)

class Writer:
    def print_invalid(self, b_num, i_num, offset, indirect):
        print("INVALID ", end="")

        if (indirect == 1):
            print("INDIRECT ", end="")
        elif (indirect == 2):
            print("DOUBLE INDIRECT ", end="")
        elif (indirect == 3):
            print("TRIPLE INDIRECT ", end="")

        print("BLOCK {} IN INODE {} AT OFFSET {}".format(b_num, i_num, offset))

    def print_reserved(self, b_num, i_num, offset, indirect):
        print("RESERVED ", end="")

        if (indirect == 1):
            print("INDIRECT ", end="")
        elif (indirect == 2):
            print("DOUBLE INDIRECT ", end="")
        elif (indirect == 3):
            print("TRIPLE INDIRECT ", end="")

        print("BLOCK {} IN INODE {} AT OFFSET {}".format(b_num, i_num, offset))

    def print_duplicate(self, b_num, i_num, offset, indirect):
        print("DUPLICATE ", end="")

        if (indirect == 1):
            print("INDIRECT ", end="")
        elif (indirect == 2):
            print("DOUBLE INDIRECT ", end="")
        elif (indirect == 3):
            print("TRIPLE INDIRECT ", end="")

        print("BLOCK {} IN INODE {} AT OFFSET {}".format(b_num, i_num, offset))


def check_block(b_num, i_num, offset, indirect):
    global blocks, used_blocks

    if b_num < 0 or b_num >= superblock.n_blocks:
        writer.print_invalid(b_num, i_num, offset, indirect)
    elif b_num < 8 and b_num > 0:
        writer.print_reserved(b_num, i_num, offset, indirect)
    elif b_num != 0:
        if (b_num not in used_blocks):
            used_blocks.append(b_num)
            blocks[b_num] = []

        blocks[b_num].append((b_num, i_num, offset, indirect))

def analyze_blocks():
    # iterating over inode block addresses
    for inode in inodes:
        for i in range(0, 15):
            b_num = inode.blocks[i]
            i_num = inode.inode_num

            if (i < 12):
                check_block(b_num, i_num, i, 0)
            if (i == 12):
                check_block(b_num, i_num, 12, 1)
            if (i == 13):
                check_block(b_num, i_num, 268, 2)
            if (i == 14):
                check_block(b_num, i_num, 65804, 3)

    # checks the indirect blocks
    for ind in indirects:
        check_block(ind.ref_block_number, ind.own_inode_num,
                    ind.byte_offset, ind.indirect_level)

    # checks for unreferenced, allocated, and duplicates
    for i in range(8, superblock.n_blocks):
        if (i not in free_blocks) and (i not in used_blocks):
            print("UNREFERENCED BLOCK {}".format(i))
        elif (i in free_blocks) and (i in used_blocks):
            print("ALLOCATED BLOCK {} ON FREELIST".format(i))

    for i in used_blocks:
        if (len(blocks[i]) > 1):
            for tup in blocks[i]:
                writer.print_duplicate(tup[0], tup[1], tup[2], tup[3])


def analyze_inodes():
    global free_inodes

    for inode in inodes:
        if inode.inode_num not in free_inodes and (inode.inode_file == '0'):
                print("UNALLOCATED INODE {} NOT ON FREELIST".format(inode.inode_num))
        elif inode.inode_num in free_inodes and (inode.inode_file !='0'):
                print("ALLOCATED INODE {} ON FREELIST".format(inode.inode_num))

def analyze_dirent():
    global free_inodes, dirents, nonzero_inodes, superblock

    findItsParent = {}
    count_linked_to_inode = {};
        
    
    for dirent in dirents:
        #if this dirent inode is allocated:
        if  (dirent.ref_inode_num not in freeInodes) and (dirent.ref_inode_num in allocated_inode_num): 
            findItsParent[dirent.ref_inode_num] = dirent.parent_inode_num
            #record what inode it refers to, increment refer count
            count_linked_to_inode[dirent.ref_inode_num] = count_linked_to_inode.get(dirent.entry_inode_num,0) + 1
            
    for inode in nonzero_inodes:    #for every "d", "f", "s" or "?" inodes
        if (inode.inode_num in count_linked_to_inode) and (inode.link_count != count_linked_to_inode[inode.inode_num]):
            print("INODE {} HAS {} LINKS BUT LINKCOUNT IS {}".format(inode.inode_num, count_linked_to_inode[inode.inode_num], inode.link_count))

        elif inode.inode_num not in count_linked_to_inode and (inode.link_count !=0 ):
            print("INODE {} HAS 0 LINKS BUT LINKCOUNT IS {}".format(inode.inode_num, inode.link_count))

# check invalid, unallocated, and unconsistance inode
    for dirent in dirents:
        if (dirent.ref_inode_num not in allocated_inode_num) or (dirent.ref_inode_num in free_inodes):
            print("DIRECTORY INODE {} NAME {} UNALLOCATED INODE {}".format(dirent.parent_inode_num, dirent.name, dirent.ref_inode_num))
        
        if (dirent.ref_inode_num < 1 ) or (dirent.ref_inode_num > superblock.n_inodes):
            print("DIRECTORY INODE {} NAME {} INVALID INODE {}".format(dirent.parent_inode_num, dirent.name, dirent.ref_inode_num))
        
        if (dirent.name = "'..'") and (dirent.parent_inode_num in findItsParent) and  (findItsParent[dirent.parent_inode_num] != dirent.ref_inode_num):
            print("DIRECTORY INODE {} NAME '..' LINK TO INODE {} SHOULD BE {}".format(dirent.parent_inode_num, dirent.ref_inode_num, findItsParent[dirent.parent_inode_num]))

        if (dirent.name = "'.'") and (dirent.parent_inode_num != dirent.ref_inode_num):
            print("DIRECTORY INODE {} NAME '.' LINK TO INODE {} SHOULD BE {}".format(dirent.parent_inode_num, dirent.ref_inode_num, dirent.parent_inode_num))
 
        
if __name__ == "__main__":
    # Initial Variable Setup
    csv_file = open(sys.argv[1], 'r')
    reader = Reader(csv_file)
    writer = Writer()
    reader.read_csv()

    # Analysis
    analyze_blocks()

    analyze_inodes()

    analyze_dirent()
    
    csv_file.close()
