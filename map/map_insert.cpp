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
	for (Iter=my_map.begin();Iter!=my_map.end();Iter++)//����Ԫ��
		cout<<"  "<<Iter->first<<","<<Iter->second<<endl;
	int count= my_map.count("d");
	cout<<"the value of count:"<<count<<endl;
	map<string,int>::iterator it=my_map.find("d");//����
	if(it!=my_map.end())
	{
		cout<<"has find the string"<<endl;
		it->second=10;//�޸�ֵ
	}
	for (Iter=my_map.begin();Iter!=my_map.end();Iter++)
                cout<<"  "<<Iter->first<<","<<Iter->second<<endl;
	if(count>0)//ɾ��
		my_map.erase(it);
	 for (Iter=my_map.begin();Iter!=my_map.end();Iter++)
                cout<<"  "<<Iter->first<<","<<Iter->second<<endl;	
	//����Ԫ�ص���Ŀ��my_map.size()
	//�ж��Ƿ�� ��my_map.empty()
	//������е�Ԫ�� ��my_map.clear()
	return 0;
}