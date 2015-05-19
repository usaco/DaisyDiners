#!/bin/bash

cd `dirname $0`
rootdir=`pwd`

botname=$*
botfile=`echo $botname | sed "s/[^a-zA-Z0-9]//g"`
botdir=$rootdir/bots/$botfile

echo "Creating directory $botdir"
mkdir $botdir

echo "Copying in bot template files"
sed "s/BOTNAME/$botfile/g" $rootdir/client/Makefile.template > $botdir/Makefile
sed "s/BOTNAME/$botname/g" $rootdir/client/bot-template.c > $botdir/$botfile.cpp

echo "Setting up Makefile for C++ compilation"
sed "s/${botfile}.c/${botfile}.cpp/g" -i $botdir/Makefile
sed "s/gcc/g++/g" -i $botdir/Makefile

echo "Symlinking client libraries"
cd $botdir
ln -s $rootdir/client/client.c
ln -s $rootdir/client/client.h
