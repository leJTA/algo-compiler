# algo_compiler
An algorithm compiler

#Requirement
boost.spirit include files

#How to compile?
run : 
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

#How to run
cd bin/Release
./algo_compiler <file_name>
