#include <iostream>
#include <string>
#include <vector>

#include "task.h"
#include "scheduler.h"

using namespace std;

int main (int argc, char** argv) 
{   
  string s1 = "home";
  string s2 = "office";
  Task a(0.0, 10.0, 4.0, s1, s2);
  Task b(0.0, 10.0, 3.0, s2, s1);
  Task c(0.0, 10.0, 2.0, "school", "shop",true);

  vector<Task*> tasks;

  vector<Task*>::iterator it;
  it = tasks.begin();
  tasks.insert(it,&a);
  it = tasks.begin()+1;
  tasks.insert(it,&b);
  it = tasks.begin()+2;
  tasks.insert(it,&c);

  vector<int> p(3);
  p[0]=0;
  p[1]=1;
  p[2]=2;
  

  Task d(0.0, 10.0, 1.0, "shop", "school",&p);
  cout << d;
  
  it = tasks.begin()+3;
  tasks.insert(it,&d);

  Scheduler sch(tasks);
  sch.solve();

  return 0;

}
