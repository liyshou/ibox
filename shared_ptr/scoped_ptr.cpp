//boost::scoped_ptr��std::auto_ptr�ǳ����ƣ���һ���򵥵�����ָ�룬���ܹ���֤���뿪�����������Զ��ͷ�
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

//scoped_ptr ��ͨ��delete ��ɾ��������󣬶�����������ͨ��delete[]��ɾ��




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