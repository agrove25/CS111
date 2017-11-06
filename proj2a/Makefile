# NAME: Andrew Grove
# EMAIL: koeswantoandrew@gmail.com
# ID: 304785991

default:
	gcc -w -Wall -Wextra -o lab2_add lab2_add.c -pthread
	gcc -w -Wall -Wextra -o lab2_list SortedList.c lab2_list.c -pthread

tests: default
	./lab2_add --iterations=10 --threads=1 > lab2_add.csv
	./lab2_add --iterations=100 --threads=1 >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=1 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv

	# lab2_add-1.png
	./lab2_add --iterations=10 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=20 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=40 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=80 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=100 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=1 --yield >> lab2_add.csv
	./lab2_add --iterations=10 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=20 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=40 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=80 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=100 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=10 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=20 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=40 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=80 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=100 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=4 --yield >> lab2_add.csv
	./lab2_add --iterations=10 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=20 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=40 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=80 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=100 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=10 --threads=12 --yield >> lab2_add.csv
	./lab2_add --iterations=20 --threads=12 --yield >> lab2_add.csv
	./lab2_add --iterations=40 --threads=12 --yield >> lab2_add.csv
	./lab2_add --iterations=80 --threads=12 --yield >> lab2_add.csv
	./lab2_add --iterations=100 --threads=12 --yield >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=12 --yield >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 --yield >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=12 --yield >> lab2_add.csv

	#lab2_add-2.png
	./lab2_add --iterations=100 --threads=2 >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=2 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=2 >> lab2_add.csv
	./lab2_add --iterations=100 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=2 --yield >> lab2_add.csv
	./lab2_add --iterations=100 --threads=8 >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=8 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=8 >> lab2_add.csv
	./lab2_add --iterations=100 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 --yield >> lab2_add.csv
	./lab2_add --iterations=100000 --threads=8 --yield >> lab2_add.csv

	#lab2_add-4.png
	./lab2_add --iterations=10000 --threads=2 --yield --sync=m >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=2 --yield --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 --yield --sync=c >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 --yield --sync=m >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=4 --yield --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 --yield --sync=c >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 --yield --sync=m >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=8 --yield --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 --yield --sync=c >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 --yield --sync=m >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=12 --yield --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 --yield --sync=c >> lab2_add.csv

	#lab2_add-5.png
	./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=1 --sync=m >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 --sync=m >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 --sync=m >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 --sync=m >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 --sync=m >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=1 --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 --sync=s >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=1 --sync=c >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=2 --sync=c >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=4 --sync=c >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=8 --sync=c >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=12 --sync=c >> lab2_add.csv
