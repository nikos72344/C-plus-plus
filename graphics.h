#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <ctime>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <gl/glut.h>

using namespace std;

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

enum WindowType
{
  mainWin,
  dialogWin
};

class Inscription
{
private:
  unsigned int ID;
  point location;
  int length, width;
  GlutRGB color;
  string content;
public:
  Inscription(string cntnt = "", point loc = { 0, 0 }, GlutRGB clor = { 0, 0, 0 });
  void SetLocation(point newLoc) { location = newLoc; }
  void SetColor(GlutRGB newColor) { color = newColor; }
  int Length(void) { return length; }
  int Width(void) { return width; }
  void Draw(void);
};

class Window;

class Button
{
private:
  unsigned int ID;
  point leftTop, rightBottom;
  GlutRGB color;
  GLdouble onOFF;
  Inscription name;
  void (*function)(Window* main, Window* dialog, unsigned int Item);
  unsigned int targetID;
  string dialogMessage;
  Window* dialogWindow;
  Window* mainWindow;
public:
  Button(point Ltop = { 0, 0 }, point rBttm = { 50, 50 }, GlutRGB clor = { 0, 0, 0 }, const char nme[] = "", void(*func)(Window* main, Window* dialog, unsigned int ScreenID) = 0, unsigned int targetScreen = 0, string dialMessge = "", Window* dialogWind = 0, Window* mainWind = 0);
  void Draw(void);
  bool MousePosition(point pos);
  void FontColor(point pos);
  void Click(int button, int state, point pos);
  void onOFFUpdate(point pos);
  bool DialogWindow(void);
  void NamePositionUpdate(void);
  void SetLeftTop(point newPos) { leftTop = newPos; this->NamePositionUpdate(); }
  void SetRightBottom(point newPos) { rightBottom = newPos; this->NamePositionUpdate(); }
  void SetName(Inscription newName) { name = newName; }
  void SetColor(GlutRGB newColor) { color = newColor; }
  void SetWindowPointers(Window* mn, Window* dlg) { mainWindow = mn; dialogWindow = dlg; }
  void SetDialog(string dialog) { dialogMessage = dialog; }
  void SetTarget(unsigned int target) { targetID = target; }
  void SetFunction(void (*func)(Window* main, Window* dialog, unsigned int Item)) { function = func; }
  unsigned int GetID(void) { return ID; }
};

class Screen
{
private:
  unsigned int ID;
public:
  vector<Inscription> inscriptors;
  vector<Button> buttons;
  Screen();
  void AddButton(Button but);
  void AddInscription(Inscription inscrpt);
  unsigned int GetID(void) { return ID; }
};

class Window
{
private:
  unsigned int ID;
  int length, width;
  WindowType type;
  point position;
  string name;
  GlutRGB leftTop, rightTop, rightBottom, leftBottom;
  vector<Screen> screens;
  unsigned int currentScreen;
public:
  Window(string Name = "Window", int len = 500, int  wid = 500, point Pos = { 50, 50 }, unsigned int id = 0, WindowType tpe = mainWin);
  void SetBackground(GlutRGB lTop, GlutRGB  rTop, GlutRGB  rBttm, GlutRGB lBttm);
  void SetWindow(void);
  void ShowWindow(void);
  void HideWindow(void);
  void AddScreen(Screen scrn);
  bool SetCurrentScreen(unsigned int ID);
  Screen& GetCurrentScreen(void);
  void Draw(void);
  void Mouse(int button, int state, int x, int y);
  void PassiveMouse(int x, int y);
  unsigned int GetID(void) { return ID; }
  friend void DialogContent(Window* win, Window* mainWin, string message, void(*yesFunc)(Window* main, Window* dialog, unsigned int Item), unsigned int targetScreen, void(*noFunc)(Window* main, Window* dialog, unsigned int Item));
};

#endif