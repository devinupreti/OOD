/////////////////////////////////////////////////////////////////////
// Test.cpp - Implements Test Functions for Project 2(OOD)Spring 18//
// ver 1.2                                                         //
// Author - Devin Upreti                                           //
/////////////////////////////////////////////////////////////////////

#include "Test.h"

#ifdef DB_TEST

#include <iostream>
using namespace Utilities;
int main()
{
	Utilities::Title("Testing Test Package");

	try {
		Utilities::Title("Testing DbCore - He said, she said database");
		putLine();

		TestExecutive ex;

		// define test structures with test function and message

		TestExecutive::TestStr ts1{ testR1, "Use C++11" };
		TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
		
		ex.registerTest(ts1);
		ex.registerTest(ts2);

		bool result = ex.doTests();
		if (result == true)
			std::cout << "\n  all tests passed";
		else
			std::cout << "\n  at least one test failed";

		std::cout << "\n  Test Package is working properly\n";
		system("pause");
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  -- " << ex.what() << " --";
	}
	std::cout << "\n\n";
}
#endif
