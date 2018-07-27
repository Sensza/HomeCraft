//============================================================================
// Name        : TXConverter.h
// Author      : Tobi
// Version     : 1.0
// Copyright   : -
// Description : XML-TXT converter in C++
//============================================================================

#ifndef TXConverter_H_
#define TXConverter_H_
#include <iostream>
#include <fstream>
#include <string>

class OpeningFileStream
{
public:
	std::string filePath;
	void userInputPath();
	void setFilePath(std::string address);
	bool fstreamActivation(std::ifstream *input, std::string address);
	bool fstreamActivation(std::ofstream *output, std::string address);
};


class TXConverter : public OpeningFileStream
{
public:
	TXConverter();
	virtual ~TXConverter();
	bool doTXTtoXML();
	bool doXMLtoTXT();
	unsigned int checkFileType();
	std::string convertLineToXML(std::string *ptr_str);
};

#endif /* TXConverter_H_ */
