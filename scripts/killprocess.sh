#!/bin/sh

pids=$(ps -A | grep 'oi_svr$' | awk '{print $1}')

for pid in ${pids}
do
  kill -9 ${pid}
  echo kill ${pid}.
done

