/*
 * stream.c
 *
 *  Created on: Feb 6, 2015
 *      Author: ashton
 */
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <openssl/md5.h>

void generateKeyStream(char *passphrase, double length, char *fileName)
{
	FILE *file;
	if(strlen(fileName)!=0)
	{
		file = fopen(fileName, "rb"); //read from file
		if(file==NULL)
		{
			fprintf(stderr, "Unable to open the file specified. Please check the filename.\n");
			return;
		}
	}
	else
		file = stdout;

	/*
	 * Begin code I did not write on my own.
	 * The following code was obtained from http://merlot.usc.edu/cs531-s15/homeworks/hw2/
	 * which is the website detailing the specs for HW2. In the section titled 'Key Generation for a Simple Stream Cipher',
	 * Professor Cheng has demonstrated code which generates a keystream using the MD5() function. I have gotten hold of the below
	 * code from there and have made modifications to it, so as to suit my requirements.
	 */

	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int md5BuffLength = strlen(passphrase) + 2 + MD5_DIGEST_LENGTH;
	char *s=malloc(md5BuffLength+1); //p contains \0 at the end

	int i=0;
	double remainingBytes = length;
	int bytesToBeWritten = 0;

	MD5((unsigned char*)passphrase, strlen(passphrase), md5_buf);

	for (;;)
	{
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, (unsigned char*)passphrase);

		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);

		MD5((unsigned char*)s, md5BuffLength, md5_buf);

		bytesToBeWritten = sizeof(md5_buf)/2; //8
		if(remainingBytes - bytesToBeWritten < 0)
			bytesToBeWritten = remainingBytes;

		fwrite(md5_buf, 1, bytesToBeWritten, file);

		remainingBytes = remainingBytes - bytesToBeWritten;

		if (++i == 100) i = 0;

		if(remainingBytes == 0) break;
	}

	free(s);

	/*
	 * End code I did not write.
	 */
}


void generateKeyStreamForEncrypt(char *passphrase, double length, unsigned char *keystream)
{
	/*
	 * Begin code I did not write on my own.
	 * The following code was obtained from http://merlot.usc.edu/cs531-s15/homeworks/hw2/
	 * which is the website detailing the specs for HW2. In the section titled 'Key Generation for a Simple Stream Cipher',
	 * Professor Cheng has demonstrated code which generates a keystream using the MD5() function. I have gotten hold of the below
	 * code from there and have made modifications to it, so as to suit my requirements.
	 */


	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int md5BuffLength = strlen(passphrase) + 2 + MD5_DIGEST_LENGTH;
	char *s=malloc(md5BuffLength+1); //p contains \0 at the end

	int i=0;
	double remainingBytes = length;
	int bytesToBeWritten = 0;

	MD5((unsigned char*)passphrase, strlen(passphrase), md5_buf);

	for (;;)
	{
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, (unsigned char*)passphrase);

		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);

		MD5((unsigned char*)s, md5BuffLength, md5_buf);

		bytesToBeWritten = sizeof(md5_buf)/2; //8
		if(remainingBytes - bytesToBeWritten < 0)
			bytesToBeWritten = remainingBytes;

		memcpy(keystream, md5_buf, bytesToBeWritten);

		remainingBytes = remainingBytes - bytesToBeWritten;

		if (++i == 100) i = 0;

		//printf("written: %d\n", bytesWritten);

		if(remainingBytes == 0) break;

		keystream+=bytesToBeWritten;
	}

	free(s);

	/*
	 * End code I did not write.
	 */
}

