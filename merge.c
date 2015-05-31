/*
 * merge.c
 *
 *  Created on: Feb 6, 2015
 *      Author: ashton
 */


#include<stdlib.h>
#include<stdio.h>
#include<string.h>

char* readImage(FILE *file, char *buffer)
{
	int bytesRead = 0;
	int totalBytesRead = 0;
	//Read image into buffer.
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
		return NULL;
	}

	//1st 3 bytes is the file format.
	if(buffer[0] != 'P' || buffer[1] != '4' || buffer[2] != '\n')
	{
		return NULL;
	}

	return buffer;
}

void mergeShares(char* pbmfile1, char* pbmfile2)
{
	FILE *pbm1, *pbm2;
	pbm1 = fopen(pbmfile1, "rb");
	if(pbm1==NULL)
	{
		fprintf(stderr, "Could not open file '%s'.\n", pbmfile1);
		return;
	}
	pbm2 = fopen(pbmfile2, "rb");
	if(pbm2==NULL)
	{
		fprintf(stderr, "Could not open file '%s'.\n", pbmfile2);
		return;
	}

	//Read images
	char *buffer1 = malloc(1000*sizeof(char));
	buffer1 = readImage(pbm1, buffer1);
	char *buffer2 = malloc(1000*sizeof(char));
	buffer2 = readImage(pbm2, buffer2);

	//Check for invalid file
	if(buffer1 == NULL)
	{
		fprintf(stderr, "'%s' is not a valid .pbm file.\n", pbmfile1);
		return;
	}
	if(buffer2 == NULL)
	{
		fprintf(stderr, "'%s' is not a valid .pbm file.\n", pbmfile2);
		return;
	}

	//find width and height.
	int width1 = 0, width2 = 0, height1 = 0, height2 = 0;
	int i1 = 3, j=0;
	char *temp = malloc(4*sizeof(char));
	while(buffer1[i1]!=' ')
		temp[j++] = buffer1[i1++];
	temp[j] = '\0';
	width1 = atoi(temp); //found width
	j=0;
	while(buffer1[i1]!='\n')
			temp[j++] = buffer1[i1++];
	temp[j] = '\0';
	height1 = atoi(temp); //found height
	free(temp);

	int i2 = 3; j=0;
	temp = malloc(4*sizeof(char));
	while(buffer2[i2]!=' ')
		temp[j++] = buffer2[i2++];
	temp[j] = '\0';
	width2 = atoi(temp); //found width
	j=0;
	while(buffer2[i2]!='\n')
			temp[j++] = buffer2[i2++];
	temp[j] = '\0';
	height2 = atoi(temp); //found height
	free(temp);

	//Error check.
	if(width1!=width2 || height1!=height2)
	{
		fprintf(stderr, "dimensions of the 2 shares do not match.\n");
		return;
	}

	//Transfer from buffer to 2D char array
	int row = 0;
	int col = 0;
	int numOfBytes = 0;
	int numOfBitsPerRow = 0;
	if(width1%8 == 0)
	{
		numOfBytes = (width1/8)*height1;
		numOfBitsPerRow = width1;
	}
	else
	{
		numOfBytes = ((width1/8)+1)*height1;
		numOfBitsPerRow = width1 + (8-(width1%8));
	}
	//Fill the image row and column-wise.
	unsigned int share1[height1][numOfBitsPerRow];
	//initialize array
	for(row=0; row<height1; row++)
		for(col=0; col<numOfBitsPerRow; col++)
			share1[row][col] = 0;
	row=0;col=0;
	i1++; //i1 was pointing to \n at the end of the <width> <height>\n line
	int byteNumber = 0;
	for(byteNumber=0; byteNumber<numOfBytes; byteNumber++) //iterate over all bytes in buffer1
	{
		unsigned char byte = buffer1[i1++];
		//printf("%02x: \n", byte);
		int i=0;
		for(i=0; i<8; i++)
		{
			share1[row][col] = ( (byte<<i) & (1<<7) ) >> 7;
			//printf("(%d, %d): ", row, col);
			//printf("%d\n", ( (byte<<i) & (1<<7) ) >> 7);
			col=(col+1)%width1;
			if(col==0) //skip padding bits at the end.
				break;
		}
		if(col==0)
			row++;
	}

	row=0;
	col=0;
	numOfBitsPerRow = 0;
	if(width2%8 == 0)
	{
		numOfBytes = (width2/8)*height2;
		numOfBitsPerRow = width2;
	}
	else
	{
		numOfBytes = ((width2/8)+1)*height2;
		numOfBitsPerRow =  width2 + (8-(width2%8));
	}
	unsigned int share2[height2][numOfBitsPerRow];
	//initialize array
	for(row=0; row<height2; row++)
		for(col=0; col<numOfBitsPerRow; col++)
			share2[row][col] = 0;
	row=0;col=0;
	i2++; //i1 was pointing to \n at the end of the <width> <height>\n line
	byteNumber = 0;
	for(byteNumber=0; byteNumber<numOfBytes; byteNumber++) //iterate over all bytes in buffer1
	{
		unsigned char byte = buffer2[i2++];
		//printf("%02x ", byte);
		int i=0;
		for(i=0; i<8; i++)
		{
			share2[row][col] = ( (byte<<i) & (1<<7) ) >> 7;
			col = (col+1)%(width2);
			if(col==0) //skip padding bits at the end.
				break;
		}
		if(col==0)
			row++;
	}

	//Print shares
	/*int x=0, y=0;
	printf("share1:\n");
	for(x=0; x<height1; x++)
	{
		for(y=0; y<numOfBitsPerRow; y++)
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
	}
	x=0, y=0;
	printf("\nshare2:\n");
	for(x=0; x<height2; x++)
	{
		for(y=0; y<numOfBitsPerRow; y++)
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

	//Merge the 2 shares
	unsigned int merged[height1][numOfBitsPerRow];
	row=0;
	col=0;
	for(row=0; row<height1; row++)
		for(col=0; col<numOfBitsPerRow; col++)
			merged[row][col] = share1[row][col] | share2[row][col];

	/*//Print merged
	x=0, y=0;
	printf("\merged:\n");
	for(x=0; x<height2; x++)
	{
		for(y=0; y<numOfBitsPerRow; y++)
		{
			if(y%8 == 0)
					printf("   %d", merged[x][y]);
			else
					printf("%d", merged[x][y]);

			if(y%2!=0)
				printf(" ");
		}
		printf("\n");
		if(x%2!=0)
			printf("\n");
	}*/

	//convert merged into stream of char bytes
	//file format.
	printf("P4\n");

	//width and height
	printf("%d %d\n", width1, height1);

	//go on accumulating 8 bits and converting them into byte.
	for(row=0; row<height1; row++)
	{
		for(col=0; col<numOfBitsPerRow; col+=8)
		{
			int bitNumber = 0;
			unsigned char byte = 0;
			while(bitNumber <= 7)
			{
				byte = byte | (merged[row][col+bitNumber] << (7-bitNumber));
				bitNumber++;
			}
			printf("%c", byte);
		}
	}

	free(buffer1);
	free(buffer2);
}
