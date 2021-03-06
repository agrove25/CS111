# NAME: Andrew Grove
# EMAIL: koeswantoandrew@gmail.com
# ID: 304785991

default:
	gcc -L -g -w -Wall -Wextra -o lab2_list SortedList.c lab2_list.c -pthread -lprofiler

clean:
	rm lab2_list *.tar.gz *.perf

dist: default tests graphs profile
	tar -czvf lab2b-304785991.tar.gz Makefile lab2_list.c \
	SortedList.c SortedList.h lab2_list.gp *.png *.csv profile.out README

graphs:
	gnuplot lab2_list.gp

tests:
	echo "NAME: Andrew Grove" >  lab2b_list.csv
	echo "EMAIL: koeswantoandrew@gmail.com" >>  lab2b_list.csv
	echo "ID: 304785991" >>  lab2b_list.csv

	@./lab2_list --threads=1  --iterations=1000 --sync=m >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=m >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=m >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=m >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=m >>  lab2b_list.csv
	@./lab2_list --threads=16 --iterations=1000 --sync=m >>  lab2b_list.csv
	@./lab2_list --threads=24 --iterations=1000 --sync=m >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=1000 --sync=s >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=s >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=s >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=s >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=s >>  lab2b_list.csv
	@./lab2_list --threads=16 --iterations=1000 --sync=s >>  lab2b_list.csv
	@./lab2_list --threads=24 --iterations=1000 --sync=s >>  lab2b_list.csv

	-@./lab2_list --threads=1  --iterations=1	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=4  --iterations=1	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=8  --iterations=1	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=12  --iterations=1	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=16  --iterations=1	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=1  --iterations=2	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=4  --iterations=2	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=8  --iterations=2	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=12  --iterations=2	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=16  --iterations=2	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=1  --iterations=4	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=4  --iterations=4	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=8  --iterations=4	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=12  --iterations=4	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=16  --iterations=4	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=1  --iterations=8	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=4  --iterations=8	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=8  --iterations=8	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=12  --iterations=8	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=16  --iterations=8	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=1  --iterations=16	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=4  --iterations=16	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=8  --iterations=16	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=12  --iterations=16	 --lists=4 --yield=id >>  lab2b_list.csv
	-@./lab2_list --threads=16  --iterations=16	 --lists=4 --yield=id >>  lab2b_list.csv

	@./lab2_list --threads=1  --iterations=10	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=10	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=10	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=10	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=20	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=20	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=20	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=20	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=16 --iterations=20	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=40	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=40	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=40	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=40	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=16 --iterations=20	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=80	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=80	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=80	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=80	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=16  --iterations=20	 --sync=m --lists=4 --yield=id >>  lab2b_list.csv

	@./lab2_list --threads=1  --iterations=10	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=10	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=10	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=10	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=20	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=20	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=20	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=20	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=16 --iterations=20	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=40	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=40	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=40	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=40	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=16 --iterations=20	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=80	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=80	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=80	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=80	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv
	@./lab2_list --threads=16  --iterations=20	 --sync=s --lists=4 --yield=id >>  lab2b_list.csv

	@./lab2_list --threads=1  --iterations=1000 --sync=m  --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=m --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=m --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=m --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=m --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=1000 --sync=m  --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=m --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=m --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=m --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=m --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=1000 --sync=m  --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=m --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=m --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=m --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=m --lists=16   >>  lab2b_list.csv

	@./lab2_list --threads=1  --iterations=1000 --sync=s  --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=s --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=s --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=s --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=s --lists=4   >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=1000 --sync=s  --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=s --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=s --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=s --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=s --lists=8   >>  lab2b_list.csv
	@./lab2_list --threads=1  --iterations=1000 --sync=s  --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=2  --iterations=1000 --sync=s --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=4  --iterations=1000 --sync=s --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=8  --iterations=1000 --sync=s --lists=16   >>  lab2b_list.csv
	@./lab2_list --threads=12 --iterations=1000 --sync=s --lists=16   >>  lab2b_list.csv

profile:
	CPUPROFILE=raw.perf ./lab2_list --iterations=1000 --threads=12 --sync=s
	pprof --text lab2_list raw.perf > profile.out
	pprof --list=listOps lab2_list raw.perf >> profile.out
