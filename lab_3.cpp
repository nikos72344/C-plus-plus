#include "graphics.h"
#include "input.h"

Manager Windows;

void DialogScreenContent(string message, void(*yesFunc)(unsigned int ScreenID), unsigned int targetScreen, void(*noFunc)(unsigned int none))
{
  int bWidth = 50;
  Screen content;
  Inscription temp(message);
  int dy = (Windows.dialog.width - temp.width - bWidth) / 3;
  int dx = (Windows.dialog.length - 2 * 2 * bWidth) / 3;
  temp.location = { (Windows.dialog.length - temp.length) / 2, dy };
  content.inscriptors.push_back(temp);
  Button yes({ dx, 2 * dy + temp.width }, { dx + 2 * bWidth, 2 * dy + temp.width + bWidth }, { 105, 105, 105 }, "yes", yesFunc, targetScreen);
  Button no({ Windows.dialog.length - dx - 2 * bWidth, 2 * dy + temp.width }, { Windows.dialog.length - dx,2 * dy + temp.width + bWidth }, { 105,105, 105 }, "no", noFunc, 0);
  content.AddButton(yes);
  content.AddButton(no);
  Windows.dialog.screens.clear();
  Windows.dialog.AddScreen(content);
}

void MainWindow(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  Windows.main.SetBackground({ 123, 104, 238 }, { 123, 104, 238 }, { 176, 196, 222 }, { 176, 196, 222 });
  vector<Screen>::iterator scrn = Windows.main.GetCurrentScreen();
  for (auto button : scrn->buttons)
    button.Draw();
  for (auto inscrptr : scrn->inscriptors)
    inscrptr.Draw();
  glutSwapBuffers();
}

void DialogWindow(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  Windows.dialog.SetBackground({ 211, 211, 211 }, { 211, 211, 211 }, { 211, 211, 211 }, { 211, 211, 211 });
  vector<Screen>::iterator scrn = Windows.dialog.GetCurrentScreen();
  for (auto button : scrn->buttons)
    button.Draw();
  for (auto inscrptr : scrn->inscriptors)
    inscrptr.Draw();
  glutSwapBuffers();
}

void MouseFunc(int button, int state, int x, int y)
{
  vector<Screen>::iterator current;
  if (Windows.workingWindow == Windows.main.ID)
    current = Windows.main.GetCurrentScreen();
  if (Windows.workingWindow == Windows.dialog.ID)
    current = Windows.dialog.GetCurrentScreen();
  for (vector<Button>::iterator i = current->buttons.begin(); i != current->buttons.end(); i++)
    i->Click(button, state, { x, y });
  glutPostRedisplay();
}

void PassiveMouseFunc(int x, int y)
{
  vector<Screen>::iterator current;
  if (Windows.workingWindow == Windows.main.ID)
    current = Windows.main.GetCurrentScreen();
  if (Windows.workingWindow == Windows.dialog.ID)
    current = Windows.dialog.GetCurrentScreen();
  for (vector<Button>::iterator i = current->buttons.begin(); i != current->buttons.end(); i++)
  {
    i->FontColor({ x, y });
    i->onOFFUpdate({ x, y });
  }
  glutPostRedisplay();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);

  unsigned int ID;
  int length = 700, width = 9 * length / 16, xPos = 50, yPos = 50;

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(length, width);
  glutInitWindowPosition(xPos, yPos);
  ID = glutCreateWindow("Lab_3");

  Window Main("Lab_3", length, width, { xPos, yPos }, ID);
  Windows.SetMain(Main);
  glutDisplayFunc(MainWindow);
  glutMouseFunc(MouseFunc);
  glutPassiveMotionFunc(PassiveMouseFunc);

  width = 200;
  length = 16 * width / 9;
  xPos = (Main.length - length) / 2 + Main.position.x;
  yPos = (Main.width - width) / 2 + Main.position.y;

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(length, width);
  glutInitWindowPosition(xPos, yPos);
  ID = glutCreateWindow("Message Window");

  Window Dialog("Message Window", length, width, { xPos, yPos }, ID);
  Windows.SetDialog(Dialog);
  Dialog.HideWindow();

  glutDisplayFunc(DialogWindow);
  glutMouseFunc(MouseFunc);
  glutPassiveMotionFunc(PassiveMouseFunc);

  Code content;
  if (!content.ReadFile(argv[1]))
    ErrorMessage(1);
  ErrorMessage(content.Translator());

  glutMainLoop();

  return 0;
}