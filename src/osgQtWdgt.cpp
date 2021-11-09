#include "osgQtWdgt.h"
#include "queryCrdHandler.h"
#include "pickObjectHandler.h"
#include "osgScene.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include <osg/Camera>
#include <osg/StateSet>
#include <osg/Material>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/Map>

#include <QtDebug>

OsgQtWdgt::OsgQtWdgt(qreal scaleX, qreal scaleY, osg::Group *scene, osgEarth::MapNode *mapNode, QWidget* parent)
    :QOpenGLWidget(parent)
{
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::StrongFocus);

    m_scaleX = scaleX;
    m_scaleY = scaleY;
    _mView = new osgViewer::View;
    _mGraphicsWindow = new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(),
                                                              this->width(), this->height() ) ;
    _mScene = scene;
    _mViewer = new osgViewer::Viewer;

    osg::Camera* camera = _mView->getCamera();
    camera->setViewport( 0, 0, this->width(), this->height() );
    float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
    camera->setProjectionMatrixAsPerspective( 30.f, aspectRatio, 1.f, 1000.f );
    camera->setGraphicsContext( _mGraphicsWindow );
    _mViewer->setCamera(camera);
    _mViewer->setSceneData(_mScene);

    manip = new osgEarth::Util::EarthManipulator;
    setSettingsManipulator(manip);

    _mViewer->getCamera()->setNearFarRatio(0.00002);
    this->setMouseTracking(true);
    _mViewer->setCameraManipulator(manip);
    _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    _mViewer->realize();
    _mViewer->setSceneData(scene);

    _mViewer->addEventHandler(new QueryCrdHandler(mapNode));
    _mViewer->addEventHandler(new PickObjectHandler());
}
OsgQtWdgt::OsgQtWdgt(qreal scaleX,qreal scaleY, QString earthFile, QWidget* parent)
    :QOpenGLWidget(parent)
{
    osg::ref_ptr<osg::Node>  earth = osgDB::readNodeFile(earthFile.toStdString());
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::StrongFocus);

    m_scaleX = scaleX;
    m_scaleY = scaleY;
    _mView = new osgViewer::View;
    _mGraphicsWindow = new osgViewer::GraphicsWindowEmbedded( this->x(), this->y(),
                                                              this->width(), this->height() ) ;
    _mScene = new osg::Group;

    _mScene->addChild(earth);
    _mScene->addChild(osgScene->mainSceneOsg);

    osg::ClearNode* back = new osg::ClearNode;
    QColor backColor = Qt::darkGray;
    back->setClearColor(osg::Vec4d(backColor.redF(),backColor.greenF(),backColor.blueF(), backColor.alphaF()));
    _mScene->addChild(back);
    _mViewer = new osgViewer::Viewer;

    osg::Camera* camera = _mView->getCamera();
    camera->setViewport( 0, 0, this->width(), this->height() );
    float aspectRatio = static_cast<float>( this->width()) / static_cast<float>( this->height() );
    camera->setProjectionMatrixAsPerspective( 30.f, aspectRatio, 1.f, 1000.f );
    camera->setGraphicsContext( _mGraphicsWindow );

    _mViewer->setCamera(camera);
    _mViewer->setSceneData(_mScene);
    manip = new osgEarth::Util::EarthManipulator;
    setSettingsManipulator(manip);
    _mViewer->getCamera()->setNearFarRatio(0.00002);

    this->setMouseTracking(true);
    _mViewer->setCameraManipulator(manip);
    _mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    _mViewer->realize();
}

void OsgQtWdgt::setScale(qreal X, qreal Y)
{
    m_scaleX = X;
    m_scaleY = Y;
    this->resizeGL(this->width(), this->height());
}

void OsgQtWdgt::paintGL()
{
    _mViewer->frame();
}

void OsgQtWdgt::resizeGL(int width, int height)
{
    this->getEventQueue()->windowResize(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
    _mGraphicsWindow->resized(this->x()*m_scaleX, this->y() * m_scaleY, width*m_scaleX, height*m_scaleY);
    osg::Camera* camera = _mViewer->getCamera();
    camera->setViewport(0, 0, this->width()*m_scaleX, this->height()* m_scaleY);
}

void OsgQtWdgt::initializeGL()
{

}
void OsgQtWdgt::setSettingsManipulator(osgEarth::Util::EarthManipulator *manipulator)
{
    //настройка скролла для манипулятора (через ктрл)
    manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN,
                                     osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP,
                                     osgGA::GUIEventAdapter::MODKEY_CTRL);

    manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_OUT,
                                     osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN ,
                                     osgGA::GUIEventAdapter::MODKEY_CTRL);

    manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN,
                                     osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP);
    manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_OUT,
                                     osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN );

//    manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_NULL,
//                                     osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN);
//    manipulator->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_NULL,
//                                     osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP);

    manipulator->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_EARTH_DRAG,
                                    osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON,
                                    osgGA::GUIEventAdapter::MODKEY_CTRL);
    manipulator->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
                                    osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON);
    manipulator->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
                                    osgGA::GUIEventAdapter::MouseButtonMask::RIGHT_MOUSE_BUTTON);

    manipulator->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
                                    osgGA::GUIEventAdapter::MouseButtonMask::MIDDLE_MOUSE_BUTTON);
    manipulator->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_ROTATE,
                                    osgGA::GUIEventAdapter::MouseButtonMask::MIDDLE_MOUSE_BUTTON,
                                    osgGA::GUIEventAdapter::MODKEY_CTRL);
    manipulator->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_ROTATE,
                                    osgGA::GUIEventAdapter::MouseButtonMask::LEFT_MOUSE_BUTTON,
                                    osgGA::GUIEventAdapter::MODKEY_SHIFT);    
}
void OsgQtWdgt::mouseMoveEvent(QMouseEvent* event)
{
    this->getEventQueue()->mouseMotion(event->x()*m_scaleX, event->y()*m_scaleY);
}

void OsgQtWdgt::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        ctrlFlag = true;

    setKeyboardModifiers(event);

    int value = s_QtKeyboardMap.remapKey(event);
    this->getEventQueue()->keyPress(value);
}

void OsgQtWdgt::keyReleaseEvent(QKeyEvent *event)
{
    ctrlFlag = false;

    if(event->isAutoRepeat())
    {
        event->ignore();
    }
    else
    {
        setKeyboardModifiers(event);

        int value = s_QtKeyboardMap.remapKey(event);
        this->getEventQueue()->keyRelease(value);
    }
}

void OsgQtWdgt::mousePressEvent(QMouseEvent* event)
{
    if (mousePress && !ctrlFlag)
        return;
    mousePress = true;
    unsigned int button = 0;
    switch (event->button()){
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MiddleButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    case Qt::NoButton:
        button = 0;
        break;

    default:
        button = 0;
        break;
    }

    if (button == 3 && !ctrlFlag && !flSecondWgt){
        manip->cancelViewpointTransition();

    }

    setKeyboardModifiers(event);
    this->getEventQueue()->mouseButtonPress(event->x()*m_scaleX,
                                            event->y()*m_scaleY,
                                            button);
}

void OsgQtWdgt::mouseReleaseEvent(QMouseEvent* event)
{
    mousePress = false;

    unsigned int button = 0;
    switch (event->button()){
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MiddleButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    case Qt::NoButton:
        button = 0;
        break;

    default:
        button = 0;
        break;
    }
    setKeyboardModifiers(event);
    this->getEventQueue()->mouseButtonRelease(event->x()*m_scaleX,
                                              event->y()*m_scaleY,
                                              button);
}

void OsgQtWdgt::mouseDoubleClickEvent(QMouseEvent* event)
{
    int button = 0;

    switch(event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;

    case Qt::MidButton:
        button = 2;
        break;

    case Qt::RightButton:
        button = 3;
        break;

    case Qt::NoButton:
        button = 0;
        break;

    default:
        button = 0;
        break;
    }

    setKeyboardModifiers(event);
    this->getEventQueue()->mouseDoubleButtonPress(event->x()*m_scaleX,
                                                  event->y()*m_scaleY,
                                                  button);
}

void OsgQtWdgt::wheelEvent(QWheelEvent* event)
{
    setKeyboardModifiers(event);

    int delta = event->delta();
    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
                osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
    this->getEventQueue()->mouseScroll(motion);
}

bool OsgQtWdgt::event(QEvent* event)
{
    bool handled = QOpenGLWidget::event(event);
    this->update();
    return handled;
}

void OsgQtWdgt::setKeyboardModifiers(QInputEvent* event)
{
    unsigned int modkey = event->modifiers() & (Qt::ShiftModifier |
                                                Qt::ControlModifier |
                                                Qt::AltModifier);
    unsigned int mask = 0;

    if(modkey & Qt::ShiftModifier) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;

    if(modkey & Qt::ControlModifier) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;

    if(modkey & Qt::AltModifier) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;

    this->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);

}
osgGA::EventQueue* OsgQtWdgt::getEventQueue() const
{
    osgGA::EventQueue* eventQueue = _mGraphicsWindow->getEventQueue();
    return eventQueue;
}


int QtKeyboardMap::remapKey(QKeyEvent* event)
{
    KeyMap::iterator itr = mKeyMap.find(event->key());

    if(itr == mKeyMap.end())
    {
        return int(*(event->text().toLatin1().data()));
    }
    else
        return itr->second;
}
