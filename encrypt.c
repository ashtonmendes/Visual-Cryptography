/*
 * encrypt.c
 *
 *  Created on: Feb 6, 2015
 *      Author: ashton
 */


#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "stream.h"

void encryptImage(char* passphrase, char* out, char* fileName)
{
	FILE *file;
	if(strlen(fileName) == 0) //read from stdin
	{
		file = stdin;
	}
	else
	{
		file = fopen(fileName, "rb"); //read in binary mode.
		if(file == NULL)
		{
			fprintf(stderr, "Unable to open the file.");
			return;
		}
		//Was able to initialize pointer to file successfully.
	}

	int bytesRead = 0;
	int totalBytesRead = 0;
	//Read image into buffer.
	char *buffer = malloc(1000*sizeof(char));
	char *bufferptr = buffer;
	int buffsize = 1000;
	int buffindex = 0;

	char *buff = malloc(1000*sizeof(char));
	char *buffptr = buff;
	do
	{
		buff = buffptr;
		memset(buffptr, 0, 1000);
		bytesRead = fread(buff, sizeof(char), 1000, file);

		if(totalBytesRead+bytesRead>=buffsize)
		{
			buffer = realloc(bufferptr, (buffsize+1000)*sizeof(char));
			bufferptr = buffer;
			buffer += totalBytesRead;
			buffsize+=1000;
		}

		int count=0;
		while(count<bytesRead)
		{
			*buffer = *buff;
			buffer++;
			buffindex++;
			buff++;
			count++;
		}

		totalBytesRead += bytesRead;
	}
	while(bytesRead > 0);
	free(buffptr);

	buffer -= totalBytesRead; //go back to beginning

	//Check for invalid file
	if(totalBytesRead < 3)
	{
		fprintf(stderr, "Not a valid .pbm file.\n");
		return;
	}

	//1st 3 bytes is the file format.
	if(buffer[0] != 'P' || buffer[1] != '4' || buffer[2] != '\n')
	{
		fprintf(stderr, "Not a valid .pbm file.\n");
		return;
	}

	//find width and height.
	int width = 0, height = 0;
	int i = 3, j=0;
	char *temp = malloc(4*sizeof(char));
	while(buffer[i]!=' ')
		temp[j++] = buffer[i++];
	temp[j] = '\0';
	width = atoi(temp); //found width
	j=0;
	while(buffer[i]!='\n')
			temp[j++] = buffer[i++];
	temp[j] = '\0';
	height = atoi(temp); //found height
	free(temp);

	//If width is a multiple of 8, num of bytes per row will be width/8, else width/8+1
	int numOfBytesPerRow = (width%8==0)?(width/8):(width/8)+1;
	int row = 0;
	int col = 0;

	//Fill the image row and column-wise.
	char image[height][numOfBytesPerRow];
	i++; //i was pointing to \n at the end of the <width> <height>\n line
	for(row=0; row<height; row++)
	{
		for(col = 0; col<numOfBytesPerRow; col++)
		{
			image[row][col] = buffer[i++];
		}
	}
	free(buffer);

	/*//Print the image
	for(row=0; row<height; row++)
	{
		for(col = 0; col<numOfBytesPerRow; col++)
		{
			printf("%02x ", (unsigned char)image[row][col]);
		}
		printf("\n");
	}*/

	//get the key stream.
	int totalBits = height*width;
	int totalBytes = (totalBits%8==0)?(totalBits/8):(totalBits/8+1); //total number of bytes of key stream required.
	unsigned char *keystream = malloc(sizeof(unsigned char)*totalBytes);
	generateKeyStreamForEncrypt(passphrase, totalBytes, keystream);

	//Convert keystream to stream of 0s and 1s.
	int key[totalBytes*8];
	int index1 = 0; //index to keystream
	int index2 = 0; //index to key
	while(index1 < totalBytes)
	{
		unsigned char byte = keystream[index1];
		int i=0;
		for(i=0; i<8; i++)
			key[index2++] = ( (byte<<i) & (1<<7) ) >> 7;
		index1++;
	}
	free(keystream);

	/*//Print the keystream
	int z=0;
	for(z=0;z<totalBytes; z++)
		if(z%8 == 0)
			printf("\n%02x", keystream[z]);
		else
			printf("%02x", keystream[z]);*/

	/*//Printing the keystream bit by bit
	int z = 0;
	for(z=0; z<totalBytes*8; z++)
		if(z%8 == 0)
			printf(" %d", key[z]);
		else
			printf("%d", key[z]);*/


	//now start encrypting. share1 and share2 contain 1's and 0's.
	int sharesHeight = height*2;
	int sharesWidth = width*2 + ((((width*2)%8)!=0)?(8-(width*2)%8):0);
	int share1[height*2][sharesWidth]; //each bit
	int share2[height*2][sharesWidth]; //each bit

	int h=0;int w=0;
	for(h=0;h<sharesHeight;h++) //initialize the bits in the shares to 0
		for(w=0;w<sharesWidth;w++)
		{
			share1[h][w] = 0;
			share2[h][w] = 0;
		}
	int keyBitsUsed = 0;
	for(row=0; row<height; row++)
	{
		int bitsProcessedInRow = 0;
		for(col=0; col<numOfBytesPerRow; col++)
		{
			unsigned char byte = image[row][col];
			//Get individual bits from each byte.
			int extractedBits = 0;
			int bit = 0;
			//printf("%02x: ", byte);
			while(extractedBits < 8 && bitsProcessedInRow < width)
			{
				//printf("%d(%d)  ", ( (byte<<extractedBits) & (1<<7) ) >> 7, key[keyBitsUsed]);
				//printf("%02d,%02d ", row*2, 2*bitsProcessedInRow);
				//printf("%d", key[keyBitsUsed]);
				bit = ( (byte<<extractedBits) & (1<<7) ) >> 7;
				int rowInShare = row*2;
				int colInShare = 2*bitsProcessedInRow;
				if(bit == 0) //white pixel
				{
					if(key[keyBitsUsed] == 0) //key also 0
					{
						share1[rowInShare][colInShare] = share2[rowInShare][colInShare] = 1;
						share1[rowInShare+1][colInShare+1] = share2[rowInShare+1][colInShare+1] = 1;

						share1[rowInShare][colInShare+1] = share2[rowInShare][colInShare+1] = 0;
						share1[rowInShare+1][colInShare] = share2[rowInShare+1][colInShare] = 0;
					}
					else //key is 1
					{
						share1[rowInShare][colInShare] = share2[rowInShare][colInShare] = 0;
						share1[rowInShare+1][colInShare+1] = share2[rowInShare+1][colInShare+1] = 0;

						share1[rowInShare][colInShare+1] = share2[rowInShare][colInShare+1] = 1;
						share1[rowInShare+1][colInShare] = share2[rowInShare+1][colInShare] = 1;
					}
				}
				else //black pixel
				{
					if(key[keyBitsUsed] == 0) //key also 0
					{
						share1[rowInShare][colInShare] = share1[rowInShare+1][colInShare+1] = 1;
						share1[rowInShare+1][colInShare] = share1[rowInShare][colInShare+1] = 0;

						share2[rowInShare][colInShare] = share2[rowInShare+1][colInShare+1] = 0;
						share2[rowInShare+1][colInShare] = share2[rowInShare][colInShare+1] = 1;
					}
					else //key is 1
					{
						share1[rowInShare][colInShare] = share1[rowInShare+1][colInShare+1] = 0;
						share1[rowInShare+1][colInShare] = share1[rowInShare][colInShare+1] = 1;

						share2[rowInShare][colInShare] = share2[rowInShare+1][colInShare+1] = 1;
						share2[rowInShare+1][colInShare] = share2[rowInShare][colInShare+1] = 0;
					}
				}

				keyBitsUsed++; //keeping track of number of bits used from keystream.
				extractedBits++; //keeping track of number of bits extracted from current byte.
				bitsProcessedInRow++; //keeping track of bits processed (padding at end should be skipped.)
			}
			//printf("   ");
		}
		//printf("\n");
	}


	//Print shares
	/*int x=0, y=0;
	printf("share1:\n");
	for(x=0; x<height*2; x++)
	{
		for(y=0; y<width*2+(8-(width*2)%8); y++)
		{
			if(y%8 == 0)
					printf("   %d", share1[x][y]);
			else
					printf("%d", share1[x][y]);

			if(y%2!=0)
				printf(" ");
		}
		printf("\n");
		if(x%2!=0)
			printf("\n");
	}*/
	/*x=0, y=0;
	printf("share2:\n");
	for(x=0; x<height*2; x++)
	{
		for(y=0; y<width*2+(8-(width*2)%8); y++)
		{
			if(y%8 == 0)
					printf("   %d", share2[x][y]);
			else
					printf("%d", share2[x][y]);

			if(y%2!=0)
				printf(" ");
		}
		printf("\n");
		if(x%2!=0)
			printf("\n");
	}*/

	//Now write the image into .1.pbm and .2.pbm files
	FILE *share1File, *share2File;
	char share1Filename[strlen(out)+strlen(".1.pbm")+1];
	char share2Filename[strlen(out)+strlen(".2.pbm")+1];
	strcpy(share1Filename, out);
	strcpy(share2Filename, out);
	strcat(share1Filename, ".1.pbm");
	strcat(share2Filename, ".2.pbm");
	share1File = fopen(share1Filename, "w");
	share2File = fopen(share2Filename, "w");

	//write the 1st line.
	fprintf(share1File, "P4\n");
	fprintf(share2File, "P4\n");

	//width and height
	fprintf(share1File, "%d %d\n", width*2, height*2);
	fprintf(share2File, "%d %d\n", width*2, height*2);

	//go on accumulating 8 bits and converting them into byte.
	for(row=0; row<sharesHeight; row++)
	{
		for(col=0; col<sharesWidth; col+=8)
		{
			//share 1.
			int bitNumber = 0;
			unsigned char byte = 0;
			while(bitNumber <= 7)
			{
				byte = byte | (share1[row][col+bitNumber] << (7-bitNumber));
				bitNumber++;
			}
			fprintf(share1File, "%c", byte);

			//share 2.
			bitNumber = 0;
			byte = 0;
			while(bitNumber <= 7)
			{
				byte = byte | (share2[row][col+bitNumber] << (7-bitNumber));
				bitNumber++;
			}
			fprintf(share2File, "%c", byte);
		}
	}

	fclose(share1File);
	fclose(share2File);
}
