
echo "start update zrt_sta_app"
cp zrt_sta_app zrt_sta_app_update
if [ $? -ne 0 ]
then 
	exit 1
fi

echo "start update zrt_sta_daemon"

cp zrt_sta_daemon zrt_sta_daemon_update 
if [ $? -ne 0 ]
then 
	exit 1
fi

echo "update finished"
