#pragma once
#pragma once
/////////////////////////////////////////////////////////////////////
// Test.h - Implements Test Functions for Project 2(OOD) Spring 18 //
// ver 1.2                                                         //
// Author - Devin Upreti                                           //
// Source(testR1, testR2) - Prof. Jim Fawcett                      //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* Test Package defines all test functions for the demonstration of the project.
*
* The package also provides a function for printing newline.
*
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* Query.h, Query.cpp
* RepositoryCore.h, RepositoryCore.cpp
* CheckIn.h, CheckIn.cpp
* CheckOut.h, CheckOut.cpp
* Browse.h, Browse.cpp
* Version.h, Version.cpp
*
* Maintenance History:
* --------------------
* ver 1.2 : 4 March 2018
* - added test functions for requirements 6 and 7
* ver 1.1 : 3 March 2018
* - added test functions for requirements 4 and 5
* ver 1.0 : 2 March 2018
* - added test functions for requirements 1 to 3
*/

#include "../Query/Query.h"
#include "../DbCore/DbCore.h"

#include "../RepositoryCore/RepositoryCore.h"
#include "../CheckIn/CheckIn.h"
#include "../CheckOut/CheckOut.h"
#include "../Browse/Browse.h"
#include "../Version/Version.h"

#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

#include <iostream>
#include <iomanip>
#include <functional>
#include <fstream>

using namespace NoSqlDb;


//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

//----< makes a sample repository and checksin packages >----------------------

RepositoryCore makeTestRepo()
{
	RepositoryCore R;
	CheckIn ci;

	Children depends;
	depends.push_back("nsDemo::pkgDemo.cpp.1");

	ci.file_name("pkgDemo.h").description("TEST FILE").dependson(depends);
	ci.sourcepath("..\\Folders\\Client\\pkgDemo.h").author("Devin").name_space("nsDemo");

	ci.closeCheckIn(R.Repo()); //closing CheckIn before adding to repo

	ci.CheckFile(R.Repo());

	depends.clear();
	depends.push_back("nsDevin::pkgDevin.cpp.1");

	ci.file_name("pkgDevin.h").description("TEST FILE 2").dependson(depends).name_space("nsDevin");
	ci.sourcepath("..\\Folders\\Client\\pkgDevin.h").author("Devin");

	ci.closeCheckIn(R.Repo());
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());

	depends.clear();
	ci.file_name("pkgDevin.cpp").description("TEST FILE 3").dependson(depends).name_space("nsDevin");
	ci.sourcepath("..\\Folders\\Client\\pkgDevin.cpp").author("Devin");

	ci.CheckFile(R.Repo());
	ci.closeCheckIn(R.Repo());
	ci.CheckFile(R.Repo());

	return R;
}

//----< makes a repository that satisfies requirement 6 >----------------------

RepositoryCore makeTestRepo2()
{
	RepositoryCore R;
	CheckIn ci;

	Children depends;
	depends.push_back("nsDemo::pkgDemo.cpp.1");

	ci.file_name("pkgDemo.h").description("TEST FILE").dependson(depends);
	ci.sourcepath("..\\Folders\\Client\\pkgDemo.h").author("Devin").name_space("nsDemo");

	ci.closeCheckIn(R.Repo()); //closing CheckIn before adding to repo

	ci.CheckFile(R.Repo()); //pkgDemo.h.1

	depends.clear();
	depends.push_back("nsDevin::pkgDevin.cpp.1");

	ci.file_name("pkgDevin.h").description("TEST FILE 2").dependson(depends).name_space("nsDevin");
	ci.sourcepath("..\\Folders\\Client\\pkgDevin.h").author("Devin");

	ci.closeCheckIn(R.Repo());
	ci.CheckFile(R.Repo()); //pkgDevin.h.1
	ci.CheckFile(R.Repo()); //pkgDevin.h.2
	
	depends.clear();
	depends.push_back("nsDevin::pkgDevin.cpp.2");
	CheckIn cih = ci;
	cih.dependson(depends);

	depends.clear();
	ci.file_name("pkgDevin.cpp").description("TEST FILE 3").dependson(depends).name_space("nsDevin");
	ci.sourcepath("..\\Folders\\Client\\pkgDevin.cpp").author("Devin");

	ci.CheckFile(R.Repo());
	ci.closeCheckIn(R.Repo());
	ci.CheckFile(R.Repo());

	cih.closeCheckIn(R.Repo());
	cih.CheckFile(R.Repo()); //pkgDevin.h.3
	cih.closeCheckIn(R.Repo());
	cih.CheckFile(R.Repo()); //pkgDevin.h.4

	return R;
}


///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 - CheckIn >----------------------------

bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - Demonstrating CheckIn");
	std::cout << "\n Creating a Core Repository : \n";
	RepositoryCore R;
	CheckIn ci; // creating a CheckIn Object
	
	std::cout << "\n creating metadata..";
	ci.file_name("pkgDevin.h").description("TEST FILE 1").name_space("nsDevin").sourcepath("..\\Folders\\Client\\pkgDevin.h").author("Devin");

	std::cout << "\n Providing dependency and category info ...";
	Children depends;
	std::vector<std::string> category;
	category.push_back("Demonstration");
	depends.push_back("nsDevin::pkgDevin.cpp.1");
	ci.dependson(depends).categories(category); // adding dependencies and category

	ci.closeCheckIn(R.Repo()); // checkIn for file pkgDevin.h
	ci.CheckFile(R.Repo());

	std::cout << "\n Checking In file again, to start a new package version...";
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());
	std::cout << "\n\n Checked In file 4 times - pkgDevin.h | Located at - ..\\Folders\\Client\\pkgDevin.h";
	std::cout << "\n File versions for file - pkgDevin.h | Created at - ..\\Folders\\Server\\";

	std::cout << "\n\n Checking In another file - pkgDevin.cpp";
	depends.clear();
	category.clear();
	ci.file_name("pkgDevin.cpp").description("TEST FILE 2").name_space("nsDevin");
	ci.sourcepath("..\\Folders\\Client\\pkgDevin.cpp").author("Devin");
	ci.dependson(depends).categories(category); // adding dependencies and category

	ci.CheckFile(R.Repo()); // pkgDevin.cpp.1

	ci.closeCheckIn(R.Repo());
	ci.CheckFile(R.Repo()); // checkIn closed for file pkgDevin.cpp version 2
	std::cout << "\n Checked In file 2 times - pkgDevin.cpp | Located at - ..\\Folders\\Client\\pkgDevin.cpp ";
	std::cout << "\n File versions for file - pkgDevin.cpp | Created at - ..\\Folders\\Server\\";

	if(R.Repo().size() != 6) { return false; }
	std::cout << "\n\n Showing Core Repository :\n";
	showDb(R.Repo());

	std::cout << "\n The following files are created at location ..\\Folders\\Server\\ : \n";
	std::cout << "\n -pkgDevin.h.1 \n -pkgDevin.h.2 \n -pkgDevin.h.3 \n -pkgDevin.h.4 \n -pkgDevin.cpp.1 \n -pkgDevin.cpp.2 \n\n";
	return true;
}

//----< demo second part of requirement #3 - CheckOut >------------------------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - Demonstrating CheckOut");
	std::cout << "\n Creating a Sample Core Repository ... \n";
	RepositoryCore R = makeTestRepo();

	std::cout << "\n Showing Core Repository : ";
	showDb(R.Repo());

	CheckOut CO; //checkOut object
	std::cout << "\n CheckingOut File pkgDevin.h, version 2";
	
	CO.retrieve("pkgDevin.h", 2, R.Repo()); // checksout specified filename and version (if version is not provided then latest version)
	
	std::cout << "\n Removed Version Info from file name";
	std::cout << "\n\n pkgDevin.h.2 depends on pkgDevin.cpp.1 | pkgDevin.h.2 -> pkgDevin.cpp.1 ";
	std::cout << "\n Therefore, the following files are created at location ..\\Folders\\CheckOutFiles\\ : \n";
	std::cout << "\n -pkgDevin.h \n -pkgDevin.cpp \n -pkgDevin.cpp.dependInfo.txt \n -pkgDevin.h.dependInfo.txt \n";
	std::cout << "\n DependInfo files contain the dependency Information for each file \n";

	putLine();
	return true;
}

//----< demo third part of requirement #3 - Browse >------------------------------------------

bool testR3c()
{
	Utilities::title("Demonstrating Requirement #3c - Demonstrating Browse");
	std::cout << "\n Creating a Sample Core Repository ... \n";
	RepositoryCore R = makeTestRepo();

	std::cout << "\n Showing Core Repository : ";
	showDb(R.Repo());

	Browse B;
	
	std::cout << "\n finding location of file - pkgDevin.h.2";
	std::cout << "\n Location - " << B.locate_file("pkgDevin.h", 2 ,R.Repo());
	putLine();

	std::cout << "\n finding location of file - pkgDemo.h | No version provided, will find latest version";
	std::cout << "\n Location - " << B.locate_file("pkgDemo.h", R.Repo());
	putLine();

	std::cout << "\n viewing contents of file - pkgDevin.h.2";
    B.browse_file("pkgDevin.h", 2, R.Repo()); // if the file is not checkedIn then it will do nothing
	putLine();

	std::cout << "\n viewing contents of file - pkgDemo.h | No version provided, will find latest version";
	B.browse_file("pkgDemo.h", R.Repo());
	putLine();

	if (B.locate_file("pkgDevin.h", 2, R.Repo()) == "") { return false; }

	putLine();
	putLine();
	return true;
}

//----< demo third part of requirement #3 - Version >------------------------------------------

bool testR3d()
{
	Utilities::title("Demonstrating Requirement #3d - Demonstrating Version");
	std::cout << "\n Creating a Sample Core Repository ... \n";
	RepositoryCore R = makeTestRepo();

	std::cout << "\n Showing Core Repository : ";
	showDb(R.Repo());

	Version V;
	std::string key = "nsDevin::pkgDevin.h.4";
	if (R.Repo().contains(key))
	{
		std::cout << "\n Increasing Version of Key : "<< key <<" to 5";
		V.increaseVersion(R.Repo()["nsDevin::pkgDevin.h.4"].payLoad());

		std::cout << "\n Showing Database after Increasing Version of Key : " << key << " to 5\n";
		showDb(R.Repo());

		if (R.Repo()[key].payLoad().version() == 4) { return false; }
	}
	
	std::cout << "\n Allowing the client to make changes to the version numbers explicitly can cause ambiguity";
	std::cout << "\n To avoid that this design uses Version in CheckIn to maintain version numbers from background";
	std::cout << "\n This ensures maintaining the integrity of data present in Core Repository";

	std::cout << "\n\n Here Version is used in foreground only for demonstration purpose.";

	putLine();
	putLine();
	return true;
}

//----< demo fifth part of requirement #3 - Repository Core >------------------------------------------

bool testR3e()
{
	Utilities::title("Demonstrating Requirement #3e - Demonstrating Repository Core");
	std::cout << "\n Creating a Sample Core Repository ... \n";
	RepositoryCore R = makeTestRepo();

	std::cout << "\n Showing Core Repository : ";
	showDb(R.Repo());

	std::cout << "\n\n testR3a, testR3b, testR3c and testR3d demonstrate the Repository Core, ";
	std::cout << "\n provides means to check-in, version, browse, and check-out source code packages \n";

	putLine();
	return true;
}

//----< demo sixth part of requirement #3 - TestExecutive >------------------------------------------

bool testR3f()
{
	Utilities::title("Demonstrating Requirement #3f - TestExecutive");
	std::cout << "\n\n TestExecutive is implemented as a package to execute a sequence of tests,\n that demonstrate core Repository functionality\n";

	putLine();
	return true;
}

//----< demo requirement #4 - CheckIn Status and Immutability >------------------------------------------

bool testR4()
{
	Utilities::title("Demonstrating Requirement #4 - CheckIn Status and Immutability");
	std::cout << "\n Creating an Empty Repository ... \n";
	RepositoryCore R;
	std::cout << "\n Showing Repository ... \n";
	showDb(R.Repo());
	std::cout << "\n\n";

	std::cout << " Checking in by accepting a single package's files and appending version number to each file name..";
	CheckIn ci; // creating a CheckIn Object
	ci.file_name("p5.h").description("TEST FILE 1").name_space("ns5");
	ci.sourcepath("..\\Folders\\Client\\p5.h").author("Devin");

	Children depends;
	std::vector<std::string> category;
	category.push_back("Demo4");
	depends.push_back("ns5::p5.cpp.1");
	ci.dependson(depends).categories(category); // adding dependencies and category
	std::cout << "\n Closing status for ns5::p5.h.1\n";
	ci.closeCheckIn(R.Repo()); // checkIn for file p5.h
	ci.CheckFile(R.Repo());

	depends.clear();
	ci.file_name("p5.cpp").description("TEST FILE 2").name_space("ns5");
	ci.sourcepath("..\\Folders\\Client\\p5.cpp").author("Devin");
	ci.dependson(depends);
	ci.CheckFile(R.Repo());
	ci.closeCheckIn(R.Repo());
	ci.CheckFile(R.Repo()); // CheckIn status of p5.cpp.2 is closed

	std::cout << "\n Showing Repository after checking in files p5.h and p5.cpp... \n";
	showDb(R.Repo());
	putLine();

	std::cout << "\n The following files are created at location ..\\Folders\\Server\\ : \n";
	std::cout << "\n -p5.h.1 \n -p5.cpp.1 \n -p5.cpp.2 \n\n";
	std::cout << " The status for ns5::p5.h.1 could not be closed because its dependency was either not closed or not present in repository";
	std::cout << "\n So it's status became 'pending' instead.\n";
	std::cout << "\n Since we checkin p5.cpp 2 times, its version becomes 2 (which is one more than last stored version)";
	std::cout << "\n\n CheckIn status of p5.cpp.2 is closed so it is immutable";
	std::cout << "\n Trying to change fields of closed status";
	ci = R.Repo()["ns5::p5.cpp.2"];
	ci.file_name("change").description("change").name_space("change");
	ci.sourcepath("change").author("change");
	R.Repo()["ns5::p5.cpp.2"] = ci.getdbe();
	showDb(R.Repo());
	std::cout << "\n\n As can be seen in the Repository, closed version of a file becomes immutable\n\n\n";
	return true;
}

//----< demo requirement #5 - Browsing on Query >------------------------------------------

bool testR5()
{
	Utilities::title("Demonstrating Requirement #5 - Browsing on Query");
	std::cout << "\n Creating a Sample Core Repository ... \n";
	RepositoryCore R = makeTestRepo();

	std::cout << "\n Showing Core Repository : ";
	showDb(R.Repo());
	putLine();

	Browse B;
	
	std::cout << "\n Browsing based on a query for File names containing 'Devin'";
	Conditions<PayLoad> cd;
	cd.name("Devin");
	Query<PayLoad> q(R.Repo());
	q.select(cd);
	if (q.keys().size() == 0) { return false; }

	std::cout << "\n Identifying the packages to be browsed by a query on the NoSql database...\n";
	B.getQueryKeys(q.keys());
	B.browse_all(R.Repo());
	std::cout << "\n As seen above, it allows packages to be browsed by displaying package description";

	std::cout << "\n\n Now displaying the full package text shall be displayed on demand, identified by one of the keys returned from the initial browse query. ";
	B.browseFilebyKey("nsDevin::pkgDevin.h.4", R.Repo());

	putLine();
	return true;
}

//----< demo requirement #6 - Submission Repo >------------------------------------------

bool testR6()
{
	Utilities::title("Demonstrating Requirement #6 - Submission Repo");
	std::cout << "\n Creating a Sample Core Repository metting submission specs ... \n";
	RepositoryCore R = makeTestRepo2();

	std::cout << "\n Showing Repository ... \n";
	showDb(R.Repo());
	std::cout << "\n\n";

	std::cout << " The Repository contains several packages, most with closed check-ins, and a few packages with open check-ins. ";
	std::cout << "\n some packages have pending checkin because, they could not be closed due to open dependencies\n\n";

	return true;
}

//----< demo requirement #7 - TestExecutive >------------------------------------------

bool testR7()
{
	Utilities::title("Demonstrating Requirement #7 - TestExecutive");
	std::cout << "\n TestExecutive executes a series of tests, from ts1 to ts7, that clearly demonstrate that the project satisfies each of the Requirements\n\n";

	return true;
}