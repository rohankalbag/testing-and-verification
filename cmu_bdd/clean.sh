#!/bin/bash
CWD=$(pwd)
cd bddlib
rm -f *.o
rm -f dependencies
cd $CWD/mem
rm -f *.o
rm -f dependencies
cd $CWD/boole
rm -f *.o
rm -f dependencies
cd $CWD/trlib
rm -f *.o
rm -f dependencies
cd $CWD
rm -f bin/*
rm -f lib/*.a
