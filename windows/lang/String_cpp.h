

#ifndef __Windows_lang_String_cpp
#define __Windows_lang_String_cpp


//===========================================================
//===========================================================
#include "String.h"

#include <stdio.h>

#include "../_windows.h"

#include "Object.h"
#include "Object_cpp.h"

#include "Exception.h"
#include "Exception_cpp.h"
//===========================================================
//===========================================================
static unsigned int strlen(const char* const str);
static unsigned int strlen(const wchar_t* const str);
static wchar_t* strcpy(wchar_t* s1, const char* const s2);
static wchar_t* strcpy(wchar_t* s1, const wchar_t* const s2);
static wchar_t* strcat(wchar_t* s1, const char* const s2);
static wchar_t* strcat(wchar_t* s1, const wchar_t* const s2);
//===========================================================
//===========================================================
String::String()
{
    _str = new wchar_t[1];
    _str[0] = L'\0';
}
String::String(bool b)
{
    _str = new wchar_t[6];
    if(b){
        ::strcpy(_str, L"true");
    }else{
        ::strcpy(_str, L"false");
    }
}
String::String(int ch)
{
    wchar_t str[64];
    wsprintfW(str, L"%d", ch);
    _str = new wchar_t[::strlen(str) +1];
    ::strcpy(_str, str);
}
String::String(double ch)
{
    char str[64];
    sprintf(str, "%lf", ch);
    _str = new wchar_t[::strlen(str) +1];
    ::strcpy(_str, str);
}
String::String(char ch)
{
    _str = new wchar_t[2];
    _str[0] = static_cast<wchar_t>(0x00FF & ch);
    _str[1] = L'\0';
}
String::String(wchar_t ch)
{
    _str = new wchar_t[2];
    _str[0] = ch;
    _str[1] = L'\0';
}
String::String(const char* const str)
{
    if(!str){
        throw NullPointerException();
    }
    _str = new wchar_t[::strlen(str) +1];
    ::strcpy(_str, str);
}
String::String(const wchar_t* const str)
{
    if(!str){
        throw NullPointerException();
    }
    _str = new wchar_t[::strlen(str) +1];
    ::strcpy(_str, str);
}
String::String(const char* const str, int offset, int count)
{
    if(!str){
        throw NullPointerException();
    }
    if(offset<0 || count<0 || offset+count>::strlen(str)){
        throw IndexOutOfBoundsException();
    }
    
    _str = new wchar_t[count +1];
    for(int i=0; i< count; ++i){
        _str[i] = static_cast<wchar_t>(str[i +offset]);
    }
    _str[count] = L'\0';
}
String::String(const wchar_t* const str, int offset, int count)
{
    if(!str){
        throw NullPointerException();
    }
    if(offset<0 || count<0 || offset+count>::strlen(str)){
        throw IndexOutOfBoundsException();
    }
    
    _str = new wchar_t[count +1];
    for(int i=0; i< count; ++i){
        _str[i] = str[i +offset];
    }
    _str[count] = L'\0';
}
String::String(const String& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    _str = new wchar_t[string.length() +1];
    ::strcpy(_str, string._str);
}
String::~String()
{
    delete [] _str;
    _str =0;
}
//===========================================================
//===========================================================
Object& String::clone()
{
    return *new String(*this);
}
void String::finalize(){}
//===========================================================
//===========================================================
String String::operator+(const String& string) const
{
    if(!&string){
        throw NullPointerException();
    }
    
    wchar_t s1[length() + string.length() +1];
    return String(::strcat(::strcpy(s1, _str), string._str));
}
//===========================================================
//===========================================================
String& String::operator=(const String& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    delete [] _str;
    _str = new wchar_t[string.length() +1];
    ::strcpy(_str, string._str);
    return *this;
}
//===========================================================
//===========================================================
String& String::operator+=(const String& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    wchar_t *str = new wchar_t[length() +string.length() +1];
    ::strcpy(str, _str);
    ::strcat(str, string._str);
    delete [] _str;
    _str = str;
    
    return *this;
}
//===========================================================
//===========================================================
wchar_t& String::operator[](const int index)
{
    if(index<0 || index>=length()){
        throw IndexOutOfBoundsException();
    }
    return _str[index];
}
wchar_t String::operator[](const int index) const
{
    if(index<0 || index>=length()){
        throw IndexOutOfBoundsException();
    }
    return _str[index];
}
//===========================================================
//===========================================================
const wchar_t* String::toCharArray() const
{
    return _str;
}
//===========================================================
//===========================================================
int String::length() const
{
    return ::strlen(_str);
}
//===========================================================
//===========================================================

//===========================================================
//===========================================================
static unsigned int strlen(const char* const str)
{
    char* ch = const_cast<char*>(str);
    int count = 0;
    while(*ch != '\0'){
        ++count;
        ++ch;
    }
    return count;
}
static unsigned int strlen(const wchar_t* const str)
{
    wchar_t* ch = const_cast<wchar_t*>(str);
    int count = 0;
    while(*ch != L'\0'){
        ++count;
        ++ch;
    }
    return count;
}
static wchar_t* strcpy(wchar_t* s1, const char* const s2)
{
    wchar_t* ch1 = s1;
    char* ch2 = const_cast<char*>(s2);
    while(*ch2 != '\0'){
        *ch1 = static_cast<wchar_t>(*ch2);
        ++ch1;
        ++ch2;
    }
    *ch1 = L'\0';
    return s1;
}
static wchar_t* strcpy(wchar_t* s1, const wchar_t* const s2)
{
    wchar_t* ch1 = s1;
    wchar_t* ch2 = const_cast<wchar_t*>(s2);
    while(*ch2 != L'\0'){
        *ch1 = *ch2;
        ++ch1;
        ++ch2;
    }
    *ch1 = L'\0';
    return s1;
}
static wchar_t* strcat(wchar_t* s1, const char* const s2)
{
    wchar_t* ch1 = s1;
    char* ch2 = const_cast<char*>(s2);
    while(*ch1 != L'\0'){
        ++ch1;
    }
    while(*ch2 != '\0'){
        *ch1 = static_cast<wchar_t>(*ch2);
        ++ch1;
        ++ch2;
    }
    *ch1 = L'\0';
    return s1;
}
static wchar_t* strcat(wchar_t* s1, const wchar_t* const s2)
{
    wchar_t* ch1 = s1;
    wchar_t* ch2 = const_cast<wchar_t*>(s2);
    while(*ch1 != L'\0'){
        ++ch1;
    }    
    while(*ch2 != L'\0'){
        *ch1 = *ch2;
        ++ch1;
        ++ch2;
    }
    *ch1 = L'\0';
    return s1;
}
//===========================================================
//===========================================================
ostream& operator<<(ostream& stream, const String& string)
{
    char str[string.length() +1];
    char* ch1 = str;
    wchar_t* ch2 = const_cast<wchar_t*>(string.toCharArray());
    
    while(*ch2 != '\0'){
        *ch1 = static_cast<char>(*ch2);
        ++ch1;
        ++ch2;
    }
    *ch1 = '\0';
    
    stream << str;
    return stream;
}
//===========================================================
//===========================================================


#endif //ifndef __Windows_lang_String_cpp

