///////////////////////////////////////////////////////////////////////
// Query.cpp - Returns information from db metadata and payload      //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

/*----< test stub >--------------------------------------------------*/

#ifdef TEST_Q

#include <chrono>
#include "Query.h"
#include "../PayLoad/PayLoad.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace NoSqlDb;

//----< demo first part of requirement #3 >----------------------------

bool testR3a(DbCore<PayLoad>& db)
{
  Utilities::title("Demonstrating Requirement #3a - creating DbElement");
  std::cout << "\n  Creating a db element with key \"Fawcett\":";

  // create some demo elements and insert into db

  DbElement<PayLoad> demoElem;

  demoElem.name("Jim");
  demoElem.descrip("Instructor for CSE687");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad(PayLoad("The good news is ..."));

  if (demoElem.name() != "Jim")
    return false;
  if (demoElem.descrip() != "Instructor for CSE687")
    return false;
  if (demoElem.dateTime().now() != DateTime().now())
    return false;
  if (std::string(demoElem.payLoad()) != std::string("The good news is ..."))
    return false;

  showHeader();
  showElem(demoElem);

  db["Fawcett"] = demoElem;
  Utilities::putline();
  return true;
}

//----< demo Requirement #6 >------------------------------------------

bool testR6(DbCore<PayLoad>& db)
{
  Utilities::title("Demonstrating Requirement #6 - Metadata Queries");

  std::cout << "\n  creating default query, q1, and displaying:\n";
  Query<PayLoad> q1(db);
  q1.show();  // complete db
  Utilities::putline();

  Keys saveKeys = q1.keys();
  std::cout << "\n  select on child keys { \"Prashar\" }\n";
  Keys keys{ "Prashar" };
  Conditions<PayLoad> conds0;
  conds0.children(keys);
  q1.select(conds0).show();
  Utilities::putline();

  std::cout << "\n  select on name containing \"Ji\" and description containing \"In\" or \"TA\"\n";
  q1.from(saveKeys);
  Conditions<PayLoad> conds1;
  conds1.name("Ji");
  conds1.description("(.*In)|(.*TA)");
  q1.select(conds1).show();
  Utilities::putline();

  DateTime dtlb, dtub;       // default time is now
  std::chrono::hours day(24);
  dtlb -= (365 * day);  // one year ago
  DbElement<PayLoad>& elem = db["Fawcett"];
  DateTime newDt;
  newDt -= (2 * 365 * day);
  //newDt -= (10 * day);
  elem.dateTime(newDt);
  std::cout << "\n  select on time lowerbound = " << dtlb.time()
    << " and upperbound = " << dtub.time()
    << "\n  after changing time of \"Fawcett\" to " << newDt.time() << "\n";
  conds1.lowerBound(dtlb);
  conds1.upperBound(dtub);
  q1.select(conds1).show();
  Utilities::putline();
  return true;
}

bool testR7(DbCore<PayLoad>& db)
{
  Utilities::title("Demonstrating Requirement #7 - Compound Queries");
  Utilities::putline();

  Utilities::title("q1 selecting on name = \"Jim\"");
  Conditions<PayLoad> conds1;
  conds1.name("Jim");
  Query<PayLoad> q1(db);
  q1.select(conds1).show();
  Utilities::putline();

  Utilities::title("q2 - selecting on description containing \"TA\"");
  Conditions<PayLoad> conds2;
  conds2.description("TA");
  Query<PayLoad> q2(db);
  q2.select(conds2).show();
  Utilities::putline();

  Utilities::title("q2 = q1 or q2");
  q2.query_or(q1).show();
  Utilities::putline();

  Utilities::title("q2 = q2 and q1");
  q2.select(conds1).show();
  Utilities::putline();
  return true;
}


int main()
{
  Utilities::Title("Demonstrating Query Package");

  DbCore<PayLoad> db;

  testR3a(db);
  //DbCore<PayLoad> dbSave = db;
  testR6(db);
  testR7(db);

  std::cout << "\n\n";
  return 0;
}
#endif

