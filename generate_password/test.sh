#!/bin/sh  
#a=`cat /sys/class/ieee80211/phy0/macaddress | cut -c 13- | sed 's/://g'`  
a=`cat /config/sample_uid | cut -c 12-18 | sed 's/://g'`  

echo $a > /bin/text.txt
