/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RepositoryCore.cpp - - provides means to check-in, version, browse, and check-out source code packages. //
// Ver 1.0																								   //
// Last Modified - 02/26/18																				   //
// Author - Devin Upreti																				   //
// Project 2. Spring 2018. Object Oriented Design.														   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RepositoryCore.h"

#ifdef TEST_RC

#include <iostream>
#include "../CheckIn/CheckIn.h"
#include "../CheckIn/CheckIn.cpp"

int main()
{
	std::cout << "Testing RepositoryCore ... ";
	std::cout << "\nPlease view in full screen \n";
	RepositoryCore R;
	//DbCore<PayLoad> db_;
	CheckIn ci;
	
	Children test;
	test.push_back("Germany");
	test.push_back("Argentina");
	test.push_back("France");	
	
	ci.file_name("SF.txt").datetime(DateTime().now()).description("TEST FILE").dependency(test,R.Repo());
	ci.path("..\\Client\\SF.txt").author("Devin"); 
	
	ci.CheckFile(R.Repo());

	test.clear();
	test.push_back("SF.txt");

	ci.file_name("Header.h").datetime(DateTime().now()).description("TEST FILE 2").dependency(test,R.Repo());
	ci.path("..\\Client\\Header.h").author("Devin"); 
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());
	ci.CheckFile(R.Repo());

	ci.file_name("Example.h").datetime(DateTime().now()).description("TEST FILE 2");
	ci.path("..\\Client\\Example.h").author("Devin"); 

	ci.CheckFile(R.Repo());

	showDb(R.Repo());
	std::cout << "\nFile Created";

	std::cout << "\n\n";
	system("pause");
	return 1;
}


#endif // TEST_RC