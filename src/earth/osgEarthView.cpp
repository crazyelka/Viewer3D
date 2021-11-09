#include "osgEarthView.h"
#include "appBoxes.h"

//#include <QDialog>
#include <QHBoxLayout>
//#include <QApplication>
//#include <QDesktopWidget>
//#include <QMap>

#include <string>
#include <osg/Notify>
#include <osg/Timer>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgGA/StateSetManipulator>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarth/GeoMath>
#include <osgEarth/GeoTransform>
#include <osgEarth/MapNode>
#include <osgEarth/TerrainEngineNode>
#include <osgEarth/Viewpoint>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/Controls>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthUtil/LogarithmicDepthBuffer>
#include <osgEarthUtil/ViewFitter>

#include <osgEarthUtil/EarthManipulator>
#include <osgEarthSymbology/GeometryFactory>
#include <osgViewer/Viewer>

using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;
using namespace osgEarth::Annotation;
using namespace osgEarth;
using namespace osgEarth::Util;

static MapNode*       s_mapNode     = 0L;


struct TetherCB : public EarthManipulator::TetherCallback
{
    void operator()(osg::Node* node)
    {
        if ( node ) {
            OE_WARN << "Tether on\n";
        }
        else {
            OE_WARN << "Tether off\n";
        }
    }
};

// An event handler that will print out the elevation at the clicked point
struct QueryElevationHandler : public osgGA::GUIEventHandler
{
    QueryElevationHandler(int i, EarthManipulator *manip, OsgEarthView *widget)
        : _mouseDown( false ),
          _terrain  ( s_mapNode->getTerrain() ),
          _i (i),
          _manip (manip),
          _widget (widget)
    {
        _map = s_mapNode->getMap();
        _path.push_back( s_mapNode->getTerrainEngine() );
        _envelope = _map->getElevationPool()->createEnvelope(_map->getSRS(), 20u);
    }

    void update( float x, float y, osgViewer::View* view )
    {

        // look under the mouse:
        osg::Vec3d world;
        osgUtil::LineSegmentIntersector::Intersections hits;
        if ( view->computeIntersections(x, y, hits) )
        {

            osgUtil::LineSegmentIntersector::Intersections intersections;

            std::string gdlist="";

            if (view->computeIntersections(x, y,intersections))

                for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
                    hitr != intersections.end();
                    ++hitr)
                {
                    std::ostringstream os;
                    if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
                    {
                        //                    osgEarth::Viewpoint vp;
                        //                    vp.setNode(hitr->nodePath.back());
                        //                    _manip->setViewpoint(vp);
                        for (int k = 0; k < hitr->nodePath.size(); k++)
                        {
                            osg::Node *node = hitr->nodePath.at(k);
                            //qDebug()<<QString::fromStdString(node->getName());

                            if (node->getName() == "form")
                                emit _widget->sigClickObject(node);

                        }

                    }
                }

        }

    }

    bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        if (ea.getEventType() == ea.RELEASE &&
                ea.getButton() == ea.LEFT_MOUSE_BUTTON)
        {
            //           qDebug()<<"PUSH";
            osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
            update( ea.getX(), ea.getY(), view );
            return true;
        }

        return false;
    }

    const Map*       _map;
    const Terrain*   _terrain;
    bool             _mouseDown;
    int              _i;
    osg::NodePath    _path;
    osg::ref_ptr<ElevationEnvelope> _envelope;
    QVector<GeoPoint*> vecPoint;
    EarthManipulator* _manip;
    OsgEarthView *_widget;
};

struct ClickToRemoveElevation : public ControlEventHandler
{
    void onClick(Control*)
    {
        Map* map = s_mapNode->getMap();
        ElevationLayerVector layers;
        map->getLayers(layers);
        map->beginUpdate();
        for (ElevationLayerVector::iterator i = layers.begin(); i != layers.end(); ++i) {
            map->removeLayer(i->get());
        }
        map->endUpdate();
    }
};

OsgEarthView::OsgEarthView()
{
    gl_base = new GlgBase;
    appBox->glBase = gl_base;
    _scene = osgDB::readNodeFile(QApplication::applicationDirPath().toStdString()+"/earth/data/maps/openstreetmap_kosmo.earth");

    _viewer.setThreadingModel(_viewer.SingleThreaded);

    addView();

    // timer fires a paint event.
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(40);

    s_mapNode = MapNode::get(view->getSceneData());
    createManip();
    //    manip->getSettings()->bindMouseClick(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN,1);
//    connect(signBox,&SignalBox::clearWorkData,this,&OsgEarthView::clearWorkData);
//    connect(signBox,&SignalBox::addGraphObjects,this,&OsgEarthView::addObjects);
    setMinimumSize(400,400);
}

void OsgEarthView::addView()
{
    QGLFormat frmt; // создать формат по умолчанию
    frmt.setDoubleBuffer(false); // задать простую буферизацию

    manip = new osgEarth::Util::EarthManipulator;
    manip->getSettings()->getBreakTetherActions().push_back( EarthManipulator::ACTION_GOTO );
    //manip->setTetherCallback(new TetherCB);        //нужен???

    osg::Group *root = new osg::Group;
    root->addChild(_scene);
    root->addChild(gl_base->root);
    osg::ClearNode* back = new osg::ClearNode;
    root->addChild(back);

    view = new osgViewer::View();
    view->setCameraManipulator(manip);
    view->setSceneData(root);
    view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,false);
    view->getCamera()->setNearFarRatio(0.0001);
    //    view->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

    _viewer.setThreadingModel(_viewer.SingleThreaded);
    _viewer.addView(view);

    QWidget *viewWdgt = new osgEarth::QtGui::ViewWidget(view,frmt);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(1);
    lay->addWidget(viewWdgt);
    setLayout(lay);
}

void OsgEarthView::paintEvent(QPaintEvent *e)
{
    // обновление всех вьювов
    if (_viewer.getRunFrameScheme() == osgViewer::ViewerBase::CONTINUOUS ||
            _viewer.checkNeedToDoFrame() )
    {
        _viewer.frame();
    }
}

void OsgEarthView::createManip()
{
    //    qDebug()<<"createManip";
//    view->addEventHandler(new QueryElevationHandler(1, manip, this));

}

void OsgEarthView::deleteManip()
{
    view->removeEventHandler(view->getEventHandlers().back());
}

void OsgEarthView::setHandler(int i)
{
    if (i)
    {
        view->removeEventHandler(view->getEventHandlers().back());
        view->addEventHandler(new QueryElevationHandler(i, manip, this));
    }
    else
    {
        view->removeEventHandler(view->getEventHandlers().back());
        return;
    }
}

QWidget *OsgEarthView::addView( osgEarth::Viewpoint vp)
{
    // the new View we want to add:
    view = new osgViewer::View();

    QGLFormat frmt; // создать формат по умолчанию
    frmt.setDoubleBuffer(false); // задать простую буферизацию

    // a widget to hold our view:
    QWidget* viewWidget = new osgEarth::QtGui::ViewWidget(view,frmt);


    EarthManipulator *secondManip = new EarthManipulator;
    //manip->setTetherCallback(new TetherCB);        //нужен???

    secondManip->getSettings()->getBreakTetherActions().push_back( EarthManipulator::ACTION_GOTO );

    view->setCameraManipulator( secondManip );
    view->setSceneData( _scene.get() );
    view->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true,false);
    view->getCamera()->setNearFarRatio(0.0001);
    //    view->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

    _viewer.setThreadingModel(_viewer.SingleThreaded);

    // add it to the composite viewer.
    _viewer.addView( view );


    viewWidget->resize(500,500);
    secondManip->setViewpoint(vp);

    return viewWidget;

}

