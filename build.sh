!/bin/bash
cd src
g++ -o ../bin/main main.cc operator.cc parser.cc scanner.cc token.cc -Wall
cd ..