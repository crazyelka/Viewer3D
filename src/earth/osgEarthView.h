#ifndef OSGEARTHVIEW_H
#define OSGEARTHVIEW_H

#include "glBase.h"

#include <QWidget>
//#include <QObject>
//#include <QHBoxLayout>


#include <osg/Notify>
#include <osgViewer/CompositeViewer>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthQt/ViewWidget>
#include <osgEarth/Random>
#include <osgEarth/FileUtils>


using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::QtGui;




struct ViewController
{
    virtual void addView() =0;
};



class OsgEarthView : public QWidget, public ViewController
{
    Q_OBJECT

public:
    OsgEarthView();

    /// Функция создания главного 3Д окна
    void addView();
    /// Функция создания дополнительного окна, с фокусировкой на объект
    QWidget* addView(Viewpoint vp);

    QWidget* viewWidget;
    osgViewer::View* view;
    osgEarth::Util::EarthManipulator *manip;

    GlgBase *gl_base;

private:
    QTimer                     _timer;
    osgViewer::CompositeViewer _viewer;
//    QHBoxLayout *lay;

    osg::ref_ptr<osg::Node>    _scene;

    void paintEvent(QPaintEvent* e);

public slots:
    void createManip();
    void deleteManip();
    void setHandler(int);
signals:
    void sigClickObject(osg::Node*);

};



#endif // OSGEARTHVIEW_H
