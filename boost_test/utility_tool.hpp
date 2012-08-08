#ifndef _UTILITY_TOOL
#define _UTILITY_TOOL

#include "CTestBoostBase.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <bitset>
using namespace std;

#include "boost/typeof/typeof.hpp"
#include "boost/optional.hpp"
#include "boost/assign.hpp"
#include "boost/swap.hpp"
#include "boost/serialization/singleton.hpp"
#include "boost/logic/tribool.hpp"
#include "boost/logic/tribool_io.hpp"
#include "uuids.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/uuid/sha1.hpp"
#include "boost/utility/binary.hpp"
#include "boost/current_function.hpp"

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

namespace ex
{
	BOOST_TRIBOOL_THIRD_STATE(unknown)
	struct demo_class{int a, b;};
}
BOOST_TYPEOF_REGISTER_TYPE(ex::demo_class)
using boost::serialization::singleton;

class SqlDB : public boost::serialization::singleton<SqlDB>
{
public:
	void connect()
	{
		cout << "connect db" << endl;
	}
	void exec(const char* sqlstr)
	{
		cout << "exec insert/update/delete:" << sqlstr << endl;
	}
	void query(const char* sqlstr)
	{
		cout << "exec select " << sqlstr << endl;
	}
};

class CTestBoostUtilityTool : public CTestBoostBase
{
public:
	CTestBoostUtilityTool()
	{
		std::cout << "Hello, Boost Utility tool" << std::endl;
	}
	virtual void Run()
	{
		TestTypeof();
		TestTypeofRegister();
		TestOptional_1();
		TestOptional_2();
		TestMakeOptional();
		TestAssign1();
		TestAssign2();
		TestAssign3();
		TestAssign4();
		TestAssign5();
		TestSwap();
		TestSingleton();
		TestTribool();

		TestUuids();
		TestSHA1();
		TestBoostBinary();
		TestBoostCurrentFunction();
	}

#ifdef UNIT_TESTING
public:
#else
private:
#endif
	vector<std::string> func()
	{
		vector<std::string> v(10);
		return v;
	}
	void TestTypeof()
	{
		BOOST_TYPEOF(2.0*3) x = 2.0 * 3;
		BOOST_AUTO(y, 3+2);

		BOOST_AUTO(&A, new double[20]);
		BOOST_AUTO(p, make_pair(1, "string"));

		BOOST_AUTO(v, func());
	}

	void TestTypeofRegister()
	{
		BOOST_AUTO(x, make_pair('test', ex::demo_class()));
		cout << typeid(x).name() << endl;
		x.second.a = 10;
		x.second.b = 20;
		cout << x.second.a << x.second.b << endl;
	}

	void TestOptional_1()
	{
		boost::optional<int> op0;
		boost::optional<int> op1(boost::none);
		assert(!op0);
		assert(op0 == op1);
		assert(op1.get_value_or(253) == 253);

		boost::optional<std::string> ops("test");
		cout << *ops << endl;

		std::vector<int> v(10);
		boost::optional<std::vector<int>& > opv(v);
		assert(opv);
		
		opv->push_back(5);
		assert(opv->size() == 11);
		opv = boost::none;
		assert(!opv);
	}

	boost::optional<double> calc(int x)
	{
		return boost::optional<double>(x != 0, 1.0/x);
	}
	boost::optional<double> sqrt_op(double x)
	{
		return boost::optional<double>(x > 0, sqrt(x) );
	}
	void TestOptional_2()
	{
		boost::optional<double> d = calc(10);
		if(d)
		{
			cout << "*d = " << *d << endl; 
		}
		d = sqrt_op(-10);
		if(!d)
		{
			cout << "no result" << endl;
		}
	}

	void TestMakeOptional()
	{
		BOOST_AUTO(x, boost::make_optional(5));
		assert(*x == 5);

		BOOST_AUTO(y, boost::make_optional<double>((*x > 10), 1.0));
		assert(!y);
		BOOST_AUTO(z, boost::make_optional<double>((*x < 10), 1.0));
		if(z)
		{
			cout << typeid(z).name() << endl;
			cout << "z = " << z << endl;
			cout << "*z = " << *z << endl;
		}
	}

	void TestAssign1()
	{
		using namespace boost::assign;
		std::vector<int> v;
		v += 1,2,3,4,5, 7*6;
		for(std::vector<int>::iterator pos = v.begin(); pos != v.end(); ++pos)
		{
			cout << *pos << " " ;
		}
		cout << endl;

		std::set<std::string> s;
		s += "cpp", "java", "c#", "python";

		std::map<int, std::string> m;
		m += std::make_pair(1, "one"), std::make_pair(2, "two");
	}

	void TestAssign2()
	{
		using namespace boost::assign;
		std::vector<int> v;
		push_back(v)(1)(2)(3)(4)(5);

		std::list<std::string> l;
		push_front(l)("cpp")("java")("c#")("python")("perl");
		push_back(l), "ruby", "php", "jsp", "asp";
		for(std::list<std::string>::iterator pos = l.begin(); pos != l.end(); ++pos )
		{
			cout << *pos << " ";
		}
		cout << endl;
		std::set<double> s;
		insert(s)(3.14)(0.888)(1.222);
		
		std::map<int, std::string> m;
		insert(m)(1, "one")(2, "two");
	}
	void TestAssign3()
	{
		using namespace boost::assign;
		vector<int> v = list_of(1)(2)(3)(4)(5);
		vector<int> v2 = (list_of(5),6,7,8,9);

		map<int, string> m = map_list_of(1, "one")(2, "tw0");
	}
	//减少重复输入repeat repeat_fun range
	void TestAssign4()
	{
		using namespace boost::assign;
		vector<int> v = (list_of(1).repeat(3,2)(3)(4),5);
		for(vector<int>::const_iterator pos = v.begin(); pos != v.end(); ++pos)
		{
			cout << *pos << " ";
		}
		cout << endl;
		std::multiset<int> ms;
		insert(ms).repeat_fun(5, &rand).repeat(2,1), 10;

		deque<int> d;
		push_front(d).range(v.begin(), v.begin()+5);
	}
	//引用初始化列表
	void TestAssign5()
	{
		using namespace boost::assign;
		int a = 1, b = 2, c = 3;
		vector<int> v = boost::assign::ref_list_of<3>(a)(b)(c);
		assert(v.size() == 3);
	}

	void TestSwap()
	{
		int a1[10];
		int a2[10];
		std::fill_n(a1, 10, 5);
		std::fill_n(a2, 10, 20);

		boost::swap(a1, a2);
	}

	
	void TestSingleton()
	{
/*		using boost::serialization::singleton;
		//typedef singleton<SqlDB_t> SqlDB;
		cout << "main() start" << endl;
		
		SqlDB::get_const_instance().connect();
		*/
	}

	
	void TestTribool()
	{
		boost::tribool tb(true);
		boost::tribool tb2(!tb);
		if(tb)
		{
			cout << "True" << endl;
		}
		tb2 = boost::indeterminate;
		assert(indeterminate(tb2));
		cout << tb2 << endl;
		if(tb2 == boost::indeterminate)
		{
			cout << "indeterminate" << endl;
		}
		cout << tb2 << endl;
		cout << (tb2 || true) << endl;
		cout << (tb2 && false) << endl;

		boost::tribool tb3(ex::unknown);
		cout << tb3 << endl;
	}

	void TestUuids()
	{
		boost::uuids::uuid u;
		assert(uuid::static_size() == 16);
		assert(u.size() == 16);
		std::vector<unsigned char> v(16, 7);
		std::copy(v.begin(), v.end(), u.begin());
		assert(u.data[0] == u.data[1] && u.data[15] == 7);

		cout << u << endl;
		std::fill_n(u.data + 10, 6, 8);

		cout << u << endl;

		//nil生成器
		u = boost::uuids::nil_generator()();
		assert(u.is_nil());

		u = boost::uuids::nil_uuid();
		assert(u.is_nil());

		//字符串生成器
		boost::uuids::string_generator sgen;
		uuid u1 = sgen("0123456789abcdef0123456789abcdef");
		cout << u1 << endl;
		uuid u2 = sgen("01234567-89ab-cdef-0123-456789abcdef");
		cout << u2 << endl;
		uuid u3 = sgen(L"{01234567-89ab-cdef-0123-456789abcdef}");
		cout << u3 << endl;

		//名字生成器
		uuid www_xxx_com = string_generator()
			("{01234567-89ab-cdef-0123-456789abcdef}");
		name_generator ngen(www_xxx_com);

		uuid u4 = ngen("mario");
		assert(!u4.is_nil() && u4.version() == uuid::version_name_based_sha1);
		uuid u5 = ngen("link");
		cout << u5 << endl;

		//随机生成器
		boost::uuids::random_generator rgen;
		uuid u6 = rgen();
		assert(u6.version() == uuid::version_random_number_based);
		cout <<u6 << endl;

		//与字符串间的转换
		string str = boost::lexical_cast<string>(u6);
		cout << "after boost::lexical_cast<string>:" << str << endl;
	}

	//密码学算法， 可以将任意长度文本压缩成只有20个字节（160位）的独一无二的摘要,
	//被广泛应用于防篡改、身份鉴定等安全认证领域
	//uuid的名字生成器用到了sha1算法
	void TestSHA1()
	{
		boost::uuids::detail::sha1 sha;
		char *szMsg = "a short message";
		sha.process_byte(0x10);
		sha.process_bytes(szMsg, strlen(szMsg));
		sha.process_block(szMsg, szMsg + strlen(szMsg));

		unsigned int digest[5];
		sha.get_digest(digest);
		for(int i = 0; i < 5; ++i)
		{
			cout << hex << digest[i];
		}
		cout << endl;
	}

	void TestBoostBinary()
	{
		cout << hex << showbase ;
		cout <<	BOOST_BINARY(0110) << endl;
		cout << BOOST_BINARY(0110 1101) << endl;
		cout << BOOST_BINARY(10110110 1101) << endl;
		cout << bitset<5>(BOOST_BINARY(0110)) << endl;


		cout << BOOST_BINARY_UL(101 1001) << endl;
		long long x = BOOST_BINARY_LL(1101);
		cout << x << endl;
	}

	double test_func()
	{
		cout << BOOST_CURRENT_FUNCTION << endl;
		return 0.9;
	}
	void TestBoostCurrentFunction()
	{
		test_func();
		cout << __FUNCTION__ << endl;
		cout << BOOST_CURRENT_FUNCTION << endl;
		
	}
};
#endif //_UTILITY_TOOL