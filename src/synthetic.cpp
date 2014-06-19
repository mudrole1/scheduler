#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "task.h"
#include "scheduler.h"

using namespace std;

int main (int argc, char** argv) 
{   
  string s1("home");
  string s2("office");

  unsigned int task_count;
  int param;

  if(argc > 1) {
    task_count = stoi(argv[1]);
    param = stoi(argv[2]);
  }
  else {
    task_count = 10;
    param = 1;
  }

  int num_prob1 =0;
  int num_prob2 =0;

  //for(int x=0; x<100; x++) 
  //{
  int d=15; //duration of task
  int overlap1 = -5; //none (with minus value two tasks are pushed away from each other)
  int overlap2 = d; //small one 
  int overlap3 = 2*d + 2; //+2 because dist is one
  int overlap4 = 2*d + 10; //+10 because dist(j,i) will be 5
  int overlap;
  double width = 1; //width affects the size of window 
   
		
  double s,e;
  
  vector<Task*> tasks;

  switch(param)
  {
    case 1:
     overlap = overlap1;
     break;
    case 2: 
     overlap = overlap2;
     break;
    case 3:
     overlap = overlap3;
     break;
    case 4:
     overlap = overlap4;
     break;
  }
  int size = width*d + 2*abs(overlap);

  for(int i=0;i<task_count;i++)
  {	
    if(i==0)
    {
      s = 0;
      e = size;
    }		
    else if(i%2 == 0)
    {
      s = e+1;
      e = s+size;
    }
    else if(i%2 == 1)
    {
      s = e-overlap;
      e = s+size;
    }
    cout << s << ";" << e << ";\n";
    tasks.push_back(new Task(i, s, e, d, s1, s2));
  }
		
  Scheduler scheduler(&tasks);
  bool worked = scheduler.solve(1);

  bool sorted = false;
  Task * pom = (Task*)NULL; 
 

  /*if(worked)  {
    cout<< "Schedule found" << worked << "\n";
    for(auto & tp : tasks) {
      cout<< "Is start time "<<tp->getExecTime() << " after start? + dur:" << tp->getDuration() << ":" 
       << (tp->getExecTime() >= tp->getStart()) << "\n";
    } 
   
  }*/
  cout << "----------------------------------\n";
  worked = scheduler.solve(2);

  /* if(worked)  {
    cout<< "Schedule found" << worked << "\n";
    for(auto & tp : tasks) {
      cout<< "Is start time "<<tp->getExecTime() << " after start? + dur:" << tp->getDuration() << ":" 
       << (tp->getExecTime() >= tp->getStart()) << "\n";
    } 
  }*/
  //}
  return 0;

}
