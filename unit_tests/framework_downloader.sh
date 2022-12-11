#!/bin/bash

# Worked with cxxtest framework version 4.4, maybe later.

DIRECTORY_NAME=cxxtest-4.4
ARCHIVE_NAME=cxxtest-4.4.tar.gz

if [[ ! -d $DIRECTORY_NAME ]]
then
  wget -O $ARCHIVE_NAME https://sourceforge.net/projects/cxxtest/files/cxxtest/4.4/$ARCHIVE_NAME
  tar -zxvf $ARCHIVE_NAME
  rm $ARCHIVE_NAME
fi
