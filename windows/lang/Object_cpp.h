

#ifndef __Windows_lang_Object_cpp
#define __Windows_lang_Object_cpp


//===========================================================
//===========================================================
#include "Object.h"
#include "../_windows.h"
#include "Number.h"
#include "String.h"
#include <typeinfo.h>
#include "Number_cpp.h"
#include "String_cpp.h"
//===========================================================
//===========================================================
Object::Object(){}
Object::~Object(){}
//===========================================================
//===========================================================
Object& Object::clone()
{
    return *new Object(*this);
}
void Object::finalize(){}
//===========================================================
//===========================================================
String& Object::toString()
{
    return String(typeid(*this).name())
        +String("@")
        +Integer::toHexString(static_cast<int>(this));
}
//===========================================================
//===========================================================


#endif //ifndef __Windows_lang_Object_cpp

