#include "graphics.h"

void DialogContent(Window* win, Window* mainWin, string message, void(*yesFunc)(Window* main, Window* dialog, unsigned int Item), unsigned int targetScreen, void(*noFunc)(Window* main, Window* dialog, unsigned int Item))
{
  int bWidth = 50;
  Screen content;
  Inscription temp(message);
  int dy = (win->width - temp.Width() - bWidth) / 3;
  int dx = (win->length - 2 * 2 * bWidth) / 3;
  temp.SetLocation({ (win->length - temp.Length()) / 2, dy });
  content.AddInscription(temp);
  Button yes({ dx, 2 * dy + temp.Width() }, { dx + 2 * bWidth, 2 * dy + temp.Width() + bWidth }, { 105, 105, 105 }, "yes", yesFunc, targetScreen, "", win, mainWin);
  Button no({ win->length - dx - 2 * bWidth, 2 * dy + temp.Width() }, { win->length - dx,2 * dy + temp.Width() + bWidth }, { 105,105, 105 }, "no", noFunc, targetScreen, "", win, mainWin);
  content.AddButton(yes);
  content.AddButton(no);
  win->screens.clear();
  win->AddScreen(content);
}

unsigned int CreateID(void)
{
  static set<unsigned int> IDs;
  unsigned int ID;
  auto res = IDs.end();
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

Window::Window(string Name, int len, int wid, point pos, unsigned int id, WindowType tpe)
{
  name = Name;
  position = pos;
  length = len;
  width = wid;
  ID = id;
  currentScreen = 0;
  type = tpe;

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
    currentScreen = scrn.GetID();
}

bool Window::SetCurrentScreen(unsigned int ID)
{
  for (auto i : screens)
    if (ID == i.GetID())
    {
      currentScreen = i.GetID();
      return true;
    }
  return false;
}

Screen& Window::GetCurrentScreen(void)
{
  int i = 0;
  for (auto scrn : screens)
  {
    if (currentScreen == scrn.GetID())
      return screens[i];
    i++;
  }
  return *screens.begin();
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

Button::Button(point Ltop, point rBttm, GlutRGB clor, const char nme[], void(*func)(Window* main, Window* dialog, unsigned int ScreenID), unsigned int targetScreen, string dialMessge, Window* dialogWindw, Window* mainWindw) : leftTop(Ltop), rightBottom(rBttm), color(clor), dialogMessage(dialMessge), function(func), targetID(targetScreen), dialogWindow(dialogWindw), mainWindow(mainWindw)
{
  ID = CreateID();
  onOFF = 127 / 255.0;
  string str = nme;
  Inscription temp(nme, { 0, 0 }, { 0, 0, 0 });
  int x = (rightBottom.x - leftTop.x - temp.Length()) / 2 + leftTop.x;
  int y = (-leftTop.y + rightBottom.y - temp.Width()) / 2 + leftTop.y + temp.Width();
  temp.SetLocation({ x, y });
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
    name.SetColor({ 255, 255, 255 });
  else
    name.SetColor({ 0,0,0 });
}

void Button::Click (int button, int state, point pos)
{
  if (MousePosition(pos) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    onOFF *= -1;
  if (MousePosition(pos) && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
  {
    onOFF *= -1;
    if (DialogWindow() && function != 0)
      function(mainWindow, dialogWindow, targetID);
  }
}

void Button::onOFFUpdate(point pos)
{
  if (!MousePosition(pos) && onOFF < 0)
    onOFF *= -1;
}

void noFunc(Window* main, Window* dialog, unsigned int Item)
{
  dialog->HideWindow();
  main->SetWindow();
}

void yesFunc(Window* main, Window* dialog, unsigned int Item)
{
  noFunc(main, dialog, Item);
  main->SetCurrentScreen(Item);
}

bool Button::DialogWindow(void)
{
  if (dialogMessage == "")
    return true;
  DialogContent(dialogWindow, mainWindow, dialogMessage, yesFunc, targetID, noFunc);
  dialogWindow->ShowWindow();
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

void Button::NamePositionUpdate(void)
{
  int x = (rightBottom.x - leftTop.x - name.Length()) / 2 + leftTop.x;
  int y = (-leftTop.y + rightBottom.y - name.Width()) / 2 + leftTop.y + name.Width();
  name.SetLocation({ x, y });
}

void Window::Draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  switch (type)
  {
  case mainWin: SetBackground({ 123, 104, 238 }, { 123, 104, 238 }, { 176, 196, 222 }, { 176, 196, 222 }); break;
  case dialogWin: SetBackground({ 211, 211, 211 }, { 211, 211, 211 }, { 211, 211, 211 }, { 211, 211, 211 }); break;
  }
  Screen& current = GetCurrentScreen();
  for (auto button : current.buttons)
    button.Draw();
  for (auto inscrptr : current.inscriptors)
    inscrptr.Draw();
  glutSwapBuffers();
}

void Window::Mouse(int button, int state, int x, int y)
{
  Screen& current = GetCurrentScreen();
  for (vector<Button>::iterator i = current.buttons.begin(); i != current.buttons.end(); i++)
    i->Click(button, state, { x, y });
  glutPostRedisplay();
}

void Window::PassiveMouse(int x, int y)
{
  Screen& current = GetCurrentScreen();
  for (vector<Button>::iterator i = current.buttons.begin(); i != current.buttons.end(); i++)
  {
    i->FontColor({ x, y });
    i->onOFFUpdate({ x, y });
  }
  glutPostRedisplay();
}