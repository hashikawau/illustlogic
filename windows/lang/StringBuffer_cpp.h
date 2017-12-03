

#ifndef __Windows_lang_StringBuffer_cpp
#define __Windows_lang_StringBuffer_cpp


//===========================================================
//===========================================================
#include "StringBuffer.h"

#include "String.h"
#include "String_cpp.h"

#include "Exception.h"
#include "Exception_cpp.h"
//===========================================================
//===========================================================
/*
static unsigned int strlen(const char* const str);
static unsigned int strlen(const wchar_t* const str);
static wchar_t* strcpy(wchar_t* s1, const char* const s2);
static wchar_t* strcpy(wchar_t* s1, const wchar_t* const s2);
static wchar_t* strcat(wchar_t* s1, const char* const s2);
static wchar_t* strcat(wchar_t* s1, const wchar_t* const s2);
*/
//===========================================================
//===========================================================
void StringBuffer::setCapacity(int length)
{
    if(_capacity > length){
        return;
    }
    
    int newCapacity = _capacity?
        _capacity:
        1;
    while(newCapacity < length){
        newCapacity *= 2;
    }
    _capacity = newCapacity;
    wchar_t *newstr = new wchar_t[_capacity +1];
    ::strcpy(newstr, _str);
    delete [] _str;
    _str = newstr;
}
//===========================================================
//===========================================================
StringBuffer::StringBuffer(int capacity)
{
    if(capacity<0){
        throw NegativeArraySizeException();
    }
    
    _capacity = capacity;
    _str = new wchar_t[_capacity +1];
    _str[0] = L'\0';
}
StringBuffer::StringBuffer(const String& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    _capacity = string.length();
    _str = new wchar_t[_capacity +1];
    ::strcpy(_str, string.toCharArray());
}
StringBuffer::StringBuffer(const StringBuffer& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    _capacity = string._capacity;
    _str = new wchar_t[_capacity +1];
    ::strcpy(_str, string._str);
}
StringBuffer::~StringBuffer()
{
    delete [] _str;
    _str =0;
}
//===========================================================
//===========================================================
StringBuffer StringBuffer::operator+(const StringBuffer& string) const
{
    if(!&string){
        throw NullPointerException();
    }
    
    wchar_t s1[length() + string.length() +1];
    return StringBuffer(::strcat(::strcpy(s1, _str), string._str));
}
//===========================================================
//===========================================================
StringBuffer& StringBuffer::operator=(const StringBuffer& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    setCapacity(string.length());
    ::strcpy(_str, string._str);
    return *this;
}
//===========================================================
//===========================================================
StringBuffer& StringBuffer::append(const StringBuffer& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    setCapacity(length() + string.length());
    ::strcat(_str, string._str);
    return *this;
}
StringBuffer& StringBuffer::operator+=(const StringBuffer& string)
{
    if(!&string){
        throw NullPointerException();
    }
    
    setCapacity(length() + string.length());
    ::strcat(_str, string._str);
    return *this;
}
//===========================================================
//===========================================================
StringBuffer& StringBuffer::deleteCharAt(int index)
{
    if(index<0 || index>=::strlen(_str)){
        throw StringIndexOutOfBoundsException();
    }
    
    wchar_t* s1 = &_str[index];
    wchar_t* s2 = &_str[index +1];
    ::strcpy(s1, s2);
    return *this;
}
StringBuffer& StringBuffer::delete_(int start, int end)
{
    if(start<0 || start>end || start>=length() || end>length()){
        throw StringIndexOutOfBoundsException();
    }
    
    wchar_t* s1 = &_str[start];
    wchar_t* s2 = &_str[end];
    ::strcpy(s1, s2);
    return *this;
}
//===========================================================
//===========================================================
StringBuffer& StringBuffer::insert(int offset, const String& str)
{
    if(offset<0 || offset>length()){
        throw StringIndexOutOfBoundsException();
    }
    
    String string;
    if(!&str){
        string = L"null";
    }else{
        string = str;
    }
    
    int preLength = length();
    int insLength = string.length();
    setCapacity(preLength + insLength);
    for(int i=preLength; i>= offset; --i){
        _str[insLength +i] = _str[i];
    }
    
    for(int i=0; i< insLength; ++i){
        _str[offset +i] = string[i];
    }
    
    return *this;
}
//===========================================================
//===========================================================
wchar_t& StringBuffer::operator[](int index)
{
    if(index<0 || index>=::strlen(_str)){
        throw IndexOutOfBoundsException();
    }
    
    return _str[index];
}
const wchar_t StringBuffer::operator[](int index) const
{
    if(index<0 || index>=::strlen(_str)){
        throw IndexOutOfBoundsException();
    }
    
    return _str[index];
}
//===========================================================
//===========================================================
String StringBuffer::substring(int start) const
{
    if(start<0 || start>length()){
        throw StringIndexOutOfBoundsException();
    }
    
    return String(_str, start, length() -start);
}
String StringBuffer::substring(int start, int end) const
{
    if(start<0 || end<0 || end>length() || start>end){
        throw StringIndexOutOfBoundsException();
    }
    
    return String(_str, start, end -start);
}
//===========================================================
//===========================================================
int StringBuffer::length() const
{
    return ::strlen(_str);
}
int StringBuffer::capacity() const
{
    return _capacity;
}
//===========================================================
//===========================================================

//===========================================================
//===========================================================
/*
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
*/
//===========================================================
//===========================================================


#endif //ifndef __Windows_lang_StringBuffer_cpp

