/*
 * @(#) ConfigurationReader.h   1.0   Mar 13, 2013
 *
 * Andrea Maesani (andrea.maesani@epfl.ch)
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
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>

#include "config/ConfigurationReader.h"
#include "config/ObstaclesConfig.h"
#include "config/RobogenConfig.h"
#include "config/StartPosition.h"
#include "config/StartPositionConfig.h"
#include "config/TerrainConfig.h"

#define DEFAULT_LIGHT_SOURCE_HEIGHT (0.1)
#define DEFAULT_LIGHT_SOURCE_X (0)
#define DEFAULT_LIGHT_SOURCE_Y (0)
#define DEFAULT_OBSTACLE_DENSITY (0.)

namespace robogen {

boost::shared_ptr<RobogenConfig> ConfigurationReader::parseConfigurationFile(
		const std::string& fileName) {

	boost::program_options::options_description desc(
			"Allowed options for Simulation Config File");
	desc.add_options()("terrainType",
			boost::program_options::value<std::string>(),
			"Terrain type: flat or rough")("terrainHeightField",
			boost::program_options::value<std::string>(),
			"Height Field for terrain generation")("terrainWidth",
			boost::program_options::value<float>(), "Terrain width")(
			"terrainHeight", boost::program_options::value<float>(),
			"Terrain height")("terrainLength",
			boost::program_options::value<float>(), "Terrain length")(
			"obstaclesConfigFile", boost::program_options::value<std::string>(),
			"Obstacles configuration file")("scenario",
			boost::program_options::value<std::string>(), "Experiment scenario")(
			"lightSourceHeight", boost::program_options::value<float>(),
			"Height of light source")
			("lightSourceX", boost::program_options::value<float>(),
			"X position of light source")
			("lightSourceY", boost::program_options::value<float>(),
			"Y position of light source")
			("timeStep",
			boost::program_options::value<float>(), "Time step duration")(
			"nTimeSteps", boost::program_options::value<unsigned int>(),
			"Number of timesteps")("startPositionConfigFile",
			boost::program_options::value<std::string>(),
			"Start Positions Configuration File");

	if (fileName == "help") {
		desc.print(std::cout);
		return boost::shared_ptr<RobogenConfig>();
	}

	boost::program_options::variables_map vm;
	boost::program_options::store(
			boost::program_options::parse_config_file<char>(fileName.c_str(),
					desc, true), vm);
	boost::program_options::notify(vm);

	// Read terrain configuration
	float terrainLength;
	float terrainWidth;
	std::string terrainType;

	if (!vm.count("terrainType")) {
		std::cout << "Undefined 'terrainType' parameter in '" << fileName << "'"
				<< std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}

	if (!vm.count("terrainLength")) {
		std::cout << "Undefined 'terrainLength' parameter in '" << fileName
				<< "'" << std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}

	if (!vm.count("terrainWidth")) {
		std::cout << "Undefined 'terrainWidth' parameter in '" << fileName
				<< "'" << std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}

	terrainType = vm["terrainType"].as<std::string>();
	terrainLength = vm["terrainLength"].as<float>();
	terrainWidth = vm["terrainWidth"].as<float>();

	boost::shared_ptr<TerrainConfig> terrain;
	if (terrainType.compare("flat") == 0) {

		terrain.reset(new TerrainConfig(terrainLength, terrainWidth));

	} else if (terrainType.compare("rugged") == 0) {

		std::string terrainHeightField;
		float terrainHeight;

		if (!vm.count("terrainHeightField")) {
			std::cout << "Undefined 'terrainHeightField' parameter in '"
					<< fileName << "'" << std::endl;
			return boost::shared_ptr<RobogenConfig>();
		}

		if (!vm.count("terrainHeight")) {
			std::cout << "Undefined 'terrainHeight' parameter in '" << fileName
					<< "'" << std::endl;
			return boost::shared_ptr<RobogenConfig>();
		}

		terrainHeightField = vm["terrainHeightField"].as<std::string>();
		terrainHeight = vm["terrainHeight"].as<float>();

		terrain.reset(
				new TerrainConfig(terrainHeightField, terrainLength,
						terrainWidth, terrainHeight));

	} else {
		std::cout << "Unknown value of 'terrainType' parameter in '" << fileName
				<< "'" << std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}

	// Read obstacles configuration
	if (!vm.count("obstaclesConfigFile")) {
		std::cout << "Undefined 'obstaclesConfigFile' parameter in '"
				<< fileName << "'" << std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}


	const boost::filesystem::path filePath(fileName);

	std::string obstaclesConfigFile =
			vm["obstaclesConfigFile"].as<std::string>();

	const boost::filesystem::path obstaclesConfigFilePath(obstaclesConfigFile);
	if (!obstaclesConfigFilePath.is_absolute()) {
		const boost::filesystem::path absolutePath =
				boost::filesystem::absolute(obstaclesConfigFilePath,
						filePath.parent_path());
		obstaclesConfigFile = absolutePath.string();
	}




	boost::shared_ptr<ObstaclesConfig> obstacles = parseObstaclesFile(
			obstaclesConfigFile);
	if (obstacles == NULL) {
		return boost::shared_ptr<RobogenConfig>();
	}

	// Read obstacles configuration
	if (!vm.count("startPositionConfigFile")) {
		std::cout << "Undefined 'startPositionConfigFile' parameter in '"
				<< fileName << "'" << std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}

	std::string startPositionFile =
			vm["startPositionConfigFile"].as<std::string>();

	const boost::filesystem::path startPositionFilePath(startPositionFile);
	if (!startPositionFilePath.is_absolute()) {
		const boost::filesystem::path absolutePath =
				boost::filesystem::absolute(startPositionFilePath,
						filePath.parent_path());
		startPositionFile = absolutePath.string();
	}


	boost::shared_ptr<StartPositionConfig> startPositions =
			parseStartPositionFile(startPositionFile);
	if (startPositions == NULL) {
		return boost::shared_ptr<RobogenConfig>();
	}

	// Read generic parameters
	if (!vm.count("scenario")) {
		std::cout << "Undefined 'scenario' parameter in '" << fileName << "'"
				<< std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}
	std::string scenario = vm["scenario"].as<std::string>();
	RobogenConfig::SimulationScenario simulationScenario;

	if (scenario.compare("racing") == 0) {
		simulationScenario = RobogenConfig::RACING;
	} else if (scenario.compare("chasing") == 0) {
		simulationScenario = RobogenConfig::CHASING;
	} else if (scenario.compare("climbing") == 0) {
		simulationScenario = RobogenConfig::CLIMBING;
	} else {
		std::cout << "Undefined 'scenario' parameter in '" << fileName << "'"
				<< std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}

	float lightSourceHeight = DEFAULT_LIGHT_SOURCE_HEIGHT;
	if (vm.count("lightSourceHeight")) {
		lightSourceHeight = vm["lightSourceHeight"].as<float>();
	}
	
	float lightSourceX = DEFAULT_LIGHT_SOURCE_X;
	if (vm.count("lightSourceX")) {
		lightSourceX = vm["lightSourceX"].as<float>();
	}
	
	float lightSourceY = DEFAULT_LIGHT_SOURCE_Y;
	if (vm.count("lightSourceY")) {
		lightSourceY = vm["lightSourceY"].as<float>();
	}

	if (!vm.count("timeStep")) {
		std::cout << "Undefined 'timeStep' parameter in '" << fileName << "'"
				<< std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}
	float timeStep = vm["timeStep"].as<float>();

	if (!vm.count("nTimeSteps")) {
		std::cout << "Undefined 'nTimesteps' parameter in '" << fileName << "'"
				<< std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}
	unsigned int nTimesteps = vm["nTimeSteps"].as<unsigned int>();

	return boost::shared_ptr<RobogenConfig>(
			new RobogenConfig(simulationScenario, nTimesteps, timeStep, terrain,
					obstacles, obstaclesConfigFile, startPositions,
					startPositionFile, lightSourceHeight, lightSourceX, lightSourceY));

}

boost::shared_ptr<ObstaclesConfig> ConfigurationReader::parseObstaclesFile(
		const std::string& fileName) {

	std::ifstream obstaclesFile(fileName.c_str());
	if (!obstaclesFile.is_open()) {
		std::cout << "Cannot find obstacles file: '" << fileName << "'"
				<< std::endl;
		return boost::shared_ptr<ObstaclesConfig>();
	}

	std::vector<osg::Vec2> coordinate;
	std::vector<osg::Vec3> size;
	std::vector<float> densities;
	float x;
	while (obstaclesFile >> x) {

		float y;
		if (!(obstaclesFile >> y)) {
			std::cout << "Malformed obstacles file: '" << fileName << "'"
					<< std::endl;
			return boost::shared_ptr<ObstaclesConfig>();
		}

		float xSize;
		if (!(obstaclesFile >> xSize)) {
			std::cout << "Malformed obstacles file: '" << fileName << "'"
					<< std::endl;
			return boost::shared_ptr<ObstaclesConfig>();
		}

		float ySize;
		if (!(obstaclesFile >> ySize)) {
			std::cout << "Malformed obstacles file: '" << fileName << "'"
					<< std::endl;
			return boost::shared_ptr<ObstaclesConfig>();
		}

		float zSize;
		if (!(obstaclesFile >> zSize)) {
			std::cout << "Malformed obstacles file: '" << fileName << "'"
					<< std::endl;
			return boost::shared_ptr<ObstaclesConfig>();
		}

		float density;
		if (!(obstaclesFile >> density)) {
			std::cout << "Malformed obstacles file: '" << fileName << "'"
					<< std::endl;
			return boost::shared_ptr<ObstaclesConfig>();
		}

		coordinate.push_back(osg::Vec2(x, y));
		size.push_back(osg::Vec3(xSize, ySize, zSize));
		densities.push_back(density);
	}

	return boost::shared_ptr<ObstaclesConfig>(
			new ObstaclesConfig(coordinate, size, densities));
}

boost::shared_ptr<StartPositionConfig> ConfigurationReader::parseStartPositionFile(
		const std::string& fileName) {

	std::ifstream startPosFile(fileName.c_str());
	if (!startPosFile.is_open()) {
		std::cout << "Cannot find start position file: '" << fileName << "'"
				<< std::endl;
		return boost::shared_ptr<StartPositionConfig>();
	}
	std::vector<boost::shared_ptr<StartPosition> > startPositions;
	float x, y, azimuth;
	while (startPosFile >> x) {
		if (!(startPosFile >> y)) {
			std::cout << "Malformed start position file: '" << fileName << "'"
					<< std::endl;
			return boost::shared_ptr<StartPositionConfig>();
		}
		if (!(startPosFile >> azimuth)) {
			std::cout << "Malformed start position file: '" << fileName << "'"
					<< std::endl;
			return boost::shared_ptr<StartPositionConfig>();
		}
		startPositions.push_back(
				boost::shared_ptr<StartPosition>(new StartPosition()));
		if (!startPositions.back()->init(osg::Vec2(x, y), azimuth)) {
			std::cout << "Problem initializing start position!" << std::endl;
			return boost::shared_ptr<StartPositionConfig>();
		}
	}

	return boost::shared_ptr<StartPositionConfig>(
			new StartPositionConfig(startPositions));

}

boost::shared_ptr<RobogenConfig> ConfigurationReader::parseRobogenMessage(
		const robogenMessage::SimulatorConf& simulatorConf) {

	// Decode obstacles
	std::vector<osg::Vec2> obstaclesCoord;
	std::vector<osg::Vec3> obstaclesSize;
	std::vector<float> obstaclesDensity;
	for (int i = 0; i < simulatorConf.obstacles_size(); ++i) {

		const robogenMessage::Obstacle& o = simulatorConf.obstacles(i);
		obstaclesCoord.push_back(osg::Vec2(o.x(), o.y()));
		obstaclesSize.push_back(osg::Vec3(o.xsize(), o.ysize(), o.zsize()));
		obstaclesDensity.push_back(o.density());

	}
	boost::shared_ptr<ObstaclesConfig> obstacles(
			new ObstaclesConfig(obstaclesCoord, obstaclesSize,
					obstaclesDensity));

	// Decode start positions
	std::vector<boost::shared_ptr<StartPosition> > startPositions;
	for (int i = 0; i < simulatorConf.startpositions_size(); ++i) {
		const robogenMessage::StartPosition& s = simulatorConf.startpositions(
				i);
		boost::shared_ptr<StartPosition> newStartPos(new StartPosition());
		newStartPos->init(osg::Vec2(s.x(), s.y()), s.azimuth());
		startPositions.push_back(newStartPos);
	}

	// Decode terrain configuration
	boost::shared_ptr<TerrainConfig> terrain(
			new TerrainConfig(simulatorConf.terrainlength(),
					simulatorConf.terrainwidth()));

	// Decode simulator configuration
	RobogenConfig::SimulationScenario simulationScenario;
	std::string scenario = simulatorConf.scenario();

	if (scenario.compare("racing") == 0) {
		simulationScenario = RobogenConfig::RACING;
	} else if (scenario.compare("chasing") == 0) {
		simulationScenario = RobogenConfig::CHASING;
	} else if (scenario.compare("climbing") == 0) {
		simulationScenario = RobogenConfig::CLIMBING;
	} else {
		std::cout << "Undefined 'scenario' parameter" << std::endl;
		return boost::shared_ptr<RobogenConfig>();
	}

	unsigned int timeSteps = simulatorConf.ntimesteps();
	float timeStepLength = simulatorConf.timestep();
	float lightSourceHeight = simulatorConf.lightsourceheight();
	float lightSourceX = simulatorConf.lightsourcex();
	float lightSourceY = simulatorConf.lightsourcey();

	return boost::shared_ptr<RobogenConfig>(
			new RobogenConfig(simulationScenario, timeSteps, timeStepLength, terrain,
					obstacles, "",
					boost::shared_ptr<StartPositionConfig>(
							new StartPositionConfig(startPositions)), "",
					lightSourceHeight, lightSourceX, lightSourceY

					));

}

}
