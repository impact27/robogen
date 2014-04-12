//
//  ClimbingScenario.h
//  RoboGen
//
//  Created by Quentin Peter on 12.04.14.
//
//

#ifndef __RoboGen__ClimbingScenario__
#define __RoboGen__ClimbingScenario__

#include <osg/Vec3>
#include "scenario/Scenario.h"

namespace robogen {
	
	/**
	 * Chasing Scenario.
	 * The robot that can reach and stay close to the light source int the given simulation time wins.
	 * The distance is computed as the euclidean distance from the light source to robot computed using as
	 * reference the core component.
	 */
	class ClimbingScenario: public Scenario {
		
	public:
		
		/**
		 * Initializes a RacingScenario
		 */
		ClimbingScenario(boost::shared_ptr<RobogenConfig> robogenConfig);
		
		/**
		 * Destructor
		 */
		virtual ~ClimbingScenario();
		
		/**
		 * Methods inherited from {@link #Scenario}
		 */
		virtual bool setupSimulation();
		virtual bool afterSimulationStep();
		virtual bool endSimulation();
		virtual double getFitness();
		virtual bool remainingTrials();
		virtual bool init(dWorldID odeWorld, dSpaceID odeSpace, boost::shared_ptr<Robot> robot);
		virtual int getCurTrial() const;
		
	private:
		
		std::vector<double> distances_;
		unsigned int curTrial_;
		
	};
	
}

#endif /* defined(__RoboGen__ClimbingScenario__) */
