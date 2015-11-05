#include<map>
#include<iostream>
using namespace std;

int main()
{
	map <string ,int> my_map;
	map<string,int>::iterator Iter;
	my_map.insert(map<string,int>::value_type("b",2));
	my_map.insert(pair<string,int>("c",3));
	my_map.insert(pair<string,int>("d",4));
	my_map.insert(make_pair<string,int>("a",5));
	for (Iter=my_map.begin();Iter!=my_map.end();Iter++)//读出元素
		cout<<"  "<<Iter->first<<","<<Iter->second<<endl;
	int count= my_map.count("d");
	cout<<"the value of count:"<<count<<endl;
	map<string,int>::iterator it=my_map.find("d");//查找
	if(it!=my_map.end())
	{
		cout<<"has find the string"<<endl;
		it->second=10;//修改值
	}
	for (Iter=my_map.begin();Iter!=my_map.end();Iter++)
                cout<<"  "<<Iter->first<<","<<Iter->second<<endl;
	if(count>0)//删除
		my_map.erase(it);
	 for (Iter=my_map.begin();Iter!=my_map.end();Iter++)
                cout<<"  "<<Iter->first<<","<<Iter->second<<endl;	
	//返回元素的数目：my_map.size()
	//判断是否空 ：my_map.empty()
	//清空所有的元素 ：my_map.clear()
	return 0;
}