# NAME: Andrew Grove
# EMAIL: koeswantoandrew@gmail.com
# ID: 304785991

./lab4b --period=5 --scale=C --log=log.txt <<-EOF
SCALE=F
STOP
START
OFF
EOF

if [ $? -ne 0 ]
then echo "FAIL: return value not zero"

if [ ! -s log.txt ]
then echo "FAIL: log.txt not created"

grep "SCALE=F" log.txt;
if [ $? -ne 0 ]
then echo "FAIL: scale not logged"

grep "START" log.txt;
if [ $? -ne 0 ]
then echo "FAIL: start not logged"

grep "STOP" log.txt;
if [ $? -ne 0 ]
then echo "FAIL: stop not logged"

grep "OFF" log.txt;
if [ $? -ne 0 ]
then echo "FAIL: scale not logged"
