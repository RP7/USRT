#!/bin/bash

declare -i s=0
while true
do
	work/pushTask HelloV -r -d 20000;
	s=$((s+1))
#	sleep 1;
done

