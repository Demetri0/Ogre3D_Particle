#include "myapplication.h"
#include "myframelistener.h"

MyApplication::MyApplication()
{
    // Об'NULL'яем члены класса
    _sceneManager = NULL;
    _root         = NULL;
    _listener     = NULL;
    _keepRunning  = true;
}
MyApplication::~MyApplication()
{
    // Подчищаем за собой ресурсы
    delete _root;
    delete _listener;
}

void MyApplication::loadResources(){
    Ogre::ConfigFile cf;
    //!cf.load("resources.cfg"); // Так писать - не хорошо, ниже пример - "как хорошо" :)

    #if OGRE_DEBUG_MODE
        cf.load("../resource/resources_d.cfg");
    #else
        cf.load("../resource/resources.cfg");
    #endif

    // Получаем итератор отсчитывающий каждую секцию config-файла
    Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();

    // Определяем 3 строки для сохранения данных, которые мы собираемся извлечь
    Ogre::String sectionName, typeName, dataname;
    while ( sectionIter.hasMoreElements() ){
        sectionName = sectionIter.peekNextKey(); // Получаем имя секции

        // Получите настройки, содержащиеся в секции и, в то же самое время продвигаем итератор секции
        Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i; // Создаём итератор для самих настроек

        // Пробежимя по каждой настройке в секции
        for (i = settings->begin(); i != settings->end(); ++i){
            typeName = i->first;  // Получаем тип
            dataname = i->second; // и имя ресурсов
            // Используем имя ресурса, тип ресурса и имя секции, чтобы добавить его к индексу ресурсов:
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataname, typeName, sectionName);
        }
    }
    // Индексация ресурсов - Загружает текстуры и наверно, ещё что-то. Текстур без этой строчки небыло.
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApplication::createScene(){
    // Cоздаём экземпляр меша и добавьте его к сцене
//    Ogre::Entity* ent = _sceneManager->createEntity("Sinbad", "Sinbad.mesh");


    // Cоздаём экземпляр меша и добавьте его к сцене
    _SinbadEnt = _sceneManager->createEntity("Sinbad", "Sinbad.mesh");
    _SinbadNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
    _SinbadNode->attachObject( _SinbadEnt );

    // Создание системы частиц
//    Ogre::ParticleSystem* partSystem = _sceneManager->createParticleSystem("Smoke","Examples/Smoke");
//    _SinbadNode->attachObject(partSystem);

    // Создание плоскости
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -5);
    Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 200, 200, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::Entity* ground= _sceneManager->createEntity("LightPlaneEntity", "plane");
    _sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(ground);
    ground->setMaterialName("Examples/BumpyMetal"); // Examples/BumpyMetal  || Examples/BeachStones

    // Освещение - Направленый свет
    Ogre::Light* light = _sceneManager->createLight("Light1");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(1,-1,0));

    // Тени
    _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}

int MyApplication::startUp(){
    _root = new Ogre::Root("../resource/plugins.cfg");

    if (!_root->restoreConfig()){      // Если настроек ещё нет
        if(!_root->showConfigDialog()) // Отображаем окно настроек
            return -1;                 // Или при нажатии Cancel - выходим.
        _root->saveConfig();           // Сохраним настройки
    }
    Ogre::RenderWindow* window = _root->initialise(true,"Window Title"); //Создание окна рендера
    _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC); // Создание менеджера сцены

    Ogre::Camera* camera = _sceneManager->createCamera("Camera"); // Создание камеры
    camera->setPosition(Ogre::Vector3(0, 0, 50)); // Установка позиции камеры
    camera->lookAt(Ogre::Vector3(0, 0, 0));       // Направление взгляда камеры
    camera->setNearClipDistance(5);               // #? Ниэр дистанц, всё ясно да?

    Ogre::Viewport* viewport = window->addViewport(camera);         // Создание порта просмотра
    viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));// Цвет фона чёрный

    //Используя порт просмотра, устанавливаем соотношение высоты и ширины
    camera->setAspectRatio( Ogre::Real( viewport->getActualWidth() ) / Ogre::Real( viewport->getActualHeight() ) );

    loadResources(); // Функция загрузки ресурсов
    createScene();   // Функция создания сцены

    Ogre::CompositorManager::getSingleton().addCompositor(viewport, "Compositor2");
    Ogre::CompositorManager::getSingleton().addCompositor(viewport, "Compositor3");
    Ogre::CompositorManager::getSingleton().addCompositor(viewport, "Compositor7");

    _listener = new MyFrameListener(window, camera, viewport, _SinbadNode, _SinbadEnt);  // Создание объекта Listener
    _root->addFrameListener(_listener);              // Привязка его к RootFrameListener
    //_root->startRendering();                       // Начинаем рендеринг

    return 0;
}
void MyApplication::renderOneFrame(){
    Ogre::WindowEventUtilities::messagePump();  // #?
    _keepRunning = _root->renderOneFrame();     // Рендерим фрейм и сохраняем значение
}
bool MyApplication::keepRunning(){
    return _keepRunning;
}
