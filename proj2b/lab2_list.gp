#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
# 8. wait time per operation
#
# output:
# lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock synchronized list operations.
# lab2b_2.png ... mean time per mutex wait and mean time per operation for mutex-synchronized list operations.
# lab2b_3.png ... successful iterations vs. threads for each synchronization method.
# lab2b_4.png ... throughput vs. number of threads for mutex synchronized partitioned lists.
# lab2b_5.png ... throughput vs. number of threads for spin-lock-synchronized partitioned lists.
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","

# how many threads/iterations we can run without failure (w/o yielding)
set title "lab2b_1: throughput vs. number of threads (comparing sync options)"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput"
set logscale y 10
set output 'lab2b_1.png'

# grep out only single threaded, un-protected, non-yield results
plot \
    "< grep -E 'list-none-m,[0-9]+,1000,1,' lab2_list.csv" using ($2):(1000000000/($7)) \
	title 'list: mutex' with linespoints lc rgb 'blue', \
     "< grep -E 'list-none-s,[0-9]+,1000,1,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title 'list: spin-lock' with linespoints lc rgb 'orange'


set title "lab2b_2: time per mutex wait / time per operation vs number of threads"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Time"
set logscale y 10
set output 'lab2b_2.png'
# note that unsuccessful runs should have produced no output
plot \
    "< grep -E 'list-none-m,[0-9]+,1000,1,' lab2_list.csv" using ($2):($7) \
  title 'per op' with linespoints lc rgb 'red', \
    "< grep -E 'list-none-m,[0-9]+,1000,1,' lab2_list.csv" using ($2):($8) \
  title 'per mutex wait' with linespoints lc rgb 'green'

set title "lab2b_3: Iterations that run without failure"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set ylabel "Throughput"
set logscale y 10
set output 'lab2b_3.png'

plot \
    "< grep -E 'list-id-none,[0-9]+,[0-9]+,4' lab2_list.csv" using ($2):($3) \
	with points lc rgb "red" title "unprotected", \
    "< grep -E 'list-id-m,[0-9]+,[0-9]+,4' lab2_list.csv" using ($2):($3) \
  with points lc rgb "orange" title "mutex", \
    "< grep -E 'list-id-s,[0-9]+,[0-9]+,4' lab2_list.csv" using ($2):($3) \
	with points lc rgb "blue" title "spin-lock"
#
# "no valid points" is possible if even a single iteration can't run
#

set title "lab2b_4: throughput when using sublists (mutex)"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput"
set logscale y 10
set output 'lab2b_4.png'

plot \
    "< grep -E 'list-none-m,[0-9],1000,1,|list-none-m,12,1000,1,' lab2_list.csv" using ($2):(1000000000/($7)) \
	title '1 list' with linespoints lc rgb 'red', \
    "< grep -E 'list-none-m,[0-9],1000,4,|list-none-m,12,1000,4,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title '4 list' with linespoints lc rgb 'green', \
    "< grep -E 'list-none-m,[0-9],1000,8,|list-none-m,12,1000,8,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title '8 list' with linespoints lc rgb 'blue', \
    "< grep -E 'list-none-m,[0-9],1000,16,|list-none-m,12,1000,16,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title '16 list' with linespoints lc rgb 'black'

set title "lab2b_5: throughput when using sublists (spin-lock)"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput"
set logscale y 10
set output 'lab2b_5.png'

plot \
    "< grep -E 'list-none-s,[0-9],1000,1,|list-none-s,12,1000,1,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title '1 list' with linespoints lc rgb 'red', \
    "< grep -E 'list-none-s,[0-9],1000,4,|list-none-s,12,1000,4,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title '4 list' with linespoints lc rgb 'green', \
    "< grep -E 'list-none-s,[0-9],1000,8,|list-none-s,12,1000,8,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title '8 list' with linespoints lc rgb 'blue', \
    "< grep -E 'list-none-s,[0-9],1000,16,|list-none-s,12,1000,16,' lab2_list.csv" using ($2):(1000000000/($7)) \
  title '16 list' with linespoints lc rgb 'black'
