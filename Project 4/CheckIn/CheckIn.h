#pragma once
/////////////////////////////////////////////////////////////////////////////////
// CheckIn.h - provides functionality to start a new package version           //
// ver 1.0																	   //
// Devin Upreti, Project 2 CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides one class:
* - CheckIn class contains a dbElement of type Payload
*	and a version object to manage the version of that payload.
*	It also defines functions that provide the functionality to 
*	checkIn a file.
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* Query.h, Query.cpp
* PayLoad.h, PayLoad.cpp
* Version.h, Version.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 1 March 2018
* - created CheckIn class and implemented associated functions
*/

#include <fstream>
#include "../DbCore/DbCore.h"
#include "../PayLoad/PayLoad.h"
#include "../Query/Query.h"
#include "../Version/Version.h"
#include <windows.h>

using namespace NoSqlDb;

//////////////////////////////////////////////////////////////////////////////////////
// CheckIn Class 
// - provides the functionality to CheckIn a file in a Repository

class CheckIn {
	public:
		CheckIn() {}
		CheckIn(DbElement<PayLoad>& dbe_) : dbe(dbe_) {}
		
		void setdbe(DbElement<PayLoad>& dbe_) { dbe = dbe_; }

		DbElement<PayLoad>& getdbe() { return dbe; }
		
		std::string& getname() { return dbe.name(); }

		CheckIn& file_name(const std::string& fname) { if (isStatusOpen()) { dbe.name(fname); } return *this; }
		CheckIn& description(const std::string& des) { if (isStatusOpen()) { dbe.descrip(des); } return *this; }
		CheckIn& datetime(const DateTime& dateTime) { if (isStatusOpen()) { dbe.dateTime(dateTime); } return *this; }

		CheckIn& dependson(const std::vector<std::string>& depend) { if (isStatusOpen()) { dbe.children(depend); } return *this; }

		CheckIn& categories(const std::vector<std::string>& cat) { if (isStatusOpen()) { dbe.payLoad().categories(cat); } return *this; }
		CheckIn& author(const std::string& author) { if (isStatusOpen()) { dbe.payLoad().author(author); } return *this; }

		CheckIn& sourcepath(const std::string& sourcepath) { if (isStatusOpen()) { source_path = sourcepath; } return *this; }
		CheckIn& path(const std::string& mypath) { dbe.payLoad().path(mypath); return *this; }

		CheckIn& name_space(const std::string& ns) { if (isStatusOpen()) { dbe.payLoad().name_space(ns); } return *this; }

		bool is_file_exist(const std::string fileName);

		void CheckFile(DbCore<PayLoad>& dbc);

		bool canClose(DbCore<PayLoad>& dbc);
		void closeCheckIn(DbCore<PayLoad>& dbc);
		bool isStatusOpen() { return dbe.payLoad().statusOpen(); }

		void resetStatus(); //used to reset status of the object after closed checkin is saved to repo
		void saveFile();

	private:
		DbElement<PayLoad> dbe;
		std::string source_path = "..\\ServerPrototype\\SaveFiles\\";
		Version V;
		void setVersion(int version) { V.resetVersion(dbe.payLoad(), version); }
};
