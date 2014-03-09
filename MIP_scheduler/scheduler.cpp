#include <vector>
#include "scheduler.h"
#include "task.h"
#include "distWrapper.h"
#include <iostream> //TODO: delete
#include "scipUser.h"
/* scip includes */
#include "objscip/objscip.h"
#include "objscip/objscipdefplugins.h"

using namespace scip;

using namespace std;

Scheduler::Scheduler(vector<Task *> tasks)
{
  tasksToS = tasks;
  numTasks = tasks.size();
  numPairs = 0;
  setPairs();
}

int Scheduler::getNumPairs()
{
  return numPairs;
}

int Scheduler::getNumTasks()
{
  return numTasks;
}

vector<vector<int>> Scheduler::getPairs()
{
  return pairs; //TODO: think if this is good, maybe return reference to it?
}

void Scheduler::setPairs()
{
  vector<vector<int>>::iterator it;
  vector<int> opair(2);  //one pair, always containing two integers

  for (int i=0; i<numTasks; i++)
  {
    for (int j=i+1; j<numTasks; j++)
    {
      double ei = tasksToS[i]->getEnd();
      double sj = tasksToS[j]->getStart();
      double dist = DistWrapper::dist(tasksToS[i]->getEndPos(),tasksToS[j]->getStartPos());
      if(ei+dist >sj)
      {
        //the combination of tasks is possible
        opair[0] = i;
        opair[1] = j;
        it = pairs.begin() + numPairs;
        pairs.insert(it,opair);
        numPairs++;
      }
    }
  }
}

int Scheduler::setPreVar(ScipUser * solver)
{
  vector<bool> pairSet(numPairs);
  SCIP_Retcode err;
  //setting pre variables, first testing now and conditions
  for(int i=0; i<(int)tasksToS.size(); i++)
  {
    Task * ac = tasksToS.at(i);
   
    //find pair
    for (int j=0; j<(int)pairs.size();j++)
    {
      vector<int> p = pairs.at(j);
      if(ac->getNow())
      {
         //if the task is in pair on first place, we would like to fix pre_ij = 1 (task i will precede task on the second place)
         if(p.at(0) == i)
         {
           pairSet[j] = true;
           err = solver->preVar(i, p.at(1), 1.0, 1.0); //seting low and up to same value will fix it to that
           if (err != SCIP_OKAY)
             return -1;
         }
         //if the task is in pair on the second place, it means that variable pre_ij must be zero, because then pre_ji is one
         else if(p.at(1) == i)
         { 
           pairSet[j] = true;
           err = solver->preVar(p.at(0), i, 0.0, 0.0); //seting low and up to same value will fix it to that
           if (err != SCIP_OKAY)
             return -1;
         }
      }
      else if(ac->getCond()) //if the task has preconditions
      {
        vector<int> * precon = ac->getPrecon();
        //for all preconditions
        for(int k=0; k<(int)precon->size(); k++)
        {
          //if i,j is in pair
          if((p.at(0) == i)&&(p.at(1) == precon->at(k)))
          { 
            //i is the task which has j as precondition, thus preij = 0
            pairSet[j] = true;
            err = solver->preVar(p.at(0), p.at(1), 0.0, 0.0); //seting low and up to same value will fix it to that
            if (err != SCIP_OKAY)
              return -1;
          }
          //if j,i is in pair
          else if((p.at(0) == precon->at(k))&&(p.at(1) == i))
          { 
            //i is the task which has j as precondition, thus preij = 1
            pairSet[j] = true;
            err = solver->preVar(p.at(0), p.at(1), 1.0, 1.0); //seting low and up to same value will fix it to that
            if (err != SCIP_OKAY)
              return -1;
          }
          //if it is not in the pair (because they are not overlapping) 
          else
          {
            err = solver->preVar(precon->at(k), i, 1.0, 1.0); //seting low and up to same value will fix it to that
            if (err != SCIP_OKAY)
              return -1;
          }
        }
      }
      else //if no special relationship is between pair, preij can be 0 or 1
      { 
        pairSet[j] = true;
        err = solver->preVar(p.at(0), p.at(1), 0.0, 1.0); //seting low and up to same value will fix it to that
        if (err != SCIP_OKAY)
          return -1;
      }
    }
  }
  return 0;
}

int Scheduler::solve()
{
  SCIP_Retcode err;
  vector<bool> pairUsed;

  ScipUser * solver = new ScipUser();
  err = solver->getEr();
  if (err != SCIP_OKAY)
    return -1;

  err = solver->fakeVar();
  if (err != SCIP_OKAY)
    return -1;

  err = solver->tVar(numTasks);
  if (err != SCIP_OKAY)
    return -1;

  int e = setPreVar(solver);
  if (e==-1)
    return -1;


  err = solver->scipSolve();
   if (err != SCIP_OKAY)
    return -1; 
    

  //call destructor
  delete solver;

  return 0;
}



