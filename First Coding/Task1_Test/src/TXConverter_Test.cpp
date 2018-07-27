//============================================================================
// Name        : TXConverter_Test.cpp
// Author      : Tobi
// Version     : 1.0
// Copyright   : -
// Description : XML-TXT converter in C++
//============================================================================

#include "TXConverter.h"
#include "TXConverter_Test.h"


void TestTXConverter::testFileCheck()
{
	TXConverter *testObject = new TXConverter();
	int *result = new int;

	//Is file type recognized properly?
	testObject->setFilePath("exampleFile.txt");
	*result = testObject->checkFileType();
	CPPUNIT_ASSERT_EQUAL(1, *result);
	testObject->setFilePath("exampleFile.xml");
	*result = testObject->checkFileType();
	CPPUNIT_ASSERT_EQUAL(2, *result);
	testObject->setFilePath("exampleFile.cpp");
	*result = testObject->checkFileType();
	CPPUNIT_ASSERT_EQUAL(3, *result);

	delete testObject;
	delete result;
}

void TestOpeningFileStream::testFilePath()
{
	TXConverter *testObject = new TXConverter();

	//Is filePath written correctly?
	testObject->setFilePath("exampleFile.txt");
	CPPUNIT_ASSERT(testObject->filePath == "exampleFile.txt");
	testObject->setFilePath("exampleFile.xml");
	CPPUNIT_ASSERT(testObject->filePath != "differentFile.doc");

	delete testObject;
}

void TestOpeningFileStream::testFstreamActivation()
{
	TXConverter *testObject = new TXConverter();
	std::ifstream inFile;
	std::ofstream outFile;

	//Is file opened properly?
	bool *result = new bool;
	*result = testObject->fstreamActivation(&inFile, "Test1.txt");
	CPPUNIT_ASSERT_EQUAL(false, *result);
	inFile.close();
	*result = testObject->fstreamActivation(&outFile, "Test4.txt");
	CPPUNIT_ASSERT_EQUAL(false, *result);
	outFile.close();
	*result = testObject->fstreamActivation(&inFile, "Test5.txt");
	CPPUNIT_ASSERT_EQUAL(true, *result);
	inFile.close();

	delete testObject;
	delete result;
}

void TestTXConverter::testConversion()
{
	TXConverter *testObject = new TXConverter();
	std::string *tmp = new std::string;

	//Is text converted properly?
	*tmp = "Kot ma Ale";
	*tmp = testObject->convertLineToXML(tmp);
	CPPUNIT_ASSERT(*tmp == "<Row>\n<Col>Kot</Col>\n<Col>ma</Col>\n<Col>Ale</Col>\n</Row>\n");
	*tmp = "           Kot";
	*tmp = testObject->convertLineToXML(tmp);
	CPPUNIT_ASSERT(*tmp == "<Row>\n<Col>Kot</Col>\n</Row>\n");


	delete testObject;
	delete tmp;
}

