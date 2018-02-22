# Branch-and-bound-cpp
This is an implementation of branch and bound in C++ for solving 0-1 Integer programming as part of an advanced software practical at the heidelberg university in the winter semester 2017-2018.
The SCIP library is used to solve the Linear Relaxation.
More about SCIP on:
http://scip.zib.de/
# Problem Formulation:
minimize c^Tx
under
    Ax<=b
    and x_i in {0,1} for x=1...m
# Implementation:
For the implementation two strategies for choosing next branching candidates 
are possible.
 # Breadth First Search
 # Minimum Lower Bound(choose the node with the lowest lower bound)
 
 
    
