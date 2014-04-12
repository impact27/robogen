//
//  ClimbingScenario.cpp
//  RoboGen
//
//  Created by Quentin Peter on 12.04.14.
//
//

#include "ClimbingScenario.h"
#include "config/RobogenConfig.h"
#include "config/StartPositionConfig.h"
#include "scenario/Environment.h"
#include "Robot.h"
#include "Models.h"

namespace robogen {
	
	ClimbingScenario::ClimbingScenario(boost::shared_ptr<RobogenConfig> robogenConfig) :
	Scenario(robogenConfig), curTrial_(0) {
		
	}
	
	ClimbingScenario::~ClimbingScenario() {
		
	}
	
	bool ClimbingScenario::setupSimulation() {
		
		this->distances_.push_back(0);
		
		return true;
		
	}
	
	bool ClimbingScenario::init(dWorldID odeWorld, dSpaceID odeSpace, boost::shared_ptr<Robot> robot) {
		
		Scenario::init(odeWorld, odeSpace, robot);
		
		boost::shared_ptr<Environment> env(new Environment());
		env->setAmbientLight(10);
		std::vector<boost::shared_ptr<LightSource> > lightSources;
		lightSources.push_back(boost::shared_ptr<LightSource>(
															  new LightSource(odeSpace, osg::Vec3(0, 0,
																								  this->getRobogenConfig()->getLightSourceHeight()), 100)));
		env->setLightSources(lightSources);
		
		this->setEnvironment(env);
		return true;
	}
	
	
	bool ClimbingScenario::afterSimulationStep() {
		
		// Compute distance from light source
		osg::Vec3 curPos = this->getRobot()->getCoreComponent()->getRootPosition();
		osg::Vec3 lightSourcePos = this->getEnvironment()->getLightSources()[0]->getPosition();
		
		osg::Vec3 temp = curPos - lightSourcePos;
		this->distances_[curTrial_] += temp.length();
		
		return true;
	}
	
	bool ClimbingScenario::endSimulation() {
		
		// Compute robot ending position
		curTrial_++;
		// Set next starting position
		this->setStartingPosition(curTrial_);
		return true;
		
	}
	
	double ClimbingScenario::getFitness() {
		
		double fitness = 1000;//0;
		for (unsigned int i = 0; i < distances_.size(); ++i) {
			double trialFit = -1.0 * distances_[i]/this->getRobogenConfig()->getTimeSteps();
			if (trialFit < fitness)
				fitness = trialFit;
		}
		return fitness;
		// We transform everything into a maximization problem
		//return -1*(fitness/distances_.size());
	}
	
	bool ClimbingScenario::remainingTrials() {
		boost::shared_ptr<StartPositionConfig> startPos = this->getRobogenConfig()->getStartingPos();
		return curTrial_ < startPos->getStartPosition().size();
	}
	
	int ClimbingScenario::getCurTrial() const {
		return curTrial_;
	}
	
}