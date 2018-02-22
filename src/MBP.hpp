/*
 * Author:Djime Gueye
 * Class to represent an MBP to give to the solver (branch and bound)
 * min c^tx
 * st Ax>=b
 * x>=0
 * 
 */
#ifndef MBP_H 
#define MBP_H
#include <iostream>
#include <vector>
#include <tree.hpp>

class MBP
{
 private:
  
	//Number of variables 
	int n_var;
	//Objective function the vector c
	std::vector<double> objective;
	
	//The A matrix
	std::vector <std::vector<double> > mat;
	
	//The vector of the constraints, b
	std::vector<double> constr;
	
	//says if the variable x_i is binary 
	std::vector<bool> binary;
	
	//Indexes of binary variables
	std::vector<int> bin_vars;
	
public:
    
     //Constructor:
     //sets the attributes:n_var, objective, mat,constr and binary
     //
     
      MBP(int n, std::vector <double>& o, std::vector <std::vector<double> >& m, std::vector <double>& c, std::vector <int>& b):
     n_var(n), objective(o), mat(m), constr(c), bin_vars(b){
		 binary.resize(n_var, false);
		 for(size_t i=0;i<b.size();i++)
		 {
			binary[b[i]] = true; 
			 }
		 }
     //Get coefficient of the variable on the objective function given the index of the variable
    double getObjective(int varIndex)
    {
		return objective[varIndex]; 
		}
	//Get the constraint given the row index
	double getConstraints(int rowIndex)
	{
		return constr[rowIndex];
		}
	//Get element of the matrix of the MBP given the row and column
	double getCoef(int row, int col)
	{
		return mat[row][col];
		}
	int getNvar()
	{
		return n_var;
		}
	//given an index check if the variable is binary
	bool varIsBinary(int index)
	{
		return binary[index];
		}
	//Get the number of constraints	
	int getNConstr()
	{
		return constr.size();
		}
	//Get the number of binary variables
	int getNBin()
	{
		return bin_vars.size();
		}
	//Get the indexes of the binary variables
	std::vector<int> getBinVars()
	{
		return bin_vars;
		}
	//Get the global index of a binary variable given its index	
	//in the list of binary variables
	int getBinVar(int ind)
	{
		 return bin_vars[ind];
		}			
	//Get the number of constraints
	int getNConstraints()
	{
		return mat.size();
		}
	
	};
#endif
