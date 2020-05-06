#include "lab_3.h"

Window* MainPtr = 0;

Window* DialogPtr = 0;

void ShowScreen(Window* main, Window* dialog, unsigned int Item) { main->SetCurrentScreen(Item); }

void MainDraw(void) { MainPtr->Draw(); }

void DialogDraw(void) { DialogPtr->Draw(); }

void MainMouse(int button, int state, int x, int y) { MainPtr->Mouse(button, state, x, y); }

void DialogMouse(int button, int state, int x, int y) { DialogPtr->Mouse(button, state, x, y); }

void MainPassiveMouse(int x, int y) { MainPtr->PassiveMouse(x, y); }

void DialogPassiveMouse(int x, int y) { DialogPtr->PassiveMouse(x, y); }

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);

  unsigned int ID;
  int MainLength = 700, MainWidth = 9 * MainLength / 16, xPos = 50, yPos = 50;
  string MainTitle = "Lab_3";

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(MainLength, MainWidth);
  glutInitWindowPosition(xPos, yPos);
  ID = glutCreateWindow(MainTitle.data());

  Window Main(MainTitle, MainLength, MainWidth, { xPos, yPos }, ID, mainWin);
  MainPtr = &Main;
  glutDisplayFunc(MainDraw);
  glutMouseFunc(MainMouse);
  glutPassiveMotionFunc(MainPassiveMouse);

  int DialogWidth = 200, DialogLength = 16 * DialogWidth / 9;
  xPos = (MainLength - DialogLength) / 2 + xPos;
  yPos = (MainWidth - DialogWidth) / 2 + yPos;
  string DialogTitle = "Message Window";

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(DialogLength, DialogWidth);
  glutInitWindowPosition(xPos, yPos);
  ID = glutCreateWindow(DialogTitle.data());;

  Window Dialog(DialogTitle, DialogLength, DialogWidth, { xPos, yPos }, ID, dialogWin);
  DialogPtr = &Dialog;
  Dialog.HideWindow();
  glutDisplayFunc(DialogDraw);
  glutMouseFunc(DialogMouse);
  glutPassiveMotionFunc(DialogPassiveMouse);

  Code content;
  if (!content.ReadFile(argv[1]))
    ErrorMessage(1);
  ErrorMessage(content.Translator(Main, Dialog));

  glutMainLoop();

  return 0;
}