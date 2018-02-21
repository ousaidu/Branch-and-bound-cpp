/*
 * Author:Djime Gueye
 * Class to represent the B&B tree
 * 
 */
#ifndef TREE_H
#define TREE_H
#include "node.hpp"
#include "LP.hpp"
#include <iostream>
#include <vector>
#include <stack>

class BandBTree
{
private:
	//Root of the tree
	Node *root;
	//Actual number of nodes of the tree
	int n_var;
	//Nodes to be processsed
	//std::vector <Node*> candidates;
	//Pointer to node with best value
	Node* bestNode;
public:
    //Constructor with argument the number of variables
    BandBTree(int n):root(NULL),n_var(n),bestNode(NULL){}
    //Constructor with argument root node and the number of variables
    BandBTree(Node* r, int n):root(r), n_var(n),bestNode(NULL){}
    //destructor
    ~BandBTree()
    {
		delete root;
		}
    Node* getRoot(){
		return root;
		}
	double getBestSol(){
		return bestNode->getLp()->getObjectiveOpt();
	}
	//Set the root of the tree
	void setRoot(Node* r)
	{
		root = r;
		}
	//Get the next candidate for bounding
	/*Node* getNextCandidate()
	{
		if(candidates.size()!=0)
		{
			Node* temp = candidates[candidates.size()-1]
		    return temp;
			}
		else
		    return NULL;	
		
		}*/
	//Remove from the candidate queue the node at the end	
	/*void removeCand()
	{
		candidates.pop_back();
		}		
	void addCandidate(Node* n)
	{
		candidates.push_back(n);
		}*/
		
	//Set the node with the best solution found yet
	void setBestNode(Node* n)
	{
		bestNode = n;
		}
	//Get a pointer to the best Node found yet
	Node* getBestNode()
	{
		return bestNode;
		}	
	
	
	
	
};
#endif
