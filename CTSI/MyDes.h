#ifndef __MyDes_h__
#define __MyDes_h__


enum {ENCRYPT,DECRYPT};

//////////////////////////////////////////////////////////////////////////

// ��/���� Type��ENCRYPT:����,DECRYPT:����
void Des_Run(char Out[8], char In[8], bool Type=ENCRYPT);
// ������Կ
void Des_SetKey(const char Key[8]);

//////////////////////////////////////////////////////////////////////////

#endif // __MyDes_h__
