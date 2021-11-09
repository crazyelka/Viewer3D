#include "queryCrdHandler.h"

#include <osgEarth/GeoData>
#include <osgEarthUtil/TerrainProfile>

#include <QDebug>

QueryCrdHandler::QueryCrdHandler(osgEarth::MapNode *mapNode)
{
    _mapNode = mapNode;
    _terrain = mapNode->getTerrain();
    _map = mapNode->getMap();
}

void QueryCrdHandler::update( float x, float y, osgViewer::View* view )
{
    osg::Vec3d world;
    osgUtil::LineSegmentIntersector::Intersections hits;
    if ( view->computeIntersections(x, y, hits) )
    {
        world = hits.begin()->getWorldIntersectPoint();
        osgEarth::GeoPoint mapPoint;
        mapPoint.fromWorld( _terrain->getSRS(), world );

        double a,b;
        _terrain->getHeight(_terrain->getSRS(),mapPoint.y(),mapPoint.x(),&a,&b);

    }
}

bool QueryCrdHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    if (ea.getEventType() == ea.MOVE)
    {
        osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
        update( ea.getX(), ea.getY(), view );
        return true;
    }

    return false;
}
