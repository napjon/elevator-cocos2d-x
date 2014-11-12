#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "Elevator.h"
#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    //pick people in floor
    void pick(int _floor);
    //deliver people inside elevator in floor
    void deliver(int _floor);
    //CallBack after elevator stopped moving
    void onStopped();
    //elevator continue the task
    void continueJob();
    //Checking if no one is using the elevator
    bool isQueueEmpty();


private:
	void initMenuLabel();
	Elevator * elevator;
	int personRequest;
	int total_person;

	std::vector<int>insidePersonToFloor;
	std::vector<int>outsidePersonInFloor;
    
    std::vector<cocos2d::LabelTTF*>labelI;
    std::vector<cocos2d::LabelTTF*>labelO;

};

#endif // __HELLOWORLD_SCENE_H__
