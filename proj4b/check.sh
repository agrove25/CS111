# NAME: Andrew Grove
# EMAIL: koeswantoandrew@gmail.com
# ID: 304785991

echo "hello?"

./lab4b --period=5 --scale=C --log="log.txt" <<-EOF
SCALE=F
STOP
START
OFF
EOF

if [ $? -ne 0 ]
then echo "FAIL: return value not zero"
else echo "SUCCESS"
fi

if [ ! -s log.txt ]
then echo "FAIL: log.txt not created"
else echo "SUCCESS"
fi

grep "SCALE=F" log.txt; \
if [ $? -ne 0 ]
then echo "FAIL: scale not logged"
else echo "SUCCESS"
fi

grep "START" log.txt; \
if [ $? -ne 0 ]
then echo "FAIL: start not logged"
else echo "SUCCESS"
fi

grep "STOP" log.txt; \
if [ $? -ne 0 ]
then echo "FAIL: stop not logged"
else echo "SUCCESS"
fi

grep "OFF" log.txt; \
if [ $? -ne 0 ]
then echo "FAIL: off not logged"
else echo "SUCCESS"
fi
