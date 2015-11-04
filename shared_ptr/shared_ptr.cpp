//智能指针的--shared_ptr
//20151104

shared_ptr<int> sp(new int(10));//一个指向整数的shared_ptr
assert(sp.unique())   ;//表明shared_ptr是智能指针的唯一持有
shared_ptr<int> sp1=sp;//第二个shared_ptr,拷贝构造函数
assert(sp==sp2 && sp.use_count() ==2);//两个shared_ptr 相等，指向同一个对象，并引入计数器
*sp2=100;   //使用解引用操作符修改被指对象
assert(*sp==100);//另一个shared_ptr的被修改
sp.reset();//销毁
assert(!sp);//不存在
~                      