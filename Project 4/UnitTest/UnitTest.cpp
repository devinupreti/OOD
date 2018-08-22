#pragma once
/////////////////////////////////////////////////////////////////////////
// UnitTest.cpp - Operates as server and executes test functions       //
// ver 1.3	                                                           //
// Devin Upreti, CSE687 - Object Oriented Design, Spring 2018          //
/////////////////////////////////////////////////////////////////////////

#include "UnitTest.h"
#include "../ServerPrototype/ServerPrototype.h"
#include "../Process/Process/Process.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <chrono> 
#include "../CppNoSqlDb/Utilities/StringUtilities/StringUtilities.h"
#include "../CppNoSqlDb/Utilities/TestUtilities/TestUtilities.h"

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using namespace Utilities;
using namespace NoSqlDb;

using Msg = MsgPassingCommunication::Message;

//----< return name of every file on path >----------------------------
Files Server::getFiles(const Repository::SearchPath& path)
{
	return Directory::getFiles(path);
}

//----< return name of every subdirectory on path >--------------------
Dirs Server::getDirs(const Repository::SearchPath& path)
{
	return Directory::getDirectories(path);
}

namespace MsgPassingCommunication
{
	// These paths, global to MsgPassingCommunication, are needed by 
	// several of the ServerProcs, below.
	// - should make them const and make copies for ServerProc usage

	std::string sendFilePath;
	std::string saveFilePath;

	//----< show message contents >--------------------------------------
	template<typename T>
	void show(const T& t, const std::string& msg)
	{
		std::cout << "\n  " << msg.c_str();
		for (auto item : t)
		{
			std::cout << "\n    " << item.c_str();
		}
	}
	
	//----< test ServerProc simply echos message back to sender >--------
	std::function<Msg(Msg)> echo = [](Msg msg) {
		Msg reply = msg;
		reply.to(msg.from());
		reply.from(msg.to());
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
		std::string filename = msg.value("fileName");
		reply.attribute("fileinfo", filename);

		ServerRepo SR;
		DbCore<PayLoad> repo = SR.getSR().Repo();
		CheckIn ci;
		ci.path(msg.value("path")).file_name(msg.value("fileName")).description(msg.value("description")).author("Devin");
		ci.sourcepath(msg.value("dstFileName"));
		std::vector<std::string> Children;
		if (msg.containsKey("child1"))
		{
			int i = 1;
			std::string childkey = "child1";
			while (msg.containsKey(childkey))
			{
				Children.push_back(msg.value(childkey));
				i = i + 1;
				childkey = "child" + std::to_string(i);
			}
			ci.dependson(Children);
		}
		ci.CheckFile(repo);
		std::cout << "\n  Showing Db after CheckIn : \n";
		NoSqlDb::showDb(repo);
		// saving file in Server's CheckIn folder
		std::string checkinpath = "../Storage/CheckedIn/" + msg.value("fileName");
		std::string fullcinPath = FileSystem::Path::getFullFileSpec(checkinpath);
		std::string fullDstPath = saveFilePath;
		fullDstPath += "/" + msg.value("fileName");
		FileSystem::File::copy(fullDstPath, fullcinPath);
		
		SR.getSR().Repo() = repo;
		return reply;
	};

	///////////////////////////////////////////////////////////
	// Generates reply message with command Browse
	std::function<Msg(Msg)> browse = [](Msg msg) {
		Msg reply;
		reply.to(msg.from());
		reply.from(msg.to());
		reply.command("Browse");

		Conditions<PayLoad> conds;
		conds.name(msg.value("namequery"));
		conds.description(msg.value("descquery"));
		std::vector<std::string> Children;
		if (msg.containsKey("child1"))
		{
			int i = 1;
			std::string childkey = "child1";
			while (msg.containsKey(childkey))
			{
				Children.push_back(msg.value(childkey));
				i = i + 1;
				childkey = "child" + std::to_string(i);
			}
			conds.children(Children);
		}
		ServerRepo SR;
		DbCore<PayLoad> repo = SR.getSR().Repo();
		Query<PayLoad> q1(repo);
		std::cout << "\n  Showing files that matched Query\n";
		q1.select(conds).show();
		std::vector<std::string> keys = q1.keys();
		for (int i = 0; i < keys.size(); i++)
		{
			std::string myString = "key" + std::to_string(i + 1);
			reply.attribute(myString, keys[i]);
		}
		int keycount = keys.size();
		reply.attribute("keycount", std::to_string(keycount));
		SR.getSR().Repo() = repo;
		return reply;
	};


	//----< getFiles ServerProc returns list of files on path >----------
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

	//----< getDirs ServerProc returns list of directories on path >-----
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

	//----< sendFile ServerProc sends file to requester >----------------
	/*
	*  - Comm sends bodies of messages with sendingFile attribute >------
	*/
	std::function<Msg(Msg)> sendFile = [](Msg msg) {
		Msg reply;
		reply.to(msg.from());
		reply.from(msg.to());
		reply.command("sendFile");
		reply.attribute("sendingFile", msg.value("fileName"));
		reply.attribute("fileName", msg.value("fileName"));
		reply.attribute("verbose", "blah blah");
		std::string path = msg.value("path");
		if (path != "")
		{
			std::string searchPath = storageRoot;
			if (path != "." && path != searchPath)
				searchPath = searchPath + "\\" + path;
			if (!FileSystem::Directory::exists(searchPath))
			{
				std::cout << "\n  file source path does not exist";
				return reply;
			}
			std::string filePath = searchPath + "/" + msg.value("fileName");
			std::string fullSrcPath = FileSystem::Path::getFullFileSpec(filePath);
			std::string fullDstPath = sendFilePath;
			if (!FileSystem::Directory::exists(fullDstPath))
			{
				std::cout << "\n  file destination path does not exist";
				return reply;
			}
			fullDstPath += "/" + msg.value("fileName");
			FileSystem::File::copy(fullSrcPath, fullDstPath);
			/*std::string checkoutpath = "../LocalStorage/CheckOutFiles/" + msg.value("fileName");
			std::string fullcoPath = FileSystem::Path::getFullFileSpec(checkoutpath);
			FileSystem::File::copy(fullDstPath, fullcoPath);*/
		}
		else
		{
			std::cout << "\n  getDirs message did not define a path attribute";
		}
		return reply;
	};

	//----< analyze code on current server path >--------------------------
	/*
	*  - Creates process to run CodeAnalyzer on specified path
	*  - Won't return until analysis is done and logfile.txt
	*    is copied to sendFiles directory
	*/
	std::function<Msg(Msg)> codeAnalyze = [](Msg msg) {
		Msg reply;
		reply.to(msg.from());
		reply.from(msg.to());
		reply.command("sendFile");
		reply.attribute("sendingFile", "logfile.txt");
		reply.attribute("fileName", "logfile.txt");
		reply.attribute("verbose", "blah blah");
		std::string path = msg.value("path");
		if (path != "")
		{
			std::string searchPath = storageRoot;
			if (path != "." && path != searchPath)
				searchPath = searchPath + "\\" + path;
			if (!FileSystem::Directory::exists(searchPath))
			{
				std::cout << "\n  file source path does not exist";
				return reply;
			}
			// run Analyzer using Process class

			Process p;
			p.title("test application");
			//std::string appPath = "c:/su/temp/project4sample/debug/CodeAnalyzer.exe";
			std::string appPath = "CodeAnalyzer.exe";
			p.application(appPath);

			//std::string cmdLine = "c:/su/temp/project4Sample/debug/CodeAnalyzer.exe ";
			std::string cmdLine = "CodeAnalyzer.exe ";
			cmdLine += searchPath + " ";
			cmdLine += "*.h *.cpp /m /r /f";
			//std::string cmdLine = "c:/su/temp/project4sample/debug/CodeAnalyzer.exe ../Storage/path *.h *.cpp /m /r /f";
			p.commandLine(cmdLine);

			std::cout << "\n  starting process: \"" << appPath << "\"";
			std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";

			CBP callback = []() { std::cout << "\n  --- child process exited ---"; };
			p.setCallBackProcessing(callback);

			if (!p.create())
			{
				std::cout << "\n  can't start process";
			}
			p.registerCallback();

			std::string filePath = searchPath + "\\" + /*msg.value("codeAnalysis")*/ "logfile.txt";
			std::string fullSrcPath = FileSystem::Path::getFullFileSpec(filePath);
			std::string fullDstPath = sendFilePath;
			if (!FileSystem::Directory::exists(fullDstPath))
			{
				std::cout << "\n  file destination path does not exist";
				return reply;
			}
			fullDstPath += std::string("\\") + /*msg.value("codeAnalysis")*/ "logfile.txt";
			FileSystem::File::copy(fullSrcPath, fullDstPath);
		}
		else
		{
			std::cout << "\n  getDirs message did not define a path attribute";
		}
		return reply;
	};
}


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
	std::cout << "  The GUI for the project is developed using WPF which uses C# and C++ CLI is used for Managed Code";
	std::cout << "\n";
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------
bool testR2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  On startup, automated tests for CheckIn, CheckOut, Browse and Connect are run.";
	std::cout << "\n  These operations are performed on the Remote Repository present at Server";
	std::cout << "\n\n  AUTOMATED TEST SPECIFICATIONS ";
	std::cout << "\n  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	std::cout << "\n  CheckIn ";
	std::cout << "\n  ---------";
	std::cout << "\n  File : 3rd element in Local FileList from top";
	std::cout << "\n  Description : Test File";
	std::cout << "\n\n  CheckOut ";
	std::cout << "\n  ---------";
	std::cout << "\n  File : 2rd element in Remote FileList from top";
	std::cout << "\n\n  Browse ";
	std::cout << "\n  ---------";
	std::cout << "\n  Query -> Description : Test File \n           (this should return the key of the file checkedin)";
	std::cout << "\n\n  You can track the last performed operation through the status bar ";
	std::cout << "\n";
	return true;
}

//----< demo requirement #3 >------------------------------------------
bool testR3()
{
	Utilities::title("Demonstrating Requirement #3");
	std::cout << "\n  The automated test run for CheckOut transfers file\n  from Server to Client's CheckedOutFiles Folder through upload and download";
	std::cout << "\n\n  Transfer of File Paths ";
	std::cout << "\n  ~~~~~~~~~~~~~~~~~~~~~~~~";
	std::cout << "\n  CheckOut : Remote -> Local CheckOut Folder";
	std::cout << "\n  CheckIn  : Local  -> Remote CheckIn Folder\n";
	return true;
}

//----< demo requirement #4 >------------------------------------------
bool testR4()
{
	Utilities::title("Demonstrating Requirement #4");
	std::cout << "\n  The automated test run for CheckIn demonstrates message communication between Client and Server";
	std::cout << "\n  The Message Communication System is implemented using Sockets Library provided by Prof.";
	std::cout << "\n";
	return true;
}

//----< demo requirement #5 >------------------------------------------
bool testR5()
{
	Utilities::title("Demonstrating Requirement #5");
	std::cout << "\n  The message printed in the Connect tab of GUI demonstrates this requirement";
	std::cout << "\n";
	return true;
}

//----< demo requirement #6 >------------------------------------------
bool testR6()
{
	Utilities::title("Demonstrating Requirement #6");
	std::cout << "\n  The automated test for CheckOut prints the transfer of file blocks ";
	std::cout << "\n  to demonstrate this requirement";
	std::cout << "\n";
	return true;
}

//----< demo requirement #7 >------------------------------------------
bool testR7()
{
	Utilities::title("Demonstrating Requirement #7");
	std::cout << "\n  Unit test suite is run for ";
	std::cout << "\n  Client at location  ../LocalStorage";
	std::cout << "\n  Server at location  ../Storage";
	std::cout << "\n  UnitTest module's main function runs automated simulation for requirment of each tab";
	std::cout << "\n";
	return true;
}




using namespace MsgPassingCommunication;

bool executeTests()
{
	TestExecutive ex; // now define test structures with test function and message
	TestExecutive::TestStr ts1{ testR1, "Use Visual Studio, C++11, C++ CLI, WPF and C#" };
	TestExecutive::TestStr ts2{ testR2, "Repository Server with CheckIn, CheckOut and Browse" };
	TestExecutive::TestStr ts3{ testR3, "Upload and Download Files" };
	TestExecutive::TestStr ts4{ testR4, "Message-passing communication system" };
	TestExecutive::TestStr ts5{ testR5, "HTTP style messages using asynchronous one-way messaging" };
	TestExecutive::TestStr ts6{ testR6, "Sending and Receiving blocks of bytes" };
	TestExecutive::TestStr ts7{ testR7, "Automated Unit Test" };

	ex.registerTest(ts1); // registering test structures with TestExecutive instance, ex
	ex.registerTest(ts2);
	ex.registerTest(ts3);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts7);

	bool result = ex.doTests(); // run tests
	return result;
}

///////////////////////////////////////////////////////////
// Main function for Server
int main()
{
	SetConsoleTitleA("Project4 Server TEST Console");
	std::cout << "\n  Please view in full screen for best experience\n";
	Utilities::Title("Testing Server Prototype");
	std::cout << "\n";
	sendFilePath = FileSystem::Directory::createOnPath("./SendFiles");
	saveFilePath = FileSystem::Directory::createOnPath("./SaveFiles");

	Server server(serverEndPoint, "ServerPrototype");
	MsgPassingCommunication::Context* pCtx = server.getContext();
	pCtx->saveFilePath = saveFilePath;
	pCtx->sendFilePath = sendFilePath;
    server.start();

	bool result = executeTests();
	std::cout << "\n\n\n\n  Printing message processing by server";
	std::cout << "\n  -------------------------------------\n";
	server.addMsgProc("echo", echo);
	server.addMsgProc("Connect", connectfn);
	server.addMsgProc("CheckIn", checkin);
	server.addMsgProc("Browse", browse);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("sendFile", sendFile);
	server.addMsgProc("codeAnalyze", codeAnalyze);
	server.addMsgProc("serverQuit", echo);
    server.processMessages();

	Msg msg(serverEndPoint, serverEndPoint);  // send to self
	msg.name("msgToSelf");
	std::cout << "\n  press enter to exit\n";
	std::cin.get();
	std::cout << "\n";
	msg.command("serverQuit");
	server.postMessage(msg);
	server.stop();
	if (result) { std::cout << "\n\n  All tests passed"; system("pause");}
	else { std::cout << "\n\n  at least one test failed\n\n"; system("pause"); }
	
	return 0;
}
