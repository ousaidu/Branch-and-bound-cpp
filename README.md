# Branch-and-bound-cpp
This is an implementation of branch and bound in C++ for solving 0-1 Integer programming as part of an advanced software practical at the heidelberg university in the winter semester 2017-2018.
The SCIP library is used to solve the Linear Relaxation.
More about SCIP on:
http://scip.zib.de/
# Problem Formulation:
minimize c^Tx
under
    Ax>=b
    x_i >=0 for all i
    and x_i in {0,1} for x = 1,...,p
# Dependencies
GMP:
The Scip library:
http://scip.zib.de/
plesase install version 4.x.x 
 
    
