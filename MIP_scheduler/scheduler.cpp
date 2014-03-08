#include <vector>
#include "scheduler.h"
#include "task.h"
#include "distWrapper.h"
#include <iostream> //TODO: delete

using namespace std;

Scheduler::Scheduler(vector<Task *> tasks)
{
  tasksToS = tasks;
  numPairs = tasks.size();
}

void Scheduler::setPairs()
{
  for (int i=0; i<numPairs; i++)
  {
    for (int j=i+1; j<numPairs; j++)
    {
      double ei = tasksToS[i]->getEnd();
      double sj = tasksToS[j]->getStart();
      double dist = DistWrapper::dist(tasksToS[i]->getEndPos(),tasksToS[j]->getStartPos());
    cout << dist;
      //if(ei+dist(pi,pj) >sj)
    }
  }
}


