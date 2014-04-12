//
//  MyManipulator.h
//  RoboGen
//
//  Created by Quentin Peter on 08.04.14.
//
//

#ifndef __RoboGen__MyManipulator__
#define __RoboGen__MyManipulator__

#include <iostream>
#include <osgGA/TrackballManipulator>

#include "Robot.h"

class MyManipulator : public osgGA::TrackballManipulator {
private:
	boost::shared_ptr<robogen::Robot> a_robot;
	virtual bool handleMouseWheel (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
	
	
	
public:
	MyManipulator(boost::shared_ptr<robogen::Robot> rob):osgGA::TrackballManipulator(),a_robot(rob){}
	virtual void updateCamera (osg::Camera &camera);
	
	
	
	
};

#endif /* defined(__RoboGen__MyManipulator__) */
