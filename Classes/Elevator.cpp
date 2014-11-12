

#include "Elevator.h"

USING_NS_CC;



//typedef enum State {
//  	None,
//  	Moving,
//  	Stopped,
//  };

bool Elevator::init()
  {
  	if (!Sprite::initWithFile("whitebox.jpg"))
  		return false;

      _mfloor = 1;
      _tfloor = 1;
      _person = 15;
      setDirection(Direction::NONE);
//      setColor(Color3B::BLUE);
      auto size = Director::getInstance()->getVisibleSize();
      setScale(size.height*0.15/getContentSize().height);
  	return true;
  }

  void Elevator::setMaxTargetFloor(int _idx)
  {
  	auto size = Director::getInstance()->getVisibleSize();
  	auto defaultHeight = 100 + size.height * 0.15 * _idx;


      if (getPositionY() <= defaultHeight &&_direction == Direction::UP)
    
    {
    if (_idx >= _mfloor)
        _mfloor = _idx;
//    if (_idx >= _tfloor)
        _tfloor = _idx;
	}

      if (getPositionY()>defaultHeight&& _direction == Direction::DOWN)
	{
		 if (_idx < _mfloor)
        _mfloor = _idx;
//    if (_idx <= _tfloor)
        _tfloor = _idx;
	}


 }