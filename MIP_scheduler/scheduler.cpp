#include <vector>
#include "scheduler.h"
#include "task.h"
#include "distWrapper.h"
#include <iostream> //TODO: delete

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




