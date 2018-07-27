//============================================================================
// Name        : TXConverter.cpp
// Author      : Tobi
// Version     : 1.0
// Copyright   : -
// Description : XML-TXT converter in C++
//============================================================================

#include "TXConverter.h"

TXConverter::TXConverter()
{

}

TXConverter::~TXConverter()
{
	// TODO Auto-generated destructor stub
}

unsigned int TXConverter::checkFileType()
{
	if(this->filePath.substr(this->filePath.length()-3,3) == "txt")
		return 1;
	if(this->filePath.substr(this->filePath.length()-3,3) == "xml")
		return 2;
	return 3;
}

void OpeningFileStream::setFilePath(std::string address)
{
	this->filePath = address;
}

bool OpeningFileStream::fstreamActivation(std::ifstream *input, std::string address)
{
	input->open(address.c_str());
	return input->fail() ? true : false;
}

bool OpeningFileStream::fstreamActivation(std::ofstream *output, std::string address)
{
	output->open(address.c_str());
	return output->fail() ? true : false;
}

void OpeningFileStream::userInputPath()
{
	char *menu = new char;
	std::string *filePath = new std::string;
	bool *correctFile = new bool;

	std::ifstream inFile;
	std::cout <<"Type \"1\" to enter file path or \"2\" to choose file from project folder: ";
	std::cin >> *menu;
	do
	{
		switch((int)(*menu)-48)
		{
			case 1:
				std::cout <<"Enter file path (e.g. /home/[user]/Desktop/[file]\"): ";
				std::cin >> *filePath;
				if(this->fstreamActivation(&inFile, *filePath))
				{
					std::cout <<"An error occurred while opening the file." << std::endl;
					*correctFile = false;
					break;
				}
				*correctFile = true;
				break;

			case 2:
				std::cout <<"Enter file name (e.g. [file_name].txt): ";
				std::cin >> *filePath;
				if(this->fstreamActivation(&inFile, *filePath))
				{
					std::cout <<"An error occurred while opening the file." << std::endl;
					*correctFile = false;
					break;
				}
				*correctFile = true;
				break;

			default:
				*correctFile = false;
				std::cout <<"Wrong path. Please try again." << std::endl;
				std::cout <<"Type \"1\" to enter file path or \"2\" to choose file from project folder: ";
				std::cin >> *menu;
				break;
		}
	}
	while(!(*correctFile));

	this->setFilePath(*filePath);

	inFile.close();
	delete menu;
	delete filePath;
	delete correctFile;
}

bool TXConverter::doTXTtoXML()
{
	std::string *ptr_str = new std::string;
	std::ifstream inFile;
	std::ofstream outFile;

	//Checking if files will open properly.
	if(this->fstreamActivation(&inFile, this->filePath))
	{
		return false;
	}
	this->filePath.replace(this->filePath.length()-3, 3, "xml");
	if(this->fstreamActivation(&outFile, this->filePath))
	{
		return false;
	}
	std::cout << "Conversion in process...\n";

	outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
	outFile << "<File>\n";

	//Reading lines from .txt and converting to .xml simultaneously. Skipping for "empty" lines.
	while(getline(inFile, *ptr_str))
	{
		outFile << this->convertLineToXML(ptr_str);
	}

	outFile << "</File>";
	inFile.close();
	outFile.close();
	delete ptr_str;

	return true;
}

std::string TXConverter::convertLineToXML(std::string *ptr_str)
{
	int *ptr_sf = new int;
	std::string *ptr_tmp = new std::string, *result = new std::string;
	*ptr_tmp = *ptr_str;

	//Eliminating spaces and empty lines.
	while((int)(ptr_tmp->at(0)) == 32)		// 32 - Ascii space
	{
		ptr_tmp->erase(0, 1);
	}

	if((int)(ptr_tmp->at(0)) != 13)			// 13 - Ascii Carriage return
	{
		*result += "<Row>\n";
		while(!(*ptr_str).empty())
		{
			//Getting a string placed between two spaces.
			*ptr_sf = ptr_str->find(' ');
			if(*ptr_sf ==  0)
				ptr_str->erase(0, 1);
			else
			{
				if(*ptr_sf == -1)
				{
					*result += "<Col>";
					*ptr_tmp = ptr_str->substr(0, (*ptr_str).length());
					ptr_str->erase(0, (*ptr_str).length());
					*result += *ptr_tmp;
					*result += "</Col>\n";
				}
				else
				{
					*result += "<Col>";
					*ptr_tmp = ptr_str->substr(0, *ptr_sf);
					ptr_str->erase(0, *ptr_sf+1);
					*result += *ptr_tmp;
					*result += "</Col>\n";
				}
			}

		}
		*result += "</Row>\n";
	}
	delete ptr_tmp;
	delete ptr_sf;

	return *result;
}



bool TXConverter::doXMLtoTXT()
{
	bool *rootElementFound = new bool, *firstColumn = new bool, *firstRow = new bool;
	*rootElementFound = false;
	*firstColumn = true;
	*firstRow = true;
	std::string *ptr_str = new std::string, *ptr_root = new std::string, *ptr_tmp = new std::string;
	std::ifstream inFile;
	std::ofstream outFile;

	//Checking if files will open properly.
	if(this->fstreamActivation(&inFile, this->filePath))
	{
		return false;
	}
	this->filePath.replace(this->filePath.length()-3, 3, "txt");
	if(this->fstreamActivation(&outFile, this->filePath))
	{
		return false;
	}

	std::cout << "Conversion in process...\n";

	//Reading lines from .xml and converting to .txt simultaneously.
	while(getline(inFile, *ptr_str))
	{
		if( ptr_str->find("<?xml") )
			continue;

		if(ptr_str->length() != 0)
		{
			//Eliminating spaces and empty lines.
			while((int)(ptr_str->at(0)) == 32)		// 32 - Ascii space
			{
				if(ptr_str->length() == 1)
				{
					break;
				}
				ptr_str->erase(0, 1);
			}
			if(ptr_str->length() == 1)
				continue;

			//Looking for root element.
			if((int)(ptr_str->at(0)) == 60 && *rootElementFound == false)		// 60 - Ascii "<"
			{
				*ptr_root = ptr_str->substr(0, (ptr_str->find(">"))+1);
				ptr_str->erase(0, (ptr_str->find(">"))+1);
				*rootElementFound = true;
				ptr_root->insert(1,"/");
			}

			//Checking if line isn't empty after root element.
			while(!ptr_str->empty())
			{
				//Eliminating spaces and empty lines.
				while((int)(ptr_str->at(0)) == 32)		// 32 - Ascii space
				{
					ptr_str->erase(0, 1);

				}
				if(ptr_str->length() == 1)
					break;

				//Finish reading from .xml file if root is closed.
				if(ptr_str->substr(0, ptr_root->length()) == *ptr_root)
					break;

				if((int)(ptr_str->at(0)) == 60)			// 60 - Ascii "<"
				{
					//Checking if it's a closing of element.
					if((int)(ptr_str->at(1)) == 47)			// 47 - Ascii "/"
					{
						if(ptr_str->substr(2, 3) == "row"
						|| ptr_str->substr(2, 3) == "Row"
						|| ptr_str->substr(2, 3) == "rOw"
						|| ptr_str->substr(2, 3) == "roW"
						|| ptr_str->substr(2, 3) == "ROw"
						|| ptr_str->substr(2, 3) == "RoW"
						|| ptr_str->substr(2, 3) == "rOW"
						|| ptr_str->substr(2, 3) == "ROW")
						{
							ptr_str->erase(0, 6);
							*firstColumn = true;
						}
						else
						{
							ptr_str->erase(0, 5);
						}
					}
					else
					{
						if(ptr_str->substr(1, 3) == "row"
						|| ptr_str->substr(1, 3) == "Row"
						|| ptr_str->substr(1, 3) == "rOw"
						|| ptr_str->substr(1, 3) == "roW"
						|| ptr_str->substr(1, 3) == "ROw"
						|| ptr_str->substr(1, 3) == "RoW"
						|| ptr_str->substr(1, 3) == "rOW"
						|| ptr_str->substr(1, 3) == "ROW")
						{
							if(*firstRow)
							{
								*firstRow = false;
								ptr_str->erase(0, 5);
							}
							else
							{
								outFile << std::endl;
								ptr_str->erase(0, 5);
							}
						}
						else
						{
							if(*firstColumn)
							{
								*firstColumn = false;
								ptr_str->erase(0, 5);
							}
							else
							{
								outFile << " ";
								ptr_str->erase(0, 5);
							}
						}
					}
				}
				else
				{
					*ptr_tmp = ptr_str->substr(0, ptr_str->find("<"));
					outFile << *ptr_tmp;
					ptr_str->erase(0, ptr_tmp->length());
				}
			}
		}
	}

	inFile.close();
	outFile.close();

	return true;
}
