#ifndef MYFRAMELISTENER_H
#define MYFRAMELISTENER_H
#include <OGRE/Ogre.h>
#include <OIS/OIS.h>

class MyFrameListener : public Ogre::FrameListener
{
  private:
    OIS::InputManager* _InputManager;
    OIS::Keyboard* _Keyboard;

    OIS::Mouse* _Mouse;
    Ogre::Camera* _Cam;
    float _movementspeed;

    float _WalkingSpeed;
    Ogre::SceneNode* _node;

    // Анимация модели
    Ogre::AnimationState* _aniState;
    Ogre::AnimationState* _aniStateTop;

    // Композиторы
    Ogre::Viewport* _viewport;
    bool _comp1, _comp2, _comp3;
    bool _down1, _down2, _down3;
  public:
    MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::Viewport* viewport, Ogre::SceneNode* node, Ogre::Entity* ent);
    ~MyFrameListener();

    bool frameStarted(const Ogre::FrameEvent&);
    /*
    bool frameEnded(const Ogre::FrameEvent& evt);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    */
};

#endif // MYFRAMELISTENER_H
