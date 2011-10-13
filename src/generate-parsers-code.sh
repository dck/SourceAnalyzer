#!/bin/bash

# C parser generation
flex -f -o tests/cpar/clexer.cc tests/cpar/clexer.l
bison --report=none -d tests/cpar/csyntaxer.y -o tests/cpar/csyntaxer.cc

exit 0;
