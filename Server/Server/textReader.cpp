#include "textReader.h"
#include <locale>
#include <codecvt>
//#include <iconv.h>

string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}


wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	int nret = mbstowcs(_Dest, _Source, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");

	return result;

}



wstring UTF2Uni(const char* src, std::wstring &t)
{
	if (src == NULL)
	{
		return L"";
	}

	int size_s = strlen(src);
	int size_d = size_s + 10;          //?

	wchar_t *des = new wchar_t[size_d];
	memset(des, 0, size_d * sizeof(wchar_t));

	int s = 0, d = 0;
	bool toomuchbyte = true; //set true to skip error prefix.

	while (s < size_s && d < size_d)
	{
		unsigned char c = src[s];
		if ((c & 0x80) == 0)
		{
			des[d++] += src[s++];
		}
		else if ((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
		{
			wchar_t &wideChar = des[d++];
			wideChar = (src[s + 0] & 0x3F) << 6;
			wideChar |= (src[s + 1] & 0x3F);

			s += 2;
		}
		else if ((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			wchar_t &wideChar = des[d++];

			wideChar = (src[s + 0] & 0x1F) << 12;
			wideChar |= (src[s + 1] & 0x3F) << 6;
			wideChar |= (src[s + 2] & 0x3F);

			s += 3;
		}
		else if ((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			wchar_t &wideChar = des[d++];

			wideChar = (src[s + 0] & 0x0F) << 18;
			wideChar = (src[s + 1] & 0x3F) << 12;
			wideChar |= (src[s + 2] & 0x3F) << 6;
			wideChar |= (src[s + 3] & 0x3F);

			s += 4;
		}
		else
		{
			wchar_t &wideChar = des[d++]; ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 

			wideChar = (src[s + 0] & 0x07) << 24;
			wideChar = (src[s + 1] & 0x3F) << 18;
			wideChar = (src[s + 2] & 0x3F) << 12;
			wideChar |= (src[s + 3] & 0x3F) << 6;
			wideChar |= (src[s + 4] & 0x3F);

			s += 5;
		}
	}

	t = des;
	delete[] des;
	des = NULL;

	return t;
}



int Uni2UTF(const wstring& strRes, char *utf8, int nMaxSize)
{
	if (utf8 == NULL) {
		return -1;
	}
	int len = 0;
	int size_d = nMaxSize;


	for (wstring::const_iterator it = strRes.begin(); it != strRes.end(); ++it)
	{
		wchar_t wchar = *it;
		if (wchar < 0x80)
		{  //
		   //length = 1;
			utf8[len++] = (char)wchar;
		}
		else if (wchar < 0x800)
		{
			//length = 2;

			if (len + 1 >= size_d)
				return -1;

			utf8[len++] = 0xc0 | (wchar >> 6);
			utf8[len++] = 0x80 | (wchar & 0x3f);
		}
		else if (wchar < 0x10000)
		{
			//length = 3;
			if (len + 2 >= size_d)
				return -1;

			utf8[len++] = 0xe0 | (wchar >> 12);
			utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
			utf8[len++] = 0x80 | (wchar & 0x3f);
		}
		else if (wchar < 0x200000)
		{
			//length = 4;
			if (len + 3 >= size_d)
				return -1;

			utf8[len++] = 0xf0 | ((int)wchar >> 18);
			utf8[len++] = 0x80 | ((wchar >> 12) & 0x3f);
			utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
			utf8[len++] = 0x80 | (wchar & 0x3f);
		}

	}


	return len;
}

string s2utfs(const  string&  strSrc)
{
	string  strRes;
	wstring  wstrUni = s2ws(strSrc);

	char*  chUTF8 = new char[wstrUni.length() * 3];
	memset(chUTF8, 0x00, wstrUni.length() * 3);
	Uni2UTF(wstrUni, chUTF8, wstrUni.length() * 3);
	strRes = chUTF8;
	delete[]chUTF8;
	return strRes;
}


string  utfs2s(const string& strutf)
{
	wstring  wStrTmp;
	UTF2Uni(strutf.c_str(), wStrTmp);
	return ws2s(wStrTmp);
}

string gbk2utf8(const string strSrc) //same as s2utfs
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> >cvt_utf8;//UTF-8<->Unicode转换器
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>cvt_ansi("CHS");//GBK<->Unicode转换器
	std::wstring ws = cvt_ansi.from_bytes(strSrc);
	std::string utf_str = cvt_utf8.to_bytes(ws);
	return utf_str;
}


string utf2gbk(const string strSrc) //same as utfs2s
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> >cvt_utf8;//UTF-8<->Unicode转换器
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>cvt_ansi("CHS");//GBK<->Unicode转换器
	std::wstring ws = cvt_utf8.from_bytes(strSrc);//UTF-8转换为Unicode
	std::string gbkstr = cvt_ansi.to_bytes(ws);//Unicode转换为GBK
	cvt_ansi.from_bytes(strSrc);
	return gbkstr;
}

using namespace std;
Text::Text(string path) :m_index(0)
{
	filebuf *pbuf;
	ifstream filestr;
	// 采用二进制打开 
	filestr.open(path.c_str(), ios::binary);
	if (!filestr)
	{
		cerr << path.c_str() << " Load text error." << endl;
		return;
	}
	// 获取filestr对应buffer对象的指针 
	pbuf = filestr.rdbuf();
	// 调用buffer对象方法获取文件大小
	m_length = (int)pbuf->pubseekoff(0, ios::end, ios::in);
	pbuf->pubseekpos(0, ios::in);
	// 分配内存空间
	m_binaryStr = new char[m_length + 1];
	// 获取文件内容
	pbuf->sgetn(m_binaryStr, m_length);
	//关闭文件
	filestr.close();
}

void Text::SetIndex(size_t index)
{
	m_index = index;
}

size_t Text::Size()
{
	return m_length;
}

bool Text::Read(std::ostringstream & str_stream)
{
	std::string str;
	while (ReadOneChar(str))
	{
		//std::string str_utfs = utfs2s(str);
		std::string str_utfs = utf2gbk(str);

		str_stream << str_utfs;
	}
	return true;
}

Text::~Text()
{
	delete[] m_binaryStr;
}

GbkText::GbkText(string path) :Text(path) {}
GbkText::~GbkText(void) {}
bool GbkText::ReadOneChar(string & oneChar)
{
	// return true 表示读取成功，
	// return false 表示已经读取到流末尾
	if (m_length == m_index)
		return false;
	if ((unsigned char)m_binaryStr[m_index] < 0x81)
	{
		oneChar = m_binaryStr[m_index];
		m_index++;
	}
	else
	{
		oneChar = string(m_binaryStr, 2);
		m_index += 2;
	}
	return true;
}

UtfText::UtfText(string path) :Text(path) {}
UtfText::~UtfText(void) {}
bool UtfText::ReadOneChar(string & oneChar)
{
	// return true 表示读取成功，
	// return false 表示已经读取到流末尾
	if (m_length == m_index)
		return false;
	//check bom
	
	size_t utf8_char_len = get_utf8_char_len(m_binaryStr[m_index]);
	if (m_index == 0 && utf8_char_len == 3)
	{
		if (m_binaryStr[m_index] == char(0xef) && m_binaryStr[m_index + 1] == (char)0xbb && m_binaryStr[m_index + 2] == (char)0xbf)
		{
			m_index += 3;
			utf8_char_len = get_utf8_char_len(m_binaryStr[m_index]);
		}
	}
	if (0 == utf8_char_len)
	{
		oneChar = "";
		m_index++;
		return true;
	}
	size_t next_idx = m_index + utf8_char_len;
	if (m_length < next_idx)
	{
		//cerr << "Get utf8 first byte out of input src string." << endl;
		next_idx = m_length;
	}
	//输出UTF-8的一个字符
	oneChar = string(m_binaryStr + m_index, next_idx - m_index);
	
	//重置偏移量
	m_index = next_idx;
	return true;
}

size_t UtfText::get_utf8_char_len(const char & byte)
{
	// return 0 表示错误
	// return 1-6 表示正确值
	// 不会 return 其他值 

	//UTF8 编码格式：
	//     U-00000000 - U-0000007F: 0xxxxxxx  
	//     U-00000080 - U-000007FF: 110xxxxx 10xxxxxx  
	//     U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx  
	//     U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
	//     U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
	//     U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  

	size_t len = 0;
	unsigned char mask = 0x80;
	while (byte & mask)
	{
		len++;
		if (len > 6)
		{
			//cerr << "The mask get len is over 6." << endl;
			return 0;
		}
		mask >>= 1;
	}
	if (0 == len)
	{
		return 1;
	}
	return len;
}

Text * TextFactory::CreateText(string textCode, string path)
{
	if ((textCode == "utf-8")
		|| (textCode == "UTF-8")
		|| (textCode == "ISO-8859-2")
		|| (textCode == "ascii")
		|| (textCode == "ASCII")
		|| (textCode == "TIS-620")
		|| (textCode == "ISO-8859-5")
		|| (textCode == "ISO-8859-7"))
	{
		return new UtfText(path);
	}
	else if ((textCode == "windows-1252")
		|| (textCode == "Big5")
		|| (textCode == "EUC-KR")
		|| (textCode == "GB2312")
		|| (textCode == "ISO-2022-CN")
		|| (textCode == "HZ-GB-2312")
		|| (textCode == "gb18030"))
	{
		return new GbkText(path);
	}
	return NULL;
}