#!/bin/bash

for task in `cat /sys/fs/cgroup/cpuset/sysdefault/tasks`; do
	echo -n $task
	echo -n '|'
	echo $task > /sys/fs/cgroup/cpuset/others/tasks
done

echo $1 > /sys/fs/cgroup/cpuset/worker0/tasks
echo $2 > /sys/fs/cgroup/cpuset/worker1/tasks
echo $3 > /sys/fs/cgroup/cpuset/worker2/tasks
echo $4 > /sys/fs/cgroup/cpuset/worker3/tasks

