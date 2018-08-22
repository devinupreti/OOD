#pragma once
/////////////////////////////////////////////////////////////////////
// UnitTest.h - Implements Provides ServerRepo for Testing	       //
// ver 1.0                                                         //
// Author - Devin Upreti                                           //
/////////////////////////////////////////////////////////////////////

#include "../ServerPrototype/ServerPrototype.h"

////////////////////////////////////////////////////////////////////////
// Server Repository for demonstration
class ServerRepo {
public:
	RepositoryCore& getSR() { return R_; }
private:
	static RepositoryCore R_;
};

RepositoryCore ServerRepo::R_;