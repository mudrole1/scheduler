//------------------------------
//guarding to avoid multiple including
#ifndef __SCHEDULER_H_INCLUDED__
#define __SCHEDULER_H_INCLUDED__

#include "scipUser.h"
//-----------------------------
// forward declared dependencies (I am using pointer to the Task, I dont need include it)
class Task;

using namespace std;

class Scheduler
{
  vector<Task *> tasksToS; //vector of tasks to be schedule
  int numPairs;
  int numTasks;
  vector<vector<int>> pairs;
  public:
  Scheduler(vector<Task *>);
  int getNumPairs();
  int getNumTasks();
  vector<vector<int>> getPairs();
  void setPairs();
  int setPreVar(ScipUser *);
  int solve();
  
  
};

#endif
