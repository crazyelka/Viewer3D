#include "pickObjectHandler.h"
#include "osgView.h"

#include <QDebug>
#include <QMenu>

PickObjectHandler::PickObjectHandler()
{
//    view = _view;
}

bool PickObjectHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
    switch(ea.getEventType())
    {
    case(osgGA::GUIEventAdapter::PUSH):
    {
        osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
        if (view) pick(view,ea);
        return false;
    }
    case(osgGA::GUIEventAdapter::KEYDOWN):
    {
        if (ea.getKey()=='c')
        {
            osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
            osg::ref_ptr<osgGA::GUIEventAdapter> event = new osgGA::GUIEventAdapter(ea);
            event->setX((ea.getXmin()+ea.getXmax())*0.5);
            event->setY((ea.getYmin()+ea.getYmax())*0.5);
            if (view) pick(view,*event);
        }
        return false;
    }
    default:
        return false;
    }
}

void PickObjectHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
{
    osgUtil::LineSegmentIntersector::Intersections intersections;

    std::string gdlist="";

    if (view->computeIntersections(ea,intersections))
    {
        for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
            hitr != intersections.end();
            ++hitr)
        {
            std::ostringstream os;
            if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
            {

                for (int k = 0; k < int(hitr->nodePath.size()); k++)
                {
                    int id = QString::fromStdString(hitr->nodePath.at(k)->getName()).toInt();


                    osg::Node *node = hitr->nodePath.at(k);


                    if (id != 0 && QString::fromStdString(node->getName()).toInt() == id){
//                        emit signBox->objLinkActivated(id);
                        break;
                    }


                }
            }
            else if (hitr->drawable.valid())
            {
//                qDebug()<<"2";
            }

            const osgUtil::LineSegmentIntersector::Intersection::IndexList& vil = hitr->indexList;
            for(unsigned int i=0;i<vil.size();++i)
            {
//                qDebug()<<"3";

            }

//            gdlist += os.str();
        }
    }
}
