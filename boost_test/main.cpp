#include "CTestBoostBase.hpp"
#include "pool.hpp"
#include "utility_tool.hpp"
#include "string_and_text.hpp"

#include "boost/smart_ptr.hpp"
#include "boost/make_shared.hpp"

#include <iostream>
using namespace std;
using namespace boost;
int main(void)
{
	boost::shared_ptr<CTestBoostBase> pb;

	//pb = boost::make_shared<CTestBoostPool>(CTestBoostPool());
	//pb = boost::make_shared<CTestBoostUtilityTool>(CTestBoostUtilityTool());

	pb = boost::make_shared<CTestBoostStringAndText>(CTestBoostStringAndText());
	pb->Run();

	cin.get();
	return 0;
}