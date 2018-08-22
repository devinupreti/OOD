#ifndef PAYLOAD_H
#define PAYLOAD_H
///////////////////////////////////////////////////////////////////////
// PayLoad.h - application defined payload                           //
// ver 1.2                                                           //
// Author - Devin Upreti											 //
// Source - Prof. Jim Fawcett, CSE687 - OOD, Spring 2018             //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single class, PayLoad:
*  - holds a payload string and vector of categories
*  - provides means to set and access those values
*  - provides methods used by Persist<PayLoad>:
*    - Sptr toXmlElement();
*    - static PayLoad fromXmlElement(Sptr elem);
*  - provides a show function to display PayLoad specific information
*  - PayLoad processing is very simple, so this package contains only
*    a header file, making it easy to use in other packages, e.g.,
*    just include the PayLoad.h header.
*
*  Required Files:
*  ---------------
*    PayLoad.h, PayLoad.cpp - application defined package
*    DbCore.h, DbCore.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.2 : 27 Feb 2018
*  - added elements path, namespace, checkInStatus ... to payload
*  - added associated functions
*  ver 1.1 : 19 Feb 2018
*  - added inheritance from IPayLoad interface
*  Ver 1.0 : 10 Feb 2018
*  - first release
*
*/

#include <string>
#include <vector>
#include <iostream>
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../DbCore/Definitions.h"
#include "../DbCore/DbCore.h"
#include "IPayLoad.h"

///////////////////////////////////////////////////////////////////////
// PayLoad class provides:
// - a std::string value which, in Project #2, will hold a file path
// - a vector of string categories, which for Project #2, will be 
//   Repository categories
// - methods used by Persist<PayLoad>:
//   - Sptr toXmlElement();
//   - static PayLoad fromXmlElement(Sptr elem);


namespace NoSqlDb
{
  class PayLoad : public IPayLoad<PayLoad>
  {
  public:
    PayLoad() = default;
    PayLoad(const std::string& path) : path_(path) {}
    static void identify(std::ostream& out = std::cout);
    PayLoad& operator=(const std::string& path)
    {
      path_ = path;
      return *this;
    }
    //operator std::string() { return path_; }

    std::string path() const { return path_; }
    std::string& path() { return path_; }
    void path(const std::string& path) { path_ = path; }

	std::string author() const { return author_; }
	std::string& author() { return author_; }
	void author(const std::string& author) { author_ = author; }

    std::vector<std::string>& categories() { return categories_; }
    std::vector<std::string> categories() const { return categories_; }
	void categories(const std::vector<std::string>& cat) { categories_ = cat; }

    bool hasCategory(const std::string& cat)
    {
      return std::find(categories().begin(), categories().end(), cat) != categories().end();
    }

	std::string status() const { return checkinStatus_; }
	std::string& status() { return checkinStatus_; }
	
	void closeStatus() { checkinStatus_ = "closed"; } // to close the status of a CheckIn object
	void openStatus() { checkinStatus_ = "open"; }
	void pendingStatus() { checkinStatus_ = "pending"; }

	bool statusOpen(); // returns true if status is open

	int version() const { return version_; }
	int& version() { return version_; }

	std::string name_space() const { return namespace_; }
	std::string& name_space() { return namespace_; }
	void name_space(const std::string& namespce) { namespace_ = namespce; }

    Sptr toXmlElement();
    static PayLoad fromXmlElement(Sptr elem);

    static void showPayLoadHeaders(std::ostream& out = std::cout);
    static void showElementPayLoad(NoSqlDb::DbElement<PayLoad>& elem, std::ostream& out = std::cout);
    static void showDb(NoSqlDb::DbCore<PayLoad>& db, std::ostream& out = std::cout);
  
  private:
    std::string path_;
	std::string namespace_ = "noNS"; // default no namespace
	std::string checkinStatus_ = "open";
	std::string author_;
	int version_ = 1;
    std::vector<std::string> categories_;
  };

  //----< checks if the status is open or closed >---------------------------------------------

  inline bool PayLoad::statusOpen()
  {
	  if (this->status() == "open") { return true; }
	  return false;
  }

  //----< show file name >---------------------------------------------

  inline void PayLoad::identify(std::ostream& out)
  {
    out << "\n  \"" << __FILE__ << "\"";
  }
  
  //----< create XmlElement that represents PayLoad instance >---------
  /*
  * - Required by Persist<PayLoad>
  */
  inline Sptr PayLoad::toXmlElement()
  {
    Sptr sPtr = XmlProcessing::makeTaggedElement("payload");
    XmlProcessing::XmlDocument doc(makeDocElement(sPtr));
    Sptr sPtrVal = XmlProcessing::makeTaggedElement("value",path_);
    sPtr->addChild(sPtrVal);
    Sptr sPtrCats = XmlProcessing::makeTaggedElement("categories");
    sPtr->addChild(sPtrCats);
    for (auto cat : categories_)
    {
      Sptr sPtrCat = XmlProcessing::makeTaggedElement("category", cat);
      sPtrCats->addChild(sPtrCat);
    }
    return sPtr;
  }
  //----< create PayLoad instance from XmlElement >--------------------
  /*
  * - Required by Persist<PayLoad>
  */
  inline PayLoad PayLoad::fromXmlElement(Sptr pElem)
  {
    PayLoad pl;
    for (auto pChild : pElem->children())
    {
      std::string tag = pChild->tag();
      std::string val = pChild->children()[0]->value();
      if (tag == "value")
      {
        pl.path(val);
      }
      if (tag == "categories")
      {
        std::vector<Sptr> pCategories = pChild->children();
        for (auto pCat : pCategories)
        {
          pl.categories().push_back(pCat->children()[0]->value());
        }
      }
    }
    return pl;
  }
  
  
  /////////////////////////////////////////////////////////////////////
  // PayLoad display functions

  inline void PayLoad::showPayLoadHeaders(std::ostream& out)
  {
    out << "\n  ";
    out << std::setw(10) << std::left << "Name";
    out << std::setw(45) << std::left << "Path";
    out << std::setw(25) << std::left << "Categories";
    out << "\n  ";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(40) << std::left << "--------------------------------------";
    out << std::setw(25) << std::left << "-----------------------";
  }


  inline void PayLoad::showElementPayLoad(NoSqlDb::DbElement<PayLoad>& elem, std::ostream& out)
  {
    out << "\n  ";
    out << std::setw(10) << std::left << elem.name().substr(0, 8);
    out << std::setw(40) << std::left << elem.payLoad().path();
    for (auto cat : elem.payLoad().categories())
    {
      out << cat << " ";
    }
  }

  inline void PayLoad::showDb(NoSqlDb::DbCore<PayLoad>& db, std::ostream& out)
  {
    showPayLoadHeaders(out);
    for (auto item : db)
    {
      NoSqlDb::DbElement<PayLoad> temp = item.second;
      PayLoad::showElementPayLoad(temp, out);
    }
  }
}
#endif
