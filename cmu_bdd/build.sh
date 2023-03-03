#!/bin/bash
CWD=$(pwd)
cd bddlib
make 
cd $CWD/mem
make
cd $CWD/boole
make 
cd $CWD/trlib
make
cd $CWD
