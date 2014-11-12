#ifndef __Elevator_H__
#define __Elevator_H__

#include "cocos2d.h"


#define MAXPERSON 15

typedef enum {
	NONE= 0,
	UP,
	DOWN
  }Direction;



class Elevator : public cocos2d::Sprite
{
public:
    virtual bool init();  
    
    CREATE_FUNC(Elevator);

    CC_SYNTHESIZE(int, _person, PersonLeft);
    CC_SYNTHESIZE(int, _tfloor, TargetFloor);
    CC_SYNTHESIZE(int, _mfloor, MaxFloor);
    CC_SYNTHESIZE(int, _direction, Direction);

    void setMaxTargetFloor(int _idx);	

private:

};

#endif // __Elevator_H__
