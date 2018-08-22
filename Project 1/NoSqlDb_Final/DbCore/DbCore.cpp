/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 2.3                                                         //
// Author - Devin Upreti                                           //
// Source(ver 1.1) - Prof. Jim Fawcett, CSE687 - OOD, Spring 2018  //
/////////////////////////////////////////////////////////////////////

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

//----< test stub >--------------------------------------------------

#ifdef TEST_DBCORE

#include <iostream>

int main()
{
	Utilities::Title("Testing DbCore class");

	try {
		DbCore<std::string> db; // creates a dbStore_ in db object with type string -> contains DbElement with payload of type string

		DbElement<std::string> demoElem; // creating a demoelement of type DbElement<std::string>

		demoElem.name("Jim");
		demoElem.descrip("Instructor for CSE687");
		demoElem.dateTime(DateTime().now());
		demoElem.payLoad("The good news is ...");

		if (demoElem.name() != "Jim")
			std::cout << "\n  Something is wrong.";
		if (demoElem.descrip() != "Instructor for CSE687")
			std::cout << "\n  Something is wrong.";
		if (demoElem.dateTime().now() != DateTime().now())
			std::cout << "\n  Something is wrong.";
		if (demoElem.payLoad() != "The good news is ...")
			std::cout << "\n  Something is wrong.";

		showHeader();
		showElem(demoElem);

		db["Fawcett"] = demoElem; // add new element to dbCore using operator[] with key as "Fawcett"
		std::cout << "\n";

		std::cout << "\n  DbCore Package is working properly\n";
		system("pause");
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  -- " << ex.what() << " --";
	}
	std::cout << "\n\n";
}
#endif