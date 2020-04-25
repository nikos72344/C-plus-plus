#include <Windows.h>
#include <cmath>
#include "lab_2.h"

const double PI = 3.1415926535897932384626433832795;

int Win_width, Win_height;

const int MARGIN = 10;

void SetWindowSize(int _Width, int _Height) {
  Win_width = _Width;
  Win_height = _Height;
}

Object::point Object::Transform(point pos)
{
  point Tpoint;
  Tpoint.x = MARGIN + (1.0 / 2) * (pos.x + 1) * (Win_width - 2 * MARGIN);
  Tpoint.y = MARGIN + (-1.0 / 2) * (pos.y - 1) * (Win_height - 2 * MARGIN);
  return Tpoint;
}

void Object::Gain(void)
{
  for (unsigned int i = 0; i < length.size(); i++)
    if (length[i] <= 10 * dot_len)
      length[i] += dot_len * (1 - i * 0.01);
}

void Object::Shrink(void)
{
  for (unsigned int i = 0; i < length.size(); i++)
    if (length[i] > dot_len)
      length[i] -= dot_len * (1 - i * 0.01);
}

void Object::DataUpdate(void)
{
  position[0].x += speed * cos(curr_angle[0]);
  position[0].y += speed * sin(curr_angle[0]);
  if (position[0].x >= 1 || position[0].y >= 1 || position[0].x <= -1 || position[0].y <= -1)
    curr_angle[0] += PI;
  if (position[0].x < -1)
    position[0].x = -1;
  if (position[0].y < -1)
    position[0].y = -1;
  if (position[0].x > 1)
    position[0].x = 1;
  if (position[0].y > 1)
    position[0].y = 1;
  if (speed > 0)
    speed -= speed_unit;
  if (speed < 0)
    speed += speed_unit;
  for (unsigned int i = position.size() - 1; i > 0; i--)
  {
    position[i] = position[i - 1];
    length[i] = length[i - 1];
    curr_angle[i] = curr_angle[i - 1];
  }
}

void Object::Draw(HDC hdc)
{ 
  for (unsigned int i = 0; i < position.size(); i++)
  {
    point pos[5];
    pos[0].x = -length[i] / 2 * cos(curr_angle[i]) - width / 2 * sin(curr_angle[i]) + position[i].x;
    pos[0].y = -length[i] / 2 * sin(curr_angle[i]) + width / 2 * cos(curr_angle[i]) + position[i].y;
    pos[1].x = -length[i] / 2 * cos(curr_angle[i]) + width / 2 * sin(curr_angle[i]) + position[i].x;
    pos[1].y = -length[i] / 2 * sin(curr_angle[i]) - width / 2 * cos(curr_angle[i]) + position[i].y;
    pos[2].x = length[i] / 2 * cos(curr_angle[i]) - width / 2 * sin(curr_angle[i]) + position[i].x;
    pos[2].y = length[i] / 2 * sin(curr_angle[i]) + width / 2 * cos(curr_angle[i]) + position[i].y;
    pos[3].x = length[i] / 2 * cos(curr_angle[i]) + width / 2 * sin(curr_angle[i]) + position[i].x;
    pos[3].y = length[i] / 2 * sin(curr_angle[i]) - width / 2 * cos(curr_angle[i]) + position[i].y;
    pos[4].x = (length[i] / 2 + dot_len) * cos(curr_angle[i]) + position[i].x;
    pos[4].y = (length[i] / 2 + dot_len) * sin(curr_angle[i]) + position[i].y;
    for (int j = 0; j < 5; j++)
    {
      pos[j] = Transform(pos[j]);
    }
    MoveToEx(hdc, (int)pos[0].x, (int)pos[0].y, NULL);
    LineTo(hdc, (int)pos[1].x, (int)pos[1].y);
    MoveToEx(hdc, (int)pos[0].x, (int)pos[0].y, NULL);
    LineTo(hdc, (int)pos[2].x, (int)pos[2].y);
    MoveToEx(hdc, (int)pos[3].x, (int)pos[3].y, NULL);
    LineTo(hdc, (int)pos[1].x, (int)pos[1].y);
    MoveToEx(hdc, (int)pos[3].x, (int)pos[3].y, NULL);
    //LineTo(hdc, (int)pos[2].x, (int)pos[2].y);
    LineTo(hdc, (int)pos[4].x, (int)pos[4].y);
    MoveToEx(hdc, (int)pos[2].x, (int)pos[2].y, NULL);
    LineTo(hdc, (int)pos[4].x, (int)pos[4].y);
  }
}

void Object::KeyRequest(int Key)
{
  switch (Key)
  {
  case 'W':
  case VK_UP: speed += 2 * speed_unit; break;
  case 'S':
  case VK_DOWN: speed -= 4 * speed_unit; break;
  case 'A':
  case VK_LEFT: for (unsigned int i = 0; i < curr_angle.size(); i++) { curr_angle[i] += PI / 18 / (i + 1); } break;
  case 'D':
  case VK_RIGHT: for (unsigned int i = 0; i < curr_angle.size(); i++) { curr_angle[i] -= PI / 18 / (i + 1); } break;
  case 'E': Gain(); break;
  case 'Q': Shrink(); break;
  }
}