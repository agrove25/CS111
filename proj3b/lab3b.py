import csv
import sys

class Analyzer:
    def __init__(self, input):
        self.csv_file = input
        self.n_blocks = 0
        self.n_inodes = 0
        self.inode_per_group = 0

    #---- READER FUNCTIONS ----#

    def read_superblock(self, entry):
        self.n_blocks = int(entry[1])
        self.n_inodes = int(entry[2])
        self.inode_per_group = int(entry[6])

    def read_entry(self, entry):
        print(entry[0])

        if (entry[0] == 'SUPERBLOCK'):
            self.read_superblock(entry)

    def read_csv(self):
        reader = csv.reader(self.csv_file, delimiter=',')

        for r in reader:
            self.read_entry(r)


    #----- ANALYSIS FUNCTIONS -----#


if __name__ == "__main__":
    csv_file = open(sys.argv[1], 'r')
    analyzer = Analyzer(csv_file)
    analyzer.read_csv()

    print(analyzer.n_blocks)
    print(analyzer.n_inodes)
    print(analyzer.inode_per_group)

    csv_file.close()
