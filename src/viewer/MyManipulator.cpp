//
//  MyManipulator.cpp
//  RoboGen
//
//  Created by Quentin Peter on 08.04.14.
//
//

#include "MyManipulator.h"
#include <osg/Version>
void MyManipulator::updateCamera (osg::Camera &camera){
	setCenter(a_robot->getCoreComponent()->getRootPosition()*1000);
	
	#if OSG_VERSION_GREATER_OR_EQUAL(3, 2, 0)
	osgGA::TrackballManipulator::updateCamera(camera);
	#endif
}

bool MyManipulator::handleMouseWheel (const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us){
	osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
	
	if(sm ==osgGA::GUIEventAdapter::SCROLL_2D){
		float dy=ea.getScrollingDeltaY();
		// perform zoom
		zoomModel( -dy*_wheelZoomFactor, true );
		us.requestRedraw();
		us.requestContinuousUpdate( isAnimating() || _thrown );
		return true;
	}
	else{
		return osgGA::TrackballManipulator::handleMouseWheel(ea, us);
	}
}