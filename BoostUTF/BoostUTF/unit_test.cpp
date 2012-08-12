#define BOOST_TEST_INCLUDED
#include "boost/test/unit_test.hpp"
#include "boost/smart_ptr.hpp"
#include "boost/assert.hpp"
#include "boost/assign.hpp"
using namespace boost;

#include <iostream>
using namespace std;

//ȫ�ֲ��Լо���
struct global_fixture
{
	global_fixture()
	{
		cout << ("global setup\n");
	}
	~global_fixture()
	{
		cout << ("global teardown\n");
	}
};
//����ȫ�ּо���
BOOST_GLOBAL_FIXTURE(global_fixture)

//��ʼ�����׼�s_smart_ptr
BOOST_AUTO_TEST_SUITE(s_smart_ptr)

//��������1��t_scoped_ptr
BOOST_AUTO_TEST_CASE(t_scoped_ptr)
{
	boost::scoped_ptr<int> p(new int(875));
	BOOST_CHECK(p);
	BOOST_CHECK_EQUAL(*p, 875);

	p.reset();
	BOOST_REQUIRE(p == 0);
}

//��������2��t_shared_ptr
BOOST_AUTO_TEST_CASE(t_shared_ptr)
{
	boost::shared_ptr<int> p = boost::make_shared<int>(100);
	BOOST_CHECK(p);
	BOOST_REQUIRE_EQUAL(*p, 100);
	BOOST_REQUIRE_EQUAL(p.use_count(), 1);

	boost::shared_ptr<int> p2(p);
	BOOST_REQUIRE_EQUAL(p2, p);
	BOOST_REQUIRE_EQUAL(p2.use_count(), 2);

	*p2 = 255;
	BOOST_REQUIRE_EQUAL(*p, 255);
	BOOST_REQUIRE_GT(*p, 200);
}

BOOST_AUTO_TEST_SUITE_END()

//�����׼��о���
struct assign_fixture
{
	assign_fixture()
	{
		cout <<("suit setup\n");
	}
	~assign_fixture()
	{
		cout << ("suit teardown\n");
	}
	vector<int> v;
};
/////////////////////////////////////////////////////////////
//BOOST_AUTO_TEST_SUITE(s_assign)
//�����׼�����ļо�
BOOST_FIXTURE_TEST_SUITE(s_assign, assign_fixture)

BOOST_AUTO_TEST_CASE(t_assign1)
{
	using namespace boost::assign;

	v += 1,2,3,4;
	BOOST_CHECK_EQUAL(v.size(), 4);
	BOOST_CHECK_EQUAL(v[2], 3);
}

BOOST_AUTO_TEST_CASE(t_assign2)
{
	using namespace boost::assign;
	boost::assign::push_back(v)(10)(20)(30);
	BOOST_CHECK_EQUAL(v.empty(), false);
	BOOST_CHECK_LT(v[0], v[1]);
}
BOOST_AUTO_TEST_SUITE_END()