#include <vector>
#include <map>
#include <string>

class CPoint
{
  public:
  CPoint(short  i, short  j, short  k, short c=0);
  const std::string print( void ) const;
  //std::string print2( void );
  long getValue2();
  void setValue2(long value);
  short  x, y, z, count;
  bool operator==(const CPoint &p1) const;
  bool operator<(const CPoint &p1) const;
  
  private:
  //long enabled;
};
class CPlane
{
  public:
  CPlane( void );
  const std::string print( void ) const;
  long addCount(long n);
  bool operator<(const CPlane &p1) const;
  bool operator==(const CPlane &p1) const;
  long x, y, z, d, count;
};
class CGrid
{
  public:
  CGrid( short n);
  CGrid( CGrid *g );
  std::string printValues( long value );
  //std::string print( void );
  void printGrid( void );
  long markPlanes(CPoint p);
  void shuffle();
  CPoint *getPointPtr(CPoint p);
  CPlane getPlane(CPoint p1, CPoint p2, CPoint p3);
  CPlane getPlane(unsigned long n);
  bool pointIsInPlane(CPoint p, CPlane pl);
  //void setValue(CPoint p, long value);
  //long getValue(CPoint p, int dbg=0);
  void addPlane(CPlane pl);
  void deletePoint(unsigned long i);
  void swap(long i, long j);
  unsigned long getValueCount(long n);
  unsigned long getPointsSize( void );
  unsigned long getPlanesSize( void );
  long recur(long depth, unsigned long &record, bool scout, int inc, int scouting_depth);
  long scouting(int depth, int inc, int scouting_depth);
  long first_recur(long depth, unsigned long &record, bool scout, int inc, int scouting_depth);
  std::string getProcessing(long long global_counter, double s);
  void printPlanes( void );
  bool isPlane(CPoint p1, CPoint p2, CPoint p3);
  private:
  std::vector<CPoint> points;
  std::map<CPlane, long> planes;
  long grid_size;
  long limit1, limit2;
  CPlane planes_grid[100][100][100];
};