/////////////////////////////////////////////////////////////////////////////////
// CheckIn.cpp - provides functionality to start a new package version         //
// ver 1.0																	   //
// Devin Upreti, Project 2 CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////////////
#include "CheckIn.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CheckIn class functions


//----< closes status if all dependencies are closed >---------

void CheckIn::closeCheckIn(DbCore<PayLoad>& dbc)
{
	if (canClose(dbc))
	{
		dbe.payLoad().closeStatus();
	}
}

//----< checks if status of file can be closed >---------

bool CheckIn::canClose(DbCore<PayLoad>& dbc)
{
	if (dbe.children().size() == 0) { return true; }
	for (auto child : dbe.children())
	{
		if (dbc.contains(child))
		{
			if (dbc[child].payLoad().statusOpen())
			{
				dbe.payLoad().pendingStatus();
				return false; // atleast one dependency is open, so we cannot close it
			}
		}
		else { dbe.payLoad().pendingStatus(); return false; }
	}
	return true;
}


//----< checks if a File is located at a specified path >---------

bool CheckIn::is_file_exist(const std::string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

//----< saves a File at a location >------------------------------

void CheckIn::saveFile()
{		
		storagepath("..\\Folders\\Server\\");
	    
		size_t lastindex = dbe.name().find_last_of(".");
		std::string rawname = dbe.name().substr(0, lastindex); // to remove extension
		std::string newpath = dbe.payLoad().path() + rawname + "\\"; // to remove extension
		
		CreateDirectory(newpath.c_str(), NULL);
		storagepath(newpath);
		
		std::ifstream  src(source_path, std::ios::binary);

		std::string location;
		location = (dbe.payLoad().path() + dbe.name()) + ".";
		location += std::to_string(dbe.payLoad().version());

		std::ofstream  dst(location, std::ios::binary); // destination file

		dst << src.rdbuf();
		src.close();
		dst.close();
}

//----< resets Status to open for a new version of file >---------

void CheckIn::resetStatus()
{
	if (!dbe.payLoad().statusOpen())
	{
		dbe.payLoad().openStatus();
	}
}

//----< ChecksIn a file into the Repository and saves its latest version >---------

void CheckIn::CheckFile(DbCore<PayLoad>& dbc)
{
	Conditions<PayLoad> cd;
	cd.name(this->getname());
	Query<PayLoad> q(dbc);
	q.select(cd);
	
	if (q.keys().size() == 0)
	{   
		//file not in db
		this->setVersion(1);
		if (this->is_file_exist(source_path) )
		{
			// file found in path
			this->saveFile();
			dbc[dbe.payLoad().name_space() + "::" + this->getname() + "." + std::to_string(dbe.payLoad().version())] = this->getdbe();
		}
		else { std::cout << "\nCannot CHECK file - " << this->getname() << " didn't found in path " << dbe.payLoad().path() << "\n"; }
	}
	else
	{	//file already in db
		int version = 1;
		for (auto x : q.keys())
		{
			if (version < dbc[x].payLoad().version()) 
			{ version = dbc[x].payLoad().version(); } // find latest version
		}
		version = version + 1; // saved version is greater than latest version
		this->setVersion(version);
		if (this->is_file_exist(source_path ) )
		{   // file found in path
			this->saveFile();
			dbc[dbe.payLoad().name_space() + "::" + this->getname() + "." + std::to_string(dbe.payLoad().version())] = this->getdbe();
		}
		else { std::cout << "\n Cannot CHECK file - " << this->getname() << " didn't found in path " << dbe.payLoad().path() << "\n"; }
	}
	this->resetStatus();
}


#ifdef TEST_CHECKIN

#include <iostream>
#include "../TestExecutive/TestExecutive.h"
#include "../TestExecutive/TestExecutive.cpp"

int main()
{
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
	return 1;
}

#endif // TEST_CHECKIN
