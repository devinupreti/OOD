#pragma once
/////////////////////////////////////////////////////////////////////////////////
// CheckOut.h - provides functionality to retrieve a package		           //
// ver 1.0																	   //
// Devin Upreti, Project 2 CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides one class:
* - CheckOut class contains functions that retrieve a file
*	from a Repository and save it in a specified location
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* Query.h, Query.cpp
* PayLoad.h, PayLoad.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 March 2018
* - created CheckOut class and implemented associated functions
*/

#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../Query/Query.h"
#include <fstream>
#include <windows.h>

using namespace NoSqlDb;

//////////////////////////////////////////////////////////////////////////////////////
// CheckOut Class 
// - provides functionality to retrieve a package


class CheckOut {
	public:
		CheckOut() {}
		void retrieve(const std::string& file_name, DbCore<PayLoad>& dbc);
		void retrieve(const std::string& file_name, int ver, DbCore<PayLoad>& dbc);
	private:
		std::vector<std::string> searchdb(const std::string& file_name, DbCore<PayLoad>& dbc);
		void save_file(std::string& key, DbCore<PayLoad>& dbc);
		void dependency_file(const std::string& key, DbCore<PayLoad>& dbc);
		std::string depend_info(const std::string& key, DbCore<PayLoad>& dbc);
};
