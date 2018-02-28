# Branch-and-bound-cpp
This is an implementation of branch and bound in C++ for solving 0-1 Integer programming as part of an advanced software practical at the heidelberg university in the winter semester 2017-2018.
The SCIP library is used to solve the Linear Relaxation.
More about SCIP on:
http://scip.zib.de/
# Problem Formulation:
minimize c<sup>T</sup>x
under
    Ax &ge; b
     x &ge; 0 
    and x<sub>i</sub> in {0,1} for i = 1,...,p
# Dependencies
GMP:
The Scip library:
http://scip.zib.de/ 
please install version 4.x.x 
# Problem Instance
The 0-1 Integer program to solve should be specified in a text file with following format.
First line: number of variables, number of binary variables and the number of  constraints separated by space

Second line: For each variable the letter b if the variable is binary otherwise c then space then the objective of the variable
All entries are separated by space.

next lines until penultimate:each lines is a line of the A matrix of the problem.Each entry is followed by space.

last line: The constraint vector. each entry followed by space.
# Solving the problem instance
The location of the text file of the problem instance should be given in the line 36 of the main(in the file test.cpp).
## Branching strategies
This implementation permits to choose between a breadth first search strategy and minimum lower bound strategy.
# Running the code
Before running make sure to have cloned or downloaded the repository in the scip opt suite root directory.
## Edit the makefile
Make sure that SCIPDIR in the makefile is the location of scip.
Run this command in terminal after editing the makefile.
```
make depend
```
 # Compiling and running
 Inside de Cloned repository compile with:
 ```
 make
 ```
Then run the linked binary whose name should be output on the terminal.
For example
```
 bin/LP.linux.x86_64.gnu.opt.spx2
 ```
