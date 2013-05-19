#!/bin/bash

work/pushTask HelloV -m

declare -i s=0
while true
do
	cgexec *:others work/multiTask HelloV -r 10000 -d 2000000;
	s=$((s+1))
#	sleep 1;
done

