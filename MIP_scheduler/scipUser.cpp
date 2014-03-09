#include "scipUser.h"

/* scip includes */
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"

#include <vector>

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

SCIP_Retcode ScipUser::fakeVar()
{
  /*creating of fake variable, it is always 1, needed in some constraints*/
   SCIP_VAR* f;
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

SCIP_Retcode ScipUser::tVar(int num_tasks)
{
  /* add t variables (execution of task) */
 
   char var_name[255];
   vector<SCIP_VAR*> t_var( num_tasks );
   for (int i = 0; i < num_tasks; i++)
   {
      t_var[i] = (SCIP_VAR*) NULL;
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
      t_var[i] = var;  
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

SCIP_Retcode ScipUser::scipSolve()
{
  SCIP_CALL( SCIPsolve(scip) );
  //statistic
  SCIP_CALL( SCIPprintBestSol(scip, NULL, FALSE) );
  return SCIP_OKAY;
}
