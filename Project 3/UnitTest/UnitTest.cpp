/////////////////////////////////////////////////////////////////////////
// UnitTest.cpp - Operates as server and executes test functions       //
// ver 1.0	                                                           //
// Devin Upreti, CSE687 - Object Oriented Design, Spring 2018          //
/////////////////////////////////////////////////////////////////////////

#include "../Process/Process/Process.h"
#include "../ServerPrototype/ServerPrototype.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <chrono> 
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using namespace Utilities;

using Msg = MsgPassingCommunication::Message;

///////////////////////////////////////////////////////////
// Gets files from given path
Files Server::getFiles(const Repository::SearchPath& path)
{
	return Directory::getFiles(path);
}

///////////////////////////////////////////////////////////
// Gets Directories from given path
Dirs Server::getDirs(const Repository::SearchPath& path)
{
	return Directory::getDirectories(path);
}

///////////////////////////////////////////////////////////
// Prints contents of given vector
template<typename T>
void show(const T& t, const std::string& msg)
{
	std::cout << "\n  " << msg.c_str();
	for (auto item : t)
	{
		std::cout << "\n    " << item.c_str();
	}
}

///////////////////////////////////////////////////////////
// Generates reply message for echo
std::function<Msg(Msg)> echo = [](Msg msg) {
	Msg reply = msg;
	reply.to(msg.from());
	reply.from(msg.to());
	return reply;
};

///////////////////////////////////////////////////////////
// Generates reply message with command getFiles
std::function<Msg(Msg)> getFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getFiles");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files files = Server::getFiles(searchPath);
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;
};

///////////////////////////////////////////////////////////
// Generates reply message with command getDirs
std::function<Msg(Msg)> getDirs = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getDirs");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files dirs = Server::getDirs(searchPath);
		size_t count = 0;
		for (auto item : dirs)
		{
			if (item != ".." && item != ".")
			{
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				reply.attribute("dir" + countStr, item);
			}
		}
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};

///////////////////////////////////////////////////////////
// Generates reply message with command Connect
std::function<Msg(Msg)> connectfn = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("Connect");
	return reply;
};

///////////////////////////////////////////////////////////
// Generates reply message with command CheckIn
std::function<Msg(Msg)> checkin = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("CheckIn");
	std::string filename = msg.value("fileinfo");
	reply.attribute("fileinfo", filename);
	return reply;
};

///////////////////////////////////////////////////////////
// Generates reply message with command CheckOut
std::function<Msg(Msg)> checkout = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("CheckOut");
	std::string filename = msg.value("fileinfo");
	reply.attribute("fileinfo", filename);
	return reply;
};

///////////////////////////////////////////////////////////////////////////
// Generates reply message with command ViewFile and views file on notepad
std::function<Msg(Msg)> viewfile = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("ViewFile");
	std::string filename = msg.value("fileinfo");
	reply.attribute("fileinfo", filename);

	std::string path = msg.value("path");
	reply.attribute("path", path);
	if (path != "")
	{
		Process p;
		p.title("View File");
		std::string appPath = "c:/windows/system32/notepad.exe";
		p.application(appPath);

		std::string cmdLine = "/A " + path;
		p.commandLine(cmdLine);
		p.create();
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};

///////////////////////////////////////////////////////////////////////////////
// Generates reply message with command ViewMetadata and views file on notepad
std::function<Msg(Msg)> viewmeta = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("ViewMetadata");
	std::string pkgname = msg.value("pkginfo");
	reply.attribute("pkginfo", pkgname);

	std::string path = msg.value("path");
	reply.attribute("path", path);
	if (path != "")
	{
		Process p;
		p.title("View Meta");
		std::string appPath = "c:/windows/system32/notepad.exe";
		p.application(appPath);
		std::string cmdLine;
		if (path.find(pkgname) != std::string::npos) { cmdLine = "/A ../Storage/" + pkgname + ".txt"; }
		else { cmdLine = "/A " + path + pkgname + "/" + pkgname + ".txt"; }
		p.commandLine(cmdLine);
		p.create();
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};

///////////////////////////////////////////////////////////////////////
// Tests for Requirements

//----< demo requirement #1 >------------------------------------------
bool testR1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	std::cout << "\n";
	std::cout << "  This project is developed using Visual Studio 2017.";
	std::cout << "\n";
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------
bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  The message printed in the Connect tab of GUI demonstrates this requirement";
	std::cout << "\n";
	return true; 
}

//----< demo requirement #3 >------------------------------------------
bool testR3()
{
	Utilities::title("Demonstrating Requirement #3");
	std::cout << "\n  The GUI shows that are tabs are created and a simulation is run for each tab's requirements";
	std::cout << "\n";
	return true;
}

//----< demo requirement #4 >------------------------------------------
bool testR4()
{
	Utilities::title("Demonstrating Requirement #4");
	std::cout << "\n  Unit test suite is run for location at ../Storage";
	std::cout << "\n  UnitTest module's main function runs automated simulation for requirment of each tab";
	std::cout << "\n";
	return true;
}


///////////////////////////////////////////////////////////
// Main function for Server
int main()
{
	Utilities::Title("Testing Server Prototype");
	std::cout << "\n";
	//StaticLogger<1>::attach(&std::cout);
	//StaticLogger<1>::start();
	Server server(serverEndPoint, "ServerPrototype");
	server.start();

	TestExecutive ex; // now define test structures with test function and message
	TestExecutive::TestStr ts1{ testR1, "Use Visual Studio and C++11" };
	TestExecutive::TestStr ts2{ testR2, "HTTP style asynchronous message passing communication" };
	TestExecutive::TestStr ts3{ testR3, "Tabs for Connect, CheckIn, CheckOut, Browse, ViewFile and Metadata" };
	TestExecutive::TestStr ts4{ testR4, "Automated Unit Test Suite" };

	ex.registerTest(ts1); // registering test structures with TestExecutive instance, ex
	ex.registerTest(ts2);
	ex.registerTest(ts3);
	ex.registerTest(ts4);

	bool result = ex.doTests(); // run tests

	std::cout << "\n\n\n  Printing message processing by server";
	std::cout << "\n  -------------------------------------";
	std::cout << "\n";

	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("serverQuit", echo);
	server.addMsgProc("Connect", connectfn);
	server.addMsgProc("CheckIn", checkin);
	server.addMsgProc("CheckOut", checkout);
	server.addMsgProc("ViewFile", viewfile);
	server.addMsgProc("ViewMetadata", viewmeta);
	server.processMessages();

	server.stop();

	if (result == true) { std::cout << "\n  all tests passed\n\n"; system("pause"); }
	else { std::cout << "\n  at least one test failed\n\n"; system("pause"); }

	return 0;
}