#include "graphics.h"
#include "lab_3.h"

extern Manager Windows;

void ShowScreen(unsigned int ID)
{
  Windows.main.SetCurrentScreen(ID);
}

unsigned int CreateID(void)
{
  static set<unsigned int> IDs;
  unsigned int ID;
  set<unsigned int>::iterator res = IDs.end();
  if (IDs.empty())
    IDs.insert(0);
  srand(time(0));
  do
  {
    ID = rand();
    res = IDs.find(ID);

  } while (res != IDs.end());
  IDs.insert(ID);
  return ID;
}

void Manager::SetMain(Window& win)
{
  main = win;
  workingWindow = main.ID;
}

void Manager::SetDialog(Window& win)
{
  dialog = win;
}

Window::Window(const char Name[], int len, int wid, point pos, unsigned int id)
{
  name = Name;
  position = pos;
  length = len;
  width = wid;
  ID = id;
  currentScreen = 0;

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, length, width, 0, 10, -10);
}

void Window::SetBackground(GlutRGB lTop, GlutRGB  rTop, GlutRGB  rBttm, GlutRGB lBttm)
{
  leftTop = lTop;
  rightTop = rTop;
  rightBottom = rBttm;
  leftBottom = lBttm;
  glBegin(GL_QUADS);
  glColor3d(leftTop.red, leftTop.green, leftTop.blue);
  glVertex3d(0, 0, -10);
  glColor3d(rightTop.red, rightTop.green, rightTop.blue);
  glVertex3d(length, 0, -10);
  glColor3d(rightBottom.red, rightBottom.green, rightBottom.blue);
  glVertex3d(length, width, -10);
  glColor3d(leftBottom.red, leftBottom.green, leftBottom.blue);
  glVertex3d(0, width, -10);
  glEnd();
}

void Window::SetWindow(void)
{
  glutSetWindow(ID);
}

void Window::ShowWindow(void)
{
  if (glutGetWindow() != ID)
    glutSetWindow(ID);
  glutShowWindow();
}

void Window::HideWindow(void)
{
  if (glutGetWindow() != ID)
    glutSetWindow(ID);
  glutHideWindow();
}

void Window::AddScreen(Screen scrn)
{
  screens.push_back(scrn);
  if (screens.size() == 1)
    currentScreen = scrn.ID;
}

bool Window::SetCurrentScreen(unsigned int ID)
{
  for (auto i : screens)
    if (ID == i.ID)
    {
      currentScreen = i.ID;
      return true;
    }
  return false;
}

vector<Screen>::iterator Window::GetCurrentScreen(void)
{
  for (vector<Screen>::iterator i =screens.begin(); i!=screens.end(); i++)
    if (currentScreen == i->ID)
      return i;
  return screens.end();
}

Inscription::Inscription(string cntnt, point loc, GlutRGB clor) : content(cntnt), location(loc), color(clor)
{
  ID = CreateID();
  length = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)content.data());
  int n = 1;
  if (content.size() < 2)
    n = content.size();
  width = glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, content[n]);
}

void Inscription::Draw(void)
{
  glBegin(GL_BITMAP);
  glColor3d(color.red, color.green, color.blue);
  glRasterPos3d(location.x, location.y, 10);
  for (auto c : content)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
  glEnd();
}

Button::Button(point Ltop, point rBttm, GlutRGB clor, const char nme[], void(*func)(unsigned int ScreenID), unsigned int targetScreen, string dialMessge) : leftTop(Ltop), rightBottom(rBttm), color(clor), dialogMessage(dialMessge), function(func), targetID(targetScreen)
{
  ID = CreateID();
  onOFF = 127 / 255.0;
  string str = nme;
  Inscription temp(nme, { 0, 0 }, { 0, 0, 0 });
  temp.location.x = (rightBottom.x - leftTop.x - temp.length) / 2 + leftTop.x;
  temp.location.y = (-leftTop.y + rightBottom.y - temp.width) / 2 + leftTop.y + temp.width;
  name = temp;
}

void Button::Draw(void)
{
  glBegin(GL_QUADS);
  glColor3d(color.red + onOFF, color.green + onOFF, color.blue + onOFF);
  glVertex3d(leftTop.x, leftTop.y, 0);
  glVertex3d(rightBottom.x, leftTop.y, 0);
  glColor3d(color.red, color.green, color.blue);
  glVertex3d(rightBottom.x, rightBottom.y, 0);
  glVertex3d(leftTop.x, rightBottom.y, 0);
  glEnd();
  name.Draw();
}

bool Button::MousePosition(point pos)
{
  if (pos.x >= leftTop.x && pos.x <= rightBottom.x && pos.y >= leftTop.y && pos.y <= rightBottom.y)
    return true;
  return false;
}

void Button::FontColor(point pos)
{
  if (MousePosition(pos))
    name.color = { 255, 255, 255 };
  else
    name.color = { 0,0,0 };
}

void Button::Click (int button, int state, point pos)
{
  if (MousePosition(pos) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    onOFF *= -1;
  if (MousePosition(pos) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
  {
    onOFF *= -1;
    if (DialogWindow())
      function(targetID);
  }
}

void Button::onOFFUpdate(point pos)
{
  if (!MousePosition(pos) && onOFF < 0)
    onOFF *= -1;
}

void noFunc(unsigned int Item)
{
  Windows.dialog.HideWindow();
  Windows.workingWindow = Windows.main.ID;
}

void (*inputFunction)(unsigned int ID) = 0;

void yesFunc(unsigned int Item)
{
  noFunc(0);
  if (inputFunction != 0)
    inputFunction(Item);
  inputFunction = 0;
}

bool Button::DialogWindow(void)
{
  if (dialogMessage == "")
    return true;
  inputFunction = function;
  DialogScreenContent(dialogMessage, yesFunc, targetID, noFunc);
  Windows.dialog.ShowWindow();
  Windows.workingWindow = Windows.dialog.ID;
  return false;
}

Screen::Screen()
{
  ID = CreateID();
}

void Screen::AddButton(Button but)
{
  buttons.push_back(but);
}

void Screen::AddInscription(Inscription inscrpt)
{
  inscriptors.push_back(inscrpt);
}