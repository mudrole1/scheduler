//------------------------------
//guarding to avoid multiple including
#ifndef __TASK_H_INCLUDED__
#define __TASK_H_INCLUDED__

#include <string>

using namespace std;
class Task
{
  double start;  //the time when the task can start
  double end;    //the time when the task needs to be finished
  double duration; 
  string s_pos; //position where the task starts
  string e_pos; //position where the task ends
  bool no;     //if the task must happen now
  bool cond;   //if the task has preceding tasks
  //vector<Task*> * precon; //tasks which needs to happen before this task
  vector<int> * precon; //indexes of tasks which needs to happen before this task

  public:
  Task (double, double, double, string, string);
  Task (double, double, double, string, string,bool);
  Task (double, double, double, string, string,vector<int> *);
  double getStart();
  double getEnd();
  double getDuration();
  string getStartPos();
  string getEndPos();
  bool getNow();
  bool getCond();
  vector<int>* getPrecon();
  friend ostream& operator<<(ostream&, const Task&);
};

#endif

