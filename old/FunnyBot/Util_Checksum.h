#pragma once
//
// MD5 Hashing Example - Using Windows Crypto API
// 
// by Napalm @ NetCore2K
//
#include <windows.h>

struct MD5Context
{
	unsigned char digest[16];
	unsigned long hHash;
};


bool CryptStartup();
void CryptCleanup();

void MD5Init(MD5Context* ctx);
void MD5Update(MD5Context* ctx, unsigned char const* buf, unsigned int len);
void MD5Final(MD5Context* ctx);

char* CalcChecksum(const char* path, char* md5str);