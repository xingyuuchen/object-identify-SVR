#!/bin/sh

if [ $# != 1 ]; then
  echo "Usage: bash pulllog.sh {your ip addr}"
  exit 0
fi

ip=$1

scp root@${ip}:/var/log/oi-svr.log ../log/oi-svr.log
