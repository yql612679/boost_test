#include "boost/test/minimal.hpp"
#include "boost/format.hpp"
#include <iostream>
using namespace std;

int test_main(int argc, char* argv[])
{
	using namespace boost;
	format fmt("%d-%d");

	BOOST_CHECK(fmt.size() != 0);	//����format�����Ѿ���ʼ��
	fmt % 12 % 34;

	BOOST_REQUIRE(fmt.str() == "12-34");	//��֤��ʽ�����
	BOOST_ERROR("��ʾһ��������Ϣ");	//��Ӱ������ִ��

	fmt.clear();
	fmt %12;
	try
	{
		cout << fmt;
	}
	catch(...)
	{
		//BOOST_FAIL("�������󣬲�����ֹ");
	}

	cin.get();
	return 0;
}