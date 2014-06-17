#include "pairs.h"
#include "task.h"
#include <vector>
#include <iostream> 

using namespace std;

Pairs::Pairs(vector<Task *> * input_tasks)
{
  tasksToS = input_tasks;
  numTasks = tasksToS -> size();
  numPairs = 0;
}

void Pairs::getPairs(vector<vector<int>> * rp)
{
  *rp = pairs;
}

int Pairs::setPairs()
{
  vector<vector<int>>::iterator it;
  vector<int> opairs(4);  //one pair, always containing two integers + order of pair, when setted by preVar method + the type of pair

  //setting pairs based on the fact if their windows are overlapping. Thus we need to decide if i precede j, or j precede i
  for (int i=0; i<numTasks; i++)
  {
    for (int j=i+1; j<numTasks; j++)
    {

      bool set = false;
      double ei = tasksToS->at(i)->getEnd();
      double si = tasksToS->at(i)->getStart();
      double ej = tasksToS->at(j)->getEnd();
      double sj = tasksToS->at(j)->getStart();
      //double dist = DistWrapper::dist(tasksToS->at(i)->getEndPos(),tasksToS->at(j)->getStartPos());

      //I before J, or I after J there is no overlapp and we dont want to add the pair.
      //for other cases, we would like to add some constraint
       
      //the combination of tasks is possible
      opairs[0] = i;//tasksToS->at(i)->getID();
      opairs[1] = j;//tasksToS->at(j)->getID();
      opairs[2] = -1; // this will be set in preVar method
      opairs[3] = -1;

      if(ei>sj)
      {
 
        //i overlaps j
        if((si<sj)&&(ei<ej))
        {
          opairs[3] = 1;
          set = true;
        }
        
        //i starts j, equal
        else if(si==sj)
        {
          if(ei<ej) //i starts j
          {
            opairs[3]=1;
            set = true;
          }
          
          else if (ei==ej)//equals
          {
            opairs[3]=1; //this should be 2, but if intervals are same and we are not using any priority, it makes no sense
                         //to chose ordering, thus fixing it for task i precedes task j
            set = true;
          }
        }
       //j during i
        else if((si<sj)&&(ei>ej))
        {
           opairs[3]=2;
           set = true;
        }
        //i finish j
        else if((ei == ej)&&(si<sj))
        {
          opairs[3]=1;
          set = true;
        }
      }
      if(ej>si)
      {
        //j overlaps i
        if((sj<si)&&(ej<ei))
        {
          opairs[3] = 0;
          set = true;
        }
        
        else if((si==sj)&&(ej < ei)) //i imeets j
        {
          opairs[3]=0;
          set = true;
        }
       //i during j
        else if((sj<si)&&(ej>ei))
        {
          opairs[3]=2;
          set = true;
        }
        else if((ei == ej)&&(sj<si))
        {
          opairs[3]=0;
          set = true;
        }
      }
       if(set)
       {
         it = pairs.begin() + numPairs;
         pairs.insert(it,opairs);
         numPairs++;
         //cout << opairs[0] << ";" << opairs[1] << opairs[3] << "\n";
       }

     // if(ei+dist >sj)
      //{     
      //}
    }
  }
  return numPairs;
}
