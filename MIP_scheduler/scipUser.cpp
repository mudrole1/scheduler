#include "scipUser.h"
#include "task.h"
#include "distWrapper.h"

/* scip includes */
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"

#include <vector>

#include <iostream> //TODO:delete this

using namespace scip;
using namespace std;

ScipUser::ScipUser()
{
  scip = NULL;
  /* initialize SCIP environment */
  catchEr = SCIPcreate(&scip);
  /* include default plugins */  //TODO: figure out what exatly this is doing
  catchEr = SCIPincludeDefaultPlugins(scip);
  /* set verbosity parameter */
  //TODO:not sure about this
  catchEr = SCIPsetIntParam(scip, "display/verblevel", 5);
  /* create empty problem */
  catchEr = SCIPcreateProb(scip, "Scheduler", 0, 0, 0, 0, 0, 0, 0);

  f= (SCIP_VAR*)NULL;
}

ScipUser::~ScipUser()
{
  catchEr = SCIPfree(&scip);
  BMScheckEmptyMemory();
}

SCIP_Retcode ScipUser::getEr()
{
  return catchEr;
}

SCIP_VAR * ScipUser::getF() {return f;}

SCIP_Retcode ScipUser::fakeVar()
{
  /*creating of fake variable, it is always 1, needed in some constraints*/
   char fn[255];
   SCIPsnprintf(fn, 255, "f");
   SCIP_CALL( SCIPcreateVar(scip,
		&f,
		fn,
		1.0,
		1.0,
		0.0,
		SCIP_VARTYPE_INTEGER,
		true,
		false,
		0, 0, 0, 0, 0)); 
   
   SCIP_CALL(SCIPaddVar(scip, f)); 
   return SCIP_OKAY; 
}

SCIP_Retcode ScipUser::tVar(int num_tasks, vector<SCIP_VAR *> * t_var)
{
  /* add t variables (execution of task) */
 
   char var_name[255];
   vector<SCIP_VAR*>::iterator it;
   for (int i = 0; i < num_tasks; i++)
   {
      SCIP_VAR* var;
      SCIPsnprintf(var_name, 255, "t_%d", i);

      SCIP_CALL( SCIPcreateVar(scip,
                     &var,                   // returns new index
                     var_name,               // name
                     0.0,                    // lower bound
                     SCIP_DEFAULT_INFINITY,                    // upper bound
                     1.0,         // objective probably means if this should be included to the minimizing, 1=yes
                     SCIP_VARTYPE_CONTINUOUS,   // variable type
                     true,                   // initial
                     false,                  // forget the rest ...
                     0, 0, 0, 0, 0) );
      SCIP_CALL( SCIPaddVar(scip, var) );
      it = t_var->begin()+i;
      t_var->insert(it,var);  
   }
   return SCIP_OKAY;
}

SCIP_Retcode ScipUser::preVar(int i, int j, SCIP_Real low, SCIP_Real up)
{
  /* add one pre_ij variable (pre_ij = 1 if a task i precede task j) */
   char var_name[255];
   vector<SCIP_VAR*> pre_var(1);
   pre_var[0] = (SCIP_VAR*) NULL;
   SCIPsnprintf(var_name, 255, "pre_i%d_j%d", i,j);
   SCIP_VAR* var;
   SCIP_CALL( SCIPcreateVar(scip,
                     &var,                   // returns new index
                     var_name,               // name
                     low,                    // lower bound
                     up,                    // upper bound
                     0.0,         // objective ??
                     SCIP_VARTYPE_BINARY,   // variable type
                     true,                   // initial
                     false,                  // 
                     0, 0, 0, 0, 0) );
    SCIP_CALL( SCIPaddVar(scip, var) );
    pre_var[0] = var;  
  
  return SCIP_OKAY;
}

SCIP_Retcode ScipUser::setTcons(vector<Task*> * tasksToS, vector<SCIP_VAR *> * t_var, SCIP_VAR * g)
{
  /* add constraint s<= t + d <=e */
   char con_name[255];
   int num_tasks = tasksToS->size();

   vector<SCIP_CONS*> t_con (num_tasks);
   for (int i = 0; i < num_tasks; i++)
   {
     SCIP_VAR* ti = (SCIP_VAR*)NULL;
     ti = t_var->at(i);
     SCIP_Real d = tasksToS->at(i)->getDuration(); 
     SCIP_Real s = tasksToS->at(i)->getStart();
     SCIP_Real e = tasksToS->at(i)->getEnd();


     SCIP_CONS* con = (SCIP_CONS*)NULL;
     SCIPsnprintf(con_name, 255, "se_%d", i);

     SCIP_CALL(SCIPcreateConsVarbound 	(scip,
		&con,
		con_name,
		ti,	//variable x
		g,      //biding variable y
		d,      //constant
		s,      //left side of eq
		e,	//right side of eq
		true,   // 	initial,
		true,    //  	separate,
		true,  //  	enforce,
		true,  //  	check,
		true,  //  	propagate,
		false, // 	local,
		false, //  	modifiable,
		false, //  	dynamic,
		false,//  	removable,
		false//  	stickingatnode 
	) );
    SCIP_CALL( SCIPaddCons(scip, con) );
    t_con[i] = con;	
   }
  return SCIP_OKAY;
}

SCIP_Retcode ScipUser::setPcons(vector<Task*> * tasksToS, vector<SCIP_VAR *> * t_var, int i, int j, SCIP_VAR * g, SCIP_CONS* con)
{
/* add constraint ti + di + dist - tj <= 0 if preij == 1*/
     char con_name[255];   
     SCIP_VAR* ti;
     SCIP_VAR* tj;
     //i,j is starting from 1
     ti = t_var->at(i);
     tj = t_var->at(j);

     //creating a constraint ti + di + dist - tj <= 0     
     SCIP_Real d = tasksToS->at(i)->getDuration();
     SCIP_Real dist = DistWrapper::dist(tasksToS->at(i)->getEndPos(),tasksToS->at(j)->getStartPos());

     SCIP_Real vals[4]; //array of values
     vals[0] = 1;
     vals[1] = d;
     vals[2] = dist;
     vals[3] = -1;  
     
     SCIP_VAR * vars[4]; //array of variables
     vars[0] = ti;
     vars[1] = g;
     vars[2] = g;
     vars[3] = tj; 
 
     SCIPsnprintf(con_name, 255, "tddt_%d%d", i,j);

     SCIP_CALL(SCIPcreateConsLinear 	(scip,
		&con,
		con_name,
		4, //number of variables
		vars,//&vars,
		vals,
		-SCIP_DEFAULT_INFINITY,//  	lhs,
		0.0,//  	rhs,
		true,   // 	initial,
		true,    //  	separate,
		true,  //  	enforce,
		true,  //  	check,
		true,  //  	propagate,
		false, // 	local,
		false, //  	modifiable,
		false, //  	dynamic,
		false,//  	removable,
		false//  	stickingatnode
	) );
  return SCIP_OKAY;
}

SCIP_Retcode ScipUser::scipSolve()
{
  SCIP_CALL( SCIPsolve(scip) );
  //statistic
  SCIP_CALL( SCIPprintBestSol(scip, NULL, FALSE) );
  return SCIP_OKAY;
}
