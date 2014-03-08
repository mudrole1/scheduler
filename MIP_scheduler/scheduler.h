//------------------------------
//guarding to avoid multiple including
#ifndef __SCHEDULER_H_INCLUDED__
#define __SCHEDULER_H_INCLUDED__

//-----------------------------
// forward declared dependencies (I am using pointer to the Task, I dont need include it)
class Task;

using namespace std;

class Scheduler
{
  vector<Task *> tasksToS; //vector of tasks to be schedule
  int numPairs;
  vector<vector<int>> pairs;
  public:
  Scheduler(vector<Task *>);
  void setPairs();
};

#endif
