//
// MD5 Hashing Example - Using Windows Crypto API
// 
// by Napalm @ NetCore2K
//

#include "stdafx.h"

#include <wincrypt.h>
#include <stdio.h>
#include <conio.h>

#include "Util_Checksum.h"


HCRYPTPROV hCryptProv;

bool CryptStartup()
{
	if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == 0)
	{
		if(GetLastError() == NTE_EXISTS)
		{
			if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0) == 0)
				return false;
		}
		else 
			return false;
	}

	return true;
}

void CryptCleanup()
{
	if(hCryptProv) 
		CryptReleaseContext(hCryptProv, 0);

	hCryptProv = 0;
}

void MD5Init(MD5Context *ctx)
{
	CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &ctx->hHash);
}


void MD5Update(MD5Context *ctx, unsigned char const *buf, unsigned int len)
{
	CryptHashData(ctx->hHash, buf, len, 0);
}

void MD5Final(MD5Context *ctx)
{
	DWORD dwCount = 16;
	CryptGetHashParam(ctx->hHash, HP_HASHVAL, ctx->digest, &dwCount, 0);

	if(ctx->hHash) 
		CryptDestroyHash(ctx->hHash);

	ctx->hHash = 0;
}

char* CalcChecksum(const char* path, char* md5str)
{
	int i, j;
	FILE* fInput = NULL;
	MD5Context md5Hash;
	unsigned char bBuffer[4096];
	unsigned char b;
	char c;

	if (!CryptStartup())
	{
		_cprintf("Error: Could not start crypto library.\n\n");
		return 0;
	}

	errno_t err_Input;

	if((err_Input = fopen_s(&fInput, path, "rb" )) != 0)
	{
		_cprintf("Error: Failed to open '%s'\n\n", path);
		CryptCleanup();
		return NULL;
	}

	memset(&md5Hash, 0, sizeof(MD5Context));
	MD5Init(&md5Hash);

	while(!feof(fInput))
	{
		unsigned int nCount = fread(bBuffer, sizeof(unsigned char), 4096, fInput);
		MD5Update(&md5Hash, bBuffer, nCount);
	}

	MD5Final(&md5Hash);

	fclose(fInput);

	int counter = 0;
	memset(md5str, '\0', 33);

	for(i = 0; i < 16; i++)
	{
		b = md5Hash.digest[i];

		for(j = 4; j >= 0; j -= 4)
		{
			c = ((char)(b >> j) & 0x0F);

			if(c < 10)
				c += '0';
			else 
				c = ('a' + (c - 10));

			md5str[counter++] = c;
		}
	}

	CryptCleanup();

	return md5str;
}