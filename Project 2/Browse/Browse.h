#pragma once
////////////////////////////////////////////////////////////////////////////////////////
// Browse.h - provides the capability to locate files and view their contents.        //
// ver 1.0																			  //
// Author - Devin Upreti															  //
////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides one class:
* - Browse class contains a set of keys that can be retrieved from Query
*	upon which it performs operations to view file and browse descriptio*ns.
*	It also contains functions that help it finding key of a file and other 
*	ops such as finding location of a file.
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
* - created Browse class and implemented associated functions
*/

#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../Query/Query.h"
#include <fstream>

using namespace NoSqlDb;

//////////////////////////////////////////////////////////////////////////////////////
// Browse Class 
// - to locate files and view their contents

class Browse {
	public:
		void getQueryKeys(std::vector<std::string>& keys_) { keys = keys_; }
		void browse_all(DbCore<PayLoad>& dbc);
		void browseFilebyKey(std::string key, DbCore<PayLoad>& dbc);

		std::string get_key(std::string file_name, DbCore<PayLoad>& dbc); // latest version
		std::string get_key(std::string file_name, int ver, DbCore<PayLoad>& dbc);

		std::string locate_file(std::string file_name , DbCore<PayLoad>& dbc); // latest version
		std::string locate_file(std::string file_name, int ver, DbCore<PayLoad>& dbc);
		
		void browse_file(std::string file_name, DbCore<PayLoad>& dbc); // latest version
		void browse_file(std::string file_name, int ver, DbCore<PayLoad>& dbc);
	private:
		std::vector<std::string> keys;

};