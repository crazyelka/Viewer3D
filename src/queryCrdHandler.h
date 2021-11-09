#ifndef QUERYCRDHANDLER_H
#define QUERYCRDHANDLER_H
#include <osgGA/GUIEventHandler>
#include <osgEarth/Map>
#include <osgEarth/Terrain>
#include <osgEarth/MapNode>
#include <osgViewer/View>

/** Класс для определения координат мыши */
class QueryCrdHandler: public osgGA::GUIEventHandler
{
public:
    QueryCrdHandler(osgEarth::MapNode *mapNode);
private:
    void update(float x, float y, osgViewer::View* view);
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );

    osgEarth::Map *_map;
    osgEarth::Terrain *_terrain;
    osgEarth::MapNode *_mapNode;

    osg::NodePath    _path;
};

#endif // QUERYCRDHANDLER_H
