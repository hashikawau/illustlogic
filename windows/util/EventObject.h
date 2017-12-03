

#ifndef __Windows_util_EventObject
#define __Windows_util_EventObject


//===========================================================
//===========================================================
class Object;
//===========================================================
//===========================================================
class EventObject
{
protected:
    unsigned int _source__;
    Object* _source;
    
public:
    explicit EventObject(Object& source);
    virtual ~EventObject();
    Object& getSource() const;
    
    explicit EventObject(unsigned int source);
    unsigned int getSource();
};
//===========================================================
//===========================================================
class EventListener
{
public:
    EventListener();
    virtual ~EventListener() =0;
};
//===========================================================
//===========================================================


#endif //ifndef __Windows_util_EventObject

