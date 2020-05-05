#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <ctime>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <gl/glut.h>

using namespace std;

void ShowScreen(unsigned int ID);

class GlutRGB
{
public:
  GLdouble red, green, blue;
  GlutRGB(GLdouble r = 0, GLdouble g = 0, GLdouble b = 0) : red(r / 255.0), green(g / 255.0), blue(b / 255.0) {}
};

struct point
{
  int x, y;
};

class Inscription
{
public:
  unsigned int ID;
  point location;
  int length, width;
  GlutRGB color;
  string content;

  Inscription(string cntnt = "", point loc = { 0, 0 }, GlutRGB clor = { 0, 0, 0 });
  void Draw();
};

class Button
{
public:
  unsigned int ID;
  point leftTop, rightBottom;
  GlutRGB color;
  GLdouble onOFF;
  Inscription name;
  void (*function)(unsigned int Item);
  unsigned int targetID;
  string dialogMessage;

  Button(point Ltop = { 0, 0 }, point rBttm = { 50, 50 }, GlutRGB clor = { 0, 0, 0 }, const char nme[] = "", void(*func)(unsigned int ScreenID) = ShowScreen, unsigned int targetScreen = 0, string dialMessge = "");
  void Draw(void);
  bool MousePosition(point pos);
  void FontColor(point pos);
  void Click(int button, int state, point pos);
  void onOFFUpdate(point pos);
  bool DialogWindow(void);
};

class Screen
{
public:
  unsigned int ID;
  vector<Inscription> inscriptors;
  vector<Button> buttons;

  Screen();
  void AddButton(Button but);
  void AddInscription(Inscription inscrpt);
};

class Window
{
public:
  unsigned int ID;
  int length, width;
  point position;
  string name;
  GlutRGB leftTop, rightTop, rightBottom, leftBottom;
  vector<Screen> screens;
  unsigned int currentScreen;

  Window(const char Name[] = "Window", int len = 500, int  wid = 500, point Pos = { 50, 50 }, unsigned int id = 0);
  void SetBackground(GlutRGB lTop, GlutRGB  rTop, GlutRGB  rBttm, GlutRGB lBttm);
  void SetWindow(void);
  void ShowWindow(void);
  void HideWindow(void);
  void AddScreen(Screen scrn);
  bool SetCurrentScreen(unsigned int ID);
  vector<Screen>::iterator GetCurrentScreen(void);
};

class Manager
{
public:
  Window main, dialog;
  unsigned int workingWindow;

  void SetMain(Window& win);
  void SetDialog(Window& win);
};

#endif