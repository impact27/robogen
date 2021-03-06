/*
 * @(#) RobogenConfig.h   1.0   Mar 12, 2013
 *
 * Andrea Maesani (andrea.maesani@epfl.ch)
 * Titus Cieslewski (dev@titus-c.ch)
 *
 * The ROBOGEN Framework
 * Copyright © 2012-2013 Andrea Maesani
 *
 * Laboratory of Intelligent Systems, EPFL
 *
 * This file is part of the ROBOGEN Framework.
 *
 * The ROBOGEN Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (GPL)
 * as published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @(#) $Id$
 */
#ifndef ROBOGEN_ROBOGEN_CONFIG_H_
#define ROBOGEN_ROBOGEN_CONFIG_H_

#include <boost/shared_ptr.hpp>
#include "config/ObstaclesConfig.h"
#include "config/StartPositionConfig.h"
#include "config/TerrainConfig.h"
#include "robogen.pb.h"

namespace robogen {

/**
 * Parameters for experimental scenarios supported by the simulator
 */
class RobogenConfig {

public:

	enum SimulationScenario {
		CHASING, RACING, CLIMBING
	};

	/**
	 * Initializes a robogen config object from configuration parameters
	 */
	RobogenConfig(SimulationScenario scenario, unsigned int timeSteps,
			float timeStepLength, boost::shared_ptr<TerrainConfig> terrain,
			boost::shared_ptr<ObstaclesConfig> obstacles,
			std::string obstacleFile,
			boost::shared_ptr<StartPositionConfig> startPositions,
			std::string startPosFile, float lightSourceHeight,float lightSourceX,float lightSourceY) :
			scenario_(scenario), timeSteps_(timeSteps), timeStepLength_(
					timeStepLength), terrain_(terrain), obstacles_(obstacles),
					obstacleFile_(obstacleFile),
					startPositions_(startPositions),
					startPosFile_(startPosFile),
					lightSourceHeight_(lightSourceHeight),
					lightSourceX_(lightSourceX),
					lightSourceY_(lightSourceY){

		simulationTime_ = timeSteps * timeStepLength;

	}

	/**
	 * Initializes a robogen config object from a message
	 */
	RobogenConfig(const robogenMessage::SimulatorConf &message);

	/**
	 * Destructor
	 */
	virtual ~RobogenConfig() {

	}

	/**
	 * @return the simulation scenario
	 */
	SimulationScenario getScenario() const {
		return scenario_;
	}

	/**
	 * @return the terrain configuration
	 */
	boost::shared_ptr<TerrainConfig> getTerrainConfig() {
		return terrain_;
	}

	/**
	 * @return the obstacles configuration
	 */
	boost::shared_ptr<ObstaclesConfig> getObstaclesConfig() {
		return obstacles_;
	}

	/**
	 * @return the number of timesteps
	 */
	unsigned int getTimeSteps() const {
		return timeSteps_;
	}

	/**
	 * @return the total simulation time
	 */
	float getSimulationTime() const {
		return simulationTime_;
	}

	/**
	 * @return the time step length
	 */
	float getTimeStepLength() const {
		return timeStepLength_;
	}

	/**
	 * @return the robot starting positions
	 */
	boost::shared_ptr<StartPositionConfig> getStartingPos() {
		return startPositions_;
	}

	/**
	 * @return the starting position configuration file
	 */
	std::string getStartPosFile(){
		return startPosFile_;
	}

	/**
	 * @return the obstacle configuration file
	 */
	std::string getObstacleFile(){
		return obstacleFile_;
	}

	/**
	 * @return the desired height of the light source
	 */
	float getLightSourceHeight(){
		return lightSourceHeight_;
	}
	/**
	 * @return the desired height of the light source
	 */
	float getLightSourceX(){
		return lightSourceX_;
	}
	/**
	 * @return the desired height of the light source
	 */
	float getLightSourceY(){
		return lightSourceY_;
	}

	/**
	 * Convert configuration into configuration message.
	 */
	robogenMessage::SimulatorConf serialize() const{
		robogenMessage::SimulatorConf ret;
		ret.set_lightsourceheight(lightSourceHeight_);
		ret.set_lightsourcex(lightSourceX_);
		ret.set_lightsourcey(lightSourceY_);
		ret.set_ntimesteps(timeSteps_);
		if (scenario_ == CHASING) {
			ret.set_scenario("chasing");
		} else if (scenario_ == RACING) {
			ret.set_scenario("racing");
		} else if (scenario_ == CLIMBING) {
			ret.set_scenario("climbing");
		}
		ret.set_terrainlength(terrain_->getLength());
		ret.set_terrainwidth(terrain_->getWidth());
		ret.set_timestep(timeStepLength_);
		obstacles_->serialize(ret);
		startPositions_->serialize(ret);
		return ret;
	}

private:

	/**
	 * The simulation scenario
	 */
	SimulationScenario scenario_;

	/**
	 * Total number of simulation timesteps
	 */
	unsigned int timeSteps_;

	/**
	 * Time step duration
	 */
	float timeStepLength_;

	/**
	 * Terrain configuration
	 */
	boost::shared_ptr<TerrainConfig> terrain_;

	/**
	 * Obstacles configuration
	 */
	boost::shared_ptr<ObstaclesConfig> obstacles_;

	/**
	 * Obstacle configuration file location
	 */
	std::string obstacleFile_;

	/**
	 * List of robot starting positions
	 */
	boost::shared_ptr<StartPositionConfig> startPositions_;

	/**
	 * Starting positions configuration file location
	 */
	std::string startPosFile_;

	/**
	 * Height of light source in chasing scenario
	 */
	float lightSourceHeight_;
	float lightSourceX_;
	float lightSourceY_;

	/**
	 * Simulation time
	 */
	float simulationTime_;

};

}

#endif /* ROBOGEN_ROBOGEN_CONFIG_H_ */
