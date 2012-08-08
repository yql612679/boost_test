#ifndef _CTESTBOOSTPOOL_HPP
#define _CTESTBOOSTPOOL_HPP
#include <iostream>
#include <vector>

using namespace std;

//#include <boost/smart_ptr.hpp>
#include "CTestBoostBase.hpp"

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/pool/pool_alloc.hpp>

struct demo_class
{
public:
	int a, b, c;
	demo_class(int a_ = 1, int b_ = 2, int c_ = 3): a(a_), b(b_), c(c_)
	{}
};

 
class CTestBoostPool: public CTestBoostBase
{
public:
	void Run()
	{
		std::cout << "Hello boost pool" << std::endl;
		TestPool();
		TestObjectPoll();
		TestSingletonPool();
		TestPoolAlloc();
	}

private:
	void TestPool()
	{
		boost::pool<> pl(sizeof(int));
		int *p = (int*)pl.malloc();
		assert(pl.is_from(p));
		pl.free(p);

		for(int i = 0; i < 100; ++i)
		{
			p = (int*)pl.ordered_malloc();
			*p = i++;
			cout << "p = " << *p << endl;
		}
	}
	void TestObjectPoll()
	{
		boost::object_pool<demo_class> pl;
		demo_class *p = pl.malloc();
		assert(pl.is_from(p));

		//p指向的内存未经初始化
		assert(p->a != 1 || p->b != 2 || p->c != 3);

		p = pl.construct(7,8,9);
		assert(p->a == 7);

		boost::object_pool<std::string> pls;
		for(int i = 0; i < 10; ++i)
		{
			std::string *ps = pls.construct("hello object_pool");
			cout << *ps <<endl;
		}
	}

	typedef boost::singleton_pool<struct pool_tag, sizeof(int)> spl;
	void TestSingletonPool()
	{
		int *p = (int *)spl::malloc();
		assert(spl::is_from(p));
		spl::release_memory();
		cout << *p << endl;
	}

	void TestPoolAlloc()
	{
		//除非有特别的要求，我们应该总使用STL实现自带的内存分配器，使用pool_alloc需要经过仔细
		//的测试，以保证他与容器可以共同工作
		std::vector<int, boost::pool_allocator<int> > v;
		v.push_back(10);

		cout << v.size() << endl; 
	}
};

#endif //_CTESTBOOSTPOOL_HPP