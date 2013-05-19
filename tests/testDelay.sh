#!/bin/bash

work/delayLog 100000000 2000000 > log & echo $! > /sys/fs/cgroup/cpuset/others/tasks

work/multiTask HelloV -r 100000000 -d 2000000 & echo $! > /sys/fs/cgroup/cpuset/others/tasks

