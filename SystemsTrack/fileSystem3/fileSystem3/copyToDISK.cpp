#include "stdafx.h"

void copy_to_disk(FILE *fp, int iter, char *cmd, struct MetaData *meta_data[], struct OtherData *other_data){
	char *fileName;
	int index = 0;
	fileName = (char *)malloc(20);
	for (int i = iter; cmd[i]; i++)
		fileName[index++] = cmd[i];
	fileName[index] = '\0';
	char *dest = (char *)malloc(BLOCK_SIZE);
	read_block(dest, 0);
	memcpy(&other_data->magicNumber, dest, 4);
	dest = dest + 4;
	for (int i = 0; i < NO_OF_FILES; i++){
		memcpy(meta_data[i], dest, sizeof(struct MetaData));
		dest = dest + sizeof(struct MetaData);
	}
	memcpy(&other_data->no_of_files, dest, 4);
	dest = dest + 4;
	memcpy(&other_data->no_of_free_blocks, dest, 4);
	dest = dest + 4;
	memcpy(other_data->bitVector, dest, 6400);
	unsigned int len = getFileSize(fileName);
	if (len == -1)
		return;
	int reqBlocks = len / BLOCK_SIZE;
	if (len%BLOCK_SIZE != 0)
		reqBlocks++;
	if (reqBlocks > 1)
		reqBlocks++;
	unsigned int *blockNum = (unsigned int *)malloc(sizeof(unsigned int)*reqBlocks);
	int count = 0;
	for (int i = 1; i < 6400; i++){
		if (count == reqBlocks)
			break;
		if (other_data->bitVector[i] == '1'){
			blockNum[count++] = i;
		}
	}
	if (count == 0){
		printf("Disk is FULL!!!\n");
		return;
	}
	for (int i = 0; i < reqBlocks; i++){
		other_data->bitVector[blockNum[i]] = '0';
	}
		


	strcpy(meta_data[other_data->no_of_files]->fileName, fileName);
	meta_data[other_data->no_of_files]->start_block = blockNum[0]*BLOCK_SIZE;
	meta_data[other_data->no_of_files]->blocks_occupied = reqBlocks;
	meta_data[other_data->no_of_files]->file_length = len;
	other_data->no_of_files++;
	other_data->no_of_free_blocks -= reqBlocks;

	char *src = (char *)malloc(BLOCK_SIZE);
	unsigned int val = 0x444E524D;
	memcpy(src, &val, 4);
	for (int index = 0; index < NO_OF_FILES; index++){
		memcpy(src + 4 + (index*sizeof(struct MetaData)), meta_data[index], sizeof(struct MetaData));
	}
	memcpy(src + sizeof(int) + (NO_OF_FILES * NO_OF_FILES), &other_data->no_of_files, sizeof(int));
	memcpy(src + sizeof(int) + (NO_OF_FILES * NO_OF_FILES) + sizeof(int), &other_data->no_of_free_blocks, sizeof(int));
	memcpy(src + sizeof(int) + (NO_OF_FILES * NO_OF_FILES) + sizeof(int) + sizeof(int), other_data->bitVector, NO_OF_DATA_BLOCKS + 4);

	FILE *fp1 = fopen(fileName, "rb+");
	if (fp1 == NULL){
		printf("Error reading File <copy_to_disk>!!\n");
		return;
	}
	write_block(src, 0);

	int start = 0;
	if (reqBlocks > 1){
		char *buf = (char *)malloc(BLOCK_SIZE);
		for (int i = 1; i < reqBlocks; i++){
			memcpy(buf + (i - 1) * 4, &blockNum[i], 4);
		}
		write_block(buf, blockNum[0]);
		start++;
	}



	char *des = (char *)malloc(BLOCK_SIZE);
	for (int i = start; i < reqBlocks; i++){
		char *des = (char *)malloc(BLOCK_SIZE);
		if (i == reqBlocks){
				fread(des, (len%BLOCK_SIZE), 1, fp1);
		}
		else{
				fread(des, BLOCK_SIZE, 1, fp1);
			
		}
		write_block(des, blockNum[i]);
	}


}

unsigned int getFileSize(char *fileName){
	FILE *fp = fopen(fileName, "rb");
	if (fp == NULL){
		printf("Error reading File <getFileSize>!!\n");
		return -1;
	}
	unsigned int len = 0;
	char ch;
	while (fread(&ch, 1, 1, fp) == 1)
		len++;
	return len;
}