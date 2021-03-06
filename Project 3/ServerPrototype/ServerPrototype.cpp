/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.2                                                             //
// Devin Upreti, CSE687 - Object Oriented Design, Spring 2018          //
// Source : Prof. Jim Fawcett										   //
/////////////////////////////////////////////////////////////////////////

#include "../Process/Process/Process.h"
#include "ServerPrototype.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <chrono>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
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

//////////////////////////////////////////////////////////////////////////////
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

#ifdef TEST_SERVER
///////////////////////////////////////////////////////////
// Main function for Server
int main()
{
  std::cout << "\n  Testing Server Prototype";
  std::cout << "\n ==========================";
  std::cout << "\n";
  //StaticLogger<1>::attach(&std::cout);
  //StaticLogger<1>::start();
  Server server(serverEndPoint, "ServerPrototype");
  server.start();

  std::cout << "\n  testing getFiles and getDirs methods";
  std::cout << "\n --------------------------------------";
  Files files = server.getFiles();
  show(files, "Files:");
  Dirs dirs = server.getDirs();
  show(dirs, "Dirs:");
  std::cout << "\n";

  std::cout << "\n  testing message processing";
  std::cout << "\n ----------------------------";
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
  return 0;
}
#endif

