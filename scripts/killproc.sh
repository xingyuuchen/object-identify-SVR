#!/bin/sh

function kill_procs() {
  while [ $# != 0 ]
  do
    echo kill $1
    kill -2 $1
    shift
  done
}

kill_procs `ps -A | grep 'oi-svr$' | awk '{print $1}'`
#kill_procs `netstat -anp | grep 5002`
