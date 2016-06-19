#!/bin/bash

unalias cd 2> /dev/null

if [ $# == 1 ];then

    cd $1

    ~/fuzzycd/fuzzycd `pwd`

else
    echo "[usage: cd yourpath]"
fi

alias cd='source ~/fuzzycd/fuzzycd.sh'

