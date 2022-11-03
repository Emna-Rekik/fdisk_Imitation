#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<inttypes.h>
#include <string.h>

#include "../headers/gpt_struct.h"
#include "../headers/unit_conversion.h"

void gpt_parsing(int fd, char arg[])
{
	char gpt_entry[128];
	char gpt_partition_type[128];
	
	int i = 0;
	int offset = 1024;
	lseek(fd,offset,SEEK_SET); 
	read(fd, gpt_entry, 128);
	
	PartitionEntryGPT * table_entry_gpt = (PartitionEntryGPT *) &gpt_entry;
	
	/*printf("%-10s %5s %8s %8s %8s %9s %6s %6s \n", "Device",	    
	"Boot", "Start", "End", "Sectors", "Size", "Id", "Type");*/
	
	//Rewrite the printf to make the table more dynamic
	int shift = 1 + strlen(arg) + 2;
	char dest[] = "%-";
	char shift_c[10];
	sprintf(shift_c, "%i", shift);
	strcat(dest, shift_c);
	strcat(dest, "s %9s %11s %11s %8s %6s \x1B[0m");

	printf(dest, "Device", "Start", "End", "Sectors", "Size", "Type");
	printf("\n");
	
	int k = 1;
	while (table_entry_gpt->lba != 0)
	{
		long sector_count = table_entry_gpt->last_lba - table_entry_gpt->lba + 1;
	
		char unit = 'K';
		char *ptr = &unit;
		int counting = 0;
		float size = ((float)(sector_count * 512) / 1024);
		size = unit_conversion (&unit, counting, size);
		
		char partition_type_char[20];
		if (table_entry_gpt->partition_type[0] == 1284361659022471976)	
		{
			strcpy(partition_type_char,"EFI System");
		}
		else if (table_entry_gpt->partition_type[0] == 5148323022478130607)
		{
			strcpy(partition_type_char,"Linux Filesystem");
		}
		else if (table_entry_gpt->partition_type[0] == 4883208950429252973)
		{
			strcpy(partition_type_char,"Linux swap");
		}
		else
		{
			strcpy(partition_type_char,"Unknown");
		}
		//printf("%ld\n", table_entry_gpt->partition_type[0]);
	
		printf("%6s%d %11lu %11lu %11lu %7.1f%c   %-11s\n",		
		arg, 
		k++,		
		table_entry_gpt->lba,		
		table_entry_gpt->last_lba, 
		sector_count,		
		size,
		unit,		
		partition_type_char);
		
		offset = offset + 128;
		lseek(fd, offset, SEEK_SET);
		read(fd, gpt_entry, 128);
		//i++;
	}
}
