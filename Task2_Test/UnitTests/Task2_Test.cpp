//============================================================================
// Name        : Task2_Test.cpp
// Author      : Tobi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "gtest/gtest.h"

#include "../headers/FunctionTask.h"

struct FunctionTaskTest : testing::Test
{
	FunctionTask *testObject;
	void SetUp() { testObject = new FunctionTask(); }
	void TearDown() { delete testObject; }
};

TEST_F(FunctionTaskTest, getConfig_setConfig_Test_1)
{
	testObject->setConfig(1);
	ASSERT_EQ(testObject->getConfig(), 1);
}

TEST_F(FunctionTaskTest, getConfig_setConfig_Test_2)
{
	testObject->setConfig(2);
	ASSERT_NE(testObject->getConfig(), 1);
}


TEST_F(FunctionTaskTest, loadConfig_Test_1)
{
	ASSERT_EQ(testObject->loadConfig("Test_ConfigPOL.txt"), true);
}

TEST_F(FunctionTaskTest, loadConfig_Test_2)
{
	ASSERT_EQ(testObject->loadConfig("Test_ConfigENG.txt"), true);
}

TEST_F(FunctionTaskTest, loadConfig_Test_3)
{
	ASSERT_EQ(testObject->loadConfig("Test_ConfigGER.txt"), true);
}

TEST_F(FunctionTaskTest, loadConfig_Test_4)
{
	ASSERT_EQ(testObject->loadConfig("Test_ConfigSWE.txt"), false);
}

TEST_F(FunctionTaskTest, loadConfig_Test_5)
{
	ASSERT_EQ(testObject->loadConfig("Test_NoFile.txt"), false);
}

TEST_F(FunctionTaskTest, loadConfig_Test_6)
{
	ASSERT_EQ(testObject->loadConfig("Config.txt"), true);
}


TEST_F(FunctionTaskTest, returnMonth_Test_1)
{
	testObject->setConfig(1);
	if(testObject->loadData())
	{
		std::string value = testObject->returnMonth(1);
		ASSERT_STREQ(value.c_str(), "Styczen");
	}
	else
		ASSERT_FALSE(1);
}

TEST_F(FunctionTaskTest, returnMonth_Test_2)
{
	testObject->setConfig(1);
	if(testObject->loadData())
	{
		std::string value = testObject->returnMonth(12);
		ASSERT_STREQ(value.c_str(), "Grudzien");
	}
	else
		ASSERT_FALSE(1);
}

TEST_F(FunctionTaskTest, returnMonth_Test_3)
{
	testObject->setConfig(1);
	if(testObject->loadData())
	{
		std::string value = testObject->returnMonth(0);
		ASSERT_STREQ(value.c_str(), "ERROR");
	}
	else
		ASSERT_FALSE(1);
}

TEST_F(FunctionTaskTest, returnMonth_Test_4)
{
	testObject->setConfig(1);
	if(testObject->loadData())
	{
		std::string value = testObject->returnMonth(13);
		ASSERT_STREQ(value.c_str(), "ERROR");
	}
	else
		ASSERT_FALSE(1);
}


TEST_F(FunctionTaskTest, loadData_Test_1)
{
	testObject->setConfig(1);
	if(testObject->loadData())
	{
		std::string value = testObject->returnMonth(1);
		ASSERT_STREQ(value.c_str(), "Styczen");
	}
	else
		ASSERT_FALSE(1);
}

TEST_F(FunctionTaskTest, loadData_Test_2)
{
	testObject->setConfig(2);
	if(testObject->loadData())
	{
		std::string value = testObject->returnMonth(1);
		ASSERT_STREQ(value.c_str(), "January");
	}
	else
		ASSERT_FALSE(1);
}

TEST_F(FunctionTaskTest, loadData_Test_3)
{
	testObject->setConfig(3);
	if(testObject->loadData())
	{
		std::string value = testObject->returnMonth(1);
		ASSERT_STREQ(value.c_str(), "Januar");
	}
	else
		ASSERT_FALSE(1);
}

TEST_F(FunctionTaskTest, loadData_Test_4)
{
	testObject->setConfig(4);
	if(testObject->loadData())
	{
		ASSERT_FALSE(1);
	}
	else
		ASSERT_TRUE(1);
}



int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
