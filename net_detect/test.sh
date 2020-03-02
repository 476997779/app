#!/bin/sh  
#a=`cat /sys/class/ieee80211/phy0/macaddress | cut -c 13- | sed 's/://g'`  
a=`cat ./andy.txt | sed -n '1p'`  

echo $a 
