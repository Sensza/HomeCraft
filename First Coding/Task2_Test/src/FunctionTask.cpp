/*
 * FunctionTask.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: tomasz
 */

#include "../headers/FunctionTask.h"


int FunctionTask::getConfig()
{
	return this->config;
}

void FunctionTask::setConfig(int conf)
{
	this->config = conf;
}

bool FunctionTask::loadConfig(std::string address)
{
	std::string result;
	std::ifstream inFile(address.c_str());
	if(inFile.fail())
		return false;
	getline(inFile, result);

	if(result == "POL")
		this->setConfig(1);
	else
		if(result == "ENG")
			this->setConfig(2);
		else
			if(result == "GER")
				this->setConfig(3);
			else
				return false;

	inFile.close();
	return true;
}

bool FunctionTask::loadData()
{
	std::ifstream inFile;
	std::istringstream issin;
	std::string tmp;

	switch(this->config)
	{
		case 1: inFile.open("monthPOL.txt");
			if(inFile.fail())
				return false; break;
		case 2: inFile.open("monthENG.txt");
			if(inFile.fail())
				return false; break;
		case 3: inFile.open("monthGER.txt");
			if(inFile.fail())
				return false; break;
		default: return false; break;
	}

	for(int i = 0; i < 12; i++)
	{
		getline(inFile, tmp);
		issin.str(tmp);
		issin >> month[i];
	}

	return true;
}

std::string FunctionTask::returnMonth(int whichMonth)
{
	if(whichMonth >= 1 && whichMonth <=12)
		return month[whichMonth-1];
	else
		return "ERROR";
}
