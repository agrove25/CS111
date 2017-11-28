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
        self.inodes = set()
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

    def print_indirect(self, b_num, i_num, offset, indirect):
        if (indirect == 0):
            print("INVALID BLOCK {} IN INODE {} AT OFFSET {}\n".format(b_num, i_num, offset))
        elif (indirect > 0):
            print("Haven't added this functionality (print_indirect)")

    #---- ANALYSIS FUNCTIONS ----#

    def add_block(self, b_num, i_num, offset, indirect):
        if b_num < 0 or b_num >= self.n_blocks:
            self.print_indirect(b_num, i_num, offset, indirect)



    #---- READER FUNCTIONS ----#

    def read_superblock(self, entry):
        self.n_blocks = int(entry[1])
        self.n_inodes = int(entry[2])
        self.inode_per_group = int(entry[6])

    def read_inode(self, entry):
        i_num = int(entry[1])
        self.inodes[entry[1]] = Inode(i_num, int(entry[6]))
        n_addresses = int(entry[11])

        for i in range(12, 12 + min(n_addresses, 12)):
            b_num = int(entry[i])
            self.add_block(b_num, i_num, 8888, 0)

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

    print(analyzer.n_blocks)
    print(analyzer.n_inodes)
    print(analyzer.inode_per_group)

    csv_file.close()
