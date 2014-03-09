//------------------------------
//guarding to avoid multiple including
#ifndef __SCIPUSER_H_INCLUDED__
#define __SCIPUSER_H_INCLUDED__

/* scip includes */
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"

using namespace scip;

class ScipUser
{
  SCIP* scip;
  SCIP_Retcode catchEr; //catching possible errors in scip
  public:
  ScipUser();
  ~ScipUser();
  SCIP_Retcode getEr();
  SCIP_Retcode fakeVar();
  SCIP_Retcode tVar(int);
  SCIP_Retcode preVar(int, int, SCIP_Real, SCIP_Real);
  SCIP_Retcode scipSolve();
};

#endif
