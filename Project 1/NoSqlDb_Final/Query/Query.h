#pragma once
/////////////////////////////////////////////////////////////////////
// Query.h - Implements Query operations for NoSql database        //
// ver 1.2                                                         //
// Author - Devin Upreti                                           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - Conditions holds requirements for associated value types in DbCore.
*	It contains fields for regex strings of these value types and a dbElement.
*	Also, contains methods to support the checking operations.
* - Query class contains methods that support query operations based on given conditions.
*   It contains a reference to the db on which operations are to be performed
*   and a set of Keys that match the given conditions.
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.2 : 31 Jan 2018
* - added method checkdbe in Conditions to check if a dbElement satisfies all conditions 
* ver 1.1 : 29 Jan 2018
* - added Query class and its methods
* ver 1.0 : 28 Jan 2018
* - created Conditions class
*/

#include "../DbCore/DbCore.h"
#include <regex>

using namespace NoSqlDb;

//////////////////////////////////////////////////////////////////////////////////////
// Conditions Class 
// - for storing conditions associated with a query

template <typename P>
class Conditions
{
	public:
		// methods to set regex strings for associated types
		Conditions & key(const std::string key_re) { key_regex_ = key_re; return *this; }
		Conditions & descrip(const std::string des_re) { descrip_regex_ = des_re; return *this; }
		Conditions & name(const std::string name_re) { name_regex_ = name_re; return *this; }
		Conditions & datetime_begin(const DateTime dt) { datetime_begin_ = dt; return *this; }
		Conditions & datetime_end(const DateTime dt) { datetime_end_ = dt; return *this; }
		Conditions & payload(const std::string payload_re) { payload_regex_ = payload_re; return *this; }
		
		std::string & key_regex() { return key_regex_; }
		std::string & descrip_regex() { return descrip_regex_; }
		std::string & name_regex() { return name_regex_; }
		std::string & payload_regex() { return payload_regex_; }
		DateTime datetime() { return datetime_;  }
		
		//to check if a dbElement satisfies all conditions 
		bool checkdbe(const DbElement<P> dbe);
	
	private:
		std::string key_regex_ = "(.*)";
		std::string name_regex_ = "(.*)";
		std::string descrip_regex_ = "(.*)";
		std::string payload_regex_ = "(.*)";
		DbElement<P> dbe_;
		DateTime datetime_begin_ = DateTime("Mon Jan 1 13:12:39 2001"); // initialisation value. Value after which all keys are created.
		DateTime datetime_end_ = DateTime("Tues Jan 1 13:12:39 2030"); // Value before which all keys are created.
};

///////////////////////////////////////////////////////////////////////////////////////////
// Definitions for methods of Conditions Class

// ------< checks if a dbElement satisfies all conditions >------------------------------
template<typename P>
bool Conditions<P>::checkdbe(const DbElement<P> dbe)
{
	dbe_ = dbe;
	if (std::regex_match(dbe_.descrip(), std::regex(descrip_regex_)))
	{
		if (std::regex_match(dbe_.name(), std::regex(name_regex_))) 
		{
			bool check = (datetime_end_ > dbe_.dateTime()) && (datetime_begin_ < dbe.dateTime());
			if (check)
			{
				if (std::regex_match(dbe_.payLoad(), std::regex(payload_regex_)))
				{
					return true;
				}
			}
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////////////////
// Query Class
// - for passing queries on database, contains a reference to the database.

template <typename P>
class Query
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	Query(DbCore<P>& db) : db_(db), keys_(db.keys()) {}
	
	DbElement<P>& getValue(const Key& key) { return db_[key]; }
	Keys& getChildren(const Key& key) { return db_[key].children(); }
	
	Query<P>& selectKeys(std::string k_regex);
	Query<P>& selectDescription(std::string d_regex);
	Query<P>& selectDatetime(const DateTime dt_begin);
	Query<P>& selectDatetime(const DateTime dt_begin, const DateTime dt_end);

	Query<P>& select(const Conditions<P>& cond); // modifies the keys_ according to the conditions
	
	Query& from(Keys& keys) { keys_ = keys; return *this; } // for operation on keys returned by previous query
	Query& OR (Keys& keys); // for operation on union

	void show();
	Keys& keys() { return keys_; }

private:
	DbCore<P>& db_;
	Keys keys_;
};

/////////////////////////////////////////////////////////////////////////////////
//Methods for Class Query

//----< creates union of keys in Query >-----------------------------

template <typename P>
Query<P>& Query<P>::OR(Keys& keys)
{
	for (size_t i = 0; i < keys.size(); i++)
	{
		if (std::find(keys_.begin(), keys_.end(), keys[i]) == keys_.end())
		{
			keys_.push_back(keys[i]);
		}
	}
	return *this;
}


//----< shows keys in Query >-----------------------------

template <typename P>
void Query<P>::show()
{
	std::cout << "\n  Keys in Query : ";
	for (auto key : keys_)
	{
		std::cout << key << " ";
	}
}

//----< find keys that have descriptions matching a specified criteria >-----------------------------

template <typename P>
Query<P>& Query<P>::selectDescription(const std::string d_regex)
{
	std::string regex = d_regex;
	std::vector<std::string> modified_keys;
	for (size_t i = 0; i < keys_.size(); ++i)
	{
		if (std::regex_match(this->getValue(keys_[i]).descrip(), std::regex(regex))) // regex matching
		{
			modified_keys.push_back(keys_[i]);
		}
	}
	keys_ = modified_keys;
	return *this;
}


//----< find keys that match specified criteria >-----------------------------

template <typename P>
Query<P>& Query<P>::selectKeys(const std::string k_regex)
{
	std::string regex = k_regex;
	std::vector<std::string> modified_keys;
	for (size_t i = 0; i < keys_.size(); ++i)
	{
		if (std::regex_match(keys_[i], std::regex(regex) )) // regex matching
		{
			modified_keys.push_back(keys_[i]);
		}
	}
	keys_ = modified_keys;
	return *this;
}


//----< DateTime Query for comparison with specified Datetime>----------------------------------

template <typename P>
Query<P>& Query<P>::selectDatetime(const DateTime begin)
{
	std::vector<std::string> modified_keys;
	DateTime dt_begin = begin;
	DateTime dt_end = DateTime().now();
	for (size_t i = 0; i < keys_.size(); ++i)
	{
		DateTime key_dt = this->getValue(keys_[i]).dateTime();
		bool check = (dt_end > key_dt) && (dt_begin < key_dt); // condition for check
		if (check)
		{
			modified_keys.push_back(keys_[i]);
		}
	}
	keys_ = modified_keys;
	return *this;
}

//----< DateTime Query for comparison with current Datetime>----------------------------------

template <typename P>
Query<P>& Query<P>::selectDatetime(const DateTime begin, const DateTime end)
{
	std::vector<std::string> modified_keys;
	DateTime dt_begin = begin;
	DateTime dt_end = end;
	for (size_t i = 0; i < keys_.size(); ++i)
	{	
		DateTime key_dt = this->getValue(keys_[i]).dateTime();
		bool check = (dt_end > key_dt) && (dt_begin < key_dt);
		if (check)
		{
			modified_keys.push_back(keys_[i]);
		}
	}
	keys_ = modified_keys;
	return *this;
}

//----< selects keys matching specified conditions >-----------------------------

template <typename P>
Query<P>& Query<P>::select(const Conditions<P>& cond) 
{	
	Conditions<P> conditions = cond;
	std::vector<std::string> modified_keys;
	for (size_t i = 0; i < keys_.size(); ++i)
	{
		bool check = conditions.checkdbe(this->getValue(keys_[i]));
		if (check)
		{
			modified_keys.push_back(keys_[i]);
		}
	}
	keys_ = modified_keys;
	return *this;
}
