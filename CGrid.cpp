#include "CGrid.hpp"
#include <sstream>
#include <iostream>
#include <numeric>
#include <iterator> //std::next
#include <string>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <vector>
#include <thread>
// random generator function:
int myrandom (int i) { return std::rand()%i;}


bool comp_points(const CPoint &p1, const CPoint &p2)
{
  return p1.count>p2.count;
}
std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
std::chrono::time_point<std::chrono::system_clock> end;
long long gc=0;
long gcd1(long a, long b)
{//4, 2, 0, -8
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

long gcd( long x, long y, long z, long d)
{
  long arr[4];
  arr[0]=x;
  arr[1]=y;
  arr[2]=z;
  arr[3]=d;
  long result = std::accumulate(arr, arr + 4, 0, gcd1);

  return result;
}

CGrid::CGrid( CGrid *g )
{
}

CGrid::CGrid( short n )
{
  for(short i=0; i<n; i++)
  {
    for(short j=0; j<n; j++)
    {
      for(short k=0; k<n; k++)
      {
        CPoint tmp(i, j, k);
        points.push_back(tmp);
      }
    }
  }
  limit1=points.size();
  limit2=0;
/*   std::cout << "Initializing planes_grid (" << n*n*n << ") ... " << std::flush;
  for(short i=0; i<n; i++)
  {
    for(short j=0; j<n; j++)
    {
      for(short k=0; k<n; k++)
      {
        planes_grid[i][j][k] = getPlane(i, j, k);
      }
    }
  }
  std::cout << " Finished!!" << std::endl; */
}

CPlane CGrid::getPlane(CPoint p1, CPoint p2, CPoint p3)
{
  gc++;
  long v1[3], v2[3];
  v1[0] = p1.x - p2.x;
  v1[1] = p1.y - p2.y;
  v1[2] = p1.z - p2.z;
  
  v2[0] = p3.x - p2.x;
  v2[1] = p3.y - p2.y;
  v2[2] = p3.z - p2.z;
  
  //a=v1, b=v2
  CPlane pl;
  pl.x = v1[1]*v2[2]-v1[2]*v2[1];//a2b3-a3b2
  pl.y = v1[2]*v2[0]-v1[0]*v2[2];//a3b1-a1b3
  pl.z = v1[0]*v2[1]-v1[1]*v2[0];//a1b2-a2b1
  pl.d = -1*(pl.x*p3.x+pl.y*p3.y+pl.z*p3.z);
  
  // we normalize the equation:
  // By dividing the equation by the minimum common denominator ...
  long g = gcd(pl.x, pl.y, pl.z, pl.d);
  if(g>1 || g<-1)
  {
    pl.x/=g;
    pl.y/=g;
    pl.z/=g;
    pl.d/=g;
  }
  // And making the x value always positive
  if(pl.x<0)
  {
    pl.x*=-1;
    pl.y*=-1;
    pl.z*=-1;
    pl.d*=-1;
  }
  else if(pl.x==0)
  {
    if(pl.y<0)
    {
      //invert signs
      pl.y*=-1;
      pl.z*=-1;
      pl.d*=-1;
    }
    else if (pl.y==0)
    {
      if(pl.z<0)
      {
        //invert signs
        pl.z*=-1;
        pl.d*=-1;
      }
    }
  }
  return pl;
}

bool CGrid::pointIsInPlane(CPoint p, CPlane pl)
{
  if(p.x*pl.x+p.y*pl.y+p.z*pl.z+pl.d==0)
    return true;
  else  
    return false;
}

unsigned long CGrid::getPlanesSize( void )
{
  return planes.size();
}

unsigned long CGrid::getPointsSize( void )
{
  return points.size();
}

void CGrid::printPlanes( void )
{
  for(std::map<CPlane, long>::iterator it = planes.begin();
      it != planes.end();
      it++)
  {
    std::cout << it->first.print() << "\n";
  }
}
void CGrid::addPlane(CPlane pl)
{
  planes.insert(std::pair<CPlane, long>(pl, 1));
}

void CGrid::shuffle( void )
{
  
  std::srand ( unsigned ( std::time(0) ) );
  // using built-in random generator:
  //std::random_shuffle ( points.begin(), points.end() );
    std::random_shuffle ( points.begin(), points.end(), myrandom);
}
/*
void CPoint::setValue2(long value)
{
  if(value<0 || value>2)
    std::cout << "ERROR setting point. value: " << value << std::endl;
  else
    enabled = value;
}

long CPoint::getValue2()
{
  return enabled;
}
*/
unsigned long CGrid::getValueCount(long n)
{
  unsigned long ret = 0;
  if(n==0)
    ret=points.size()-limit1;
  else if (n==1)
    ret=limit1-limit2;
  else if (n==2)
    ret=limit2;
  return ret;
}

CPoint::CPoint(short i, short j, short k, short c)
{
  x = i;
  y = j;
  z = k;
  count = c;
}
const std::string CPoint::print( void ) const
{
  std::ostringstream oss;
  oss << "(" << x << "," << y << "," << z << ")";
  return oss.str();
}

/*std::string CPoint::print2( void )
{
  std::ostringstream oss;
  oss << x << " \t" << y << " \t" << z << " \t(" << getValue() << ") \t";
  return oss.str();
}*/
CPlane::CPlane( void )
{
  x = -1000;
  y = -1000;
  z = -1000;
  d = -1000;
  count = 0;
}

const std::string CPlane::print( void ) const
{
  std::ostringstream oss;
  std::string strX;
  std::string res;
  res+=std::__cxx11::to_string(x)+"x";
  if(y>=0 && res!="") res+="+";
  res+=std::__cxx11::to_string(y)+"y";
  if(z>=0 && res!="") res+="+";
  res+=std::__cxx11::to_string(z)+"z";
  if(d>=0 && res!="") res+="+";
  res+=std::__cxx11::to_string(d);
  
  oss << res << " (" << count << ") ";
  
  return oss.str();
}

long CPlane::addCount(long n)
{
  count+=n;
  return count;
}

bool CPlane::operator<(const CPlane &p1) const
{
  //std::cout << "comparing \"" << print() << "\" and \"" << p1.print() << "\"";
  bool ret;
  if( x < p1.x ) ret = true;
  else if( x > p1.x ) ret = false;
  else if( y < p1.y ) ret = true;
  else if( y > p1.y ) ret = false;
  else if( z < p1.z ) ret = true;
  else if( z > p1.z ) ret = false;
  else if( d < p1.d ) ret = true;
  else if( d > p1.d ) ret = false;
  else ret = false;
  //std::cout << "[[ " << ret << " ]]" << std::endl;
  return ret;
}
bool CPoint::operator<(const CPoint &p1) const
{
  //std::cout << "comparing \"" << print() << "\" and \"" << p1.print() << "\"";
  bool ret;
  if( x < p1.x ) ret = true;
  else if( x > p1.x ) ret = false;
  else if( y < p1.y ) ret = true;
  else if( y > p1.y ) ret = false;
  else if( z < p1.z ) ret = true;
  else if( z > p1.z ) ret = false;
  else ret = false;
  //std::cout << "[[ " << ret << " ]]" << std::endl;
  return ret;
}


bool CPoint::operator==(const CPoint &p1) const
{
  if( x != p1.x ) return false;
  if( y != p1.y ) return false;
  if( z != p1.z ) return false;
  return true;
}

bool CPlane::operator==(const CPlane &p1) const
{
  if( x != p1.x ) return false;
  if( y != p1.y ) return false;
  if( z != p1.z ) return false;
  if( d != p1.d ) return false;
  return true;
}
/*
void CGrid::setValue(CPoint p, long value)
{
  std::vector<CPoint>::iterator it;
  it = std::find(points.begin(), points.end(), p);
  if (it != points.end())
  {
    values[it->getValue2()]--;
    it->setValue2(value);
    values[value]++;
    return;
  }
  
  std::cout << "ERROR en setValue()" << std::endl;
  return;
}
*/
void CGrid::swap(long i, long j)
{
  CPoint tmp = points[i];
  points[i]=points[j];
  points[j]=tmp;
}
/*
long CGrid::getValue(CPoint p, int dbg)
{
  std::vector<CPoint>::iterator it;
  it = std::find(points.begin(), points.end(), p);
  if (it != points.end())
    return it->getValue2();
  
  std::cout << "ERROR in getValue() ["<< dbg << "], finding: " << p.print() << std::endl;
  return -1;
}
*/

long CGrid::markPlanes(CPoint p)
{  
  long point_count = 0;
  for(long i=0; i<limit2-1; i++)
  {
    //std::cout << "  x02" << std::endl;
    if(points[i]==CPoint(4,5,2)) std::cout << "ALARM 1 **************" << std::endl;
    
    if(points[i]==p)
    {
      //std::cout << "-1 P1: " << p.print() << ", P2: " << points[i].print() << std::endl;
      continue;
    }
    //std::cout << "  x03" << std::endl;
    for(long j=0; j<limit2; j++)
    {
      std::cout << "-2 P1: " << p.print() << ", P2: " << points[i].print() << ", P3: " << points[j].print() << std::endl;
      if(points[j]==CPoint(4,5,2)) std::cout << "ALARM 1 **************" << std::endl;
      //std::cout << "  x04" << std::endl;
      if(points[j]==p)
      {
        //std::cout << "-2 P1: " << p.print() << ", P2: " << points[i].print() << ", P3: " << points[j].print() << std::endl;
        continue;
      }
      //std::cout << "  x05" << std::endl;
      CPlane pl = getPlane(p, points[i], points[j]);
      if(pl.x==0 && pl.y==0 && pl.z==0)
      {
        //std::cout << "-3 P1: " << p.print() << ", P2: " << points[i].print() << ", P3: " << points[j].print() << std::endl;
        continue;
      }
      //std::cout << "  x06" << std::endl;
      point_count++;
      for(long k=limit2; k<limit1;)
      {
        //std::cout << "  x07 limit1:" << limit1 << std::endl;
        if(pointIsInPlane(points[k], pl)==true)
        {
          //std::cout << "  x08" << std::endl;
          swap(k, limit1-1);
          //std::cout << "  x09" << std::endl;
          limit1--;
          point_count++;
        }
        else
          k++;
      }
    }
  }
  std::cout << " -- getValueCount(1):" << getValueCount(1) << std::endl;
  return point_count;
}

bool CGrid::isPlane(CPoint p1, CPoint p2, CPoint p3)
{
  CPlane pl = getPlane(p1, p2, p3);
  if(pl.x==0 && pl.y==0 && pl.z==0)
  {
    //std::cout << "P1: " << p.print() << ", P2: " << points[i].print() << ", P3: " << points[j].print() << std::endl;
    return false;
  }
  return true;
}

std::string CGrid::printValues( long value )
{
  std::ostringstream oss;
  if(value==2)
  {
    for(long i=0; i<limit2; i++)
    {
        oss << points[i].print() << ",";
    }
  }
  return oss.str();
}
  /*
std::string CGrid::print( void )
{
  std::ostringstream oss;
  for(std::vector<CPoint>::iterator it = points.begin();
      it != points.end();
      it++)
  {
    oss << getValue(*it);
  }
  return oss.str();
}*/

std::string CGrid::getProcessing(long long gc, double s)
{
  std::ostringstream oss;
  std::cout << "gc: " << gc << ", seconds: " << s << std::endl;
  double rate = ((double)gc)/s;
  std::string unit = " C/s";
  if(rate>1000)
  {
    rate=rate/1000;
    unit = " KC/s";
  }
  if(rate>1000)
  {
    rate=rate/1000;
    unit = " MC/s";
  }
  if(rate>1000)
  {
    rate=rate/1000;
    unit = " GC/s";
  }
  if(rate>1000)
  {
    rate=rate/1000;
    unit = " TC/s";
  }
  
  oss << rate << unit;
    
  return oss.str();
}
long CGrid::first_recur(long depth, unsigned long &record, bool scout, int inc, int scouting_depth)
{
  for(long i=limit2;i<limit1-2;i+=inc)
  {
    for(long j=i+1;j<limit1-1;j+=inc)
    {
      for(long k=j+1; k<limit1; k+=inc)
      {
        if(isPlane(points[i], points[j], points[k]))
        {
          long limit1_tmp = limit1;
          swap(i, limit2);
          limit2++;
          if(limit1<limit2) limit1=limit2;
          swap(j, limit2);
          limit2++;
          if(limit1<limit2) limit1=limit2;
          swap(k, limit2);
          limit2++;
          if(limit1<limit2) limit1=limit2;
          markPlanes(points[limit2-1]);
          recur(0, record, scout, inc, scouting_depth);
          limit1=limit1_tmp;
          swap(k, limit2-1);
          limit2--;
          swap(j, limit2-1);
          limit2--;
          swap(i, limit2-1);
          limit2--;
        }
      }
    }
  }
  return 0;
}

long CGrid::recur(long depth, unsigned long &record, bool scout, int inc, int scouting_depth)
{
  long tmp_rec=0;
  //scouting(0, inc, scouting_depth);
  //std::sort(points.begin()+limit2, points.begin()+limit1, comp_points);
  
  
  for(int i=limit2; i<limit1; i++)
  {
    std::cout << "i:" << i << ", " << points[i].print() << ", count:" << points[i].count << std::endl;
  }
  std::cout << printValues(2);
  std::cout << "\n[R] points: " << getValueCount(2) << ", record: " << record << ", points left: " << getValueCount(1) << ". ";
  std::cout << "******************************************************************************" << std::endl;
  return 0;
  
  for(long i=limit2; i<limit1; i+=inc)
  {
    long limit1_tmp = limit1;
    swap(i, limit2);
    limit2++;
    if(limit1<limit2) limit1=limit2;
    
    markPlanes(points[limit2-1]);
    
    if(record<getValueCount(2))
    {
      record=getValueCount(2);
      std::cout << "0: " << getValueCount(0) << ", limit1: " << getValueCount(1) << ", limit2: " << getValueCount(2) << std::endl;
      std::cout << printValues(2);
      std::cout << "\n[R] points: " << getValueCount(2) << ", record: " << record << ", points left: " << getValueCount(1) << ". ";
      end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end-start;
      std::time_t end_time = std::chrono::system_clock::to_time_t(end); 
      std::cout << "+ finished computation at " << std::ctime(&end_time) << "    elapsed time: " << elapsed_seconds.count() 
                << "s" << "    processing: " << getProcessing(gc, elapsed_seconds.count()) << std::endl;
      gc=0;
      start = std::chrono::system_clock::now();
    }
    recur(depth+1, record, true, inc, scouting_depth);
    swap(i, limit2-1);
    limit2--;
    limit1=limit1_tmp;
  }
  return tmp_rec;
}

long CGrid::scouting(int depth, int inc, int scouting_depth)
{
  long ret=0;
  std::cout << "\nm1 (" << depth << "), ";
  for(long i=limit2; i<limit1; i+=inc)
  {
    for(unsigned long x=0; x<points.size(); x++)
    {
      if(points[x]==CPoint(4,5,2))
      {
        std::cout << "FOUND!: " << x << ", limit2: " << limit2 << ", limit1:" << limit1 << std::endl;
      }
    }
    if(points[i]==CPoint(4,5,2)) std::cout << "ALARM *********************" << std::endl;
  std::cout << "\nm2, ";
    long limit1_tmp = limit1;
    long res=0;
    swap(i, limit2);
    limit2++;
    if(limit1<limit2) limit1=limit2;
    
    markPlanes(points[limit2-1]);
    if(scouting_depth==depth)
    {
      std::cout << ", values... limit1: " << limit1 << ", limit2: " << limit2 << ", getValueCount(1):" << getValueCount(1) << ", getValueCount(2):" << getValueCount(2) << std::endl;
      std::cout << "m3, ";
      res = getValueCount(1);
    }
    else
      res = scouting(depth+1, inc, scouting_depth);
    swap(i, limit2-1);
    limit2--;
    limit1=limit1_tmp;
    
    if(depth==0)
    {
  std::cout << "m4, ";
      points[i].count=res;
    }
    else if( res > ret)
    {
  std::cout << "m5, ";
      ret = res;
    }
  }
  std::cout << ", ret:" << ret << std::endl;
  return ret;
}