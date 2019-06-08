#include "stdafx.h"

void Generate_Intermediate_Code(int iter, char *cmd, struct MetaData *meta_data[], struct OtherData *other_data){
	char *source, *destination;
	int sIndex = 0, dIndex = 0;
	source = (char *)malloc(20);
	destination = (char *)malloc(20);
	int i;
	for (i = iter; cmd[i] != ' '; i++){
		source[sIndex++] = cmd[i];
	}
	source[sIndex] = '\0';
	for (int j = i + 1; cmd[j]; j++)
		destination[dIndex++] = cmd[j];
	destination[dIndex] = '\0';

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
	unsigned int startIndx = -1;
	unsigned int length = -1;
	unsigned int ReqBlocks = -1;
	for (int i = 0; i < NO_OF_FILES; i++){
		if (comparestr(meta_data[i]->fileName, source)){
			startIndx = meta_data[i]->start_block;
			length = meta_data[i]->file_length;
			ReqBlocks = meta_data[i]->blocks_occupied;
			break;
		}
	}
	if (startIndx == -1){
		printf("Source not found!!\n");
		return;
	}
	char *file = (char *)malloc(BLOCK_SIZE * ReqBlocks);
	int fInd = 0;
	int startBlock = startIndx / BLOCK_SIZE;

	if (ReqBlocks > 1){
		char *buf = (char *)malloc(BLOCK_SIZE);
		read_block(buf, startBlock);
		for (int i = 1; i < ReqBlocks; i++){
			unsigned int num;
			memcpy(&num, buf + 4 * (i - 1), 4);
			//printf("%u\n", num);
			if (i == ReqBlocks - 1){
				char *dest = (char *)malloc(BLOCK_SIZE);
				read_block(dest, num);
				memcpy(file + fInd, dest, length); 
				fInd += length;
			}
			else{
				char *dest = (char *)malloc(BLOCK_SIZE);
				read_block(dest, num);
				memcpy(file + fInd, dest, BLOCK_SIZE);
				fInd += BLOCK_SIZE;
			}
		}
		send_data_to_parser(file, length, iter,cmd,meta_data,other_data,destination);
		return;
	}


	for (int i = 0; i < ReqBlocks; i++){
		if (i == ReqBlocks - 1){
			char *dest = (char *)malloc(BLOCK_SIZE);
			read_block(dest, startBlock + i);
			memcpy(file + fInd, dest, length);
			fInd += length;
		}
		else{
			char *dest = (char *)malloc(BLOCK_SIZE);
			read_block(dest, startBlock + i);
			memcpy(file + fInd, dest, BLOCK_SIZE);
			fInd += BLOCK_SIZE;
			}

		}
	
	send_data_to_parser(file, length, iter, cmd, meta_data, other_data,destination);

}

void send_data_to_parser(char *file, int len, int iter, char *cmd, struct MetaData *meta_data[], struct OtherData *other_data,char *destination){
	char *ls;
	ls = (char *)malloc(sizeof(50000));
	int index = 0;
	struct All_tables *all_tables;
	all_tables = (struct All_tables *)malloc(sizeof(struct All_tables));
	all_tables->symIndex = 0;
	all_tables->labelIndex = 0;
	all_tables->intermediateIndex = 0;
	all_tables->memoryIndex = 0;
	all_tables->stk = (struct stack *)malloc(sizeof(struct stack));
	all_tables->stk->top = -1;
	for (int i = 0; i<len ; i++){
		if (file[i] == '\r'){
			ls[index] = '\0';
			tokenize(ls, all_tables);
			i++;
			index = 0;
			continue;
		}
		ls[index++] = file[i];
	}
	ls[index] = '\0';
	tokenize(ls, all_tables);
	write_to_file(destination,len,all_tables,meta_data,other_data);
}