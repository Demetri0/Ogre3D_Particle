#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H
#include <OGRE/Ogre.h>
#include "myframelistener.h"

class MyApplication
{
  private:
    Ogre::SceneManager* _sceneManager;
    Ogre::Root* _root;
    MyFrameListener* _listener;
    bool _keepRunning;

    Ogre::SceneNode* _SinbadNode;
    Ogre::Entity* _SinbadEnt;
  public:
    MyApplication();
    ~MyApplication();
    void loadResources();
    void createScene();
    int  startUp();
    void renderOneFrame();
    bool keepRunning();
};

#endif // MYAPPLICATION_H
