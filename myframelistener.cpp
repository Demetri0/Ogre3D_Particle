#include "myframelistener.h"
#include <iostream>

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, Ogre::Viewport* viewport, Ogre::SceneNode* node, Ogre::Entity* ent){
    /* OIS будет инициализирован, используя список параметров, нам также нужен хендл
     * окна в форме строки для списка параметров; создаём три переменные для хранения данных:
     */
    OIS::ParamList parameters;
    unsigned int windowHandle = 0;
    std::ostringstream windowHandleString;

    // Получаем хендл окна рендера и преобразуем его в строку:
    win->getCustomAttribute("WINDOW", &windowHandle);
    windowHandleString << windowHandle;

    // Добавляем строку, содержащую хендл окна, в список параметров, используя ключ "WINDOW":
    parameters.insert( std::make_pair( "WINDOW", windowHandleString.str() ) );

    // Используем список параметров, чтобы создать InputManager:
    _InputManager = OIS::InputManager::createInputSystem(parameters);

    // С помощью менеджера ввода создаём клавиатуру:
    _Keyboard = static_cast<OIS::Keyboard*>( _InputManager->createInputObject(OIS::OISKeyboard, false) );

    // Мышь и камера
    _Cam = cam;
    _movementspeed = 50.0f;

    // Инициализируем мышь
    _Mouse = static_cast<OIS::Mouse*>( _InputManager->createInputObject( OIS::OISMouse, false ) );

    // Композиторы
    _viewport = viewport;
    _comp1 = false;
    _comp2 = false;
    _comp3 = false;
    _down1 = false;
    _down2 = false;
    _down3 = false;

    // Скорость перемещения и перемещаемый узел
    _WalkingSpeed = 50.0f;
    _node = node;

    // Извлекаем нужные состояния анимации
    _aniState = ent->getAnimationState("RunTop");
    _aniState->setLoop(false);
    _aniStateTop = ent->getAnimationState("RunBase");
    _aniStateTop->setLoop(false);
}

MyFrameListener::~MyFrameListener(){
    // Уничтожаем созданное в конструкторе
    _InputManager->destroyInputObject(_Keyboard);
    _InputManager->destroyInputObject(_Mouse);
    OIS::InputManager::destroyInputSystem(_InputManager);
}


bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt){
    bool walked = false; // Ходила ли сущность в этом кадре
    // Клавиатура
    _Keyboard->capture();
    Ogre::Vector3 translate(0,0,0);
    if( _Keyboard->isKeyDown(OIS::KC_ESCAPE) ){
        return false;
    }
    if( _Keyboard->isKeyDown(OIS::KC_W) ){
        translate += Ogre::Vector3(0,0,-1);
    }
    if( _Keyboard->isKeyDown(OIS::KC_S) ){
        translate += Ogre::Vector3(0,0,1);
    }
    if( _Keyboard->isKeyDown(OIS::KC_A) ){
        translate += Ogre::Vector3(-1,0,0);
    }
    if( _Keyboard->isKeyDown(OIS::KC_D) ){
        translate += Ogre::Vector3(1,0,0);
    }
    if( _Keyboard->isKeyDown(OIS::KC_1) && ! _down1 ){
        _down1 = true;
        _comp1 = !_comp1;
        Ogre::CompositorManager::getSingleton().
        setCompositorEnabled(_viewport, "Compositor2", _comp1);
    }
    if( _Keyboard->isKeyDown(OIS::KC_2) && ! _down2 ){
        _down2 = true;
        _comp2 = !_comp2;
        Ogre::CompositorManager::getSingleton().
        setCompositorEnabled(_viewport, "Compositor3", _comp2);
    }
    if( _Keyboard->isKeyDown(OIS::KC_3) && ! _down3 ){
        _down3 = true;
        _comp3 = !_comp3;
        Ogre::CompositorManager::getSingleton().
        setCompositorEnabled(_viewport, "Compositor7", _comp3);
    }
    if( !_Keyboard->isKeyDown(OIS::KC_1) ){
        _down1 = false;
    }
    if( !_Keyboard->isKeyDown(OIS::KC_2) ){
        _down2 = false;
    }
    if( !_Keyboard->isKeyDown(OIS::KC_3) ){
        _down3 = false;
    }
    _Cam->moveRelative(translate * evt.timeSinceLastFrame * _movementspeed);

    // Мышь
    _Mouse->capture();
    float rotX = _Mouse->getMouseState().X.rel * evt.timeSinceLastFrame* -1;
    float rotY = _Mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
    _Cam->yaw( Ogre::Radian(rotX) );
    _Cam->pitch( Ogre::Radian(rotY) );

    // Вектор перемещения и поворот
    Ogre::Vector3 SinbadTranslate(0,0,0);
    float _rotation = 0.0f;
    if(_Keyboard->isKeyDown(OIS::KC_UP))
    {
        SinbadTranslate += Ogre::Vector3(0,0,-1);
        _rotation = 3.14f;
        walked = true;
    }
    if(_Keyboard->isKeyDown(OIS::KC_DOWN))
    {
        SinbadTranslate += Ogre::Vector3(0,0,1);
        _rotation = 0.0f;
        walked = true;
    }
    if(_Keyboard->isKeyDown(OIS::KC_LEFT))
    {
        SinbadTranslate += Ogre::Vector3(-1,0,0);
        _rotation = -1.57f;
        walked = true;
    }
    if(_Keyboard->isKeyDown(OIS::KC_RIGHT))
    {
        SinbadTranslate += Ogre::Vector3(1,0,0);
        _rotation = 1.57f;
        walked = true;
    }
    if(walked) // Если модель перемещается, включаем анимацию; если анимация закончилась, зацикливаем её
    {
        _aniState->setEnabled(true);
        _aniStateTop->setEnabled(true);
        if( _aniState->hasEnded() )
        {
            _aniState->setTimePosition(0.0f);
        }
        if( _aniStateTop->hasEnded() )
        {
            _aniStateTop->setTimePosition(0.0f);
        }
    }
    else // Если модель не перемещалась, отключаем анимацию и устанавливаем её в стартовую позицию:
    {
        _aniState->setTimePosition(0.0f);
        _aniState->setEnabled(false);
        _aniStateTop->setTimePosition(0.0f);
        _aniStateTop->setEnabled(false);
    }

    _aniState->addTime(evt.timeSinceLastFrame);
    _aniStateTop->addTime(evt.timeSinceLastFrame);

    _node->translate(SinbadTranslate * evt.timeSinceLastFrame * _WalkingSpeed);
    _node->resetOrientation();
    _node->yaw(Ogre::Radian(_rotation));

    return true;
}
/*
bool MyFrameListener::frameEnded(const Ogre::FrameEvent& evt){
    std::cout << "Frame ended" << std::endl;
    return true; // false - Остановит рендеринг и выйдет из приложения
}
bool MyFrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt){
    std::cout << "Frame queued" << std::endl;
    return true; // false - Остановит рендеринг и выйдет из приложения
}
*/
