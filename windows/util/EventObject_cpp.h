

#ifndef __Windows_util_EventObject_cpp
#define __Windows_util_EventObject_cpp


//===========================================================
//===========================================================
#include "EventObject.h"
#include "../lang/Object.h"
#include "../lang/Object_cpp.h"
//===========================================================
//===========================================================
EventObject::EventObject(Object& source):
    _source(&source),
    _source__(static_cast<unsigned int>(&source))
{}
EventObject::~EventObject(){}
Object& EventObject::getSource() const
{
    return *_source;
}
//===========================================================
//===========================================================
EventObject::EventObject(unsigned int source):
    _source(static_cast<Object*>(source)),
    _source__(source)
{}
unsigned int EventObject::getSource()
{
    return _source__;
}
//===========================================================
//===========================================================
EventListener::EventListener(){}
EventListener::~EventListener(){}
//===========================================================
//===========================================================


#endif //ifndef __Windows_util_EventObject_cpp

