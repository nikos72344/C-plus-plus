#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <iterator>

using namespace std;

class Sentence
{
public:
  list<string> Words;
  unsigned int Length;
  Sentence() : Length(0) {};
  Sentence(string& str);
  Sentence& operator+=(string& str);
  friend ostream& operator<<(ostream& os, Sentence& sen);
};

Sentence::Sentence(string& str) : Length(0)
{

  string::iterator start = str.begin();
  for (string::iterator i = str.begin(); i != str.end(); i++)
  {
    if (*i == ' ')
    {
      string temp("", (int)(i - start));
      copy(start, i, temp.begin());
      *this += temp;
      start = i + 1;
    }
  }
  string temp("", (int)(str.end() - start));
  copy(start, str.end(), temp.begin());
  *this += temp;
}

Sentence& Sentence::operator+=(string& str)
{
  Words.push_back(str);
  Length += str.length();
  return *this;
}

ostream& operator<<(ostream& os, Sentence& sen)
{
  ostream_iterator<string, char>out(os, "|");
  copy(sen.Words.begin(), sen.Words.end(), out);
  return os;
}

class Text
{
private:
  vector<Sentence> SentencesList;
  void Out(Sentence& sen);
  void MoveTo(Sentence& origin, list<string>::iterator beg, list<string>::iterator end, Sentence& dest);
  Sentence* Split(Sentence& sen);
public:
  unsigned int StringLen;
  Text(unsigned int Str_Len = 80) : StringLen(Str_Len) {};
  Text& operator+=(string& str);
  bool ReadFile(const char* File_Name);
  void Show(void);
  bool Align(void);
};

void Text::Out(Sentence& sen)
{
  int indentation = (StringLen - (sen.Length + sen.Words.size() - 1)) / 2;
  for (int i = 0; i < indentation; i++)
    cout << " ";
  list<string>::iterator str = sen.Words.begin();
  for (unsigned int i = 0; i < (sen.Words.size() - 1); i++)
  {
    cout << *str << " ";
    str++;
  }
  cout << *str << endl;
}

void Text::MoveTo(Sentence& origin, list<string>::iterator beg, list<string>::iterator end, Sentence& dest)
{
  list<string>::iterator pos = end, temp;
  pos--;
  while (true)
  {
    dest.Words.push_front(*pos);
    dest.Length += pos->size();
    temp = pos;
    if (pos == beg)
    {
      origin.Length -= pos->size();
      origin.Words.erase(pos);
      break;
    }
    pos--;
    origin.Length -= temp->size();
    origin.Words.erase(temp);
  }
}

Sentence* Text::Split(Sentence& sen)
{
  Sentence* newSen = new Sentence;
  *newSen = sen;
  string temp = *newSen->Words.begin();
  newSen->Words.pop_front();
  newSen->Length -= temp.size();
  temp.erase(temp.begin(), temp.begin() + StringLen - 1);
  newSen->Words.push_front(temp);
  newSen->Length += temp.size();
  sen.Words.begin()->erase(sen.Words.begin()->begin() + StringLen - 1, sen.Words.begin()->end());
  temp = *sen.Words.begin();
  sen.Words.clear();
  sen.Words.push_back(temp);
  sen.Length = temp.size();
  *sen.Words.begin() += "-";
  return newSen;
}

Text& Text::operator+=(string& str)
{
  Sentence temp(str);
  this->SentencesList.push_back(str);
  return *this;
}

bool Text::ReadFile(const char* File_Name)
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

void Text::Show(void)
{
  for (vector<Sentence>::iterator i = SentencesList.begin(); i != SentencesList.end(); i++)
    cout << *i << "]";
}

bool Text::Align(void)
{
  if (StringLen < 2)
    return false;
  for (vector<Sentence>::iterator i = SentencesList.begin(); i != SentencesList.end(); i++)
  {
    if ((i->Length + i->Words.size() - 1) > StringLen)
    {
      vector<Sentence>::iterator next = i + 1;
      if (next == SentencesList.end())
      {
        Sentence temp;
        SentencesList.push_back(temp);
        next = SentencesList.end() - 1;
        i = next - 1;
      }
      if (i->Words.begin()->size() > StringLen)
      {
        Sentence* temp = Split(*i);
        MoveTo(*temp, temp->Words.begin(), temp->Words.end(), *next);
        delete temp;
      }
      else
      {
        unsigned int len = i->Length + i->Words.size() - 1;
        list<string>::iterator j = i->Words.end();
        while (StringLen < len)
        {
          j--;
          len -= (j->size() + 1);
        }
        MoveTo(*i, j, i->Words.end(), *next);
      }
    }
    Out(*i);
  }
  return true;
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    cerr << "Not enough input arguments.\n";
    return 1;
  }
  int len = atoi(argv[2]);
  if (!len && strcmp(argv[2], "0"))
  {
    cerr << "Input file's name first and the string's length then;\n";
    return 2;
  }
  if (len < 2)
  {
    cerr << "I'm afraid your string is too short.\n";
    return 3;
  }
  Text text(len);
  if (!text.ReadFile(argv[1]))
  {
    cerr << "Couldn't open the file.\n";
    return 4;
  }
  if (!text.Align())
  {
    cerr << "I'm afraid your string is too short.\n";
    return 5;
  }
  return 0;
}