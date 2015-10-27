#ifndef __MyCrypto_h__
#define __MyCrypto_h__



#include <string>
using namespace std;

string encrypt(const char *plaintext,char private_key[]);
string decrypt(const char *ciphertext,char private_key[]);


#endif // __MyCrypto_h__
