#!/bin/bash
cd src
cd MembraneBuilder
g++ -c -O3 *.cpp
g++ -o PCG *.o
rm *.o
mv PCG ../../
cd ../
cd Pointillism
g++ -c -O3 *.cpp
g++ -o PLM *.o
rm *.o
mv PLM ../../
cd ..

