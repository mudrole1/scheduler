//------------------------------
//guarding to avoid multiple including
#ifndef __SCIPUSER_H_INCLUDED__
#define __SCIPUSER_H_INCLUDED__

/* scip includes */
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"

#include "task.h"

#include <vector>

using namespace scip;
using namespace std;

class ScipUser
{
  SCIP* scip;
  SCIP_Retcode catchEr; //catching possible errors in scip
  SCIP_VAR * f; //pointer to fake variable, we need to have it global to some error, probably internal SCIP
  public:
  ScipUser();
  ~ScipUser();
  SCIP_Retcode getEr();
  SCIP_VAR * getF();
  SCIP_Retcode fakeVar();
  SCIP_Retcode tVar(int, vector<SCIP_VAR *> *);
  SCIP_Retcode preVar(int, int, SCIP_Real, SCIP_Real);
  SCIP_Retcode setTcons(vector<Task*> *, vector<SCIP_VAR *> *, SCIP_VAR *);
  SCIP_Retcode setPcons(vector<Task*> *, vector<SCIP_VAR *> *, int, int, SCIP_VAR *, SCIP_CONS*);
  SCIP_Retcode scipSolve();
};

#endif
