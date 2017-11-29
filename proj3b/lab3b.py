import csv
import sys

class Inode:
    def __init__(self, inode_num, n_links):
        self.inode_num = inode_num
        self.n_links = n_links
        self.state = 'USED'

class Block:
    def __init__(self, block_num):
        self.block_num = block_num
        self.state = 'USED'

class Analyzer:
    def __init__(self, input):
        self.csv_file = input
        self.n_blocks = 0
        self.n_inodes = 0
        self.inode_per_group = 0

        self.inodes = dict()
        self.blocks = dict()

    #---- PRINT FUNCTIONS ----#

    def print_invalid(self, b_num, i_num, offset, indirect):
        print("INVALID ", end="")

        if (indirect == 1):
            print("INDIRECT ", end="")
        elif (indirect == 2):
            print("DOUBLE INDIRECT ", end="")
        elif (indirect == 3):
            print("TRIPLE INDIRECT ", end="")

        print("BLOCK {} IN INODE {} AT OFFSET {}".format(b_num, i_num, offset))

    #---- ANALYSIS FUNCTIONS ----#

    def add_block(self, b_num, i_num, offset, indirect):
        if b_num < 0 or b_num >= self.n_blocks:
            self.print_invalid(b_num, i_num, offset, indirect)
        else:
            self.blocks[b_num] = Block(b_num)

    #---- READER FUNCTIONS ----#

    def read_superblock(self, entry):
        self.n_blocks = int(entry[1])
        self.n_inodes = int(entry[2])
        self.inode_per_group = int(entry[6])

    def read_inode(self, entry):
        i_num = int(entry[1])
        self.inodes[entry[1]] = Inode(i_num, int(entry[6]))
        n_addresses = int(entry[11])

        for i in range(12, 24):
            self.add_block(int(entry[i]), i_num, i-12, 0)
        self.add_block(int(entry[24]), i_num, 12, 1)
        self.add_block(int(entry[25]), i_num, 268, 2)
        self.add_block(int(entry[26]), i_num, 65804, 3)

        # NOT SURE IF THE ABOVE OR THE ONE BELOW
        """
        for i in range(12, 12 + min(n_addresses, 12)):
            b_num = int(entry[i])
            print(b_num)
            self.add_block(b_num, i_num, 8888, 0)
        if (n_addresses >= 13):
            self.add_block(int(entry[24]), i_num, 8888, 1)
        if (n_addresses >= 14):
            self.add_block(int(entry[25]), i_num, 8888, 2)
        if (n_addresses >= 15):
            self.add_block(int(entry[26]), i_num, 8888, 3)
        """

    def read_entry(self, entry):
        if (entry[0] == 'SUPERBLOCK'):
            self.read_superblock(entry)
        elif (entry[0] == 'INODE'):
            self.read_inode(entry)


    def read_csv(self):
        reader = csv.reader(self.csv_file, delimiter=',')

        for r in reader:
            self.read_entry(r)



if __name__ == "__main__":
    csv_file = open(sys.argv[1], 'r')
    analyzer = Analyzer(csv_file)
    analyzer.read_csv()

    csv_file.close()
