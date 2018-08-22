#pragma once
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RepositoryCore.h - - provides means to check-in, version, browse, and check-out source code packages.   //
// Ver 1.0																								   //
// Last Modified - 02/26/18																				   //
// Author - Devin Upreti																				   //
// Project 2. Spring 2018. Object Oriented Design.														   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides one class:
* - RepositoryCore class contains a DbCore of type PayLoad
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* PayLoad.h, PayLoad.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 26 Feb 2018
*  - first release
*/

#include "../PayLoad/PayLoad.h"
#include "../DbCore/DbCore.h"

namespace NoSqlDb
{	

	//////////////////////////////////////////////////////////////////////////////////////
	// RepositoryCore Class methods 

	class RepositoryCore
	{
	public:
		RepositoryCore() {}
		RepositoryCore(DbCore<PayLoad>& db) : repo(db){}
		DbCore<PayLoad>& Repo() { return repo; }
	private:
		DbCore<PayLoad> repo;
	};
}



