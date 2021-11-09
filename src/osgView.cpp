#include <QtWidgets>

#include <osgEarth/MapNode>
#include <osgEarth/TerrainEngineNode>
#include <osgEarthUtil/Controls>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthUtil/ViewFitter>
#include <osgEarthDrivers/xyz/XYZOptions>
#include <osgEarth/Cache>

#include "osgView.h"
#include "osgScene.h"
#include "osgQtWdgt.h"
//#include "dataBox.h"
//#include "enums.h"
//#include "appBoxes.h"
//#include "subjClasses.h"

////observers
//#include "airFieldObsv3D.h"
//#include "rlsObsv3D.h"
//#include "akObsv3D.h"
//#include "wpAmObsv3D.h"
//#include "wpCmObsv3D.h"
//#include "eisObsv3D.h"
//#include "aimPntObsv3D.h"

#include <QLayout>

//using namespace osgEarth::Util;
//using namespace osgEarth::Util::Controls;
//using namespace osgEarth;

OsgScene *osgScene;

//===========================================================================================================================
OsgView::OsgView(QWidget *parent) : QWidget(parent)
{
    osgScene = new OsgScene;

    createActions();
    createViewer();
    setMinimumSize(400,400);

//    connect(signBox,&SignalBox::addGraphObjects,this,&OsgView::addObjects);
//    connect(signBox,&SignalBox::mousePosChanged,this,&OsgView::setCoords);
//    connect(lightSldr, &QSlider::valueChanged, this, &OsgView::setOpacity);
//    connect(sizeModelsSldr, &QSlider::valueChanged, this,&OsgView::setSizeModel);
//    connect(signBox,&SignalBox::clearWorkData,this,&OsgView::clearWorkData);
//    connect(signBox,&SignalBox::goRedraw,this,&OsgView::redraw);
}

void OsgView::clearWorkData()
{
//    for (int i = 1; i <= int(osgScene->mainSceneOsg->getNumChildren()); i ++){
//        //osgScene->mainSceneOsg->removeChild(i);
//    }
//    for (int i = 0; i < objList.count(); i++)
//        delete objList.at(i);
//    objList.clear();
}
void OsgView::redraw()
{
//    if(objList.isEmpty()) return;
//    for(auto gi : objList){gi->drawIn=0; gi->update();}
}
void OsgView::setCoords(double fi, double la)
{
    QString fiSfx = (fi>0)?" С":" Ю";
    QString laSfx = (la>0)?" В":" З";

//    QString fiStr = appBox->degToStr(qAbs(fi));
//    QString laStr = appBox->degToStr(qAbs(la));
//    fiLbl->setText(fiSfx+"Ш: "+fiStr);
//    laLbl->setText(laSfx+"Д: "+laStr);
}
void OsgView::setOpacity(double i)
{
    filter->setBrightnessContrast(osg::Vec2f(1-i/10,1));
}
void OsgView::setSizeModel(int i)
{
    osgScene->sizeModels = i*i;
    redraw();
//    appBox->iMsg(QString("Модели объектов увеличены в ")+QString::number(i)+QString(" раз."));
}
void OsgView::createActions()
{
    lightAct = new QAction(QIcon(":sunLight"),tr("Ярче"), this);
    connect(lightAct, &QAction::triggered, this, &OsgView::setLightFltr);

    darkAct = new QAction(QIcon(":sunDark"),tr("Темнее"), this);
    connect(darkAct, &QAction::triggered, this, &OsgView::setLightFltr);

    zoomOutModelsAct = new QAction(QIcon(":zoomOut"),tr("Меньше"), this);
    connect(zoomOutModelsAct, &QAction::triggered, this, &OsgView::setValueModelsSize);

    zoomInModelsAct = new QAction(QIcon(":zoomIn"),tr("Больше"), this);
    connect(zoomInModelsAct, &QAction::triggered, this, &OsgView::setValueModelsSize);
}

void OsgView::setLightFltr()
{
    int val = lightSldr->value();
    if(sender() == lightAct) --val;
    if(sender() == darkAct)  ++val;
    lightSldr->setValue(val);
}
void OsgView::setValueModelsSize()
{
    int val = sizeModelsSldr->value();
    if(sender() == zoomOutModelsAct) --val;
    if(sender() == zoomInModelsAct)  ++val;
    sizeModelsSldr->setValue(val);
}

void OsgView::createViewer()
{
    osg::ref_ptr<osg::Node>  earth = osgDB::readNodeFile(QApplication::applicationDirPath().toStdString()
                                                         +"/earth/data/maps/openstreetmap_kosmo.earth");

    osgScene->root = new osg::Group;
    osgScene->map = MapNode::get(earth);


    osgScene->root->addChild(earth);
    osgScene->root->addChild(osgScene->mainSceneOsg);

    osg::ClearNode* back = new osg::ClearNode;
    QColor backColor = Qt::darkGray;
    back->setClearColor(osg::Vec4d(backColor.redF(),backColor.greenF(),backColor.blueF(), backColor.alphaF()));
    osgScene->root->addChild(back);

    //фильтр яркости/контрастности
    filter = new BrightnessContrastColorFilter;
    ImageLayerVector imageLayers;
    osgScene->map->getMap()->getLayers(imageLayers);
    ImageLayer* layer = imageLayers[0].get();
    layer->addColorFilter( filter );

    QWidget *topPanel = new QWidget();
    topPanel->setFixedHeight(26);
//    QToolButton *lightBtn = FuncBox::createToolButton(lightAct);
//    QToolButton *darkBtn = FuncBox::createToolButton(darkAct);
//    lightSldr = FuncBox::createSlider(0,10);

//    QToolButton *outBtn = FuncBox::createToolButton(zoomOutModelsAct);
//    QToolButton *inBtn = FuncBox::createToolButton(zoomInModelsAct);
//    sizeModelsSldr = FuncBox::createSlider(1,20);
//    sizeModelsSldr->setTickInterval(20);

    fiLbl = new QLabel();
    laLbl = new QLabel();
    fiLbl->setFixedWidth(90);
    laLbl->setFixedWidth(90);

//    QHBoxLayout *topLayout = new QHBoxLayout();
//    topLayout->setMargin(1);
//    topLayout->addStretch(1);
//    topLayout->addWidget(lightBtn);
//    topLayout->addWidget(lightSldr);
//    topLayout->addWidget(darkBtn);
//    topLayout->addSpacing(10);
//    topLayout->addWidget(outBtn);
//    topLayout->addWidget(sizeModelsSldr);
//    topLayout->addWidget(inBtn);
//    topLayout->addSpacing(20);
//    topLayout->addWidget(fiLbl);
//    topLayout->addWidget(laLbl);
//    topLayout->addSpacing(5);
//    topPanel->setLayout(topLayout);

    OsgQtWdgt *viewWdgt= new OsgQtWdgt(1,1, osgScene->root, osgScene->map,this);
    osgScene->manip = viewWdgt->manip;

    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->setMargin(1);
    mainLay->setSpacing(0);
    mainLay->addWidget(topPanel);
    mainLay->addWidget(viewWdgt);

    setLayout(mainLay);

}

void OsgView::addObjects()
{
//#define INIT_3D_OBSV(TYPE,SUBJ) \
//    {TYPE *obsv=new TYPE(SUBJ,osgScene); SUBJ->attach(obsv); objList.append(obsv); break;}
//    for(auto subj : qAsConst(dataBox->subjectMap))
//    {
//        switch (subj->typeSubj)
//        {
//        case BE::Rls:       INIT_3D_OBSV(RlsObsv3D,subj);
//        case BE::AirField:  INIT_3D_OBSV(AirFieldObsv3D,subj);
//        case BE::Eis:       INIT_3D_OBSV(EisObsv3D,subj);
//        case BE::AkFuel:
//        case BE::AkAdf:
//        case BE::AkAwacs:
//        case BE::AkConv:
//        case BE::AkAis:
//        case BE::AkStrike:
//        case BE::PakDa:     INIT_3D_OBSV(AkObsv3D,subj);
//        case BE::WpAm:      INIT_3D_OBSV(WpAmObsv3D,subj);
//        case BE::WpCm:      INIT_3D_OBSV(WpCmObsv3D,subj);
//        case BE::AimPnt:    INIT_3D_OBSV(AimPntObsv3D,subj);
//        }
//    }
}


