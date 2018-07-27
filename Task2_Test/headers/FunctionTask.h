/*
 * FunctionTask.h
 *
 *  Created on: Jul 23, 2018
 *      Author: tomasz
 */

#ifndef HEADERS_FUNCTIONTASK_H_
#define HEADERS_FUNCTIONTASK_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class FunctionTask
{
	int config;
	std::string month[12];
public:
	int getConfig();
	void setConfig(int conf);
	bool loadConfig(std::string address);
	bool loadData();
	std::string returnMonth(int whichMonth);
};



#endif /* HEADERS_FUNCTIONTASK_H_ */
