#pragma once
/////////////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Executes all the tests for Project 1 (OOD), S18     //
// ver 1.0                                                                 //
// Author - Devin Upreti                                                   //
/////////////////////////////////////////////////////////////////////////////

#include "../Test/Test.h"
#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../CheckIn/CheckIn.h"
#include "../CheckIn/CheckIn.cpp"
#include "../CheckOut/CheckOut.h"
#include "../CheckOut/CheckOut.cpp"
#include "../Browse/Browse.h"
#include "../Browse/Browse.cpp"
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../RepositoryCore/RepositoryCore.h"
#include <functional>
#include <string>
#include <iostream>
#include <vector>

using namespace NoSqlDb;
using namespace Utilities;

int main()
{
	Utilities::Title("Testing CoreRepoistory");
	putLine();
	std::cout << "\n Please View in Full Screen";
	std::cout << "\n Workspace address for the Repository Testbed is ..\\Folders\\ \n";
	
	TestExecutive ex; // now define test structures with test function and message
	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "Demonstrate CheckIn" };
	TestExecutive::TestStr ts3b{ testR3b, "Demonstrate CheckOut" };
	TestExecutive::TestStr ts3c{ testR3c, "Demonstrate Browse" };
	TestExecutive::TestStr ts3d{ testR3d, "Demonstrate Version" };
	TestExecutive::TestStr ts3e{ testR3e, "Demonstrate Repository Core" };
	TestExecutive::TestStr ts3f{ testR3f, "TestExecutive" };
	TestExecutive::TestStr ts4{ testR4, "CheckIn Status and Immutability" };
	TestExecutive::TestStr ts5{ testR5, "Browsing on Query" };
	TestExecutive::TestStr ts6{ testR6, "Submission Repo" };
	TestExecutive::TestStr ts7{ testR7, "Implemented Tests" };
	
	ex.registerTest(ts1); // registering test structures with TestExecutive instance, ex
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts3c);
	ex.registerTest(ts3d);
	ex.registerTest(ts3e);
	ex.registerTest(ts3f);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts7);

	bool result = ex.doTests(); // run tests
	if (result == true) { std::cout << "\n  all tests passed\n\n"; system("pause"); }
	else { std::cout << "\n  at least one test failed\n\n"; system("pause"); }
	return 0;
}



