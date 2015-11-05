#include<map>
#include<iostream>
using namespace std;
//删除大于100的值
typedef struct itemstruct
{
        int a;
        char b[20];
}items;
items s[3]={{100,"what"},{20,"is"},{300,"who"}};;
int main()
{
        map<string,items> my_map;
        string str[3]={"1st","2st","3st"};
        for(int i=0;i<3;i++)
                my_map.insert(make_pair<string, items>(str[i],s[i]));
        map<string, items> ::iterator it;
        for(it=my_map.begin();it!=my_map.end();it++)
        {
                cout<<" "<<it->first<<"."<<it->second.a<<endl;
                if(it->second.a>100)
                        my_map.erase(it);
        }
        for(it=my_map.begin();it!=my_map.end();it++)
                cout<<" "<<it->first<<" . "<<it->second.a<< ".  "<<it->second.b<<endl;
        return 0;