/*
 * decrypt.c
 *
 *  Created on: Feb 6, 2015
 *      Author: ashton
 */

#include<stdlib.h>
#include<stdio.h>
#include "merge.h"

void decryptImage(char* pbmfile)
{
	FILE *mergedFile;

	if(pbmfile==NULL)
		mergedFile = stdin;
	else
	{
		mergedFile = fopen(pbmfile, "rb");

		if(mergedFile == NULL)
		{
			fprintf(stderr, "Unable to open the file '%s'.\n", pbmfile);
			return;
		}
	}

	//Read the merged image.
	char *buffer = malloc(1000*sizeof(char));
	buffer = readImage(mergedFile, buffer);

	//Check for errors
	if(buffer == NULL)
	{
		fprintf(stderr, "'%s' is not a valid pbm file.\n", pbmfile);
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

	//Transfer from buffer to 2D char array
	int row = 0;
	int col = 0;
	int numOfBytes = 0;
	int numOfBitsPerRow = 0;
	if(width%8 == 0)
	{
		numOfBytes = (width/8)*height;
		numOfBitsPerRow = width;
	}
	else
	{
		numOfBytes = ((width/8)+1)*height;
		numOfBitsPerRow = width + (8-(width%8));
	}
	//Fill the image row and column-wise.
	unsigned int mergedImg[height][numOfBitsPerRow];
	//initialize array
	for(row=0; row<height; row++)
		for(col=0; col<numOfBitsPerRow; col++)
			mergedImg[row][col] = 0;
	row=0;col=0;
	i++; //i was pointing to \n at the end of the <width> <height>\n line
	int byteNumber = 0;
	for(byteNumber=0; byteNumber<numOfBytes; byteNumber++) //iterate over all bytes in buffer1
	{
		unsigned char byte = buffer[i++];
		int i=0;
		for(i=0; i<8; i++)
		{
			mergedImg[row][col] = ( (byte<<i) & (1<<7) ) >> 7;
			col=(col+1)%width;
			if(col==0) //skip padding bits at the end.
				break;
		}
		if(col==0)
			row++;
	}

	/*//Print merged image
	int x=0, y=0;
	printf("merged image:\n");
	for(x=0; x<height; x++)
	{
		for(y=0; y<numOfBitsPerRow; y++)
		{
			if(y%8 == 0)
					printf("   %d", mergedImg[x][y]);
			else
					printf("%d", mergedImg[x][y]);

			if(y%2!=0)
				printf(" ");
		}
		printf("\n");
		if(x%2!=0)
			printf("\n");
	}*/

	//Create original image from merged image.
	int orgImgWidth = 0;

	if(((width/2)%8) == 0)
	{
		orgImgWidth = width/2;
	}
	else
	{
		orgImgWidth = (width/2) + (8-((width/2)%8));
	}
	unsigned int originalImg[height/2][orgImgWidth];
	//initialize the array
	row=0;
	col=0;
	for(row=0; row<height/2; row++)
		for(col=0; col<orgImgWidth; col++)
			originalImg[row][col] = 0;
	row=0;
	col=0;

	for(row=0; row<height-1; row+=2)
		for(col=0; col<numOfBitsPerRow-1; col+=2)
		{
			if(mergedImg[row][col] == 1 && mergedImg[row][col+1] == 1 && mergedImg[row+1][col] == 1 && mergedImg[row+1][col+1] == 1) //indicates black pixel in original image.
				originalImg[row/2][col/2] = 1;

			else //white pixel
				originalImg[row/2][col/2] = 0;
		}

	//Print original image
	/*printf("original image:\n");
	for(x=0; x<height/2; x++)
	{
		for(y=0; y<orgImgWidth; y++)
		{
			if(y%8 == 0)
				printf("  %d", originalImg[x][y]);
			else
				printf("%d", originalImg[x][y]);
		}
		printf("\n");
	}*/

	//file format.
	printf("P4\n");

	//width and height
	printf("%d %d\n", width/2, height/2);

	//go on accumulating 8 bits and converting them into byte.
	for(row=0; row<height/2; row++)
	{
		for(col=0; col<orgImgWidth; col+=8)
		{
			int bitNumber = 0;
			unsigned char byte = 0;
			while(bitNumber <= 7)
			{
				byte = byte | (originalImg[row][col+bitNumber] << (7-bitNumber));
				bitNumber++;
			}
			printf("%c", byte);
		}
	}


	free(buffer);
}














