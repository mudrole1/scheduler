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

  int num_prob1 =0;
  int num_prob2 =0;
  ifstream myfile;
  string line;
  myfile.open ("../src/tasks.txt", std::ios_base::in);
  int s,e,d,w;
  int num=0;

  vector<Task*> tasks;

  if (myfile.is_open())
  {
    //while ( getline (myfile,line) )
    while (myfile >> s >> e >> d >>w)
    {
      cout << s <<";" << e <<";" << d <<";" << w << "\n";
      tasks.push_back(new Task(num+1, s, e, d, s1, s2));
      num++;
      if(num%10 == 0)
      {
        Scheduler scheduler(&tasks);
        bool worked = scheduler.solve(3);
        tasks.erase(tasks.begin(),tasks.end());
        cout << "zkouska" << tasks.size() << "\n";
        if(worked)  {
          cout<< "Schedule found" << worked << "\n";
         
        } 
        else {
          cout<< "No schedule found"<< endl;
          num_prob2++;
        }
      }
    }
    myfile.close();
  } 
  
  cout << "Problems:" << num_prob1 << ";" << num_prob2 << "\n";
  return 0;

}
