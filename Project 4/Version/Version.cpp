/////////////////////////////////////////////////////////////////////////////////
// Version.cpp - manages version numbering for all files held in the Repository//
// ver 1.0																	   //
// Devin Upreti, Project 2 CSE687 - Object Oriented Design, Spring 2018        //
/////////////////////////////////////////////////////////////////////////////////

#include "Version.h"

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

	Version V;
	std::string key = "nsDevin::pkgDevin.h.4";
	if (R.Repo().contains(key))
	{
		std::cout << "\n Increasing Version of Key : " << key << " to 5";
		V.increaseVersion(R.Repo()["nsDevin::pkgDevin.h.4"].payLoad());

		std::cout << "\n Showing Database after Increasing Version of Key : " << key << " to 5\n";
		showDb(R.Repo());
	}
	return 1;
}

#endif // TEST_CHECKIN

