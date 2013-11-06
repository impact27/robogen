/*
 * @(#) PartList.cpp   1.0   Nov 5, 2013
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
#include "PartList.h"

namespace robogen {

//first define init functions that will populate these maps

std::map<char, std::string> initPartTypeMap() {
	std::map<char, std::string> partTypeMap;
	partTypeMap['K'] = PART_TYPE_ACTIVE_CARDAN;
	partTypeMap['I'] = PART_TYPE_ACTIVE_HINGE;
	partTypeMap['J'] = PART_TYPE_ACTIVE_WHEEL;
	partTypeMap['G'] = PART_TYPE_ACTIVE_WHEG;
	partTypeMap['E'] = PART_TYPE_CORE_COMPONENT;
	partTypeMap['F'] = PART_TYPE_FIXED_BRICK;
	partTypeMap['L'] = PART_TYPE_LIGHT_SENSOR;
	partTypeMap['B'] = PART_TYPE_PARAM_JOINT;
	partTypeMap['C'] = PART_TYPE_PASSIVE_CARDAN;
	partTypeMap['H'] = PART_TYPE_PASSIVE_HINGE;
	partTypeMap['W'] = PART_TYPE_PASSIVE_WHEEL;
	partTypeMap['R'] = PART_TYPE_ROTATOR;
	partTypeMap['T'] = PART_TYPE_TOUCH_SENSOR;
	return partTypeMap;
}

std::map<std::string, unsigned int> initPartTypeArityMap() {
	std::map<std::string, unsigned int> partTypeArityMap;
	partTypeArityMap[PART_TYPE_ACTIVE_CARDAN] = 1;
	partTypeArityMap[PART_TYPE_ACTIVE_HINGE] = 1;
	partTypeArityMap[PART_TYPE_ACTIVE_WHEEL] = 0;
	partTypeArityMap[PART_TYPE_ACTIVE_WHEG] = 0;
	partTypeArityMap[PART_TYPE_CORE_COMPONENT] = 5;
	partTypeArityMap[PART_TYPE_FIXED_BRICK] = 5;
	partTypeArityMap[PART_TYPE_LIGHT_SENSOR] = 0;
	partTypeArityMap[PART_TYPE_PARAM_JOINT] = 1;
	partTypeArityMap[PART_TYPE_PASSIVE_CARDAN] = 1;
	partTypeArityMap[PART_TYPE_PASSIVE_HINGE] = 1;
	partTypeArityMap[PART_TYPE_PASSIVE_WHEEL] = 1;
	partTypeArityMap[PART_TYPE_ROTATOR] = 1;
	partTypeArityMap[PART_TYPE_TOUCH_SENSOR] = 0;
	return partTypeArityMap;
}

std::map<std::string, unsigned int> initPartTypeParamCountMap() {
	std::map<std::string, unsigned int> partTypeParamCountMap;
	partTypeParamCountMap[PART_TYPE_ACTIVE_CARDAN] = 0;
	partTypeParamCountMap[PART_TYPE_ACTIVE_HINGE] = 0;
	partTypeParamCountMap[PART_TYPE_ACTIVE_WHEEL] = 1;
	partTypeParamCountMap[PART_TYPE_ACTIVE_WHEG] = 1;
	partTypeParamCountMap[PART_TYPE_CORE_COMPONENT] = 0;
	partTypeParamCountMap[PART_TYPE_FIXED_BRICK] = 0;
	partTypeParamCountMap[PART_TYPE_LIGHT_SENSOR] = 0;
	partTypeParamCountMap[PART_TYPE_PARAM_JOINT] = 3;
	partTypeParamCountMap[PART_TYPE_PASSIVE_CARDAN] = 0;
	partTypeParamCountMap[PART_TYPE_PASSIVE_HINGE] = 0;
	partTypeParamCountMap[PART_TYPE_PASSIVE_WHEEL] = 1;
	partTypeParamCountMap[PART_TYPE_ACTIVE_HINGE] = 0;
	partTypeParamCountMap[PART_TYPE_TOUCH_SENSOR] = 0;
	return partTypeParamCountMap;
}

std::map<std::string, std::vector<std::string> > initPartTypeMotorsMap() {
	std::map<std::string, std::vector<std::string> > partTypeMotorsMap;
	{
		std::vector<std::string> motors;
		motors.push_back(PART_TYPE_ACTIVE_CARDAN + std::string("-tilt-1"));
		motors.push_back(PART_TYPE_ACTIVE_CARDAN + std::string("-tilt-2"));
		partTypeMotorsMap[PART_TYPE_ACTIVE_CARDAN] = motors;
	}

	{
		std::string singleMotorParts[] = { PART_TYPE_ACTIVE_HINGE,
		PART_TYPE_ACTIVE_WHEEL, PART_TYPE_ACTIVE_WHEG,
		PART_TYPE_ACTIVE_HINGE };
		int numSingleMotorParts = 4;
		for (int i = 0; i < numSingleMotorParts; i++) {
			std::vector<std::string> motors;
			motors.push_back(singleMotorParts[i]);
			partTypeMotorsMap[singleMotorParts[i]] = motors;
		}
	}
	return partTypeMotorsMap;
}
std::map<std::string, std::vector<std::string> > initPartTypeSensorsMap() {
	std::map<std::string, std::vector<std::string> > partTypeSensorsMap;
	{
		std::vector<std::string> sensors;
		sensors.push_back("x-acceleration");
		sensors.push_back("y-acceleration");
		sensors.push_back("z-acceleration");
		sensors.push_back("Pitch");
		sensors.push_back("Roll");
		sensors.push_back("Yaw");
		partTypeSensorsMap[PART_TYPE_CORE_COMPONENT] = sensors;
	}

	{
		std::vector<std::string> sensors;
		sensors.push_back(PART_TYPE_LIGHT_SENSOR);
		partTypeSensorsMap[PART_TYPE_LIGHT_SENSOR] = sensors;
	}

	{
		std::vector<std::string> sensors;
		sensors.push_back(PART_TYPE_TOUCH_SENSOR + std::string("-left"));
		sensors.push_back(PART_TYPE_TOUCH_SENSOR + std::string("-right"));
		partTypeSensorsMap[PART_TYPE_TOUCH_SENSOR] = sensors;
	}

	return partTypeSensorsMap;
}

//initialize the maps
std::map<char, std::string> PART_TYPE_MAP = initPartTypeMap();
std::map<std::string, unsigned int> PART_TYPE_ARITY_MAP =
		initPartTypeArityMap();
std::map<std::string, unsigned int> PART_TYPE_PARAM_COUNT_MAP =
		initPartTypeParamCountMap();
std::map<std::string, std::vector<std::string> > PART_TYPE_MOTORS_MAP =
		initPartTypeMotorsMap();
std::map<std::string, std::vector<std::string> > PART_TYPE_SENSORS_MAP =
		initPartTypeSensorsMap();

} /* namespace robogen */
