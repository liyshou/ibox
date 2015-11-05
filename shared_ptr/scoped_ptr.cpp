//boost::scoped_ptr和std::auto_ptr非常类似，是一个简单的智能指针，它能够保证在离开作用域后对象被自动释放
#include<string>
#include<iostream>
#include<boost/scoped_ptr.hpp>

class implementation
{
public:
	~implementation(){std::cout<<"destroying implementation\n";}
	void do_something(){std::cout<<"did something\n";}
	
};
void test()
{
	boost::scoped_ptr<implementation>impl(new implementation());
	impl->do_something();
}
void main()
{
	std::cout<<"Test Begin ..\n";
	test();
	std::cout<<"Test End.\n";
}

//scoped_ptr 是通过delete 来删除管理对象，而数组对象必须通过delete[]来删除




#include<string>
#include<iostream>
#include<boost/scoped_ptr.hpp>
#include<boost/scoped_array.hpp>
#include<boost/config.hpp>
#include<boost/detalil/lightweight_test.hpp>

void test()
{
	long *lp=new long;
	boost::scoped_ptr<long>sp(lp);
	BOOST_TEST(sp.get()==lp);
	BOOST_TEST(lp=sp.get());
}