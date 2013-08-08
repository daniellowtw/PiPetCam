#!/bin/bash
#Helper script to start RaspiFastCamD

pid_file=/tmp/raspifastcamd.pid

if [ ! -e $pid_file ]; then
	echo "Error: The pid file $pid_file does not exist, looks like raspifastcamd is not running."
	exit 1
fi

pid=$(cat $pid_file)

echo "Pid of raspifastcamd was $pid"

kill $pid

echo "Killed raspifastcamd"

rm $pid_file

exit 0
