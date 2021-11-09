#ifndef OSGSCENE_H
#define OSGSCENE_H

#include <osgText/Font>
#include <osg/Group>
#include <osg/CoordinateSystemNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/Map>
#include <osg/ClusterCullingCallback>

#include <QApplication>

#include <QDebug>

/** Базовый клас сцены, хранит основные параметры */
class OsgScene
{
public:
    osg::ref_ptr<osgText::Font> font_main;      /** Шрифт названий объектов */
    osg::ref_ptr<osgText::Font> font_map;       /** Шрифт значков объектов */
    osg::ref_ptr<osg::CoordinateSystemNode> csn;/** Система координат Земли, для расчета нормали*/

    osg::ref_ptr<osg::Group> mainSceneOsg;      /** Главная сцена для добавления всех объектов */
    osg::ref_ptr<osg::Group> root;              /** Корень сцены для Земли и объектов отрисовки */

    osgEarth::Util::EarthManipulator *manip;    /** Манипулятор сцены */
    osgEarth::MapNode *map;                     /** Map для чтения earth файла */

    int sizeModels =1;
    uint detailsBortRLS =3;
    uint detailsLandRLS =3;

    OsgScene();

};

extern OsgScene *osgScene;

#endif // OSGSCENE_H
