#include <iostream>
#include <map>
#include <algorithm>
#include "CGrid.hpp"
#include <string> //stoi
#include <thread> //sleep
#define GRID_SIZE 2

//long recur(CGrid &grid, long n, long depth);

int main(int argc, char *argv[])
{
  long gsize = GRID_SIZE;
  if(argc==2)
    gsize=std::stoi(argv[1]);
 
  std::cout << "m1" << std::endl;
  std::chrono::seconds secs(5);
  std::this_thread::sleep_for(secs); 
  //return 0;
  CGrid grid(gsize);
  long n = gsize*gsize*gsize;
  std::cout << "Grid: " << n << std::endl;
  
  // using built-in random generator:
  //grid.shuffle();
  unsigned long record=0;
  int scouting_depth=0;
  int inc=1;
  grid.first_recur(0, record, true, inc, scouting_depth);

  return 0;
}

bool comp_planes(const CPlane &p1, const CPlane &p2)
{
  return p1.count<p2.count;
}
