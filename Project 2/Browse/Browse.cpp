////////////////////////////////////////////////////////////////////////////////////////
// Browse.cpp - provides the capability to locate files and view their contents.      //
// ver 1.0																			  //
// Author - Devin Upreti															  //
////////////////////////////////////////////////////////////////////////////////////////

#include "Browse.h"

/////////////////////////////////////////////////////////////////////////////////////////\
// Browse class functions

//----< find the key of a file with given name's latest version in the Repository >---------------------------------------------------

std::string Browse::get_key(std::string file_name, DbCore<PayLoad>& dbc)
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
	int latest_version = 1;
	for (auto x : found_keys)
	{	// find latest_version
		if (latest_version < dbc[x].payLoad().version()) { latest_version = dbc[x].payLoad().version(); }
	}
	for (auto x : found_keys)
	{	// return file's latest_version
		if (latest_version == dbc[x].payLoad().version())
		{
			return x;
		}
	}
	return "";
}

//----< find the key of a file with given name and version in the Repository >---------------------------------------------------

std::string Browse::get_key(std::string file_name, int ver, DbCore<PayLoad>& dbc)
{
	Conditions<PayLoad> cd;
	cd.name(file_name);
	Query<PayLoad> q(dbc);
	q.select(cd);
	std::vector<std::string> found_keys;
	for (auto x : q.keys())
	{
		// for each element in the db returned by query
		if (dbc[x].name() == file_name)
		{
			found_keys.push_back(x); // add key to answer
		}
	}
	for (auto x : found_keys)
	{	// return file's  given version
		if (ver == dbc[x].payLoad().version())
		{
			return x;
		}
	}
	return "";
}


//----< find the location of a file with given name and version >---------------------------------------------------

std::string Browse::locate_file(std::string file_name, int ver, DbCore<PayLoad>& dbc)
{
	std::string x = get_key(file_name, ver, dbc);
	if (x != "")
	{
		std::string location;
		location = (dbc[x].payLoad().path() + dbc[x].name()) + ".";
		location += std::to_string(dbc[x].payLoad().version());
		return location;
	}
	return "Not found";
}

//----< find the location of a file's latest version with given name  >-----------------------------------------------

std::string Browse::locate_file(std::string file_name, DbCore<PayLoad>& dbc)
{
	std::string x = get_key(file_name, dbc);
	if (x != "")
	{
		std::string location;
		location = (dbc[x].payLoad().path() + dbc[x].name()) + ".";
		location += std::to_string(dbc[x].payLoad().version());
		return location;
	}
	return "Not Found";
}

//----< prints a file's latest version with given name  >-----------------------------------------------

void Browse::browse_file(std::string file_name, DbCore<PayLoad>& dbc)
{
	std::string location = locate_file(file_name, dbc);
	if (location == "Not Found")
	{
		std::cout << "\nFile "<< file_name <<" not found\n";
	}
	else
	{
		//file in location
		std::ifstream f(location);

		if (f.is_open())
		{
			std::cout << "\n#######################################################\n";
			std::cout << "DISPLAYING FILE CONTENTS \n";
			std::cout << file_name << " | version - " << location.back() << "\n";
			std::string key = get_key(file_name, dbc);
			std::cout << "description - " << dbc[key].descrip() << "\n\n";
			std::cout << f.rdbuf() << "\n\n";
			std::cout << "#######################################################\n";
		}
	}
}

//----< prints a file's specified version with given name  >-----------------------------------------------

void Browse::browse_file(std::string file_name, int ver, DbCore<PayLoad>& dbc)
{
	std::string location = locate_file(file_name,ver, dbc);
	if (location == "Not Found")
	{
		std::cout << "\nFile " << file_name << " not found\n";
	}
	else
	{
		//file in location
		std::ifstream f(location);

		if (f.is_open())
		{
			std::cout << "\n#######################################################\n";
			std::cout << "DISPLAYING FILE CONTENTS \n";
			std::cout << file_name << " | version - " << location.back() << "\n";
			std::string key = get_key(file_name, dbc);
			std::cout << "description - " << dbc[key].descrip() << "\n\n";
			std::cout << f.rdbuf() << "\n\n";
			std::cout << "#######################################################\n";
			
		}
	}
}

//----< browses all files in the current key set  >-----------------------------------------------

void Browse::browse_all(DbCore<PayLoad>& dbc)
{
	std::cout << "\nDisplaying files in the current Query Set : \n";
	for (auto key : keys)
	{
		std::cout << "Key - " << key << "\n";
		std::cout << dbc[key].name() << " | version - " << dbc[key].payLoad().version() << "\n";
		std::cout << "description - " << dbc[key].descrip() << "\n\n";
	}
}

//----< prints a file specified by given key  >---------------------------------------------

void Browse::browseFilebyKey(std::string key, DbCore<PayLoad>& dbc)
{
	for (auto x : keys)
	{
		if (x == key)
		{
			std::string location;
			location = (dbc[key].payLoad().path() + dbc[key].name()) + ".";
			location += std::to_string(dbc[key].payLoad().version());

			std::ifstream f(location);
			if (f.is_open())
			{
				std::cout << "\n\n#######################################################\n";
				std::cout << "DISPLAYING FILE CONTENTS \n";
				std::cout << dbc[key].name() << " | version - " << location.back() << "\n";
				std::cout << "description - " << dbc[key].descrip() << "\n\n";
				std::cout << f.rdbuf() << "\n\n";
				std::cout << "#######################################################\n";
			}
			return;
		}
	}
	std::cout << "\nFile not in current Query Set.\n";
}

//----< test stub >--------------------------------------------------

#ifdef TEST_BROWSE

#include <iostream>
#include "../TestExecutive/TestExecutive.h"
#include "../TestExecutive/TestExecutive.cpp"

using namespace NoSqlDb;

int main()
{
	Utilities::Title("Testing Browse");
	try {
		RepositoryCore R = makeTestRepo();
		std::cout << "\n Showing Core Repository : ";
		showDb(R.Repo());

		Browse B;

		std::cout << "\n finding location of file - pkgDevin.h.2";
		std::cout << "\n Location - " << B.locate_file("pkgDevin.h", 2, R.Repo());
		std::cout << "\n";

		std::cout << "\n finding location of file - pkgDemo.h | No version provided, will find latest version";
		std::cout << "\n Location - " << B.locate_file("pkgDemo.h", R.Repo());
		putLine();

		std::cout << "\n viewing contents of file - pkgDevin.h.2";
		B.browse_file("pkgDevin.h", 2, R.Repo()); // if the file is not checkedIn then it will do nothing
		putLine();

		std::cout << "\n viewing contents of file - pkgDemo.h | No version provided, will find latest version";
		B.browse_file("pkgDemo.h", R.Repo());
		putLine();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  -- " << ex.what() << " --";
	}
	std::cout << "\n\n";
}
#endif