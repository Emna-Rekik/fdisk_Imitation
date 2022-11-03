#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<inttypes.h>
#include <string.h>

#include "../headers/mbr_struct.h"
#include "../headers/unit_conversion.h"

void mbr_parsing(int fd, char arg[])
{
	char buf[512];
	char buf2[512];
	
	lseek(fd,0,SEEK_SET);
	read(fd, buf, 512);
	
	PartitionEntryMBR * table_entry_ptr = (PartitionEntryMBR *) &buf[446];
	PartitionEntryMBR * table_entry_ptr2 = (PartitionEntryMBR *) &buf2[446];
	
	/*printf("%-10s %5s %8s %8s %8s %9s %6s %6s \n", "Device",	    
	"Boot", "Start", "End", "Sectors", "Size", "Id", "Type");*/
	
	//Rewrite the printf to make the table more dynamic
	int shift = 1 + strlen(arg) + 2;
	char dest[] = "%-";
	char shift_c[10];
	sprintf(shift_c, "%i", shift);
	strcat(dest, shift_c);
	strcat(dest, "s %5s %8s %11s %11s %9s %3s %5s \x1B[0m"); //"%6s%d %7c %8u %8u %8u %8.1f%c %6X   %-8s\n"

	printf(dest, "Device",	"Boot", "Start", "End", "Sectors", "Size", "Id", "Type");
	printf("\n");
	    
	int i = 0;
	int extended_partition_number = -1;
	int offset;
	while (table_entry_ptr[i].lba != 0)
	{
		char unit = 'K';
		char *ptr = &unit;
		int counting = 0;
		float size = ((float)(table_entry_ptr[i].sector_count * 512) / 1024);
		size = unit_conversion (&unit, counting, size);
		
		char partition_type_char[20];
		if (table_entry_ptr[i].partition_type == 131) // 131 = 83 en Hexa	
		{
			strcpy(partition_type_char,"Linux");
		}
		else if (table_entry_ptr[i].partition_type == 5)
		{
			strcpy(partition_type_char,"Extended");
		}
		else
		{
			strcpy(partition_type_char,"Unknown");
		}
		
		printf("%6s%d %7c %8u %11u %11u %8.1f%c %3X  %-8s\n",		
		arg, 
		i + 1,		
		table_entry_ptr[i].status == 0x80 ? '*' : ' ',		
		table_entry_ptr[i].lba,		
		table_entry_ptr[i].lba + table_entry_ptr[i].sector_count -1, 
		table_entry_ptr[i].sector_count,		
		size,
		unit,		
		table_entry_ptr[i].partition_type,
		partition_type_char);
		i++;
		
		if(table_entry_ptr[i].partition_type == 5)
		{
			extended_partition_number = i;
			offset = table_entry_ptr[i].lba;
		}
	}
	
	// We jump the 4 main partitions and start counting
	int count = 5;
	int j = 0;
	int k = j;
	int temp = offset;
	
	if (extended_partition_number != -1)
	{
		while (table_entry_ptr2[k].lba != 0)
		{
			k=0;
			//Here we will find the partition table of the Extended Disk
			lseek(fd,offset*512,SEEK_SET); 
			read(fd, buf2, 512);
			table_entry_ptr2 = (PartitionEntryMBR *) &buf2[446];
			
			char unit = 'K';
			char *ptr = &unit;
			int counting = 0;
			float size = ((float)(table_entry_ptr2[j].sector_count * 512) / 1024);
			size = unit_conversion (&unit, counting, size);
			
			printf("%6s%d %7c %8u %11u %11u %8.1f%c %3X  %-8s\n",		
			arg, 
			count,		
			table_entry_ptr2[j].status == 0x80 ? '*' : ' ',		
			table_entry_ptr2[j].lba + offset,		
			table_entry_ptr2[j].lba + table_entry_ptr2[j].sector_count -1 + offset, 
			table_entry_ptr2[j].sector_count,		
			size,
			unit,		
			table_entry_ptr[j].partition_type,
			"Linux");
			j++;
			count++;
			k++;
			offset = temp;
			offset = offset + table_entry_ptr2[j].lba;
			j = 0;
		}
	}
}
