#!/bin/bash

###################################################################
# Lauren Egerton
# CS 372/Spring 2017
# May 7, 2017
# Project 1
# Description: shell program to compile chatclient.c, change permissions
# for chatserve.py, and display the server's hostname for the client to
# use.
###################################################################

chmod +x chatserve

gcc -o chatclient chatclient.c

echo
echo "The server host name is:" $(hostname)
echo


