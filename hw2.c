/*
 ===========================================================================================================================================================
 Name        : hw2.c
 Author      : Ashton Mendes
 Version     :
 Description : Implementation of a slightly modified version of Doug Stinson's Visual Cryptography (http://www.cacr.math.uwaterloo.ca/~dstinson/visual.html)
 ===========================================================================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "stream.h"
#include "encrypt.h"
#include "merge.h"
#include "decrypt.h"

char* getPassphrase(char* option)
{
	char *passphrase = (option+3); //skip 3 characters at the beginning

	if(passphrase[0] == '"' && passphrase[strlen(passphrase)-1] == '"') //inverted comma check
	{
		return NULL;
	}

	return passphrase;
}

double getLength(char* option)
{
	char *length = (option+3); //skip 3 characters at the beginning
	double len = strtod(length, NULL);
	return len;
}

void getOption(char* optionName, char* optionValue, char* option)
{
	while(*option!='=')
	{
		*optionName = *option;
		optionName++;
		option++;
	}

	//include = sign also in optionName
	*optionName = *option;
	*(++optionName) = '\0';
	option++;

	while(*option!='\0')
	{
		*optionValue = *option;

		if(*(option+1) == '\0')
		{
			*(optionValue+1) = '\0';
		}

		optionValue++;
		option++;
	}
}

int main(int argc, char*argv[])
{
	int numOfArgs = argc - 1;

	if(numOfArgs == 0)
	{
		fprintf(stderr, "Please enter commandline arguments.\nCorrect usage is: \nhw2 stream -p=pphrase -l=len\nhw2 encrypt -p=pphrase -out=name [pbmfile]\nhw2 merge pbmfile1 pbmfile2\nhw2 decrypt [pbmfile]");
		return EXIT_FAILURE;
	}

	//command can be either stream or encrypt or merge or decrypt
	if(strcmp(argv[1],"stream")==0)
	{
		numOfArgs--;

		if(numOfArgs < 2)
		{
			fprintf(stderr, "'stream' requires the usage of 2 options.\nCorrect usage is: \nhw2 stream -p=pphrase -l=len\n");
			return EXIT_FAILURE;
		}
		if(numOfArgs > 2)
		{
			fprintf(stderr, "'stream' only accepts 2 options.\nCorrect usage is: \nhw2 stream -p=pphrase -l=len\n");
			return EXIT_FAILURE;
		}

		//numOfArgs = 2

		//################################### GET PASSPHRASE AND LEN ##############################

		char* passphrase = malloc(100);
		double len = 0;

		char *optionName = NULL;
		char *optionValue = NULL;

		//FOR 1st and 2nd arguments.
		int i=2;
		for(i=2; i<=3; i++)
		{
			optionName = malloc(100);
			optionValue = malloc(100);

			getOption(optionName, optionValue, argv[i]);

			if(strcmp(optionName, "-p=")==0)
			{
				strcpy(passphrase, optionValue);
			}
			else if(strcmp(optionName, "-l=")==0)
			{
				len = strtod(optionValue, NULL);
				if(len <= 0)
				{
					fprintf(stderr, "len should be a number greater than 0.\n");
					return EXIT_FAILURE;
				}
			}
			else
			{
				fprintf(stderr, "'stream' used with invalid arguments.\nCorrect usage is: \nhw2 stream -p=pphrase -l=len\n");
				return EXIT_FAILURE;
			}

			free(optionName);
			free(optionValue);
		}
		//###############################################################################################################################

		//Now finally we have the passphrase and length.
		generateKeyStream(passphrase, len, "");

		free(passphrase);
	}
	else if(strcmp(argv[1],"encrypt")==0)
	{
		numOfArgs--;

		if(numOfArgs < 2)
		{
			fprintf(stderr, "'encrypt' used with too few arguments.\nCorrect usage is: \nhw2 encrypt -p=pphrase -out=name [pbmfile]\n");
			return EXIT_FAILURE;
		}
		if(numOfArgs > 3)
		{
			fprintf(stderr, "'encrypt' used with too many arguments.\nCorrect usage is: \nhw2 encrypt -p=pphrase -out=name [pbmfile]\n");
			return EXIT_FAILURE;
		}

		char* passphrase = malloc(100);
		char* out = malloc(100);
		char* pbmFilename = malloc(100);

		//################################### GET PASSPHRASE AND OUT AND PBMFILENAME ##############################

		//FOR 1st and 2nd arguments.
		int i=2;
		for(i=2; i<=3; i++)
		{
			char *optionName = malloc(100);
			char *optionValue = malloc(100);

			getOption(optionName, optionValue, argv[i]);

			if(strcmp(optionName, "-p=")==0)
			{
				strcpy(passphrase, optionValue);
			}
			else if(strcmp(optionName, "-out=")==0)
			{
				strcpy(out, optionValue);
			}
			else
			{
				fprintf(stderr, "'encrypt' used with invalid arguments.\nCorrect usage is: \nhw2 encrypt -p=pphrase -out=name [pbmfile]\n");
				return EXIT_FAILURE;
			}

			free(optionName);
			free(optionValue);
		}
		if(argv[4])
			strcpy(pbmFilename, argv[4]);
		//###############################################################################################################################


		//######### ERROR CHECKING ##########
		if(strlen(passphrase)==0)
		{
			fprintf(stderr, "'passphrase' cannot be empty.\n");
			return EXIT_FAILURE;
		}
		if(strlen(out)==0)
		{
			fprintf(stderr, "'out' cannot be empty.\n");
			return EXIT_FAILURE;
		}
		//###################################


		//Now we finally have the passphrase and out.
		if(strlen(pbmFilename) == 0)
		{
			encryptImage(passphrase, out, "");
		}
		else
		{
			encryptImage(passphrase, out, pbmFilename);
		}


		free(passphrase);
		free(out);
		free(pbmFilename);
	}
	else if(strcmp(argv[1],"merge")==0)
	{
		numOfArgs--;

		//Error checking
		if(numOfArgs < 2)
		{
			printf("'merge' used with missing arguments.\nCorrect usage is:\nhw2 merge pbmfile1 pbmfile2\n");
			return EXIT_FAILURE;
		}
		if(numOfArgs > 2)
		{
			printf("'merge' used with too many arguments.\nCorrect usage is:\nhw2 merge pbmfile1 pbmfile2\n");
			return EXIT_FAILURE;
		}

		mergeShares(argv[2], argv[3]);
	}
	else if(strcmp(argv[1],"decrypt")==0)
	{
		numOfArgs--;

		//Error checking
		if(numOfArgs > 1)
		{
			printf("'decrypt' used with too many arguments.\nCorrect usage is:\nhw2 decrypt [pbmfile]\n");
			return EXIT_FAILURE;
		}

		if(argv[2])
		{
			decryptImage(argv[2]);
		}
		else
		{
			decryptImage(NULL); //file will be read from stdin
		}
	}
	else
	{
		fprintf(stderr, "Malformed command.\nCorrect usage is: \nhw2 stream -p=pphrase -l=len\nhw2 encrypt -p=pphrase -out=name [pbmfile]\nhw2 merge pbmfile1 pbmfile2\nhw2 decrypt [pbmfile]");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
