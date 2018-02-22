#include "LP.hpp"
#include <sstream>
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include "scip_exception.hpp"
#include "scip/pub_message.h"
#import <assert.h> 
//Empty Constructor
LP::LP(ObjSense o=MIN)
{
	// initialize scip
    SCIP_CALL_EXC(SCIPcreate(& scip));
    
    // load default plugins linke separators, heuristics, etc.
   SCIP_CALL_EXC( SCIPincludeDefaultPlugins(scip) );

   // disable scip output to stdout
   SCIPmessagehdlrSetQuiet(SCIPgetMessagehdlr(scip), TRUE);

   // create an empty problem
   SCIP_CALL_EXC( SCIPcreateProb(scip, "lp", NULL, NULL, NULL, NULL, NULL, NULL, NULL) );
   objSense=o;
   if(objSense==MAX)
       SCIP_CALL_EXC(SCIPsetObjsense(scip, SCIP_OBJSENSE_MAXIMIZE));
   
    //default objective sense
    //objSense=MIN;
    
    //Set the number of variables and constraints
	n_cons=0;
	n_vars=0;
	//Set that the problem has not been built yet
	built = false;
	//
	status = LP_UNSOLVED;
	}
LP::~LP()
{
	//Release scip environment	
	//Release variables	
	for(size_t i=0;i<vars.size();i++)
		SCIP_CALL_EXC( SCIPreleaseVar(scip, & vars[i]));
    vars.clear();	
	//Release constraints
	for( size_t i = 0; i < constraints.size(); ++i ) 
        SCIP_CALL_EXC( SCIPreleaseCons(scip, &constraints[i]) ); 
    constraints.clear();
    // after releasing all vars and cons we can free the scip problem
    // remember this has allways to be the last call to scip
    SCIP_CALL_EXC( SCIPfree(&scip) ); 
    //Delete the constraints
    for(size_t i=0;i<cons.size();i++)
        delete cons[i];    
	
	}
//Copy Constructor
LP::LP(const LP& lp)
{
	
	}	
//Set the objective sense of the LP
void LP::setObjSense(ObjSense o_s)
{
	this->objSense = o_s;
	}	


//Add Variable to the LP
//and add the constraint that the variable is >=0	
void LP::addVarToLP(VarType vType, double objective)
{
	SCIP_VAR* scip_var;
	
	switch(vType)
	{
		case BINARY:
		{
	        // create  the variable to the problem
            SCIP_CALL_EXC( SCIPcreateVar(scip, & scip_var,"", 0.0, 1.0, objective, SCIP_VARTYPE_BINARY, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL) );
            
           // add the variable to the problem
           SCIP_CALL_EXC( SCIPaddVar(scip, scip_var) );
           
           //Store the variable for later access
           vars.push_back(scip_var);
           //Create the new constraint
           
			}
			break;
		case INTEGER:
		{
			
			// create  the variable to the problem suppose there are no supplementary conditions on the variable
             SCIP_CALL_EXC( SCIPcreateVar(scip, & scip_var,"", 0, SCIPinfinity(scip), objective, SCIP_VARTYPE_INTEGER, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL) );
             
           // add the variable to the problem
           SCIP_CALL_EXC( SCIPaddVar(scip, scip_var) );
           
           //Store the variable for later access
           vars.push_back(scip_var);
           
             
			
			}
			break;
		case CONTINUOUS:
		{
			// create  the variable to the problem and suppose there are no supplementary conditions to the variable
             SCIP_CALL_EXC( SCIPcreateVar(scip, & scip_var,"", 0, SCIPinfinity(scip), objective, SCIP_VARTYPE_CONTINUOUS, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL) );
             
             // add the variable to the problem
             SCIP_CALL_EXC( SCIPaddVar(scip, scip_var) );
             
             //Store the variable for later access
             vars.push_back(scip_var);
			}
			break;		
		}
		//Update the number of variables of the problem
		n_vars++;
		//After adding the new variable constraint have yet to be defined
		built = false;	
		
	}
void LP::addBinaryRelaxedVarToLP(double objective)
{
	SCIP_VAR* scip_var;
	
	// create  the variable
    SCIP_CALL_EXC( SCIPcreateVar(scip, & scip_var,"", 0.0, 1.0, objective, SCIP_VARTYPE_CONTINUOUS, TRUE, FALSE, NULL, NULL, NULL, NULL, NULL) );
	
	// add the variable to the problem
    SCIP_CALL_EXC( SCIPaddVar(scip, scip_var) );
    
    //Store the variable for later access
    vars.push_back(scip_var);
    //Update the number of variables of the LP
    n_vars++;
    //After adding the new variable constraint have yet to be defined
    built = false;
	
	
	}	

//Add a constraint to the LP
//cType: The type of the constraint
//val: the value of the constraint
//coef: List with the coefficient of each variable identified with its index	
void LP::addConsToLP(ConsType cType, double val, std::vector<double>& coef)
{
	assert(coef.size()==n_vars);
	//n_cons++;
	SCIP_CONS* scip_cons;
	mat.resize(n_cons+1);
	for(size_t i=0;i<(n_cons+1);i++)
	    mat[i].resize(n_vars);
	
	switch (cType)
	{
		case LESSER_EQUAL:
		{
			
			//Create constraint
			SCIP_CALL_EXC(SCIPcreateConsLinear(scip, &scip_cons,"",0,NULL,NULL,-SCIPinfinity(scip),val, TRUE,TRUE,TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE,FALSE));
			
			        
			//Add scip variable to the scip constraint
			for(size_t i=0;i<coef.size();i++)
			{
				SCIP_CALL_EXC( SCIPaddCoefLinear(scip, scip_cons, vars[i], coef[i]));
				mat[n_cons][i]=coef[i];
			}
			//add the constraint to the problem
			SCIP_CALL_EXC( SCIPaddCons(scip, scip_cons));
			// store the constraint for later access
            constraints.push_back(scip_cons);
			//Create constraint
			//Add new constraint to the constraint list
			Cons *temp = new Cons(LESSER_EQUAL, val, n_cons);
			cons.push_back(temp); 	
				
			
			}
			break;
		case GREATER_EQUAL:
		{
			//Create constraint
		    SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & scip_cons, "", 0, NULL, NULL,val,SCIPinfinity(scip) ,TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE) );
		    //Add scip variable to the scip constraint
		    for(size_t i=0;i<coef.size();i++)
		    {
				SCIP_CALL_EXC( SCIPaddCoefLinear(scip, scip_cons, vars[i], coef[i]));
				mat[n_cons][i]=coef[i];
			}
			//add the constraint to the problem
			SCIP_CALL_EXC( SCIPaddCons(scip, scip_cons));
			// store the constraint for later access
            constraints.push_back(scip_cons);	
			//Add new constraint to the constraint list
			Cons *temp = new Cons(GREATER_EQUAL, val, n_cons);
			cons.push_back(temp);
				
				
			}
			break;
		case EQUAL:
		{
			//Create constraint
		    SCIP_CALL_EXC( SCIPcreateConsLinear(scip, & scip_cons, "", 0, NULL, NULL, val, val ,TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE) );
		    //Add scip variable to the scip constraint
		    for(size_t i=0;i<coef.size();i++)
		    {
				SCIP_CALL_EXC( SCIPaddCoefLinear(scip, scip_cons, vars[i], coef[i]));
				mat[n_cons][i]=coef[i];
			}
			//add the constraint to the problem
			SCIP_CALL_EXC( SCIPaddCons(scip, scip_cons));
			// store the constraint for later access
            constraints.push_back(scip_cons);
            //Create constraint
			//Add new constraint to the constraint list
			Cons *temp = new Cons(EQUAL, val, n_cons);
			cons.push_back(temp);
			
			}
			break;		
		
		}
	n_cons++;	
	built = true;
	
	
	}	

void LP::solve()
{
	    
		// start the solving process
        SCIP_CALL_EXC( SCIPsolve(scip) );
        // get the best found solution from scip
        SCIP_SOL * sol = SCIPgetBestSol(scip);
        if(sol==NULL)
            status = LP_NOSOLUTION;
        else
            status = LP_SOLVED;
        this->sol.resize(n_vars);    
         if(status==LP_SOLVED)
         { 
		    for(size_t i=0;i<vars.size();i++)
			    this->sol[i] = SCIPgetSolVal(scip, sol, vars[i]);
			objectiveOpt = SCIPgetSolOrigObj(scip, sol);
		    }
	     
}
void LP::displaySol()
{
	// get the best found solution from scip
    //SCIP_SOL * sol = SCIPgetBestSol(scip);
    
    
    if(status==LP_SOLVED)
    {
		for(size_t i=0;i<vars.size();i++)
		{
			std::cout<<"x_"<<i<<"= "<<(sol[i])<<std::endl;
			}
			std::cout<<std::endl;
			std::cout<<"The optimal value is: "<<objectiveOpt<<std::endl;
		}
	else
	
        std::cout<<"No solutions"<<std::endl;
		
		
}
