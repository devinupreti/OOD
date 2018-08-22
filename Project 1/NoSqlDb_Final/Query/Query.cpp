/////////////////////////////////////////////////////////////////////
// Query.cpp - Implements Query operations for NoSql database      //
// ver 1.2                                                         //
// Author - Devin Upreti                                           //
/////////////////////////////////////////////////////////////////////

#include "Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"


//----< test stub >--------------------------------------------------

#ifdef TEST_QUERY

#include <iostream>

using namespace NoSqlDb;

int main()
{
	Utilities::Title("Testing Query class");
	try {
		DbCore<std::string> db; // creates a dbStore_ in db object with type string -> contains DbElement with payload of type string
								
		DbElement<std::string> demoElem; // creating a demoelement of type DbElement<std::string>
		
		demoElem.name("Jim");
		demoElem.descrip("Instructor for CSE687");
		demoElem.dateTime(DateTime().now());
		demoElem.payLoad("The good news is ...");
		
		showHeader();
		showElem(demoElem);

		db["Fawcett"] = demoElem; // add new element to dbCore using operator[] with key as "Fawcett"

		Query<std::string> q1(db);
		Conditions<std::string> c;

		std::string r = "(.*)(CSE687)(.*)";
		c.descrip(r);
		std::cout << "\n  showing keys of db elements containing 'CSE687' in description : ";
		q1.select(c).show();
		std::cout << "\n";
		std::vector<std::string> demoKeys = q1.keys();
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