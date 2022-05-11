#!/bin/bash

## Instructions counter.
## Year: 2020
## Author: Manel Lurbe Sempere <malursem@gap.upv.es>

for cpu in $(seq 1 79);
do
	{ sudo cpufreq-set -g ondemand -c $cpu; }
done;

echo "CPU Free..."