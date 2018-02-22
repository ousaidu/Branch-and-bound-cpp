/**Author: Djime Gueye
 * This program reads a problem from a text file, which should be given 
 * as an argument.
 * The problem is then first solved with the branch and bound implementations 
 * then with scip.
 * first using breadth first search then 
 * minimum lower bound strategies.
 * The problem should be formulated this way.
 * min c^t*x
 * und Ax >= b
 * the format of the input should be:
 * first line: number of variables, number of binary variables number of constraints separated by space
 * seconde line:for each variable b if it is binary otherwise c then the objective coefficient separated by space
 * next number of contraints lines: Each line is a line of the matrix(in their order)each entries separated by space
 * last line: the value of the constraints
 * */
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include "var.hpp"
#include "cons.hpp"
#include "LP.hpp"
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include "scip_exception.hpp"
#include "scip/pub_message.h"
#include "branchandbound.hpp"
#include <sstream>
#include <fstream>

int main(int argc, char *argv[])
{
	//if(argc==2)
	//{
		
		std::ifstream input("exp5.txt");
		std::string line;
		int n_var;
		int n_bin;
		int n_constr;
		std::vector <double> objective;
		std::vector<double> constraints;
		std::vector<bool> binary;
		std::vector< std::vector <double> > mat;
		bool correctInput=true;
		if(input.is_open())
		{
			
			int i=1;
			//To store the line of the matrix
			//that will be filled next
			int lineMatrix = 0;
			while(getline(input, line))
			{
				
				std::istringstream iss1(line);
			    switch(i)
			    {
					//If we are in the first line
					//we read the number of variables, number of binary variables, and the number of constraints
				    case 1:
				    {
						int j=1;
						while(iss1)
						{
							std::string sub;
							iss1>>sub;
							std::istringstream iss2(sub);
							switch(j)
							{
								//read the number of variables
								case 1:
								{
									if(!(iss2>>n_var))
									{
										std::cout<<"Incorrect format of the textfile"<<std::endl;
										correctInput=false;
										}
									}
									break;
								//read the number of binary variables	
								case 2:
								{
									if(!(iss2>>n_bin))
									{
										std::cout<<"Incorrect format of the textfile"<<std::endl;
										correctInput=false;
										}
									
									}
									break;
								//read the number of constraints	
								case 3:
								{
									if(!(iss2>>n_constr))
									{
										std::cout<<"The number of constraints should be a number "<<std::endl;
										correctInput=false;
										}
									}
									break;		
								}
							
							j++;
							}
						
						
					}
				    break;
				    case 2:
				    //We are reading the coefficients of the objective function
				    {
						
						int j=1;
						while(iss1)
						{
							if(j%2!=0)
							{
								std::string sub;
								iss1>>sub;
								if(sub.compare("b")==0)
								{
									binary.push_back(true);
									}
								else if(sub.compare("c")==0)
								    binary.push_back(false);	
								
								
								}
							if(j%2==0)
							{
							    std::string sub;
						        iss1>>sub;
							    double coef=atof(sub.c_str());
							    objective.push_back(coef);
							
						    }
						    j++;	   
							}
						
					
					}
				    break;
				    default :
				    //We first read the matrix of the problem
				    //and then the constraints
				    {
						
						//We are reading the matrix
						if(i<n_constr+3)
						{
							mat.resize(lineMatrix+1);
						    mat[lineMatrix].resize(n_var);
						    int j=0;
						    while(iss1)
						    {
								
								std::string sub;
						        iss1>>sub;
								double coef=atof(sub.c_str());
								mat[lineMatrix][j]=coef;
								j++;    
							
								
								}
						    
							lineMatrix++;
							
							}
							else
							//We are done reading the matrix 
							//we are reading the constraints
							{
								
								while(iss1)
								{
									
									std::string sub;
						            iss1>>sub;
						            if(sub.compare("")!=0)
						            {
										double myConstr=atof(sub.c_str());						           									
						                constraints.push_back(myConstr);
										}						            
						                
									
									}
						
								
								}
						
						}		
					
				}
				i++;
				
				}
				
			}
		 input.close();
		 
		std::vector<int> myBinary;
		for(int i=0;i<binary.size();i++)
		{
			if(binary[i])
			    myBinary.push_back(i);
			}
	   	MBP *myProblem = new MBP(objective.size(),objective,mat,constraints,myBinary);
	   	BranchandBound *problem = new BranchandBound(myProblem,MLB);
	   	problem->setlimit(100);
	   	time_t start1 = time(0);
	   	problem->solve();
	   	time_t end1 = time(0);
	   	//solving time in seconds
       double diff1 = difftime(end1, start1);
       //std::cout<<problem->getObjVal()<<std::endl;
       problem->showOptSol();
       std::cout<<"solving time in seconds: "<<diff1<<std::endl;
       delete problem; 
	   delete myProblem;
	   std::cout<<"*******************Solving with scip******************"<<std::endl;
	   SCIP *scip;
	   std::vector<SCIP_VAR*> vars;
	   std::vector<SCIP_CONS*> cons;
	   //initialize scip
	   SCIP_CALL_EXC( SCIPcreate(& scip) );
	    // load default plugins linke separators, heuristics, etc.
       SCIP_CALL_EXC( SCIPincludeDefaultPlugins(scip) );
       // disable scip output to stdout
       SCIPmessagehdlrSetQuiet(SCIPgetMessagehdlr(scip), TRUE);
       // create an empty problem
       SCIP_CALL_EXC( SCIPcreateProb(scip, "", NULL, NULL, NULL, NULL, NULL, NULL, NULL) );

       // set the objective sense
	   SCIP_CALL_EXC( SCIPsetObjsense(scip, SCIP_OBJSENSE_MINIMIZE) );
	   //create the variables
	   for(int i=0;i<n_var;i++)
	   {
		   SCIP_VAR* var;
		   //Create the SCIP_VAR variable object
		   if(binary[i])
		   {
			   SCIP_CALL_EXC( SCIPcreateVar(scip, & var, "", 0.0, 1.0, objective[i], SCIP_VARTYPE_BINARY, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL) );
			   }
		       
		   else
		       SCIP_CALL_EXC( SCIPcreateVar(scip, & var,"", 0, SCIPinfinity(scip), objective[i], SCIP_VARTYPE_CONTINUOUS, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL) );
		   // add the SCIP_VAR object to the scip problem
           SCIP_CALL_EXC( SCIPaddVar(scip, var) );
           //Store the variable for later use
           vars.push_back(var);
              
		           
		   }
	   //create constraints
	   for(int i=0;i<constraints.size();i++)
	   {
		   SCIP_CONS* constr;
		   //Create constraint
		   SCIP_CALL_EXC( SCIPcreateConsLinear(scip, &constr, "", 0, NULL, NULL,constraints[i],SCIPinfinity(scip) ,TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE) );
		   //Add the variables to the constraint
		   for( size_t j = 0; j < n_var; ++j )
               SCIP_CALL_EXC( SCIPaddCoefLinear(scip, constr, vars[j], mat[i][j]) );
           // add the constraint to scip
           SCIP_CALL_EXC( SCIPaddCons(scip, constr) );    
           // store the constraint for later on
           cons.push_back(constr);
		   }
		//store time we started solving at
		time_t start = time(0);   
	   // this tells scip to start the solution process
       SCIP_CALL_EXC( SCIPsolve(scip) );
       //End time
       time_t end = time(0);
       //solving time in seconds
       double diff = difftime(end, start);
       // get the best found solution from scip
       SCIP_SOL * sol = SCIPgetBestSol(scip);	   
	   if(sol==NULL)
	       std::cout<<"no solution found"<<std::endl;
	   else
	   {
		   std::cout<<"Optimal objective value: "<<SCIPgetSolOrigObj(scip, sol)<<std::endl;
		   for(int i=0;i<n_var;i++)
		       std::cout<<"x_"<<i<<"= "<<SCIPgetSolVal(scip, sol, vars[i])<<std::endl;
		   
		   }
		   std::cout<<"solving time: "<<diff<<std::endl;
	   //Free allocated memory
	   //first the variables
	   for(int i=0;i<n_var;i++)
	       SCIP_CALL_EXC( SCIPreleaseVar(scip, & vars[i]) );	 
	   vars.clear();  
	   //Free constraints      
	   for(int i=0;i<n_constr;i++)
	       SCIP_CALL_EXC( SCIPreleaseCons(scip, &cons[i]) );  
	   cons.clear();
	   //free the scip problem
	   SCIP_CALL_EXC( SCIPfree(&scip) ); 
	       
	}
/*
int main()
{
	//WE
	//The matrix of the MBP
	std::vector <std::vector <double> > mat(3, std::vector <double>(6));
	double row1[6] = {-2,6,-3,4,1,-2};
	double row2[6] = {-5,-3,1,3,-2,1};
	double row3[6] = {5,-1,4,-2,2,-1};
	for(size_t i=0;i<3;i++)
	{
		switch(i)
		{
			case 0:
			{
				for(size_t j=0;j<6;j++)
				    mat[i][j] = row1[j];    
				}
				break;
			case 1:
			{
				for(size_t j=0;j<6;j++)
				    mat[i][j] = row2[j];    
				}
				break;
			case 2:
			{
				for(size_t j=0;j<6;j++)
				    mat[i][j] = row3[j];    
				}
				break;								
			}
		}
	//The 	constraints
	std::vector<double> constr;
	constr.push_back(2);
	constr.push_back(-2);
	constr.push_back(3);
	//The indices of the binary variables of the problem
	std::vector<int> binary;
	binary.push_back(0);
	binary.push_back(1);
	binary.push_back(2);
	//binary.push_back(3);
	//binary.push_back(4);
	//binary.push_back(5);
	
	std::vector<double> objective;
	objective.push_back(3);
	objective.push_back(5);
	objective.push_back(6);
	objective.push_back(9);
	objective.push_back(10);
	objective.push_back(10);
	MBP *myProblem = new MBP(6,objective,mat,constr,binary);
	//for(size_t i=0;i<3;i++)
	//{
		//for(size_t j=0;j<6;j++)
		//{
			//std::cout<<myProblem->getCoef(i,j)<<" ";
			//}
			//std::cout<<std::endl;
		//}
	//std::cout<<"Bin "<<myProblem->getNBin()<<std::endl;
	//std::cout<<"N_cons "<<myProblem->getNConstr()<<std::endl;
	//First we build the test problem and solve it with SCIP
	std::cout<<"<--------------------SOLVING WITH SCIP-------------------------------->"<<std::endl;	
	LP lp(MIN);
    lp.addVarToLP(BINARY,3);
	lp.addVarToLP(BINARY,5);
	lp.addVarToLP(BINARY,6);
	lp.addVarToLP(CONTINUOUS,9);
	lp.addVarToLP(CONTINUOUS,10);
	lp.addVarToLP(CONTINUOUS,10);
	
	std::vector<double> vrow1;
	std::vector<double> vrow2;
	std::vector<double> vrow3;
	for(size_t i=0;i<6;i++)
	{
		vrow1.push_back(row1[i]);
		vrow2.push_back(row2[i]);
		vrow3.push_back(row3[i]);
		
		}
	//Add each constraint to our LP
	//vrow1,vrow2,vrow3 are the coefficients on the row corresponding to the constraint		
	lp.addConsToLP(GREATER_EQUAL,2,vrow1);
	lp.addConsToLP(GREATER_EQUAL,-2,vrow2);
	lp.addConsToLP(GREATER_EQUAL,3,vrow3);
	lp.solve();
	lp.displaySol();
	std::cout<<"<---------------------------SOLVING WITH BRANCH AND BOUND------------------------->"<<std::endl;
    BranchandBound *problem = new BranchandBound(myProblem,MLB);
    problem->solve();
    //std::cout<<problem->getObjVal()<<std::endl;
    problem->showOptSol();
    std::cout<<"Fini"<<std::endl;
    delete problem; 
	delete myProblem;
		
}*/


/*int main()
{
	
	LP lp(MAX);
    lp.addVarToLP(BINARY,1);
	lp.addVarToLP(BINARY,4);
	lp.addVarToLP(BINARY,3);
	lp.addVarToLP(BINARY,2);
	lp.addVarToLP(BINARY,6);
	lp.addVarToLP(BINARY,2);
	lp.addVarToLP(BINARY,13);
	std::vector<double> vars_for_cons;
	int a[]={1,4,3,2,6,2,13};
	for(int i=0;i<7;i++)
	{
		vars_for_cons.push_back(a[i]);
		
		}
	
	lp.addConsToLP(LESSER_EQUAL,15.0,vars_for_cons);
	lp.solve();
	lp.displaySol();
	
	
	return 0;
	
	
}*/
