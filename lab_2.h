#ifndef __LAB_2_
#define __LAB_2_

#include <vector>

using namespace std;

const double dot_len = 0.04;

const int trace_num = 15;

const double speed_unit = dot_len;

void SetWindowSize(int _Width, int _Height);

class Object
{
private:
  vector<double> length;
  double width;
  vector<double> curr_angle;
  double speed;
  struct point
  {
    double x, y;
  };
  vector<point> position;
  point Transform(point pos);
  void Gain(void);
  void Shrink(void);
public:
  Object() : width(dot_len), speed(0) { for (int i = 0; i < trace_num; i++) { length.push_back(5 * dot_len); curr_angle.push_back(0); position.push_back({ 0, 0 }); } };
  void DataUpdate(void);
  void Draw(HDC hdc);
  void KeyRequest(int Key);
};

#endif