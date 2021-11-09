#ifndef PICKOBJECTHANDLER_H
#define PICKOBJECTHANDLER_H


#include <osgGA/GUIEventHandler>
#include <osgViewer/View>

class OsgView;

/** Класс отслеживания нажатия на объект сцены */
class PickObjectHandler : public osgGA::GUIEventHandler
{
public:
    PickObjectHandler();

private:

    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
    virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
};

#endif // PICKOBJECTHANDLER_H
