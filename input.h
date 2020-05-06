#ifndef __INPUT_H
#define __INPUT_H

#include "graphics.h"
#include "lab_3.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

void ErrorMessage(unsigned int Error);

class Line
{
public:
  vector<string> words;
  Line(string& str);
  Line& operator+=(string& str);
};

class Code
{
private:
  vector<Line> CodeLines;
public:
  Code& operator+=(string& str);
  bool ReadFile(const char* File_Name);
  unsigned int Translator(Window& Input, Window& Dialog);
};

#endif