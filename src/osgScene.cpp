#include "osgScene.h"

OsgScene::OsgScene()
{

    mainSceneOsg = new osg::Group;
    root = new osg::Group;

    font_main = osgText::readFontFile(QApplication::applicationDirPath().toStdString()+"/earth/data/fonts/DejaVuMonoSans.ttf");
    font_map = osgText::readFontFile(QApplication::applicationDirPath().toStdString()+"/earth/data/fonts/ObjectFont.ttf");

    csn = new osg::CoordinateSystemNode;
    csn->setEllipsoidModel(new osg::EllipsoidModel());

}

