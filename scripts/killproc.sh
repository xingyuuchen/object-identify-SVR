#!/bin/sh

signal="-2"

if [ $# == 1 ]; then
  signal="$1"
fi

function kill_procs() {
  while [ $# != 0 ]
  do
    echo kill ${signal} $1
    kill ${signal} $1
    shift
  done
}

kill_procs `ps -A | grep 'oi-svr$' | awk '{print $1}'`
kill_procs `ps -A | grep 'reverse-proxy$' | awk '{print $1}'`
#kill_procs `netstat -anp | grep 5002`
