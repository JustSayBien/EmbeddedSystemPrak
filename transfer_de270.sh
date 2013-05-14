#!/bin/bash

# Copy binary to board;

# stty settings:
# $ stty -F /dev/ttyS0
# speed 57600 baud; line = 0;
# min = 1; time = 0;
# ignbrk -brkint -icrnl -imaxbel
# -opost -onlcr
# -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke

# on gentoo it is sufficient to do a
# $ stty -F /dev/ttyS0 speed 57600 -opost

#stty -g -F /dev/ttyS0
#401:0:1cb1:0:3:1c:7f:15:4:0:1:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0

#set tty with command:
#stty -F /dev/ttyS0 401:0:1cb1:0:3:1c:7f:15:4:0:1:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0


DEVICE=/dev/ttyS0

if [ -n "$1" ]
then
	BIN=$1
else
	BIN=main.bin
fi

echo ">transfering ${BIN} to ${DEVICE}"

    dd if=${BIN} of=${DEVICE} 2>&1 | tee transfer.log
    echo "Please check #transferred_bytes!"
    #echo "Value on 7seg should be (DE2):"
    #grep [bB]ytes transfer.log | sed 's/\([0-9]*\).*/ibase=10;obase=16;\1/' | bc | sed 's/\([0-9A-F]\{2\}\)\([0-9A-F]\{2\}\).*/\2 \1/' | sed 's/B/b/g' | sed 's/D/d/g'
    echo "Value on 7seg should be (DE2-70):"
    grep [bB]ytes transfer.log | sed 's/\([0-9]*\).*/ibase=10;obase=16;\1/' | bc
    echo "If values differ, check if tty is set up correctly, eg. by invoking"
    echo "stty -F /dev/ttyS0 speed 115200 -opost"
    #echo "stty -F /dev/ttyS0 401:0:1cb1:0:3:1c:7f:15:4:0:1:0:11:13:1a:0:12:f:17:16:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0:0"

