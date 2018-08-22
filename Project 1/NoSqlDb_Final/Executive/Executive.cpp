#pragma once
/////////////////////////////////////////////////////////////////////////
// Executive.cpp - Executes all the tests for Project 1 (OOD), S18     //
// ver 1.0                                                             //
// Author - Devin Upreti                                               //
/////////////////////////////////////////////////////////////////////////

#include "../Query/Query.h"
#include "../Test/Test.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"

using namespace Utilities;

using namespace XmlProcessing;

int main()
{   Utilities::Title("Testing DbCore - He said, she said database");
	putLine();
	TestExecutive ex; // now define test structures with test function and message
	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4{ testR4, "Addition and Deletion of Key-Value pairs" };
	TestExecutive::TestStr ts5a{ testR5a, "Addition and Deletion of Relationships" };
	TestExecutive::TestStr ts5b{ testR5b, "Editing text metadata" };
	TestExecutive::TestStr ts5c{ testR5c, "Replacement of value's instance" };
	TestExecutive::TestStr ts6a{ testR6a, "Query support for value of specified Key" };
	TestExecutive::TestStr ts6b{ testR6b, "Query support for Children of specified Key" };
	TestExecutive::TestStr ts6c{ testR6c, "Query support for finding all keys matching a regular expression" };
	TestExecutive::TestStr ts6d{ testR6d, "Query support for finding all keys containing specified string in metadata" };
	TestExecutive::TestStr ts6e{ testR6e, "Query support for finding all keys created before a Datetime" };
	TestExecutive::TestStr ts7a{ testR7a, "Support for Queries on the set of keys returned by a previous query(AND)" };
	TestExecutive::TestStr ts7b{ testR7b, "Support for Queries on the union of results of one or more previous queries(OR)" };
	TestExecutive::TestStr ts8a{ testR8a, "Persistance - Saving xml file for database" };
	TestExecutive::TestStr ts8b{ testR8b, "Retrieval of Database from Saved XML File" };
	TestExecutive::TestStr ts9{ testR9, "Implementation of Payload type as a string" };
	TestExecutive::TestStr ts10{ testR10, "Implementation of Packages" };
	TestExecutive::TestStr ts11{ testR11, "XML file for package structure" };
	ex.registerTest(ts1); // registering test structures with TestExecutive instance, ex
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4);
	ex.registerTest(ts5a);
	ex.registerTest(ts5b);
	ex.registerTest(ts5c);
	ex.registerTest(ts6a);
	ex.registerTest(ts6b);
	ex.registerTest(ts6c);
	ex.registerTest(ts6d);
	ex.registerTest(ts6e);
	ex.registerTest(ts7a);
	ex.registerTest(ts7b);
	ex.registerTest(ts8a);
	ex.registerTest(ts8b);
	ex.registerTest(ts9);
	ex.registerTest(ts10);
	ex.registerTest(ts11);
	bool result = ex.doTests(); // run tests
	if (result == true) { std::cout << "\n  all tests passed\n\n"; system("pause"); }
	else { std::cout << "\n  at least one test failed\n\n"; system("pause"); }
	return 0;
}

