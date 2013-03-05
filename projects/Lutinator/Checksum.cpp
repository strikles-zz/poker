//
// MD5 Hashing Example - Using Windows Crypto API
// 
// by Napalm @ NetCore2K
//
#include <windows.h>
#include <wincrypt.h>
#include <string.h>
#include <stdio.h>

#include "Checksum.h"

HCRYPTPROV hCryptProv;

BOOL CryptStartup()
{
	if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == 0)
	{
		if(GetLastError() == NTE_EXISTS)
		{
			if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0) == 0)
				return FALSE;
		}
		else 
			return FALSE;
	}

	return TRUE;
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

char* CalcChecksum(char* path, char* md5str)
{
	int i, j;
	FILE* fInput;
	MD5Context md5Hash;
	unsigned char bBuffer[4096];
	unsigned char b;
	char c;
	
	if (!CryptStartup())
	{
		printf("Error: Could not start crypto library.\n\n", path);
		return 0;
	}
	
	errno_t err_Input = 0;
	err_Input = fopen_s(&fInput, path, "rb");

	if (err_Input != 0)
	{
		printf("Error: Failed to open '%s'\n\n", path);
		CryptCleanup();
		return 0;
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
			//printf("%c", c);
		}
	}

	//printf("\nChecksum of '%s' is: %s\n", path, md5str);
	CryptCleanup();

	return md5str;
}