#ifndef OSGVIEW_H
#define OSGVIEW_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include <osgEarthUtil/BrightnessContrastColorFilter>
#include <osgViewer/Viewer>
#include <osgEarth/MapNode>

//using namespace osgEarth;
using namespace osgEarth::Util;
//class OsgScene;
class QSlider;
class BaseObsv3D;

class OsgView : public QWidget
{
    Q_OBJECT

public:
    QLabel *fiLbl;
    QLabel *laLbl;
    QSlider *lightSldr;
    QSlider *sizeModelsSldr;

    QAction *lightAct;
    QAction *darkAct;
    QAction *zoomInModelsAct;
    QAction *zoomOutModelsAct;

    QVector<BaseObsv3D *> objList;

    void clearWorkData();
    void setCoords(double fi, double la);
    void setOpacity(double i);
    void setSizeModel(int i);
    void createActions();
    void setLightFltr();
    void setValueModelsSize();
    void redraw();

    OsgView(QWidget * parent = nullptr);

    /** Функция создания главного 3Д окна */
    void createViewer();

    QWidget* viewWidget;
    BrightnessContrastColorFilter* filter;

private:

    void addObjects();

signals:
    void sigClickObject(osg::Node*);

};



#endif // OSGVIEWER_H
