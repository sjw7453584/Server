#ifndef __FIELD_H__
#define __FIELD_H__
#include <string>
#include <sstream>
class Field
{
public:
	Field() {};
	// Field(Field &f);
	Field& operator =(std::string val) { mValue = val; return *this; }
	~Field() {}
	std::string GetString() const { return mValue; }
	std::string GetValue() const { return mValue; }
	float GetFloat() const { return static_cast<float>(atof(mValue.c_str())); }
	double GetDouble() const { return static_cast<double>(atof(mValue.c_str())); }
	bool GetBool() const { return atoi(mValue.c_str()); }
	int16 GetInt16() const { return static_cast<int16>(atol(mValue.c_str())); }
	int32 GetInt32() const { return  static_cast<int32>(atol(mValue.c_str())); }
	uint8 GetUInt8() const { return static_cast<uint8>(atol(mValue.c_str())); }
	uint16 GetUInt16() const { return static_cast<uint16>(atol(mValue.c_str())); }
	uint32 GetUInt32() const { return static_cast<uint32>(atol(mValue.c_str())); }
	uint64 GetUInt64() const
	{
		std::istringstream ss(mValue);
		uint64 value;
		if (ss >> value)
		{
			return value;
		}
		else
		{
			return 0;
		}
	}

private:
	std::string mValue;
};
#endif
