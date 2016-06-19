#!/bin/bash

INSTALL_PATH=~/fuzzycd/

if !(test -d $INSTALL_PATH); then
    mkdir $INSTALL_PATH
fi

cp fuzzycd $INSTALL_PATH
cp fuzzycd.sh $INSTALL_PATH
cp fuzzycd.config $INSTALL_PATH

str1='alias cd="source '${INSTALL_PATH}'fuzzycd.sh"'
str2='alias cdf="'${INSTALL_PATH}'fuzzycd; source '${INSTALL_PATH}'command.sh"'

echo ${str1} >> ~/.bashrc
echo ${str2} >> ~/.bashrc


