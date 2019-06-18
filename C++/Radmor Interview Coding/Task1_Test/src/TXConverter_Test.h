//============================================================================
// Name        : TXConverter_Test.h
// Author      : Tobi
// Version     : 1.0
// Copyright   : -
// Description : XML-TXT converter in C++
//============================================================================

#ifndef TXCONVERTER_TEST_H_
#define TXCONVERTER_TEST_H_

#include <iostream>
#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>
#include "TXConverter.h"


class TestTXConverter : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE(TestTXConverter);
	CPPUNIT_TEST(testFileCheck);
	CPPUNIT_TEST(testConversion);
	CPPUNIT_TEST_SUITE_END();
public:
	void testFileCheck();
	void testConversion();
};


class TestOpeningFileStream : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE(TestOpeningFileStream);
	CPPUNIT_TEST(testFilePath);
	CPPUNIT_TEST(testFstreamActivation);
	CPPUNIT_TEST_SUITE_END();
public:
	void testFilePath();
	void testFstreamActivation();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestTXConverter);
CPPUNIT_TEST_SUITE_REGISTRATION(TestOpeningFileStream);

#endif /* TXCONVERTER_TEST_H_ */
