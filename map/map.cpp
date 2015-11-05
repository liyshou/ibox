 #include<map>
#include<iostream>
using namespace std;
//map的swap交换的是容器，而不是元素
int main()
{
	map <int,int> m1,m2,m3;
	map <int ,int>::iterator m1_Iter;
	m1.insert(pair<int,int> (1,10) );
	m1.insert(pair<int,int> (1,20) );
	m2.insert (pair<int,int> (10,100));
	m2.insert (pair<int,int> (20,200));
	m3.insert (pair<int,int> (30,300));
	cout<<"the original map m1 is:";
	for(m1_Iter=m1.begin();m1_Iter !=m1.end();m1_Iter++)
		cout<<" "<<m1_Iter->second;
		cout<< "."<<endl;
	m1.swap(m2);
	cout<<"After sqapping with m2,map m1 is:";
	for(m1_Iter=m2.begin();m1_Iter !=m2.end();m1_Iter++)
		cout<<" "<<m1_Iter->second;
		cout<< "."<<endl;
	swap(m1,m3);
	cout<<"after swqping with m3,map m1 is:";
	for (m1_Iter=m1.begin();m1_Iter !=m1.end();m1_Iter++)
		cout<< " "<<m1_Iter->second;
		cout<<"."<<endl;

	return 0;
}