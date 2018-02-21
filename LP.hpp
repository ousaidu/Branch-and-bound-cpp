/*
 * Author:Djime Gueye
 * Class to represent a LP to give to the solver
 * cx >=b
 * 
 */
#ifndef LP_H
#define LP_H
#include <iostream>
#include "var.hpp"
#include <vector>
#include "cons.hpp"
#include <scip/scip.h>

//The status of the LP
//Unsolved:not solved yet
//Solved: The Lp has been solved and there are solutions
//no  solutions
enum LPSolvingStatus
{
	LP_UNSOLVED,LP_SOLVED,LP_NOSOLUTION
	};
enum ObjSense
{
	MIN,MAX
	};
	



class LP
{
private:


        
    //Number of continuous variables
    size_t n_vars;
    
    //Number of constraints of the LP
    size_t n_cons;
    
    //Variables of the LP
    std::vector<SCIP_VAR*> vars;
    
    //Constraints of the LP
    std::vector<SCIP_CONS*> constraints;
    
    //Objective Sense of the LP
    ObjSense objSense;
    
    //Solution of the LP with sol[i] being the value of the i.th variable 
    std::vector <double> sol;
    
    //pointer to scip structure
    SCIP* scip;
    
    //Objective value of the optimal solution
    double objectiveOpt;
    
    //Status of the solving
    LPSolvingStatus status;
    
    //Has the problem already been built
    //Constraints and objective function have been set to the existing variables
    bool built;
    
    //Matrix c of the LP
    std::vector< std::vector <double> > mat;
    //Constraints of the LP
    //Here we will store informations about the constraint
    //That we will be able to check 
    std::vector<Cons*> cons;
    
    
     
public:
    //Set the objective sense of the LP
    void setObjSense(ObjSense);
    
    // constructor
    LP(ObjSense);
    
    //Copy an LP
    LP(const LP&);
   
    //Destructor
    ~LP();
    
    //Add new variable to the LP
    //VarType : the type of the variable
    //double : coefficient of the variable in the objective function
    void addVarToLP(VarType, double );
    //Add new variable to LP with the variable in [0,1]
    void addBinaryRelaxedVarToLP(double);
    
    //Add a constraaint to the LP
    //cType: The type of the constraint
    //val: the value of the constraint
    //coef: List with the coefficient of each variable identified with its index	
    void addConsToLP(ConsType, double, std::vector<double>& );
    
    //solve
    void solve();
    
    //Show the solution
    void displaySol();
	
	//Get the value of the variable x_i in the solution
	//before calling this test if solutions exist
	double getVal(int i)
	{
		return sol[i];
		} 
	double getObjectiveOpt()
	{
		return objectiveOpt;
		}
	//get the number of variables
	int getNvars()
	{
		return this->n_vars;
		}
	//Get an element of the matrix of the LP
	double getElementMat(int row, int col)
	{
		return mat[row][col];
		};
	int getNCons()
	{
		return n_cons;
		}
	//Get the type of a constraint identified by its index
	ConsType getConsType(int i)
	{
		return cons[i]->getType();
		}
	
	//get the solving status
	LPSolvingStatus getLPSolvingStatus()
	{
		return status;
		}
	//Get value of a constraint given its index
	double getValCons(int i)
	{
		return cons[i]->getValue();
		}			 				
};




















#endif
