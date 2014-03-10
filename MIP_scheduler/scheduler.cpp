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

Scheduler::Scheduler(vector<Task *> * tasks)
{
  tasksToS = tasks;
  numTasks = tasks->size();
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

  //setting pairs based on the fact if their windows are overlapping. Thus we need to decide if i precede j, or j precede i
  for (int i=0; i<numTasks; i++)
  {
    for (int j=i+1; j<numTasks; j++)
    {
      double ei = tasksToS->at(i)->getEnd();
      double sj = tasksToS->at(j)->getStart();
      double dist = DistWrapper::dist(tasksToS->at(i)->getEndPos(),tasksToS->at(j)->getStartPos());

      if(ei+dist >sj)
      {
        //the combination of tasks is possible
        opair[0] = tasksToS->at(i)->getID();
        opair[1] = tasksToS->at(j)->getID();
        it = pairs.begin() + numPairs;
        pairs.insert(it,opair);
        numPairs++;
      }
    }
  }
}

int Scheduler::findTaskNow()
{
  int index = -1;
  for(int i=0; i<(int)tasksToS->size();i++)
  {
    Task * ac = tasksToS->at(i);
    if (ac->getNow())
    {
      index = i;
      break;
    }      
  }
  return index;
}

vector<int> Scheduler::findConditions()
{
  vector<int> con;
  vector<int>::iterator it;
  int vi=0;
  for(int i=0; i<(int)tasksToS->size();i++)
  {
    Task * ac = tasksToS->at(i);
    if (ac->getCond())
    {
      it = con.begin() + vi;
      con.insert(it,i);
      vi++;
    }
  }
  return con;
}

int Scheduler::setPreVar(ScipUser * solver)
{
  vector<bool> pairSet;
  pairSet.resize(numPairs,false);
  SCIP_Retcode err;
  int i,j;
  //setting pre variables, first testing now and conditions
  //if task now exist, we need to set pre variables first

  i = findTaskNow();
  if(i != -1)
  {
    int tid = tasksToS->at(i)->getID();
    //go throw pairs where task i is
    for (j=0; j<(int)pairs.size();j++)
    {
      vector<int> p = pairs.at(j);
      //if the task is in pair on first place, we would like to fix pre_ij = 1 (task i will precede task on the second place)
      if(p.at(0) == tid)
      {
        pairSet[j] = true;
        err = solver->preVar(p.at(0), p.at(1), 1.0, 1.0); //seting low and up to same value will fix it to that
        if (err != SCIP_OKAY)
          return -1;
      }
      //if the task is in pair on the second place, it means that variable pre_ij must be zero, because then pre_ji is one
      else if(p.at(1) == tid)
      { 
        pairSet[j] = true;
        err = solver->preVar(p.at(0), p.at(1), 0.0, 0.0); //seting low and up to same value will fix it to that
        if (err != SCIP_OKAY)
          return -1;
      }
    }   
  }

  //then solve conditions
  vector<int> taskWithCond = findConditions();
  if(taskWithCond.size() != 0)
  {
    //for tasks witch have preconditions
    for(i =0; i<(int)taskWithCond.size(); i++)
    { 
      Task * ac = tasksToS->at(taskWithCond.at(i));

      int idt = ac -> getID();
      vector<Task*> * precon = ac->getPrecon();
      vector<bool> preSet;
      preSet.resize(precon->size(),false); //for each precondition we need to set some variable
      //for all preconditions for single task
      for(int k=0; k<(int)precon->size(); k++)
      {
        int idp = precon->at(k)->getID();
        //set possible pairs
        for (j=0; j<(int)pairs.size();j++)
        {
          vector<int> p = pairs.at(j);
          //if i,j is in pair
          if((p.at(0) == idt)&&(p.at(1) == idp))
          { 
            if(!pairSet[j])
            {
              //i is the task which has j as precondition, thus preij = 0
              pairSet[j] = true;
              preSet[k] = true;
              err = solver->preVar(p.at(0), p.at(1), 0.0, 0.0); //seting low and up to same value will fix it to that
              if (err != SCIP_OKAY)
                return -1;
            }
            else
            { //if pair already set, notify that the conditions is solved
              preSet[k] = true;
            }
          }
          //if j,i is in pair
          else if((p.at(0) == idp)&&(p.at(1) == idt))
          { 
            if(!pairSet[j])
            {
              //i is the task which has j as precondition, thus preij = 1
              pairSet[j] = true;
              preSet[k] = true;
              err = solver->preVar(p.at(0), p.at(1), 1.0, 1.0); //seting low and up to same value will fix it to that
              if (err != SCIP_OKAY)
                return -1;
            }
            else
            { //if pair already set, notify that the conditions is solved
              preSet[k] = true;
            }
          }
        }
   //TODO: fix the error here
        //if the pair does not exists (because tasks are not overlapping) condition for precondition k was not established
        if(!preSet[k])
        {
           preSet[k] = true;
           err = solver->preVar(idp, idt, 1.0, 1.0); //seting low and up to same value will fix it to that
           if (err != SCIP_OKAY)
              return -1;
           //also we need to add it to the pairs, to be able to create constraints;
           vector<vector<int>>::iterator it;
           it = pairs.begin()+numPairs;
           vector<int> opair(2);
           opair[0] = idp;
           opair[1] = idt;
           pairs.insert(it,opair); 
           pairSet[numPairs]=true;
           numPairs++;     
      
        }
      }
    }
  }

  //set the rest of pairs
  for (j=0; j<(int)pairs.size();j++)
  {
    vector<int> p = pairs.at(j);
    if(!pairSet[j])
    {
      pairSet[j] = true;
      err = solver->preVar(p.at(0), p.at(1), 0.0, 1.0); //seting low and up to same value will fix it to that
      if (err != SCIP_OKAY)
        return -1;
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

  //err = solver->fakeVar();
  //if (err != SCIP_OKAY)
  //  return -1;

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



