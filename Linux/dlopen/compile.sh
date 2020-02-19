#!/bin/bash
g++ -shared -fPIC -o libadd.so add.cpp
echo ===============================
g++ -o main main.cpp -ldl
