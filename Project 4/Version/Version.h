#pragma once
/////////////////////////////////////////////////////////////////////////////////
// Version.h - manages version numbering for all files held in the Repository  //
// ver 1.0																	   //
// Devin Upreti, Project 2 CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides one class:
* - Version class contains functions that maintain the version
*	field of a payload object
*
* Required Files:
* ---------------
* PayLoad.h, PayLoad.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 March 2018
* - created Version class 
*/

#include "../PayLoad/PayLoad.h"

using namespace NoSqlDb;

//////////////////////////////////////////////////////////////////////////////////////
// Version Class 
// - manages version numbering

class Version
{
	public:
		void increaseVersion(PayLoad& p) { p.version() += 1; }
		void decreaseVersion(PayLoad& p) { p.version() -= 1; }
		void resetVersion(PayLoad& p) { p.version() = 1; }
		void resetVersion(PayLoad& p, int ver) { p.version() = ver; }
};