/**
* Author:Djime Gueye
* Class to solve an MBP using branch and bound
**/
#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H
#include <iostream>
#include "MBP.hpp"
#include "tree.hpp"
#include "node.hpp"
#include "LP.hpp"
#include <assert.h>
#include <cmath>

//Enumeration for the statuses about the solving
//NOTSOLVING:solving process not started yet
//SOLVING: In the process of solving
//FINISHED: The solving process came normally to an end
//INTERRUPTED:The solving exceeded the time limit and has been interrupted
enum MBPSolvingStatus
{
	
	NOTSOLVING,SOLVING,FINISHED,INTERUPPTED
	
	};
//Enumeration for the statuses about the solutions
//SOLVED:The problem is  solved and we have solutions
//UNSOLVED: The problem has not been solved yet
//NOSOLUTION: There are no feasible solutions
enum MBPSolutionStatus
{
	SOLVED, NOSOLUTION, UNSOLVED
	
	};
//Enumeration for the possible strategies
// to choose next candidate for branching
//BFS: Breadth first search all the leaves 
// on a same level will be branched one after another
//MLB: Minmum lower bound strategy the node with the minimum lower bound will be branched first

enum Strategy
{
	BFS, MLB
	};		

class BranchandBound
{
private:
    //Time limit after which the solving should be interuppted
    //in seconds
    double limit;
    //the Solving status
    MBPSolvingStatus solvingStatus;
    //The solution status
    MBPSolutionStatus solutionStatus;
	//MBP Problem to solve
	MBP* problem;
	//Branch and Bound tree
	BandBTree* tree;
	//best feasible solution found(upper bound)
	double *upperBound;
	//The lower bound which is the minimum value of an solution to relaxation among
	//the actual nodes
	//double lowerBound;
	//lower bound
	double lowerBound;
	//State in the problem(are we finished solving)
	bool state;
	//the strategy thet will be used for choosing candidates
	//the standard being bfs
	Strategy strategy;
	//Candidates for branching
	//std::vector<Node> candidates;
	//The time of solving in seconds
	double duration;
	
	
public:
    //Constructor with MBP instance as argument
    BranchandBound(MBP* p,Strategy strat=BFS):limit(10),solvingStatus(NOTSOLVING), solutionStatus(UNSOLVED),problem(p),tree(new BandBTree(p->getNvar())),upperBound(NULL),state(false),strategy(strat)
    {}
    //Destructor
    ~BranchandBound()
    {
		
		delete tree;
		delete upperBound;
		}
    //Copy a LP problem
    LP* copyLP(LP*);
    //solve the MBP
    void solve();
    //get the value of the objective function for the optimal solution
    double getObjVal();
    //get the optimality gap
    double getOptGap()
    {
		double optimalValue=tree->getBestNode()->getLp()->getObjectiveOpt();
		assert(optimalValue!=0);
		return (std::fabs(optimalValue-this->lowerBound))/(optimalValue);
		}
    //Print the optimal solution
    void showOptSol()
    {
		if(tree->getBestNode()==NULL)
		    std::cout<<"The problem has no solutions"<<std::endl;
		else
		    tree->getBestNode()->getLp()->displaySol();
		if(solvingStatus==INTERUPPTED)
		    std::cout<<"The solving process heas been interrupted because the time limit ";
		    std::cout<<"has been exceeded"<<std::endl;   
		
		}
	//	
    //test the feasibility of an optimal solution of a subproblem
	bool isFeasible(Node* n)
	{
		bool feasible=true;
		for(size_t i=0;i<problem->getNvar();i++)
		{
			
			if(problem->varIsBinary(i))
			{
				double val = n->getLp()->getVal(i);
				if((0!=val && 1!=val))
				{
					feasible = false;
					break;
					}
				
				}
			
			}
		return feasible;
		}	
	//Branch operation
	//Node*: subproblem to branch
	//int: index of the variable we are branching on
	//int: value of the variable in the new constraint(in the case of mbp 0 or 1)
	//return the corresponding subproblem
	Node* branch(Node*,int,int);
	//bound operation 	
	void bound(Node*);
	//Create a copy of an LP 
	//to which we can add new constraints and construct a new sub problem	
	LP* copyLP(LP);	
    //Get the index of the var that we are branching on on this Node
    //Not necessary yet
    //int getVarToBranch(Node*);
    //Set the time limit
    void setlimit(double l)
    {
		limit = l;
		}
	//Get the time limit
	double getLimit()
	{
		return limit;
		}
	//Get the variable that will be branched on next
	int getVarToBranch(Node*);
	//Add candidate in BFS
	//Get the solving time
	double getDuration()
	{
		if(solvingStatus==FINISHED||solvingStatus==INTERUPPTED)
		    return duration;
		else
		    return -1;    
		}
		double getLowerBound()
		{
			return this->lowerBound;
			}
		
};
#endif
