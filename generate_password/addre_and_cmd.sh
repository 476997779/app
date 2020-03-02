#!/bin/sh 

ADDR=/config
SAMPLE=/sample_uid
CMD_UID=1KUDWNTJE5EUUG83111A

/bin/mkdir $ADDR
/bin/chmod 0777 $ADDR
/bin/touch $ADDR$SAMPLE
/bin/echo $CMD_UID > $ADDR$SAMPLE

