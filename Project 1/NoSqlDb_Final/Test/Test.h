#pragma once
/////////////////////////////////////////////////////////////////////
// Test.h - Implements Test Functions for Project 1(OOD) Spring 18 //
// ver 1.3                                                         //
// Author - Devin Upreti                                           //
// Source(testR1, testR2, testR3) - Prof. Jim Fawcett              //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides DbProvider class:
* - Is used to maintain state of DbCore between different test functions
* - It contains a static dbCore element, which is shared between all instances of dbProvider
*
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
* XmlDocument.h, XmlDocument.cpp
* XmlElement.h, XmlElement.cpp
*
* Maintenance History:
* --------------------
* ver 1.3 : 4 Feb 2018
* - added test functions for requirement 9
* ver 1.2 : 1 Feb 2018
* - added test functions for requirement 8
* ver 1.1 : 31 Jan 2018
* - added test functions for requirements 6 and 7
* ver 1.0 : 28 Jan 2018
* - added test functions for requirements 4 and 5
*/

#include "../Query/Query.h"
#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <fstream>

using namespace NoSqlDb;

using namespace XmlProcessing;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<std::string>& db() { return db_; }
private:
	static DbCore<std::string> db_; // this element will have 1 copy shared between all objects
};

DbCore<std::string> DbProvider::db_;

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

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbCore<std::string> db; // creates a dbStore_ in db object with type string -> contains DbElement with payload of type string
	DbProvider dbp; // static dbStore object db_ in dbp
	dbp.db() = db; // static element &db_ = db, which is DbCore<String> (Key,DbElement)

				   // create some demo elements and insert into db

	DbElement<std::string> demoElem; // creating a demoelement of type DbElement<std::string>

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("The good news is ...");

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad() != "The good news is ...")
		return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem; // add new element to dbCore using operator[] with key as "Fawcett"
	dbp.db() = db; 
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");
	DbProvider dbp;
	DbCore<std::string> db = dbp.db(); 
	DbElement<std::string> demoElem = db["Fawcett"];
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;
	demoElem.name("Jianan");
	demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;
	demoElem.payLoad("You didn't demonstrate Requirement #4");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;
	demoElem.payLoad("You didn't demonstrate Requirement #5");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;
	demoElem.payLoad("You didn't demonstrate Requirement #6");
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;
	if (db.size() != 6)
		return false;
	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();
	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");
	showHeader();
	showElem(db["Fawcett"]);
	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db); // display the results
	putLine();
	putLine();
	dbp.db() = db; // to maintain state of db within differenet test functions
	return true;
}

//----< demonstration of requirement #4 - Addition and Deletion of key/value pairs >---------------------------

bool testR4()
{
	Utilities::title("Demonstrating Requirement #4 - addition/deletion of key/value pairs");

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	DbElement<std::string> demoElem;

	demoElem.name("Devin");
	demoElem.descrip("Student in CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("Added Element in Database");

	db["Upreti"] = demoElem; // addition of key/value pair
	if (!db.contains("Upreti"))
		return false;

	std::cout << "\n  Added element with key 'Upreti' in db";

	//display
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  database keys are: ";
	showKeys(db);

	putLine();

	std::cout << "\n  Deleting element with key 'Salman' in db";
	db.deleteKey("Salman");
	if (db.contains("Salman"))
		return false;
	std::cout << "\n  Deleted element with key 'Salman' in db";

	//display
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  database keys are: ";
	showKeys(db);

	dbp.db() = db;
	putLine();
	return true;
}

//----< demo of requirement #5a - Addition and Deletion of Relationships >---------------------------

bool testR5a()
{
	Utilities::title("Demonstrating Requirement #5a - addition and deletion of relationships");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	showHeader();
	showElem(db["Fawcett"]);
	putLine();

	std::cout << "\n  Adding child 'newChild' to key 'Fawcett'";
	db["Fawcett"].addchild("newChild");

	std::vector<std::string>::iterator it;
	it = find(db["Fawcett"].children().begin(), db["Fawcett"].children().end(), "newChild");
	if (it == db["Fawcett"].children().end())
		return false;

	std::cout << "\n  Added child 'newChild' to key 'Fawcett'";
	putLine();

	std::cout << "\n  Showing the element with with key 'Fawcett' :";
	showHeader();
	showElem(db["Fawcett"]);
	putLine();

	std::cout << "\n  Deleting child 'Salman' from key 'Fawcett'";
	db["Fawcett"].deletechild("Salman");

	it = find(db["Fawcett"].children().begin(), db["Fawcett"].children().end(), "Salman");
	if (it != db["Fawcett"].children().end())
		return false;
	std::cout << "\n  Deleted child 'Salman' from key 'Fawcett'";
	putLine();

	std::cout << "\n  Showing the element with with key 'Fawcett' :";
	showHeader();
	showElem(db["Fawcett"]);
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demo of requirement #5b - Editing text metadata >---------------------------

bool testR5b()
{
	Utilities::title("Demonstrating Requirement #5b - changing metadata");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	DbElement<std::string> demoElem = db["Upreti"];

	std::cout << "\n  Showing the element with with key 'Upreti' :";
	showHeader();
	showElem(demoElem);
	putLine();

	std::cout << "\n  Editing metadata from key 'Upreti'";
	demoElem.descrip("Changed metadata");
	db["Upreti"] = demoElem;
	if (db["Upreti"].descrip() != "Changed metadata")
		return false;
	std::cout << "\n  Edited metadata from key 'Upreti'";
	putLine();

	std::cout << "\n  Showing the element with with key 'Upreti' :";
	showHeader();
	showElem(demoElem);
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #5c - Replacement of value's instance >---------------------------

bool testR5c()
{
	Utilities::title("Demonstrating Requirement #5c - Replacement of value's instance");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	DbElement<std::string> demoElem = db["Upreti"];
	std::cout << "\n  Showing element with Key 'Upreti' :";
	showHeader();
	showElem(db["Upreti"]);
	putLine();

	std::cout << "\n  Replacing value instance from key 'Upreti'";
	DbElement<std::string> replaceElem;
	replaceElem.name("NewName");
	replaceElem.dateTime(DateTime().now());
	replaceElem.descrip("new Description");
	replaceElem.payLoad("I am the replacement");
	db["Upreti"] = replaceElem;

	if (db["Upreti"].name() == "Devin")
		return false;
	std::cout << "\n  Replaced value instance from key 'Upreti'";
	putLine();

	std::cout << "\n  Showing element with Key 'Upreti' :";
	showHeader();
	showElem(db["Upreti"]);
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #6a - Query for value of a given key >---------------------------

bool testR6a()
{
	Utilities::title("Demonstrating Requirement #6a - Query for value of specified Key");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	std::cout << "\n  Using query to get value from key 'Upreti'";
	Query<std::string> q1(db);
	DbElement<std::string> demoElem = q1.getValue("Upreti"); // check what happens when you pass key not in db
	if (demoElem.name() != db["Upreti"].name())
		return false;
	std::cout << "\n  Value retrieved using Query from key 'Upreti'";

	std::cout << "\n\n  Showing value retrieved using Query from key 'Upreti' : ";
	showHeader();
	showElem(demoElem);
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #6b - Query for children of a given key >---------------------------

bool testR6b()
{
	Utilities::title("Demonstrating Requirement #6b - Query for children of specified Key");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	std::cout << "\n  Using query to get children from key 'Fawcett'";
	Query<std::string> q1(db);
	std::vector<std::string> demoChildren = q1.getChildren("Fawcett"); // check what happens when you pass key not in db
	if (demoChildren != db["Fawcett"].children())
		return false;
	std::cout << "\n  Value retrieved using Query from key 'Fawcett'";

	std::cout << "\n\n  Showing children retrieved using Query from key 'Fawcett' : ";
	if (demoChildren.size() > 0)
	{
		std::cout << "\n    child keys: ";
		for (auto key : demoChildren)
		{
			std::cout << " " << key;
		}
	}
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #6c - Query for all keys matching a regular expression >---------------------------

bool testR6c()
{
	Utilities::title("Demonstrating Requirement #6c - Query for all keys matching a regular expression");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	Query<std::string> q1(db);
	std::cout << "\n  Showing Current Keys in the Database: ";
	q1.show();

	Conditions<std::string> c;
	std::string r = "(A)(.*)";
	std::cout << "\n\n  Finding all Keys in the Database that start with Letter 'A' through Query: ";
	q1.selectKeys(r).show();
	putLine();
	std::vector<std::string> demoKeys = q1.keys();

	//check
	if (std::find(demoKeys.begin(), demoKeys.end(), "Arora") == demoKeys.end())
	{
		return false;
	} // if key is Arora is not in specified Query, then return false
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #6d - Query for all keys containing a string in metadata >---------------------------

bool testR6d()
{
	Utilities::title("Demonstrating Requirement #6d - Query for all keys containing a string in metadata");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	Query<std::string> q1(db);

	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	Conditions<std::string> c;
	std::string r = "(.*)(CSE687)(.*)";
	c.descrip(r);
	std::cout << "\n  showing keys of db elements containing 'CSE687' in description : ";
	q1.select(c).show();
	putLine();
	std::vector<std::string> demoKeys = q1.keys();

	//check
	if (std::find(demoKeys.begin(), demoKeys.end(), "Fawcett") == demoKeys.end())
	{
		return false;
	}

	std::string r2 = "(.*)(TA)(.*)";
	c.descrip(r2);
	std::cout << "\n  showing keys of db elements containing 'TA' in description : ";
	Query<std::string> q2(db);
	q2.select(c).show();

	demoKeys = q2.keys();
	//check
	if (std::find(demoKeys.begin(), demoKeys.end(), "Arora") == demoKeys.end())
	{
		return false;
	}

	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #6e - Query for all keys created with in a specified time interval >---------------------------

bool testR6e()
{
	Utilities::title("Demonstrating Requirement #6e - Query for all keys created before datetime");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	Query<std::string> q1(db);
	Conditions<std::string> c;

	DateTime dt_begin = DateTime("Sat Feb 3 13:12:39 2010");
	std::cout << "\n  Showing all keys of elements created after given DateTime (1 DateTime parameter query)";
	std::cout << "\n  BEGIN - " << std::string(dt_begin) << "| END - not specified [Will take current DateTime]";
	q1.selectDatetime(dt_begin).show();
	putLine();

	DateTime dt_end = DateTime().now();
	std::cout << "\n  Elements created within specified DateTime Interval (2 DateTime parameter query)";
	std::cout << "\n  BEGIN - " << std::string(dt_begin) << "; END - " << std::string(dt_end);
	q1.selectDatetime(dt_begin, dt_end).show();
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #7a - Queries on the set of keys returned by a previous query(AND)>---------------------------

bool testR7a()
{
	Utilities::title("Demonstrating Requirement #7a - Queries on the set of keys returned by a previous query(AND)");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	Query<std::string> q1(db);
	Conditions<std::string> c1;

	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::string name_r = "(J)(.*)";
	c1.name(name_r);
	std::cout << "\n  First Query for Keys whose names start with Letter 'J'";
	q1.select(c1).show(); // Query1 finds keys whose elements have names starting with letter 'J'
	putLine();

	Query<std::string> q2(db);
	Conditions<std::string> c2;
	std::string des_r = "(.*)(TA)(.*)";
	c2.descrip(des_r); // Condition2 keys whose elements have 'TA' in their description
	std::cout << "\n  Second Query (on Keys from First Query) for description contains 'TA'";
	q2.from(q1.keys()).select(c2).show(); // query2 on keys returned by query1
	putLine();

	std::vector<std::string> demoKeys = q2.keys();

	//check
	if (std::find(demoKeys.begin(), demoKeys.end(), "Sun") == demoKeys.end())
	{
		return false;
	}

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #7b - Queries on the union of results of one or more previous queries(OR)>-------------

bool testR7b()
{
	Utilities::title("Demonstrating Requirement #7b - Queries on the union of results of one or more previous queries(OR)");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	Query<std::string> q1(db);
	Conditions<std::string> c1;

	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();

	std::string name_r = "(.*)(l)(.*)";
	c1.name(name_r);
	std::cout << "\n  First Query for Keys whose names have the Letter 'l'";
	q1.select(c1).show(); // Query1 finds keys whose elements have letter 'a' in their names
	putLine();

	Query<std::string> q2(db);
	Conditions<std::string> c2;
	name_r = "(.*)(a)(.*)";
	c2.name(name_r); // Condition2 keys whose elements have 'TA' in their description
	std::cout << "\n  Second Query for keys whose name contains letter 'a'";
	q2.select(c2).show(); // query2 on keys returned by query1
	putLine();

	Conditions<std::string> c3;
	name_r = "(.*)(i)(.*)";
	c3.name(name_r);
	std::cout << "\n  Third Query[OR], on Union of the keys from previous two queries, whose name contains letter 'i'";
	q2.OR(q1.keys()).select(c3).show();
	putLine();

	std::vector<std::string> demoKeys = q2.keys();

	//check
	if (std::find(demoKeys.begin(), demoKeys.end(), "Sun") == demoKeys.end())
	{
		return false;
	}

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #8a - Creates a file from Database >-------------

bool testR8a()
{
	Utilities::title("Demonstrating Requirement #8a - Persistance - Saving the database to an XML file");
	putLine();

	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	std::cout << "\n  Demonstrating Persistance with XmlDocument";
	std::cout << "\n  Showing Database";
	std::cout << "\n ------------------------";

	showDb(db);
	std::cout << "\n";
	std::cout << "\n  Creating XML representation of Mock Database using XmlDocument";
	std::cout << "\n ----------------------------------------------------------------";
	
	std::string file_name = "new_test.xml";
	db.dbtoXMLfile(file_name);
	
	std::ifstream inFile;
	inFile.open(file_name);
	if (!inFile) { return false;}
	inFile.close();
	std::cout << "\n  Save Successful | FILE CREATED : "<<file_name;
	putLine();

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #8b - Retrieval of Database from Saved XML File >---------------

bool testR8b()
{
	Utilities::title("Demonstrating Requirement #8b - Retrieval of Database from Saved XML File");
	putLine();
	using Sptr = std::shared_ptr<AbstractXmlElement>;

	std::string file_name = "new_test.xml";
	std::ifstream inFile;
	inFile.open(file_name);
	if (!inFile) { return false; }

	
	std::cout << "\n  Creating XmlDocument from XML string";
	std::cout << "\n --------------------------------------";

	std::string Xml = "";
	std::string temp;
	while (std::getline(inFile, temp)) {
		temp += "\n";
		Xml += temp;
	}

	DbCore<std::string> newDb;
	XmlDocument newXDoc(Xml, XmlDocument::str); 
	inFile.close();

	std::cout << newXDoc.toString();
	std::cout << "\n";
	
	newDb.xmltodb(newXDoc);
	
	showDb(newDb);
	putLine();
	std::cout<<"\n  Keys in the database : ";
	showKeys(newDb);
	putLine();
	std::cout << "\n  New Database successfully generated from saved XML file : "<<file_name;
	std::cout << "\n  If we augument the XML file, then the database loaded from that file will reflect those changes.\n";

	if (!newDb.contains("Fawcett")) { return false; }

	putLine();
	return true;
}

//----< demonstration of requirement #9 - Implementation of Payload type as a string >---------------

bool testR9()
{
	Utilities::title("Demonstrating Requirement #9 - Implementation of Payload type as a string");
	putLine();
	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	DbElement<std::string> dbe = db["Upreti"];

	std::cout << "\n  Showing element with key 'Upreti'";
	showHeader();
	showElemwithPayload(db["Upreti"]);
	putLine();

	std::cout << "\n  Changing Payload for element with key 'Upreti'";
	db["Upreti"].payLoad_obj().payload_element("PAYLOAD CHANGED");
	putLine();
	if (db["Upreti"].payLoad() == "I am the replacement")
		return false;
	
	std::cout << "\n  Adding strings to Payload vector for element with key 'Upreti'";
	db["Upreti"].payLoad_obj().addtovector("Apple");
	db["Upreti"].payLoad_obj().addtovector("Bob");
	db["Upreti"].payLoad_obj().addtovector("Cat");

	std::cout << "\n  Showing element with key 'Upreti'";
	showHeader();
	showElemwithPayload(db["Upreti"]);
	putLine();

	std::vector<std::string>::iterator it;
	it = find(db["Upreti"].payLoadV().begin(), db["Upreti"].payLoadV().end(), "Apple");
	if (it == db["Upreti"].payLoadV().end())
		return false;

	dbp.db() = db;
	putLine();
	return true;
}

//----< demonstration of requirement #10 - Implementation of Packages >---------------

bool testR10()
{
	Utilities::title("Demonstrating Requirement #10 - Implementation of Packages");
	putLine();
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show \n  Executive, DBCore, Query, Test as implemented packages.\n";

	putLine();
	return true;
}

//----< demonstration of requirement #11 - XML file for package structure >---------------

bool testR11()
{
	Utilities::title("Demonstrating Requirement #11 - XML file for package structure");
	putLine();

	using Sptr = std::shared_ptr<AbstractXmlElement>;

	std::string file_name = "xml_diagram.xml";
	std::ifstream inFile;
	inFile.open(file_name);
	if (!inFile) { return false; }

	std::cout << "\n  Creating XmlDocument from XML string";
	std::cout << "\n --------------------------------------";

	std::string Xml = "";
	std::string temp;
	while (std::getline(inFile, temp)) {
		temp += "\n";
		Xml += temp;
	}
	DbCore<std::string> newDb;
	XmlDocument newXDoc(Xml, XmlDocument::str);
	inFile.close();

	std::cout << newXDoc.toString();
	std::cout << "\n";
	newDb.xmltodb(newXDoc);
	std::cout << "  Db loaded from above XML file, describing project structure:\n";
	showDb(newDb);
	putLine();

	putLine();
	return true;
}