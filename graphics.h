#ifndef __GRAPHICS
#define __GRAPHICS

#include "input.h"
#include <gl/glut.h>
#include <string>
#include <vector>
#define LENGTH 30
#define WIDTH 10

using namespace std;

const double PI = 3.1415926535897932384626433832795;

struct point
{
  int x, y;
};

enum bonusType
{
  none,
  newBall,
  extension
};

class GlutRGB
{
public:
  GLdouble red, green, blue;
  GlutRGB(GLdouble r = 0, GLdouble g = 0, GLdouble b = 0) : red(r / 255.0), green(g / 255.0), blue(b / 255.0) {}
};

class Title
{
private:
  point location;
  int length, width;
  GlutRGB color;
  string content;
public:
  Title(string cntnt = "", point loc = { 0, 0 }, GlutRGB clor = { 0, 0, 0 });
  void SetLocation(point newLoc) { location = newLoc; }
  void SetColor(GlutRGB newColor) { color = newColor; }
  int Length(void) { return length; }
  int Width(void) { return width; }
  void Draw(void);
};

class Object
{
protected:
  point location;
  double angle;
  double speed;
  GlutRGB up, bottom;
public:
  Object(point Loc, double Angle, GlutRGB Up, GlutRGB Bottom) : location(Loc), angle(Angle), speed(0), up(Up), bottom(Bottom) {}
  point Location(void) { return location; }
  double Angle(void) { return angle; }
  double Speed(void) { return speed; }
  void Calculations(vector<point> Dots);
  virtual void Draw(void) = 0;
};

class Bonus;

class Platform : public Object
{
private:
  int length, width;
public:
  Platform(point Loc = { 0, 0 }, double Angle = PI / 2, GlutRGB Up = { 0.0, 0.0, 0.0 }, GlutRGB Bottom = { 0.0, 0.0, 0.0 }, int Len = LENGTH, int Wid = WIDTH) : Object(Loc, Angle, Up, Bottom), length(Len), width(Wid) {}
  void SetSpeed(double Speed) { speed = Speed; }
  void SetLocation(point Loc) { location = Loc; }
  void SetAngle(double Angle) { angle = Angle; }
  void Draw(void);
  friend Bonus;
};

class Ball : public Object
{
private:
  double radius;
public:
  Ball(point Loc = { 0, 0 }, double Angle = PI / 2, GlutRGB Up = { 0, 0, 0 }, GlutRGB Bottom = { 0, 0, 0 }, double Radius = 5) : Object(Loc, Angle, Up, Bottom), radius(Radius) {}
  void SetSpeed(double Speed) { speed = Speed; }
  void SetLocation(point Loc) { location = Loc; }
  void SetAngle(double Angle) { angle = Angle; }
  void Draw(void);
};

class Bonus : public Object
{
protected:
  bonusType type;
public:
  Bonus(point Loc = { 0, 0 }, double Angle = -PI / 2, GlutRGB Up = { 0, 0, 0 }, GlutRGB Bottom = { 0, 0, 0 }, bonusType Type = none) : Object(Loc, Angle, Up, Bottom), type(Type) {}
  virtual int ApplyBonus(void) = 0;
  void SetSpeed(double Speed) { speed = Speed; }
  void SetLocation(point Loc) { location = Loc; }
  void SetAngle(double Angle) { angle = Angle; }
  void Draw(void);
};

class Level;

class NewBall : public Bonus
{
public:
  NewBall(point Loc = { 0,0 }, double Angle = -PI / 2, GlutRGB Up = { 0,0,0 }, GlutRGB Bottom = { 0, 0, 0 }) : Bonus(Loc, Angle, Up, Bottom, newBall) {}
  void ApplyBonus(Level& Lvl) {}
};

class Extension : public Bonus
{
public:
  Extension(point Loc = { 0,0 }, double Angle = -PI / 2, GlutRGB Up = { 0,0,0 }, GlutRGB Bottom = { 0, 0, 0 }) : Bonus(Loc, Angle, Up, Bottom, extension) {}
  void ApplyBonus(Platform& Pltfrm) {}
};

class Brick : public Object
{
private:
  int length, width;
  int durability;
  Bonus* bonus;
public:
  Brick(point Loc = { 0,0 }, double Angle = PI / 2, GlutRGB Up = { 0, 0, 0 }, GlutRGB Bottom = { 0, 0, 0 }, int Len = LENGTH, int Wid = WIDTH, int Durblty = 1, Bonus* Bnus = 0) : Object(Loc, Angle, Up, Bottom), length(Len), width(Wid), durability(Durblty), bonus(Bnus) {}
  void SetBonus(Bonus* bns) { bonus = bns; }
};

class Screen;

class Input;

class Level
{
private:
  vector<Brick> bricks;
  int rows;
  vector<Platform> platform;
  vector<Ball> balls;
public:
  Level(void) : rows(0) {}
  friend Screen;
  friend Input;
  friend Bonus;
};

class Screen
{
private:
  int winLength, winWidth;
  vector<Level> levels;
  int currentLevel;
  Level GameOver;
public:
  Screen(int wLength, int wWidth) : winLength(wLength), winWidth(wWidth), currentLevel(-1) {}
  void Display(void);
  void Mouse(int button, int state, int x, int y);
  void PassiveMouse(int x, int y);
  void Keyboard(unsigned char key, int x, int y);
  void SpecialKeyboard(int key, int x, int y);
  void PositionUpdate(Object* obj);
  bool IsBump(Object& One, Object& Two);
  void DataUpdate(void);
  int WinLength(void) { return winLength; }
  int WinWidth(void) { return winWidth; }
  friend Input;
};

#endif