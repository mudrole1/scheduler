#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "task.h"
#include "scheduler.h"
#include <fstream>

using namespace std;

int main (int argc, char** argv) 
{   
  string s1("home");
  string s2("office");

  unsigned int task_count;

  if(argc > 1) {
    task_count = stoi(argv[1]);
  }
  else {
    task_count = 10;
  }

  int num_prob1 =0;
  int num_prob2 =0;
  ofstream myfile;
  myfile.open ("../src/tasks.txt");
  myfile << "start, end, duration, assigned time\n";
  srand(time(NULL));
  for(int x=0; x<100; x++) 
  {
  int mind = 2; //minimal duration of tasks
  int maxd = 30;  //maximal duration of tasks
  int maxw = 20; //maximum waiting time between two tasks
  int maxs = 240; //maximum window size
  int taken=0;
  int d=0; //duration of task
  int w=0; //random waiting time between tasks
  int s; //starting time of task
  int e; //ending time of task
  int pw=0;//previous w
		
  int sum=0;
  
  vector<Task*> tasks;


  for(int i=0;i<task_count;i++)
  {			
    w =w+d+(int) round(rand() % maxw) +1;
    s = w-(int) round(rand() % (maxw*5)); //set random start of task
    if(s<0)
      s=0;
    d = (int) round(rand() % (maxd-mind)+mind);
    taken=w-s + d;
    e=(int) round(rand() % (maxs))+taken+s+1;
    sum=sum+w;
    myfile << s << ";" << e << ";" << d << ";" << w << "\n";
    tasks.push_back(new Task(i, s, e, d, s1, s2));
    if((i>0)&&(pw>=w))
      num_prob1++;

    pw = w+d;
  }
		
  myfile << "\n";
  Scheduler scheduler(&tasks);
  bool worked = scheduler.solve();

  bool sorted = false;
  Task * pom = (Task*)NULL; 
 

  if(worked)  {
    cout<< "Schedule found" << worked << "\n";
    for(auto & tp : tasks) {
      cout<< "Is start time "<<tp->getExecTime() << " after start? + dur:" << tp->getDuration() << ":" 
       << (tp->getExecTime() >= tp->getStart()) << "\n";
    } 
    /*while(!sorted)
    {
      sorted = true;
      for(int i=0;i<task_count-1;i++)
      {
        if(tasks.at(i)->getExecTime() > tasks.at(i+1)->getExecTime())
        {
          pom = tasks.at(i);
          tasks.at(i) = tasks.at(i+1);
          tasks.at(i+1) = pom;
          sorted = false;
        }
      }
    }
    for(int i=0;i<task_count;i++)
    {  
       Task * tp = tasks.at(i);
       for(int j=i+1;j<task_count;j++)
       {
         Task * tp2 = tasks.at(j);
         if(tp->getExecTime()+tp->getDuration() > tp2->getExecTime())
         {
           cout << "problem\n";
           num_prob1++;
         }
       }
    }*/
  }
  else {
    cout<< "No schedule found"<< endl;
    num_prob2++;
  }
}
myfile.close();
cout << "Problems:" << num_prob1 << ";" << num_prob2 << "\n";
  return 0;

}
