#!/bin/bash

work/pushTask HelloV -m

declare -i s=0
while true
do
	work/pushTask HelloV -r -d 20000000;
	s=$((s+1))
#	sleep 1;
done

