import csv
import sys

# ------------ FILE SYSTEM INFORMATION ---------------#
superblock = None
group = None
free_inodes = []
free_blocks = []
inodes = {}     # indexes are inode numbers
dirents = []
indirects = []


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
        if (entry[0] == 'SUPERBLOCK'):
            superblock = Superblock(entry)
        elif (entry[0] == 'GROUP'):
            group = Group(entry)
        elif (entry[0] == 'BFREE'):
            free_blocks.append(int(entry[1]))
        elif (entry[0] == 'IFREE'):
            free_inodes.append(int(entry[1]))
        elif (entry[0] == 'INODE'):
            inodes[int(entry[1])] = Inode(entry)
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


if __name__ == "__main__":
    csv_file = open(sys.argv[1], 'r')
    reader = Reader(csv_file)
    reader.read_csv()

    csv_file.close()
