#ifndef __INPUT_H
#define __INPUT_H

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
  unsigned int Translator(void);
};

#endif