#include "HelloWorldScene.h"

USING_NS_CC;



//struct PersoninFloor
//{
//    int floor,
//    int countPerson,
//    int ElevatorState,
//
//};


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
     CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3");
    
    

    Size size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    


    elevator = Elevator::create();
    elevator->setPosition(origin + Vec2(size.width/2,100));
    this->addChild(elevator);

   

    //init vector
    for (int i = 0; i < 5; i++)
    {
        insidePersonToFloor.push_back(0);
        outsidePersonInFloor.push_back(0);
    }
    
     initMenuLabel();

    return true;
}

void HelloWorld::initMenuLabel()
{

    Size size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


  Vector<MenuItem*>vec_menulabel;
  personRequest = 0;

    for (int i = 0; i < 5; i++)
    {
        auto defaultHeight = 100 +size.height*0.15*i;

        //Person Request inside Elevator
        
        auto menulabelI = MenuItemLabel::create(LabelTTF::create(StringUtils::format("%i", i+1), "Thonburi.ttf", 50),
            [&,defaultHeight,i](Ref * sender){
                //
                if (elevator->getPersonLeft() == MAXPERSON){
                    MessageBox("No one in elevator!", "Unavailable");
                    return;
                }

                if (personRequest >= (MAXPERSON-elevator->getPersonLeft())){
                    MessageBox("Every person already have their request", "Request Overloaded");
                    return;
                }
                
                personRequest++;
                insidePersonToFloor[i]++;
                elevator->setMaxTargetFloor(i);
                labelI[i]->setString(StringUtils::format("%i+",insidePersonToFloor[i]));

                //First Task for elevator
                if (elevator->getDirection() == Direction::NONE){
                    if (defaultHeight > elevator->getPositionY())
                        elevator->setDirection(Direction::UP);
                    else
                        elevator->setDirection(Direction::DOWN);
                    
                    
                    elevator->setMaxTargetFloor(i);
                    continueJob();
                }


            });
        

        menulabelI->setPosition(origin + Vec2(size.width *0.1, defaultHeight));

        //Person Request Outside Elevator
        auto menulabelO = MenuItemLabel::create(LabelTTF::create(StringUtils::format("%i", i+1), "Thonburi.ttf", 50),
                [&,defaultHeight,i](Ref * sender){
                
                outsidePersonInFloor[i]++;
                labelO[i]->setString(StringUtils::format("%i+",outsidePersonInFloor[i]));
                    
                

                //First Task for elevator
                if (elevator->getDirection() == Direction::NONE){
                    if (defaultHeight > elevator->getPositionY())
                        elevator->setDirection(Direction::UP);
                    else
                        elevator->setDirection(Direction::DOWN);
                    
                    
                    elevator->setMaxTargetFloor(i);
                    continueJob();
                }

                });

        menulabelO->setPosition(origin + Vec2(size.width *0.9, defaultHeight));

        vec_menulabel.pushBack(menulabelI);
        vec_menulabel.pushBack(menulabelO);
        
        auto countI = LabelTTF::create(StringUtils::format("%i+",insidePersonToFloor[i]), "Thonburi.ttf", 15);
        countI->setPosition(Vec2(size.width*0.05, defaultHeight));
        this->addChild(countI);
        labelI.push_back(countI);
        
        auto countO = LabelTTF::create(StringUtils::format("%i+",outsidePersonInFloor[i]), "Thonburi.ttf", 15);
        countO->setPosition(Vec2(size.width*0.95, defaultHeight));
        this->addChild(countO);
        labelO.push_back(countO);
        
    }
    
    auto res_button = MenuItemLabel::create(LabelTTF::create("Reset", "Thonburi.ttf", 100),
                                            [&](Ref * sender){
                                                
                                                Director::getInstance()->replaceScene(HelloWorld::createScene());
                                                
                                            });
    res_button->setPosition(Vec2(size.width/2, size.height*0.9));
    vec_menulabel.pushBack(res_button);
    
    

    auto menu = Menu::createWithArray(vec_menulabel);
    menu->setPosition(origin + Vec2::ZERO);
    this->addChild(menu);
    
    
    auto labelL = LabelTTF::create("Inside", "Thonburi.ttf", 25);
    labelL->setPosition(origin + Vec2(size.width*0.1, size.height*0.8));
    this->addChild(labelL);
    
    auto labelR = LabelTTF::create("Outside", "Thonburi.ttf", 25);
    labelR->setPosition(origin + Vec2(size.width*0.9, size.height*0.8));
    this->addChild(labelR);
    
    

}




void HelloWorld::pick(int _floor)
{
    auto personCount = outsidePersonInFloor[_floor];

    auto amountToTake = elevator->getPersonLeft() - personCount;
   //Take all the people outside in the floor is there's available slot
    
    if (amountToTake > 0 )
    {
        elevator->setPersonLeft(amountToTake);
        outsidePersonInFloor[_floor] = 0;
    }
    else
 //if larger than available slot, take only a part 
    {
        outsidePersonInFloor[_floor] -= elevator->getPersonLeft();
        elevator->setPersonLeft(0);
        
        
    }
    labelO[_floor]->setString(StringUtils::format("%i+",outsidePersonInFloor[_floor]));
}

void HelloWorld::deliver(int _floor)
{
    personRequest-= insidePersonToFloor[_floor];
    elevator->setPersonLeft(elevator->getPersonLeft() + insidePersonToFloor[_floor]);
    insidePersonToFloor[_floor] = 0;
    
    labelI[_floor]->setString(StringUtils::format("%i+",insidePersonToFloor[_floor]));
}

void HelloWorld::onStopped()
{
    elevator->stopAllActions();
  auto _floor = elevator->getTargetFloor();
  deliver(_floor);
  pick(_floor);
    
    //Switch Direction
    if (elevator->getTargetFloor() == elevator->getMaxFloor()){
    
      if (outsidePersonInFloor[elevator->getMaxFloor()] == 0 || insidePersonToFloor[elevator->getMaxFloor()] == 0)
    {
//        bool switched = false;
        if (elevator->getDirection() == Direction::UP){
            elevator->setDirection(Direction::DOWN);
            for(int i =4; i < 0; i--)
            {
                if (insidePersonToFloor[i]>0 || outsidePersonInFloor[i]>0){
//                    if (!switched){
                        elevator->setTargetFloor(i);
//                        switched = true;
//                    }
                    elevator->setMaxFloor(MIN(elevator->getMaxFloor(),i));
                }
            }
        }
        
        else if (elevator->getDirection() == Direction::DOWN){
            elevator->setDirection(Direction::UP);
            for(int i = 0 ; i<5; i++)
            {
                if (insidePersonToFloor[i]>0 || outsidePersonInFloor[i]>0){
//                    if (!switched){
                        elevator->setTargetFloor(i);
//                        switched = true;
//                    }
                    elevator->setMaxFloor(MAX(elevator->getMaxFloor(),i));
                }
            }
        }
    }
        else
            elevator->setTargetFloor(elevator->getMaxFloor());
        
    }


    
    float delay = (outsidePersonInFloor[elevator->getTargetFloor() > 0 ]) ? 3.0 : 0.0;
    auto callfuncCont = CallFunc::create(std::bind(&HelloWorld::continueJob, this));
    elevator->runAction(Sequence::create(DelayTime::create(delay),callfuncCont, NULL));
    

}

void HelloWorld::continueJob()
{

    Size size = Director::getInstance()->getVisibleSize();

    //If Empty, back to default Position
    if (isQueueEmpty()){
        elevator->runAction(MoveTo::create(1.0, Vec2(elevator->getPositionX(),100)));
        elevator->setDirection(Direction::NONE);
        MessageBox("Back to Base", "Request Done");
        return;
    }

    auto callfunc = CallFunc::create(std::bind(&HelloWorld::onStopped, this));
    int _idxFloor =   elevator->getTargetFloor();
    auto getCurrentFloor = (elevator->getPositionY()- 100)/(size.height*0.15);
    elevator->runAction(Sequence::create(
        MoveTo::create(0.5*fabs(getCurrentFloor - elevator->getTargetFloor()),
                       Vec2(elevator->getPositionX(),100 +size.height*0.15*_idxFloor)),
        callfunc,
        NULL));

}

bool HelloWorld::isQueueEmpty()
{
    for (int i = 0; i < 5; i++)
    {
        if (insidePersonToFloor[i] > 0 || outsidePersonInFloor[i] > 0)
            return false;
    }
    
    if (elevator->getPersonLeft() < MAXPERSON)
        return false;
    
    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
