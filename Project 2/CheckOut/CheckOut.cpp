/////////////////////////////////////////////////////////////////////////////////
// CheckOut.h - provides functionality to retrieve a package		           //
// ver 1.0																	   //
// Devin Upreti, Project 2 CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////////////

#include "CheckOut.h"

//////////////////////////////////////////////////////////////////////////////////////
// CheckOut Class methods 

//----< finds keys in the Repo for given filename >-----------------------------------------------

std::vector<std::string> CheckOut::searchdb(const std::string& file_name, DbCore<PayLoad>& dbc)
{
	Conditions<PayLoad> cd;
	cd.name(file_name);
	Query<PayLoad> q(dbc);
	q.select(cd);
	std::vector<std::string> found_keys;
	for (auto x : q.keys())
	{
		//for each element in the db returned by query
		if (dbc[x].name() == file_name)
		{
			found_keys.push_back(x); // add key to answer
		}
	}
	return found_keys;
}

//----< ChecksOut a file's latest version with given file_name>-----------------------------------------------

void CheckOut::retrieve(const std::string& file_name, DbCore<PayLoad>& dbc)
{
	std::vector<std::string> keys = searchdb(file_name, dbc);
		int latest_version = 1;
		for (auto x : keys)
		{	// find latest_version
			if (latest_version < dbc[x].payLoad().version()) { latest_version = dbc[x].payLoad().version(); }
		}
		std::string key;
		for (auto x : keys)
		{	// save file's latest_version
			if (dbc.contains(x))
			{
				if (latest_version == dbc[x].payLoad().version())
				{
					save_file(x, dbc);
					key = x;
					break;
				}
			}
		}
		dependency_file(key, dbc);
		if (dbc[key].children().size() == 0) { return; }
		for (auto child : dbc[key].children())
		{
			//each child is a key
			if (dbc.contains(child))
			{
				int ver = dbc[child].payLoad().version();
				retrieve(dbc[child].name(), ver, dbc);
			}
		}
	return;
}

//----< ChecksOut a file's specifed version with given file_name>-----------------------------------------------

void CheckOut::retrieve(const std::string& file_name, int ver,  DbCore<PayLoad>& dbc)
{
	std::vector<std::string> keys = searchdb(file_name, dbc);
	std::string key;
	for (auto x : keys)
	{	// save file's given version
		if (dbc.contains(x))
		{
			if (ver == dbc[x].payLoad().version())
			{
				save_file(x, dbc);
				key = x;
				break;
			}
		}
	}
	dependency_file(key, dbc);
	if (dbc[key].children().size() == 0) { return; }
	for (auto child : dbc[key].children())
	{
		//each child is a key
		if (dbc.contains(child))
		{
			int ver = dbc[child].payLoad().version();
			retrieve(dbc[child].name(), ver, dbc);
		}
	}	
	return;
}

//----< saves a dependency Information file >------------------------------------------------------

void CheckOut::dependency_file(const std::string& key, DbCore<PayLoad>& dbc)
{
	DbElement<PayLoad> dbe = dbc[key];

	std::string tosavepath = "..\\Folders\\CheckOutFiles\\";

	std::string rawname = dbe.name() + ".dependInfo.txt"; // to remove extension
	std::string newpath = tosavepath + rawname ; // to remove extension
	tosavepath = newpath;

	std::ofstream  dst(tosavepath, std::ios::binary); // destination file

	dst << depend_info(key, dbc);
	dst.close();
}

//----< provides dependency information associated with a key >------------------------------------------------------

std::string CheckOut::depend_info(const std::string& key, DbCore<PayLoad>& dbc)
{
	if (dbc.contains(key))
	{
		if (dbc[key].children().size() == 0) { return "\nThis file has no dependencies."; }
		std::string  s = "\n This file has the following dependencies : \n";
		for (auto child : dbc[key].children())
		{
			if(dbc.contains(child))
			{ 
			s += "\n ";
			s += dbc[child].name() + "." + std::to_string(dbc[child].payLoad().version());
			}
		}
		return s;
	}
	else
	{
		return "";
	}
}

//----< saves a file in a specified location >------------------------------------------------------

void CheckOut::save_file(std::string& key, DbCore<PayLoad>& dbc)
{
	DbElement<PayLoad> dbe = dbc[key];
	
	std::string tosavepath = "..\\Folders\\CheckOutFiles\\";

	size_t lastindex = dbe.name().find_last_of(".");
	std::string rawname = dbe.name().substr(0, lastindex); // to remove extension
	std::string newpath = tosavepath + rawname + "\\"; // to remove extension

	CreateDirectory(newpath.c_str(), NULL);
	tosavepath = newpath;

	std::string source_path = (dbe.payLoad().path() + dbe.name()) + ".";
	source_path += std::to_string(dbe.payLoad().version());
	std::ifstream  src(source_path, std::ios::binary);

	tosavepath = tosavepath + dbe.name() + ".";
	std::ofstream  dst(tosavepath, std::ios::binary); // destination file

	dst << src.rdbuf();
	src.close();
	dst.close();
}

//--------< test stub >-----------------------------------------------------------------

#ifdef TEST_CHECKOUT

#include <iostream>
#include "../TestExecutive/TestExecutive.h"
#include "../TestExecutive/TestExecutive.cpp"

int main()
{
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
	return 1;
}

#endif // TEST_CHECKIN

