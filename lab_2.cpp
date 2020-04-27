#include <Windows.h>
#include <cmath>
#include "lab_2.h"

int Win_width, Win_height;

const int MARGIN = 10;

void SetWindowSize(int _Width, int _Height) {
  Win_width = _Width;
  Win_height = _Height;
}

void Transform(double* x, double* y)
{
  *x = MARGIN + (1.0 / 2) * ((*x) + 1) * (Win_width - 2 * MARGIN);
  *y = MARGIN + (-1.0 / 2) * ((*y) - 1) * (Win_height - 2 * MARGIN);
}

void Object::DotsCount(double x[], double y[], int i)
{
  x[0] = -trace.length[i] / 2 * cos(trace.angle[i]) - width / 2 * sin(trace.angle[i]) + trace.x[i];
  y[0] = -trace.length[i] / 2 * sin(trace.angle[i]) + width / 2 * cos(trace.angle[i]) + trace.y[i];
  x[1] = -trace.length[i] / 2 * cos(trace.angle[i]) + width / 2 * sin(trace.angle[i]) + trace.x[i];
  y[1] = -trace.length[i] / 2 * sin(trace.angle[i]) - width / 2 * cos(trace.angle[i]) + trace.y[i];
  x[2] = trace.length[i] / 2 * cos(trace.angle[i]) - width / 2 * sin(trace.angle[i]) + trace.x[i];
  y[2] = trace.length[i] / 2 * sin(trace.angle[i]) + width / 2 * cos(trace.angle[i]) + trace.y[i];
  x[3] = trace.length[i] / 2 * cos(trace.angle[i]) + width / 2 * sin(trace.angle[i]) + trace.x[i];
  y[3] = trace.length[i] / 2 * sin(trace.angle[i]) - width / 2 * cos(trace.angle[i]) + trace.y[i];
  x[4] = (trace.length[i] / 2 + dot_len) * cos(trace.angle[i]) + trace.x[i];
  y[4] = (trace.length[i] / 2 + dot_len) * sin(trace.angle[i]) + trace.y[i];
  for (int j = 0; j < 5; j++)
    Transform(&x[j], &y[j]);
}

void operator<<(HDC& hdc, Object& obj)
{
  for (unsigned int i = 0; i < obj.trace.angle.size(); i++)
  {
    double x[5], y[5];
    obj.DotsCount(x, y, i);
    MoveToEx(hdc, (int)x[0], (int)y[0], NULL);
    LineTo(hdc, (int)x[1], (int)y[1]);
    MoveToEx(hdc, (int)x[0], (int)y[0], NULL);
    LineTo(hdc, (int)x[2], (int)y[2]);
    MoveToEx(hdc, (int)x[3], (int)y[3], NULL);
    LineTo(hdc, (int)x[1], (int)y[1]);
    MoveToEx(hdc, (int)x[3], (int)y[3], NULL);
    LineTo(hdc, (int)x[4], (int)y[4]);
    MoveToEx(hdc, (int)x[2], (int)y[2], NULL);
    LineTo(hdc, (int)x[4], (int)y[4]);
  }
}

Object& Object::operator+=(double speed_unit)
{
  speed += 2 * speed_unit;
  return *this;
}

Object& Object::operator-=(double speed_unit)
{
  speed -= 4 * speed_unit;
  return *this;
}

Object& Object::operator<<(double angle)
{
  for (unsigned int i = 0; i < trace.angle.size(); i++)
    trace.angle[i] += angle / (i + 1);
  return *this;
}

Object& Object::operator>>(double angle)
{
  for (unsigned int i = 0; i < trace.angle.size(); i++)
    trace.angle[i] -= angle / (i + 1);
  return *this;
}

Object& Object::operator++(void)
{
  for (unsigned int i = 0; i < trace.length.size(); i++)
    if (trace.length[i] <= 10 * dot_len)
      trace.length[i] += dot_len * (1 - i * 0.01);
  return *this;
}

Object& Object::operator--(void)
{
  for (unsigned int i = 0; i < trace.length.size(); i++)
    if (trace.length[i] > dot_len)
      trace.length[i] -= dot_len * (1 - i * 0.01);
  return *this;
}

void Object::IsHit(void)
{
  if (trace.x[0] >= 1 || trace.y[0] >= 1 || trace.x[0] <= -1 || trace.y[0] <= -1)
    trace.angle[0] += PI;
  if (trace.x[0] < -1)
    trace.x[0] = -1;
  if (trace.y[0] < -1)
    trace.y[0] = -1;
  if (trace.x[0] > 1)
    trace.x[0] = 1;
  if (trace.y[0] > 1)
    trace.y[0] = 1;
}

void Object::Demping(void)
{
  if (speed > 0)
    speed -= speed_unit;
  if (speed < 0)
    speed += speed_unit;
}

void Object::DataUpdate(void)
{
  trace.x[0] += speed * cos(trace.angle[0]);
  trace.y[0] += speed * sin(trace.angle[0]);
  IsHit();
  Demping();
  for (unsigned int i = trace.angle.size() - 1; i > 0; i--)
  {
    trace.x[i] = trace.x[i - 1];
    trace.y[i] = trace.y[i - 1];
    trace.length[i] = trace.length[i - 1];
    trace.angle[i] = trace.angle[i - 1];
  }
}