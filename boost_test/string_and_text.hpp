#include "CTestBoostBase.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <bitset>
#include <assert.h>
using namespace std;

#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"
#include <iomanip>
#include "boost/algorithm/string.hpp"
#include "boost/tokenizer.hpp"
#include "boost/xpressive/xpressive_dynamic.hpp"
using boost::io::group;

class CTestBoostStringAndText : public CTestBoostBase
{
public:
	CTestBoostStringAndText()
	{
		std::cout << "Hello, BoostStringAndText" << std::endl;
	}
	virtual void Run()
	{
		TestLexicalCast();
		TestFormat();
		TestFormat2();
		TestStringAlgo();
		TestStringAlgo2();
		TestStringAlgo3();
		TestStringAlgo4();
		TestTokenizer();
		TestTokenizer2();
		TestXpressive();
		TestXpressiveRegexSearch();
		TestXpressiveRegexReplace();
		TestXpressiveRegexIterator();
		TestXpressiveRegexTokenIterator();
	}
private:
	template <typename T>
	bool num_vaild(const char *str)
	{
		try
		{
			boost::lexical_cast<T>(str);
			return true;
		}
		catch(boost::bad_lexical_cast&)
		{
			return false;
		}
	}
private:
	void TestLexicalCast()
	{
		int x = boost::lexical_cast<int>("100");
		long y = boost::lexical_cast<long>("2000");
		float pai = boost::lexical_cast<float>("3.14159e5");
		double e = boost::lexical_cast<double>("2.785788");
		cout << x << " "<< y << " "<< pai << " "<< e << " "<< endl;

		string str = boost::lexical_cast<string>(456789);
		cout << str <<endl;
		cout << boost::lexical_cast<string>(0.03445) << endl;
		cout << boost::lexical_cast<string>(0x10) << endl;

		///////////////////////
		assert(num_vaild<double>("3.14"));
		assert(!num_vaild<int>("3.14"));
		assert(num_vaild<int>("65535"));
	}
	void TestFormat()
	{
		cout << boost::format("%s:%d+%d=%d\n")%"sum" % 1 % 2 % (1+2);

		boost::format fmt("(%1% + %2%) * %2% = %3%\n");
		fmt %2 % 5;
		fmt % ((2+5)*5);
		cout << fmt.str() << endl;
	}
	void TestFormat2()
	{
		using namespace boost;
		format fmt("%1% %2% %3% %2% %1% \n");
		cout << fmt%1%2%3;

		fmt.bind_arg(2, 10);	//将第二个输入参数固定为数字10
		cout << fmt%1%3;		//输入其余两个参数
		fmt.clear();	//清空缓冲，但绑定的参数不变

		//在%操作符中使用group()，制定IO流操纵符第一个参数显示为八进制
		cout <<fmt % group(showbase, oct, 111) % 333;

		fmt.clear_binds();	//清除所有绑定参数

		//设置第一个格式化项， 十六进制， 宽度为8， 右对齐， 不足位用*填充
		fmt.modify_item(1, group(hex, right, showbase, setw(8), setfill('*')));
		cout << fmt % 49 % 20 % 100;
	}

	void TestStringAlgo()
	{
		string str("readme.txt");
		if(boost::ends_with(str, "txt"))
		{
			cout << boost::to_upper_copy(str) + " UPPER" << endl;
			assert(boost::ends_with(str, "txt"));
		}

		boost::replace_first(str, "readme", "followme");
		cout << str << endl;

		vector<char> v(str.begin(), str.end());
		vector<char> v2 = boost::to_upper_copy(boost::erase_first_copy(v, "txt"));
		for(int i = 0; i < v2.size(); ++i)
		{
			cout << v2[i];
		}
		cout << endl;
	}

	void TestStringAlgo2()
	{
		string str("I Don't Know.\n");
		cout << boost::to_upper_copy(str);
		cout << str;
		boost::to_lower(str);
		cout << str;

		str = "Power Bomb";
		assert(boost::iends_with(str, "bomb"));
		assert(!boost::ends_with(str, "bomb"));

		assert(boost::starts_with(str, "Pow"));
		assert(boost::contains(str, "er"));

		string str2 = boost::to_lower_copy(str);
		assert(boost::iequals(str, str2));

		string str3("power suit");
		assert(boost::ilexicographical_compare(str, str3));
		assert(boost::all(str2.substr(0,5), boost::is_lower()));
	}
	void TestStringAlgo3()
	{
		boost::format fmt("|%s|\n");

		string str = "  samus aran    ";
		cout << fmt % boost::trim_copy(str); //删除两端的空格
		cout << fmt % boost::trim_left_copy(str); //删除左端空格

		boost::trim_right(str);
		cout << fmt % str;

		string str2 = "2012 Happy new Year!!!";
		cout << fmt % boost::trim_left_copy_if(str2, boost::is_digit());
		cout << fmt % boost::trim_right_copy_if(str2, boost::is_punct());
		cout << fmt % boost::trim_copy_if(str2, boost::is_punct() || boost::is_digit() || boost::is_space());
	}

	void TestStringAlgo4()
	{
		boost::format fmt("|%s|.pos = %d\n");
		string str = "Long long ago, there was a king";

		boost::iterator_range<string::iterator> rge;	//迭代器区间
		rge = boost::find_first(str, "long");
		cout << fmt % rge % (rge.begin() - str.begin());

		rge = boost::ifind_first(str, "long");
		cout << fmt % rge % (rge.begin() - str.begin());

		rge = boost::find_nth(str, "ng", 2);
		cout << fmt % rge % (rge.begin() - str.begin());

		rge = boost::find_head(str, 4);
		cout << fmt % rge % (rge.begin() - str.begin());

		rge = boost::find_tail(str, 5);
		cout << fmt % rge % (rge.begin() - str.begin());

		rge = boost::find_first(str, "samus");
		cout << fmt % rge % (rge.begin() - str.begin());
		assert(rge.empty() && !rge );

		string str2 = "Samus beat the monster.\n";
		cout << boost::replace_first_copy(str2, "Samus", "samus");

		boost::replace_last(str2, "beat", "kill");
		cout << str2;

		boost::replace_tail(str2, 9, "ridley.\n");
		cout << str2;
		cout << boost::ierase_all_copy(str2, "samus");
		cout << boost::replace_nth_copy(str2, "l", 1, "L");
		cout << boost::erase_tail_copy(str2, 8);
		cout << endl;
	}

	void TestTokenizer()
	{
		string str("Link raise the master-sword.");

		boost::tokenizer<> tok(str);

		for(BOOST_AUTO(pos, tok.begin()); pos != tok.end(); ++pos)
		{
			cout << "[" << *pos << "]";
		}
		cout << endl;
	}
	template <typename T>
	void Print(T &tok)
	{
		for(BOOST_AUTO(pos, tok.begin()); pos != tok.end(); ++pos)
		{
			cout << "[" << *pos << "]";
		}
		cout << endl;
	}

	void TestTokenizer2()
	{
		//char_separator
		char *str = "Link ;; <master-sword> zelda";

		boost::char_separator<char> sep;
		boost::tokenizer<boost::char_separator<char>, char*> tok(str, str+strlen(str), sep);
		Print(tok);

		tok.assign(str, str + strlen(str), boost::char_separator<char>(" ;-<>", "", boost::keep_empty_tokens));
		Print(tok);


		string str2 = "id,100,name,\"mario\"";
		boost::escaped_list_separator<char> sep2;
		boost::tokenizer<boost::escaped_list_separator<char> > tok2(str2, sep2);
		Print(tok2);
	}

	void TestXpressive()
	{
		using namespace boost::xpressive; 
		cregex creg = cregex::compile("a.c");
		assert(regex_match("abc", creg));
//		assert(regex_match("a-b", creg));
		assert(!regex_match("ac", creg));
		assert(!regex_match("abd", creg));

		cregex reg = cregex::compile("\\d{6}(1|2)\\d{3}(0|1)\\d[0-3]\\d\\d{3}(X|\\d)", boost::xpressive::icase);
		assert(regex_match("330683198802104116", reg));
	}
	void  TestXpressiveRegexSearch()
	{
		using namespace boost::xpressive; 
		char *str = "there is a POWER-suit item";
		cregex reg = cregex::compile("(power)-(.{4})", icase);
		assert(regex_search(str, reg));
		
		cmatch what;
		regex_search(str, what, reg);
		assert(what.size() == 3);
		cout << what[1] << what[2] << endl;
		assert(!regex_search("error message", reg));
	}

	void  TestXpressiveRegexReplace()
	{
		using namespace boost::xpressive; 
		string str("readme.txt");
		sregex reg1 = sregex::compile("(.*)(me)");
		sregex reg2 = sregex::compile("(t)(.)(t)");

		cout << regex_replace(str, reg1, "manual") << endl;
		cout << regex_replace(str, reg1, "$1you") << endl;
		cout << regex_replace(str, reg1, "$&$&") << endl;
		cout << regex_replace(str, reg2, "$1N$3") << endl;

		str = regex_replace(str, reg2, "$1$3");
		cout << str << endl;
	}

	void TestXpressiveRegexIterator()
	{
		using namespace boost::xpressive; 
		string str("Power-bomb, power-suit, pOWer-bean all items\n");
		sregex reg = sregex::compile("power-(\\w{4})", boost::xpressive::icase);
		sregex_iterator pos(str.begin(), str.end(), reg);
		sregex_iterator end;
		while(pos != end)
		{
			cout << "[" << (*pos)[0] << "]";
			++pos;
		}
		cout << endl;
	}
	void TestXpressiveRegexTokenIterator()
	{
		using namespace boost::xpressive; 
		char *str = "*Link*||+Mario+||Zelda!!!||Metroid";
		//查找所有的单词，无视标点符号
		cregex reg = cregex::compile("\\w+");
		boost::xpressive::cregex_token_iterator pos(str, str+strlen(str), reg);
		while(pos != cregex_token_iterator())
		{
			cout << "[" << *pos << "]";
			++pos;
		}
		cout << endl;

		cregex spilt_reg = cregex::compile("\\|\\|");
		boost::xpressive::cregex_token_iterator pos2(str, str+strlen(str), spilt_reg, -1);
		while(pos2 != cregex_token_iterator())
		{
			cout << "[" << *pos2 << "]";
			++pos2;
		}
		cout << endl;
	}
};

