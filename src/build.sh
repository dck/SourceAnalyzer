#!/bin/bash
gcc -g -O0 -I/usr/local/BerkeleyDB.5.1/include -L/usr/local/BerkeleyDB.5.1/lib -ldb sa3test.c
