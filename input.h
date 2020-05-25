#ifndef __INPUT_H
#define __INPUT_H

#include "graphics.h"
#include <string>
#include <vector>

using namespace std;

class Line
{
public:
  vector<string> words;
  Line(string& str);
  Line& operator+=(string& str);
};

class Screen;

class Input
{
private:
  vector<Line> CodeLines;
public:
  Input& operator+=(string& str);
  bool ReadFile(const char* File_Name);
  unsigned int Translator(Screen& Main);
};

#endif