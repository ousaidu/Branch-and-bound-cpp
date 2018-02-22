/**
* Author:Djime Gueye
* Class for the nodes of the branch and bound tree
**/
#ifndef NODE_H
#define NODE_H
#include <iostream>
#include "LP.hpp"
//The status of the node
//solved: this node's LP has been solved
//unsolved:this node's LP has not been solved yet 
enum NodeStatus
{
	NODE_SOLVED, NODE_UNSOLVED,NODE_NOSOLUTION
	};
class Node
{
private:
    //Value of the objective function of the subproblem associated to the node
    //Which is the solution of the relaxed problem
    double optObjFunct;
    //Subproblem associated to this node 
    LP* lp;
    //lower bound 
	double lowerBound;
	//Father of the node
	Node* father;
	//Left child
	Node *left;
	//Right child
	Node* right;
	//Is this node pruned
	bool pruned;
	//Are we done with this node
	bool processed;
	//Index of variables to branch yet 
	//New vars will be added at the end so that the 
	//Newest variable branched will be at the end
	//std::vector<int> varsBranched;
	//Variables that have yet to be branched on
	//std::vector<int> varsToBranch;
	//Values of the variable, NULL if we have not branched on the variable
	//std::vector<double> vars;
	//To store the Values of the variables in a solution
	std::vector<double> sol;
	//the status of the node
	NodeStatus status;
	
	
public:
    //Constructor Node *f:father node, Node* l: left child node, 
    //Node* r: right child
    
    Node(Node* f, Node *l, Node* r):father(f),left(l),right(r),pruned(false),processed(false),status(NODE_UNSOLVED)
    {}
    //Destructor
    ~Node()
    {
		
		delete left;
		delete right;
		}
		
    //Number of variables on which we already branched in this node
    /*int getNvarBranched()
    {
		return varsBranched.size();
		}*/	
	//prune the node
	void prune()
	{
		pruned = true;
		}
	//Pointer to the Lp of this node	
	LP* getLp()
	{
		return lp;
		}
	//Set the subproblem of the node
	void setLp(LP* l)
	{
		this->lp = l;
		}
	//Solve the LP on this node	
	void solve()
	{
		lp->solve();
		
		LPSolvingStatus s = lp->getLPSolvingStatus();
		if(s==LP_NOSOLUTION)
			status=NODE_NOSOLUTION;
		else
		{
			status = NODE_SOLVED;
			lowerBound = lp->getObjectiveOpt();
			for(size_t i=0;i<lp->getNvars();i++)
		        sol.push_back(lp->getVal(i));
			
		} 
		    	
	}
	//Add a variable to be branched at the end of the queue	
	/*void addVarToVarsBranched(int v)
	{
		varsBranched.push_back(v);
		}*/
	/*void setVarsToBranch(std::vector<int> &v)
	{
		varsToBranch = v;
		}*/
	//Add the index of a variable to branch to the list of variables to be branched on	
	/*void addVarToBranch(int var)
	{
		varsToBranch.push_back(var);
		}*/ 
	//Get the status of the node
	NodeStatus getNodeStatus()
	{
		return status;
		}
	//SEt the status of the node
	void setStatus(NodeStatus n)
	{
		status = n;
		}
	//Get if the node has been pruned
	bool isPruned()
	{
		return this->pruned;
		}
	//Set the lower bound of this subproblem					
	void setLowerBound(double lB)
	{
		lowerBound = lB;
		}
	//Get the lower bound of the subproblem	
	double getLowerBound()
	{
		return lowerBound;
		}
	//Get pointer to the left child
	Node* getLeftChild()
	{
		return this->left;
		}
	//Get pointer to the left child
	Node* getRightChild()
	{
		return this->right;
		}
	void setRightChild(Node*n)
	{
		right = n;
		}
	void setLeftChild(Node*n)
	{
		left = n;
		}	
	//< Operator to sort the nodes according to their lower bound
	//Node a<Node b <-> a.lowerBound < b.lowerBound
	bool operator<(const Node &other) const
	{
		return (this->lowerBound < other.lowerBound);
		}			
	//Method to compare Node pointers, will
	//be given as argument to the function std::sort from
	//<algorithm>
	static bool compare(const Node* n1, const Node* n2)
	{
		return *n1 < *n2;
		}
	bool isProcessed()
	{
		return processed;
		}
	//Get the optimal objective value of the relaxation	
	double getOptObjFunct()
	{
		return optObjFunct;
		}									
};
#endif
