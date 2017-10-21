./sample

echo "... testing trivial shell session"
let PORT=6661
./lab1b-server --port=6661 > SVR_OUT &
./lab1b-client --port=6661 --log=LOG_1 > STDOUT 2> STDERR <<-EOF
	PAUSE 1
	EXPECT "/bin/bash"
	SEND "echo \$SHELL\n"
	WAIT 1
	SEND "exit 6\n"
	PAUSE 1
	CLOSE
EOF