# NAME: Andrew Grove
# EMAIL: koeswantoandrew@gmail.com
# ID: 304785991

default:
	gcc -w -Wall -Wextra -o lab2_add lab2_add.c -pthread
	gcc -w -Wall -Wextra -o lab2-list SortedList.c lab2_list.c -pthread

tests: default
	./lab2_add --iterations=10 --threads=1 > lab2_add.csv
	./lab2_add --iterations=100 --threads=1 >> lab2_add.csv
	./lab2_add --iterations=1000 --threads=1 >> lab2_add.csv
	./lab2_add --iterations=10000 --threads=1 >> lab2_add.csv

	# lab2_add-1.png
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

	#lab2_list-1.png
	./lab2_list --threads=1 --iterations=10 >> lab2_list.csv
	./lab2-list --threads=1 --iterations=100 >> lab2_list.csv
	./lab2-list --threads=1 --iterations=1000 >> lab2_list.csv
	./lab2-list --threads=1 --iterations=10000 >> lab2_list.csv
	./lab2-list --threads=1 --iterations=20000 >> lab2_list.csv

	#lab2_list-2.png
	./lab2_list --threads=2 --iterations=1 >> lab2_list.csv
	./lab2_list --threads=2 --iterations=10 >> lab2_list.csv
	./lab2_list --threads=2 --iterations=100 >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 >> lab2_list.csv
	./lab2_list --threads=4 --iterations=10 >> lab2_list.csv
	./lab2_list --threads=4 --iterations=100 >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1000 >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 >> lab2_list.csv
	./lab2_list --threads=8 --iterations=10 >> lab2_list.csv
	./lab2_list --threads=8 --iterations=100 >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1000 >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10 >> lab2_list.csv
	./lab2_list --threads=12 --iterations=100 >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1000 >> lab2_list.csv

	./lab2_list --threads=2 --iterations=1 --yield=i >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=i >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=i >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=i >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=d >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=d >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=d >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=d >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=il >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=il >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=il >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=il >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=dl >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=dl >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=dl >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=dl >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=i >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=i >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=i >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=i >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=d >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=d >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=d >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=d >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=il >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=il >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=il >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=il >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=dl >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=dl >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=dl >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=dl >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=i >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=i >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=i >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=i >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=d >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=d >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=d >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=d >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=il >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=il >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=il >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=il >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=dl >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=dl >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=dl >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=dl >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=i >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=i >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=i >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=i >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=d >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=d >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=d >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=d >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=il >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=il >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=il >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=il >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=dl >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=dl >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=dl >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=dl >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=i >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=i >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=i >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=i >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=d >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=d >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=d >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=d >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=il >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=il >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=il >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=il >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=dl >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=dl >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=dl >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=dl >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=i >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=i >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=i >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=i >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=d >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=d >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=d >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=il >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=il >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=il >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=dl >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=dl >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=dl >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl >> lab2_list.csv

	#lab2_list-3.png
	./lab2_list --threads=2 --iterations=1 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv

	./lab2_list --threads=2 --iterations=1 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=2 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=4 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=8 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=16 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8 --iterations=32 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

	#lab2_list-4.png
	./lab2_list --threads=1 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=1 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=2 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=4--iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=4--iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=8--iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=8--iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=12--iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=12--iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=16--iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=16--iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=24--iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=24--iterations=1000 --sync=m >> lab2_list.csv
