//����ָ���--shared_ptr
//20151104

shared_ptr<int> sp(new int(10));//һ��ָ��������shared_ptr
assert(sp.unique())   ;//����shared_ptr������ָ���Ψһ����
shared_ptr<int> sp1=sp;//�ڶ���shared_ptr,�������캯��
assert(sp==sp2 && sp.use_count() ==2);//����shared_ptr ��ȣ�ָ��ͬһ�����󣬲����������
*sp2=100;   //ʹ�ý����ò������޸ı�ָ����
assert(*sp==100);//��һ��shared_ptr�ı��޸�
sp.reset();//����
assert(!sp);//������
~                      