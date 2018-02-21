/* author: Djime Gueye
 * Class for the variables
 * 
 * 
*/



#ifndef VAR_H
#define VAR_H
enum VarType
{
	BINARY,INTEGER,CONTINUOUS

};

// Class for variables
class Var
{
private:
    //Type of the variable
	VarType type;
	//Index of the column of the variable in the Matrix of the LP
	int colIndex;
	//Coefficient of the variable in the objective function
	double objective;
	//Lower bound of the variable
	//double* low;
	//Upper bound of the variable
	//double *upper;
	
	
	
	
public:
    //Constructor with arguments for colIndex and type
    Var(VarType v, int col_i, double obj):type(v),colIndex(col_i),objective(obj){}
    //Copy constructor
    Var(const Var& v)
    {
		this->type = v.type;
		this->colIndex = v.colIndex;
		this->objective = v.objective;
		}
    //Getter for the ColumnIndex
    int getColIndex(){
		return colIndex;
		}
	//Getter for the type of the variable
	VarType getType(){
		return type;
		}	
	double getObjective(){ 
		return objective;
		}
};


#endif
