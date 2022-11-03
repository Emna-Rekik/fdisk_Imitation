#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<inttypes.h>
#include <string.h>
#include <errno.h>

#include "headers/mbr_struct.h"
#include "headers/gpt_struct.h"
#include "headers/parsing_headers.h"
#include "headers/unit_conversion.h"
 
int main(int argc, char **argv) 
{ 
	/* 5452415020494645₁₆ en Hexa = 6075990659671082565 en Decimal. (EFI PART) */
	const long long EFI_PART = 6075990659671082565;
	
	char buf[512];
	char buf2[512];
	char buf3[64];
	
	if(argc != 2)
	{
		fprintf(stderr,"usage: %s <disk_path>\n", argv[0]);
		exit(1);
	}
	
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		printf("Error msg: Failed to create and open the file\n");
		fprintf(stderr,"error= %i\n",errno);
	}
	    
	read(fd, buf, 512);
	
	//0xaa55 = 43605 en decimal
	uint16_t * mbr_check_ptr = (uint16_t *) &buf[446+16*4];
	if(*mbr_check_ptr != 43605)
	{
		printf("This file is not an Disk file.\n");
		exit(-1);
	}
	
	// 5452415020494645₁₆ en Hexa = 6075990659671082565 en Decimal (EFI PART)
	lseek(fd,512,SEEK_SET); 
	read(fd, buf3, 64);
	//uint64_t * EFI_PART = (uint64_t *) &buf3;
	int64_t * efi_part = malloc(sizeof(uint64_t));
	*efi_part = *((uint64_t *) &buf3);
	char type;
	if (*efi_part != EFI_PART)
	{
		type = 'd';
	}
	free(efi_part);
	    
	// Make the text Bold
	printf("\x1B[1m");
	if (type == 'd')
	{
		printf("Disklabel type: dos\n\n");
	}
	else
	{
		printf("Disklabel type: gpt\n\n");
	}
	
	char arg[50];
	strcpy(arg, argv[1]);
		
	if (type =='d')
	{
		mbr_parsing(fd, arg);
	}
	else
	{
		
		gpt_parsing(fd, arg);
	}
	return 0;
}
