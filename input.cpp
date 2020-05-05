#include "input.h"
#include "graphics.h"

extern Manager Windows;

Screen* tempScreen = 0;

Button* tempButton = 0;

Line::Line(string& str)
{
  string temp;
  for (auto i : str)
  {
    if (i != ' ')
      temp += i;
    else
    {
      words.push_back(temp);
      temp.clear();
    }
  }
  words.push_back(temp);
}

Line& Line::operator+=(string& str)
{
  words.push_back(str);
  return *this;
}

Code& Code::operator+=(string& str)
{
  Line temp(str);
  this->CodeLines.push_back(str);
  return *this;
}

bool Code::ReadFile(const char* File_Name)
{
  ifstream fin(File_Name);
  if (!fin.is_open())
    return false;
  char buff[10];
  string temp;
  while (true)
  {
    fin.getline(buff, 10);
    temp += buff;
    if (fin.eof())
    {
      *this += temp;
      break;
    }
    if (fin.fail())
      fin.clear();
    else
    {
      *this += temp;
      temp.clear();
    }
  }
  fin.close();
  return true;
}

int GetCommand(string& command)
{
  if (command == "SCREEN")
    return 1;
  if (command == "BUTTON")
    return 2;
  if (command == "lt")
    return 3;
  if (command == "rb")
    return 4;
  if (command == "color")
    return 5;
  if (command == "open")
    return 6;
  if (command == "dialog")
    return 7;
  return 0;
}

bool NewScreen(vector<Line>::iterator curr, vector<Screen>& Screens, map<string, unsigned int>& ScreenID)
{
  if (tempScreen != 0)
  {
    if (tempButton != 0)
    {
      tempScreen->AddButton(*tempButton);
      delete tempButton;
      tempButton = 0;
    }
    Screens.push_back(*tempScreen);
    delete tempScreen;
    tempScreen = 0;
  }
  tempScreen = new Screen;
  if (tempScreen == 0)
    return false;
  string tempStr;
  for (unsigned int i = 1; i < curr->words.size() - 1; i++)
  {
    tempStr += curr->words[i];
    tempStr += ' ';
  }
  tempStr += curr->words[curr->words.size() - 1];
  pair<string, unsigned int> data(tempStr, tempScreen->ID);
  ScreenID.insert(data);
  return true;
}

void InscriptionPositionUpdate(void)
{
  tempButton->name.location.x = (tempButton->rightBottom.x - tempButton->leftTop.x - tempButton->name.length) / 2 + tempButton->leftTop.x;
  tempButton->name.location.y = (-tempButton->leftTop.y + tempButton->rightBottom.y - tempButton->name.width) / 2 + tempButton->leftTop.y + tempButton->name.width;
}

bool NewButton(vector<Line>::iterator curr)
{
  if (tempButton != 0)
  {
    tempScreen->AddButton(*tempButton);
    delete tempButton;
    tempButton = 0;
  }
  tempButton = new Button;
  if (tempButton == 0)
    return false;
  string tempStr;
  for (unsigned int i = 1; i < curr->words.size() - 1; i++)
  {
    tempStr += curr->words[i];
    tempStr += ' ';
  }
  tempStr += curr->words[curr->words.size() - 1];
  tempButton->name = tempStr;
  InscriptionPositionUpdate();
  return true;
}

bool SetLeftTopPosition(vector<Line>::iterator curr)
{
  if (curr->words.size() != 3)
    return false;
  int x = atoi(curr->words[1].data());
  int y = atoi(curr->words[2].data());
  tempButton->leftTop = { x, y };
  InscriptionPositionUpdate();
  return true;
}

bool SetRightBottom(vector<Line>::iterator curr)
{
  if (curr->words.size() != 3)
    return false;
  int x = atoi(curr->words[1].data());
  int y = atoi(curr->words[2].data());
  tempButton->rightBottom = { x, y };
  InscriptionPositionUpdate();
  return true;
}

bool SetColor(vector<Line>::iterator curr)
{
  if (curr->words.size() != 4)
    return false;
  GLdouble r = (GLdouble)atoi(curr->words[1].data());
  GLdouble g = (GLdouble)atoi(curr->words[2].data());
  GLdouble b = (GLdouble)atoi(curr->words[3].data());
  tempButton->color = { r, g, b };
  return true;
}

bool SetTarget(vector<Line>::iterator curr,  map<unsigned int, string>& ButtonID)
{
  if (curr->words[1] != "SCREEN")
    return false;
  string tempStr;
  for (unsigned int i = 2; i < curr->words.size() - 1; i++)
  {
    tempStr += curr->words[i];
    tempStr += ' ';
  }
  tempStr += curr->words[curr->words.size() - 1];
  pair<unsigned int, string> data(tempButton->ID, tempStr);
  ButtonID.insert(data);
  tempButton->function = ShowScreen;
  return true;
}

void SetDialog(vector<Line>::iterator curr)
{
  string tempStr;
  for (unsigned int i = 1; i < curr->words.size() - 1; i++)
  {
    tempStr += curr->words[i];
    tempStr += ' ';
  }
  tempStr += curr->words[curr->words.size() - 1];
  tempButton->dialogMessage = tempStr;
}

bool ButtonScreenLink(vector<Screen>& Screens, map<string, unsigned int>& ScreenID, map<unsigned int, string>& ButtonID)
{
  for (vector<Screen>::iterator scrn = Screens.begin(); scrn != Screens.end(); scrn++)
    for (vector<Button>::iterator bttn = scrn->buttons.begin(); bttn != scrn->buttons.end(); bttn++)
    {
      map<unsigned int, string>::iterator butScrn = ButtonID.find(bttn->ID);
      if (butScrn != ButtonID.end())
      {
        map<string, unsigned int>::iterator ScrnID = ScreenID.find(butScrn->second);
        if (ScrnID == ScreenID.end())
          return false;
        bttn->targetID = ScrnID->second;
      }
    }
  return true;
}

void FreeMemory(void)
{
  delete tempButton;
  delete tempScreen;
}

unsigned int Code::Translator(void)
{
  vector<Line>::iterator curr = CodeLines.begin();
  if (curr->words[0] != "SCREEN")
    return 2;
  map<string, unsigned int> ScreenID;
  map<unsigned int, string> ButtonID;
  vector<Screen> Screens;
  vector<Screen>::iterator currScreen = Screens.begin();
  while (curr != CodeLines.end())
  {
    string command = curr->words[0];
    switch (GetCommand(command))
    {
    case 1: if (!NewScreen(curr, Screens, ScreenID)) { FreeMemory(); return 3; } break;
    case 2: if (!NewButton(curr)) { FreeMemory(); return 4; } break;
    case 3: if (!SetLeftTopPosition(curr)) { FreeMemory(); return 5; } break;
    case 4: if (!SetRightBottom(curr)) { FreeMemory(); return 6; } break;
    case 5: if (!SetColor(curr)) { FreeMemory(); return 7; } break;
    case 6: if (!SetTarget(curr, ButtonID)) { FreeMemory(); return 8; } break;
    case 7: SetDialog(curr); break;
    default: return 9;
    }
    curr++;
  }
  tempScreen->AddButton(*tempButton);
  delete tempButton;
  Screens.push_back(*tempScreen);
  delete tempScreen;
  if (!ButtonScreenLink(Screens, ScreenID, ButtonID))
    return 10;
  for (auto i : Screens)
    Windows.main.AddScreen(i);
  return 0;
}

void ErrorMessage(unsigned int Error)
{
  switch (Error)
  {
  case 0: return;
  case 1: cerr << "Couldn't open the file.\n"; exit(1);
  case 2: cerr << "Undefined main screen.\n"; exit(2);
  case 3: cerr << "Couldn't allocate memory.\n"; exit(3);
  case 4: cerr << "Couldn't allocate memory.\n"; exit(4);
  case 5: cerr << "Wrong position syntax.\n"; exit(5);
  case 6: cerr << "Wrong position syntax.\n"; exit(6);
  case 7: cerr << "Wrong color syntax.\n"; exit(7);
  case 8: cerr << "Wrong target syntax.\n"; exit(8);
  case 9: cerr << "Wrong command.\n"; exit(9);
  case 10: cerr << "Wrong target screen.\n"; exit(10);
  default: cerr << "Unknown error.\n"; exit(-1);
  }
}
