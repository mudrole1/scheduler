#include <string>
#include <iostream>
#include "task.h"

using namespace std;

/*constructor without parameter now, automatically set now to false*/
Task::Task(double s, double e, double d, string start_pos, string end_pos)
{
  start = s;
  end = e;
  duration = d;
  s_pos = start_pos;
  e_pos = end_pos;
  no = false;
}

/*constructor with parameter now*/
Task::Task(double s, double e, double d, string start_pos, string end_pos,bool now)
{
  start = s;
  end = e;
  duration = d;
  s_pos = start_pos;
  e_pos = end_pos;
  no = now;
}

double Task::getStart() {return start;}
double Task::getEnd() {return end;}
double Task::getDuration() {return duration;}
string Task::getStartPos() {return s_pos;}
string Task::getEndPos() {return e_pos;}


std::ostream& operator<<(std::ostream& os, const Task& t)
{
  if(t.no)
    os << "[" << t.start << "," <<t.end << "," << t.duration << "," <<t.s_pos<< "," << t.e_pos << "," << "now]";
  else
    os << "[" << t.start << "," <<t.end << "," << t.duration << "," <<t.s_pos<< "," << t.e_pos  << "]";
  return os;
}

