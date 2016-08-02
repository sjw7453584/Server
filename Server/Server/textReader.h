#ifndef TEXT_H
#define TEXT_H
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
string s2utfs(const  string&  strSrc);
string  utfs2s(const string& strutf);
class Text
{
protected:
	char * m_binaryStr;
	size_t m_length;
	size_t m_index;
public:
	Text(string path);
	void SetIndex(size_t index);
	virtual bool ReadOneChar(string &oneChar) = 0;
	bool Read(std::ostringstream & str_stream);
	size_t Size();
	virtual ~Text();
};

class GbkText :public Text
{
public:
	GbkText(string path);
	~GbkText(void);
	bool ReadOneChar(string & oneChar);
};


class UtfText :public Text
{
public:
UtfText(string path);
~UtfText(void);
bool ReadOneChar(string & oneChar);
private:
size_t get_utf8_char_len(const char & byte);
};

class TextFactory
{
public:
static Text * CreateText(string textCode, string path);
};
#endif	