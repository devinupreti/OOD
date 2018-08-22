#pragma once 
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 2.3                                                         //
// Author - Devin Upreti                                           //
// Source(ver 1.1) - Prof. Jim Fawcett, CSE687 - OOD, Spring 2018  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides three classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
* - Payload provides a generic class inside DbElement.
*   It contains a value payload_element and a vector payload_vector.
*	For Project 1 (CSE 687 Spring 18), they are implemented as strings.
*
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* XmlDocument.h, XmlDocument.cpp
* XmlElement.h, XmlElement.cpp
*
* Maintenance History:
* --------------------
* ver 2.3 : 4 Feb 2018
* - added Payload class and its methods
* ver 2.2 : 1 Feb 2018
* - added methods in dbCore to support conversion to and from xml file
* ver 2.1 : 27 Jan 2018
* - added methods in dbElement to support addition and deletion of relationships
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlElement/XmlElement.h"
#include <fstream>


using namespace XmlProcessing;


namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// Payload class
	// - contains an element of type T and a vector of the same type
	// - generic class 
	template<typename T>
	class Payload
	{
	public:
		T& payload_element() { return payload_element_; }
		T payload_element() const { return payload_element_; }
		void payload_element(const T& element_value) { payload_element_ = element_value; }

		std::vector<T>& payload_vector() { return payload_vector_; }
		std::vector<T> payload_vector() const { return payload_vector_; }
		
		void addtovector(const std::string& value) { payload_vector_.push_back(value); }
		
	private:
		T payload_element_;
		std::vector<T> payload_vector_;
	};
	
	
  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

  template<typename T>
  class DbElement
  {
  public:
    using Key = std::string;
    using Children = std::vector<Key>;

    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }
	Children& addchild(Key key);
	Children& deletechild(Key key);

	Payload<T>& payLoad_obj() { return payLoad_; }
	Payload<T> payLoad_obj() const { return payLoad_; }

    T& payLoad() { return payLoad_.payload_element(); }
    T payLoad() const { return payLoad_.payload_element(); }
    void payLoad(const T& payLoad) { payLoad_.payload_element(payLoad) ; }

	std::vector<T>& payLoadV() { return payLoad_.payload_vector(); }
	std::vector<T> payLoadV() const { return payLoad_.payload_vector(); }

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
    Payload<T> payLoad_;
  };
  
  /////////////////////////////////////////////////////////////////////
  // Methods of DbElement
  // 

  //----< to add a child to the set of Children >----------------------------------
  
  template<typename T>
  typename DbElement<T>::Children& DbElement<T>::addchild(Key child)
  {
	  std::vector<std::string>::iterator it;
	  it = find(children_.begin(), children_.end(), child);
	  if (it != children_.end())
	  {
		  //child already in db
		  std::cout << "\n  Child '" << child << "' is already added as a child";
		  std::cout << std::endl;
		  return children_;
	  }
	  //child not in db
	  children_.push_back(child);
	  return children_;
  }

  //----< to delete a child from the set of Children >----------------------------------
  
  template<typename T>
  typename DbElement<T>::Children& DbElement<T>::deletechild(Key child)
  {
	  std::vector<std::string>::iterator it;
	  it = find(children_.begin(), children_.end(), child);
	  if (it == children_.end())
	  {
		  //child not in db
		  std::cout << "\n  '" << child << "' is not a child." << std::endl;
		  return children_;
	  }
	  //child in db
	  children_.erase(it);
	  return children_;

  }

  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations
  
  template <typename T>
  class DbCore
  {
  public:
    using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
    size_t size();
    void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
    DbElement<T>& operator[](const Key& key);
    DbElement<T> operator[](const Key& key) const;
    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

	DbStore& deleteKey(Key key); // method that takes key and deletes it from db

    // methods to get and set the private database hash-map storage
    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }
	// methods to support xml file operations
	void dbtoXMLfile(std::string file_name);
	void xmltodb(XmlDocument & newXDoc);

  private:
    DbStore dbStore_;
    bool doThrow_ = false;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods

  //----< to retrieve db data from xml file >----------------------------------

  template<typename T>
  void DbCore<T>::xmltodb(XmlDocument & newXDoc)
  {   
	  using Sptr = std::shared_ptr<AbstractXmlElement>;
	  std::vector<Sptr> found = newXDoc.element("db").select(); // for db tag element
	  if (found.size() > 0)
	  {	  AbstractXmlElement::Attributes attribs = found[0]->attributes();
		  if (attribs.size() > 0)
			  std::cout << "\n  database type = " << "\"" << attribs[0].second << "\"\n"; // print attribute
	  }
	  std::cout << "\n  Building Databse from XmlDocument, using saved Xml file\n -------------------------------------------------------- \n";
	  using Key = std::string;
	  std::vector<Sptr> records = newXDoc.descendents("dbRecord").select();
	  for (auto pRecord : records)
	  {	  Key key;
		  DbElement<std::string> elem;
		  std::vector<Sptr> pChildren = pRecord->children();
		  for (auto pChild : pChildren)
		  {  if (pChild->tag() == "key") // if tag element is key
			  { key = pChild->children()[0]->value(); }
			 else
			  { std::vector<Sptr> pValueChildren = pChild->children();
				  for (auto pValueChild : pValueChildren)
				  {
					  if (pValueChild->tag() == "name") // if tag element is name
					  { elem.name(pValueChild->children()[0]->value()); }
					  
					  if (pValueChild->tag() == "description") // if tag element is description
					  { elem.descrip(pValueChild->children()[0]->value()); }
					  
					  if (pValueChild->tag() == "datetime") // if tag element is datetime
					  { elem.dateTime(pValueChild->children()[0]->value()); }
					  
					  if (pValueChild->tag() == "payload") // if tag element is payload
					  { elem.payLoad(pValueChild->children()[0]->value()); }
					  
					  if (pValueChild->tag() == "children") // if tag element is children then find child
					  { std::vector<Sptr> ptoChild = pValueChild->children();
						  for (auto relation : ptoChild)
						  { elem.children().push_back(relation->children()[0]->value()); }
					  }
				  }
			  }
		  } dbStore()[key] = elem; // add retrieved values to the db
	  }  
  }

  //----< to create file for persistance of db >----------------------------------

  template<typename T>
  void DbCore<T>::dbtoXMLfile(std::string file_name)
  {
	  using Sptr = std::shared_ptr<AbstractXmlElement>;

	  Sptr pDb = makeTaggedElement("db");
	  pDb->addAttrib("type", "testDb");
	  Sptr pDocElem = makeDocElement(pDb);
	  XmlDocument xDoc(pDocElem);
	  
	  for (auto item : dbStore()) //creating tags
	  {
		  Sptr pRecord = makeTaggedElement("dbRecord");
		  pDb->addChild(pRecord); // <dbRecord> </dbRecord> added 

		  Sptr pKey = makeTaggedElement("key", item.first);
		  pRecord->addChild(pKey); // <key> </key> added

		  Sptr pValue = makeTaggedElement("value");
		  pRecord->addChild(pValue); // <value> </value> added
		  Sptr pName = makeTaggedElement("name", item.second.name());
		  pValue->addChild(pName); // <name> </name> added
		  Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
		  pValue->addChild(pDescrip); // <description> </description> added
		  Sptr pDateTime = makeTaggedElement("datetime", std::string(item.second.dateTime()));
		  pValue->addChild(pDateTime); // <datetime> </datetime> added
		  Sptr pChildren = makeTaggedElement("children");
		  pValue->addChild(pChildren); // <children> </children> added
		  if (item.second.children().size() != 0)
		  {
			  for (std::size_t i = 0; i < item.second.children().size(); ++i)
			  {
				  Sptr pChild = makeTaggedElement("child", item.second.children()[i]);
				  pChildren->addChild(pChild); // <child> </child> added
			  }
		  }
		  Sptr pPayload = makeTaggedElement("payload", item.second.payLoad());
		  pValue->addChild(pPayload); // <payload> </payload> added
		  
	  }
	  std::string Xml = xDoc.toString();
	  std::cout << Xml << std::endl;
	  
	  std::cout << "\n  Saving XML Representation of the Database to a file.";
	  std::ofstream outfile(file_name);
	  outfile << Xml << std::endl;
	  outfile.close();
	  putLine();
  }



  //----< to delete Key from db >----------------------------------
  
  template<typename T>
  typename DbCore<T>::DbStore& DbCore<T>::deleteKey(Key key)
  {
	  if (!contains(key))
	  {
		  std::cout << "\n  Key already not in Database";
		  return dbStore_;
	  }
	  dbStore_.erase(key);
	  return dbStore_;
  }

  //----< check does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  
  //----< returns current key set for db >-----------------------------

  template<typename T>
  typename DbCore<T>::Keys DbCore<T>::keys()
  {
    DbCore<T>::Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  *  - In order to create a key-value pair in the database like this:
  *      db[newKey] = newDbElement
  *    we need to index with the new key and assign a default element. 
  *    That will be replaced by newDbElement when we return from operator[]
  *  - However, if we want to index without creating new elements, we need
  *    to throw an exception if the key does not exist in the database.
  *  - The behavior we get is determined by doThrow_.  If false we create
  *    a new element, if true, we throw. Creating new elements is the default
  *    behavior.
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      if (doThrow_)
        throw(std::exception("key does not exist in db"));
      else
        return (dbStore_[key] = DbElement<T>()); // () does the trick
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      throw(std::exception("key does not exist in db"));
    }
    return dbStore_[key];
  }
  
  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(10) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(25) << std::left << "Payload";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(25) << std::left << "-----------------------";
  }
  
  //----< display DbElements along with Payload vector>-----------------------------------------

  template<typename T>
  void showElemwithPayload(const DbElement<T>& el, std::ostream& out = std::cout)
  {
	  out << "\n  ";
	  out << std::setw(26) << std::left << std::string(el.dateTime());
	  out << std::setw(10) << std::left << el.name();
	  out << std::setw(25) << std::left << el.descrip();
	  out << std::setw(25) << std::left << el.payLoad();
	  typename DbElement<T>::Children children = el.children();
	  if (children.size() > 0)
	  {
		  out << "\n    child keys: ";
		  for (auto key : children)
		  {
			  out << " " << key;
		  }
	  }
	  if (el.payLoadV().size() > 0)
	  {
		  out << "\n    PAYLOAD VECTOR contains : ";
		  for (auto key : el.payLoadV())
		  {
			  out << " " << key;
		  }
	  }
	  else { out << "\n    PAYLOAD VECTOR is empty "; }
  }

 
  //----< display DbElements >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(10) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
    out << std::setw(25) << std::left << el.payLoad();
    typename DbElement<T>::Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }
  }

  //----< display all records in database >----------------------------

  template<typename T>
  void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showElem(item.second, out);
    }
  }
}
