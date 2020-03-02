#!/bin/sh
WLAN=$1
MAC=$2
para=`iw dev $WLAN station dump | grep $MAC`
echo $para
get_mac=`echo $para | cut -c 9-25`
echo $get_mac
if [ $get_mac = $MAC ]
then
	echo 1 > is_dev_on.txt
else
	echo 0 > is_dev_on.txt
fi
