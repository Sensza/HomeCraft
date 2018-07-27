//============================================================================
// Name        : Task1_Test.cpp
// Author      : Tobi
// Version     : 1.0
// Copyright   : -
// Description : XML-TXT converter in C++
//============================================================================

#include "TXConverter.h"
#include "TXConverter_Test.h"


int main()
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());
	runner.addTest(new TestTXConverter());
	runner.addTest(new TestOpeningFileStream());
	runner.run();

	return 0;
}
