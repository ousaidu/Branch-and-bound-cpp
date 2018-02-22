/*author:Djime Gueye
 * Class for the constraints
 * 
 */

#ifndef CONS_H
#define CONS_H
#include <iostream>
#include <vector>
#include "var.hpp"

//Constraint types
enum ConsType
{
	EQUAL,LESSER_EQUAL,GREATER_EQUAL

	};
//Class for constraints
class Cons
{
private:	
	//Type of the Constraint
	ConsType type;
	//Value of constraint
	double value;
	//Index of the row of this constraint
	int rowIndex;
	//Variables attached to this constraint
	//std::vector <Var*> vars;
	//number of variables to the constraint
	//int nVar;
	

public:
    //Constructor with variables attached tothe constraint
    /*Cons(ConsType c_t, double v, int r_i, std::vector <Var*> vars_):type(c_t),value(v),row_index(r_i){
		vars.resize(vars_.size());
		for(int i=0;i<vars_.size();i++)
		    vars[i] = vars_[i];
		    
		}*/
	//Constructor without variables attached tothe constraint
	Cons(ConsType c_t, double v, int r_i):type(c_t),value(v),rowIndex(r_i)
	{}
	//Constructor tht does not set the row_index
	Cons(ConsType c_t, double v):type(c_t),value(v)
	{}	
	//Copy constructor
	Cons(const Cons& c)
	{
		this->type = c.type;
		this->value = c.value;
		this->rowIndex = c.rowIndex;
		
		
		}	
    //Getter for the attribute type
    ConsType getType(){
		return type;
		}
	//Getter for attribute value
	double getValue(){
		return value;	
	}
	//Getter for attribute rowIndex
	int getRowIndex(){
	    return rowIndex;
	}
	
};
























































#endif
