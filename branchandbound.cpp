/**
* Author:Djime Gueye
*Implementation of the branch and bound class
**/


#include "branchandbound.hpp"
#include <algorithm>
#include <time.h>
#include <cmath>
LP* BranchandBound::copyLP(LP* l)
{
	
	LP *copy;
	copy = new LP(MIN);
	//Add the variables to the LP
	for(size_t i=0;i<l->getNvars();i++)
	{
		if(problem->varIsBinary(i))
		{
			copy->addBinaryRelaxedVarToLP(problem->getObjective(i));
			
			}
		else
		    copy->addVarToLP(CONTINUOUS,problem->getObjective(i));	
		
		} 
	//Add the constraints
	for(size_t i=0;i<l->getNCons();i++)
	{
		std::vector<double> coefs;
		for(size_t j=0;j<l->getNvars();j++)
			coefs.push_back(l->getElementMat(i,j));

		copy->addConsToLP(l->getConsType(i), l->getValCons(i), coefs);

		}
		return copy;
	}
	
//Divide a problem in subproblems
//Node* n:The problem to be divided
//int i: the index of the variable that will be branched on
//int j: Value of the variable(supplementary constraint)
Node* BranchandBound::branch(Node* n, int i, int j)
{
	if(n->getNodeStatus()!=NODE_UNSOLVED && (n->isPruned()==false))
	{
		Node* right;
		Node* left;
		std::vector<double> temp;
		temp.resize(this->problem->getNvar());
		temp[i]=1.0;
		//Create right subproblem with additional constraint that the given variable 
		//is equal to 1
		if(j==1)
		{
		    right = new Node(n, NULL,NULL);
		    n->setRightChild(right);
		    right->setLp(this->copyLP(n->getLp()));
		    right->getLp()->addConsToLP(EQUAL, 1.0,temp);
		}
		//Create left subproblem with additional constraint that the given variable 
		//is equal to 0		
		else if(j==0)
		{ 
		    left = new Node(n, NULL,NULL);
		    n->setLeftChild(left);
		    left->setLp(this->copyLP(n->getLp()));
	        left->getLp()->addConsToLP(EQUAL, 0.0,temp);
	    }
	    //if(n->getLeftChild()!=NULL && n->getRightChild()!=NULL)    
	        //n->setProcessed(true);
	    
	    return (j==0)?left:right;
		}
	else
        return NULL;
	}	
//Solve the relaxation, determine the bound and prune (if relevant) 	
void BranchandBound::bound(Node* n)
{
	//Solve the relaxation
	n->solve();
	
	NodeStatus s = n->getNodeStatus();
	if(s==NODE_SOLVED)
	{
		double opt = n->getLp()->getObjectiveOpt();
		//std::cout<<"Node: lowerbound: "<<opt<<" ";
		//set the (local) lower bound
		n->setLowerBound(opt);
		if(upperBound!=NULL)
		{	 //if the lower bound solution of this subproblem is greater equal to the global upper bound
			 //we prune the node.   
             if(n->getLowerBound()>=*(this->upperBound))
             {
				 n->prune();
				 std::cout<<"worse than global upper bound: node pruned"<<std::endl;
				 std::cout<<"upperBound is: "<<*(this->upperBound)<<std::endl; 
				 }
                 
             if(this->isFeasible(n))
             {
			      //if the optimal solution of the subproblem is feasible and less than the upper bound
			      //we prune the node.And its objective value is the new upper bound
			     
				  if(opt < *(this->upperBound))
				  {
					    delete upperBound; 
						upperBound = new double(opt);
						tree->setBestNode(n);
						std::cout<<"new best solution:"<<std::endl;
						n->getLp()->displaySol();
						std::cout<<"The new optimality gap is "<<this->getOptGap()<<std::endl;
						
				  }
				  n->prune();
				  
					    
			 }    
					        
						
		}
		//If the upper bound was previously infinity(Not set)
		else
		{
			if(this->isFeasible(n))
			{
				upperBound = new double(opt);
				tree->setBestNode(n);
				n->prune();
				
				
				
				}
			
			}
			
	}
	// there are no solutions prune the node
	else
	{
        n->prune();
	}
	
	
}	

void BranchandBound::solve()
{
	//store start time
    time_t start = time(0);
	solvingStatus = SOLVING;
	//Relax binary variables and solve the LP
	LP *lp = new LP(MIN);
	int n = problem->getNvar();
	//Add the variables to the LP
	for(size_t i=0;i<n;i++)
	{
		if(problem->varIsBinary(i))
		    lp->addBinaryRelaxedVarToLP(problem->getObjective(i));
		else
		    lp->addVarToLP(CONTINUOUS, problem->getObjective(i));    
		 
		
		}
	//Add the constraints
	//And the variables to the respective constraint
	n = problem->getNConstr();
	int m = problem->getNvar();
	//For each constraint 
	//We create a list consisting of the index of each variable and the coefficient of that variable
	for(size_t i=0;i<n;i++)
	{
		//Store the coefficients for each row
		std::vector<double> coefs;
		for(size_t j=0;j<m;j++)
		{
			coefs.push_back(problem->getCoef(i,j));
			}
		lp->addConsToLP(GREATER_EQUAL, problem->getConstraints(i), coefs);
		
		}
	//Set the root of the branch and bound tree	
	Node * root = new Node(NULL,NULL,NULL);
	tree->setRoot(root);
	root->setLp(lp);	
	root->solve();
	//Check if there are solutions
	LPSolvingStatus lpStatus = root->getLp()->getLPSolvingStatus();
	//If there are no solutions to the root problem
	//the MBP has no solution.
	//The solving process is finished
	if(lpStatus==LP_NOSOLUTION)
	{
		solutionStatus = NOSOLUTION;
		root->prune();
		}
	//Check if the solution in the root node is feasible	
	else
	{
		//Set the lower bound of the problem
	     root->setLowerBound(root->getLp()->getObjectiveOpt());
	     //Set the lowest lower bound of the tree
	     lowerBound = root->getLp()->getObjectiveOpt();
		//If the solution is feasible
		//it is the optimal solution
		if(this->isFeasible(root))
		{
			tree->setBestNode(root);
			upperBound = new double(root->getLowerBound());
			solvingStatus = FINISHED;
			solutionStatus = SOLVED;
			root->prune();
			std::cout<<"new best solution: "<<std::endl;
			root->getLp()->displaySol();
			std::cout<<"optimality gap"<<this->getOptGap()<<std::endl;
			}
		//Otherwise we build the subproblems
		else
		{   
			switch(strategy)
			{
				//Solve using BFS
				case BFS:
				{
					std::vector<Node*> candidates;
					std::vector<int> bVar = problem->getBinVars();
					candidates.push_back(root);
					for(size_t i=0;i<bVar.size();i++)
					{
						int s = candidates.size();
						std::vector<Node*> newCandidates;
						Node *left=NULL;
						Node *right=NULL;
						for(size_t j =0;j<s;j++)
						{
							left = this->branch(candidates[j], bVar[i], 0);
							right = this->branch(candidates[j], bVar[i], 1);
							this->bound(left);
							this->bound(right);
							if(left->getLowerBound()<this->lowerBound)
							{
								this->lowerBound=left->getLowerBound();
								
								}
							    
							if(right->getLowerBound()<this->lowerBound)
							{
								this->lowerBound=right->getLowerBound();
								
								
								}
							    
							if(i == (bVar.size()-1))
							{
								left->prune();
								right->prune();
								}
							if(left->isPruned()==false)
							    newCandidates.push_back(left);
							if(right->isPruned()==false)
							    newCandidates.push_back(right);    
						    if(difftime(time(0), start)>=limit)
					        {
						         solvingStatus = INTERUPPTED;
						             break;
						     }    	
								        
							    
							        
							}
							//Prune relevant nodes if a  new best feasible solution has been found
							if(tree->getBestNode()==left)
							{
								std::vector<Node*>::iterator it = newCandidates.begin();
								while(it!=newCandidates.end())
								{
									if((*it)->getOptObjFunct()>left->getLowerBound())
									    it = newCandidates.erase(it);
									    
									it++;    
									    
									}
								}
							//Prune relevant nodes if a feasible solution has been found	
							if(tree->getBestNode()==right)
							{
								std::vector<Node*>::iterator it = newCandidates.begin();
								while(it!=newCandidates.end())
								{
									if((*it)->getOptObjFunct()>right->getLowerBound())
									    it = newCandidates.erase(it);
									 it++;   
									}
								
								}	
					    if(solvingStatus==INTERUPPTED)
					        break;
					    candidates=newCandidates;    
						
						
						}
					
					
						}
			    break;
			    //Solve using minimum lower bound
				case MLB:
				{
					//The elements of the candidatelist will be sorted 
					//according to their lower bound in increasing order
					std::vector<Node*> candidates;
					candidates.push_back(root);
					while(candidates.size()!=0)
					{
						Node* left = NULL;
						Node *right =NULL;
						//get the index of the variable we are going to branch on next
						int i_var = this->getVarToBranch(candidates[0]);
						if(i_var!=-1)
						{
							left = this->branch(candidates[0], i_var, 0);
							right = this->branch(candidates[0], i_var,1);
							candidates.erase(candidates.begin());
							this->bound(left);
							this->bound(right);
						    //Prune relevant nodes if a  new best feasible solution has been found
							if(tree->getBestNode()==right)
							{
								std::vector<Node*>::iterator it = candidates.begin();
								while(it!=candidates.end())
								{
									if((*it)->getOptObjFunct()>right->getLowerBound())
									    it = candidates.erase(it);
									 it++;   
									}
								
								}
							//Prune relevant nodes if a  new best feasible solution has been found
							if(tree->getBestNode()==left)
							{
								std::vector<Node*>::iterator it = candidates.begin();
								while(it!=candidates.end())
								{
									if((*it)->getOptObjFunct()>left->getLowerBound())
									    it = candidates.erase(it);
									    
									it++;    
									    
									}
								}													
							if(left->getLowerBound()<this->lowerBound)
							    this->lowerBound=left->getLowerBound();
							if(right->getLowerBound()<this->lowerBound)
							    this->lowerBound=right->getLowerBound();
							if(left->isPruned()==false)
							    candidates.push_back(left);
						    if(right->isPruned()==false)
					            candidates.push_back(right);  
					        //Sort the candidates for branching ascendantly on their lower bound
					        std::sort(candidates.begin(),candidates.end(),Node::compare);       
							    
							}
									
							
						else
						{
							candidates[0]->prune();
							}	
						if(difftime(time(0), start)>=limit)
					    {
						    solvingStatus = INTERUPPTED;
						    break;
						  }
						 
						      	
						
					}
					
					
					}
				break;
		
				}
			
			      
		}	
	}
	if(solvingStatus!=INTERUPPTED)
	    solvingStatus = FINISHED;
	//state = true;
	this->duration = difftime(time(0), start);
}
double BranchandBound::getObjVal()
{
	return tree->getBestNode()->getLp()->getObjectiveOpt();
	}
//Get the index of the next variable 
//We assume that the variable are branched in every path
//in the order they are in the list of the binary variables.
//This way the index of the next variable to be branched on a node
//is the number of constraints of the subproblem
// minus the number of constraints of the initial problem
//If we have already branched all the variables return -1
//

int BranchandBound::getVarToBranch(Node* n)
{
	int index = n->getLp()->getNCons()-this->problem->getNConstraints();
	
	return (index==problem->getNBin()-1)?-1:problem->getBinVar(index);
	
	}
