

#ifndef __Windows_lang_String
#define __Windows_lang_String


//===========================================================
//===========================================================
#include "Object.h"
//===========================================================
//===========================================================
class String: public Object
{
private:
    wchar_t* _str;
    
public:
    String();
    explicit String(bool b);
    explicit String(int ch);
    explicit String(double ch);
    String(char ch);
    String(wchar_t ch);
    String(const char* const str);
    String(const wchar_t* const str);
    String(const char* const str, int offset, int count);
    String(const wchar_t* const str, int offset, int count);
    String(const String& string);
    ~String();
    virtual Object& clone();
    virtual void finalize();
    
    String operator+(const String& string) const;
    String& operator=(const String& string);
    String& operator+=(const String& string);
    wchar_t& operator[](const int index);
    wchar_t operator[](const int index) const;
    int length() const;
    
    const wchar_t* toCharArray() const;
};
//===========================================================
//===========================================================


#endif //ifndef __Windows_lang_String

