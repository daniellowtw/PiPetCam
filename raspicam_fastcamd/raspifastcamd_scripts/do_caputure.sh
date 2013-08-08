#!/bin/bash
#Helper script to capture an image with raspifastcamd

pid_file=/tmp/raspifastcamd.pid

if [ ! -e $pid_file ]; then
	echo "Error: The pid file $pid_file does no exist, looks like raspifastcamd is not running."
	exit 1
fi

kill -USR1 $(cat $pid_file)