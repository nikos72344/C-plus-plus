#ifndef __LAB_2_
#define __LAB_2_

#include <vector>

using namespace std;

const double PI = 3.1415926535897932384626433832795;

const double dot_len = 0.04;

const int trace_num = 15;

const double speed_unit = dot_len;

void SetWindowSize(int _Width, int _Height);

class Object
{
private:
  struct figureVector
  {
    vector<double> x, y, length, angle;
  }trace;
  double width;
  double speed;
  void DotsCount(double x[], double y[], int i);
  void IsHit(void);
  void Demping(void);
public:
  Object() : width(dot_len), speed(0) { for (int i = 0; i < trace_num; i++) { trace.x.push_back(0), trace.y.push_back(0), trace.length.push_back(5 * dot_len); trace.angle.push_back(0); } };
  void DataUpdate(void);
  friend void operator<<(HDC& hdc, Object& obj);
  Object& operator+=(double speed_unit);
  Object& operator-=(double speed_unit);
  Object& operator<<(double angle);
  Object& operator>>(double angle);
  Object& operator++(void);
  Object& operator--(void);
};

#endif