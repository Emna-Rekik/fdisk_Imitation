#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

typedef struct {
	uint8_t status;
	uint8_t first_chs[3];
	uint8_t partition_type;
	uint8_t last_chs[3];
	uint32_t lba;
	uint32_t sector_count;
}PartitionEntry;

float unit_conversion (char* unit, int count, float size);

int main(int argc, char **argv)
{
	char buf[512];
	char buf2[512];

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
		printf("This file is not an MBR Disk file.\n");
		exit(-1);
	}

	PartitionEntry * table_entry_ptr = (PartitionEntry *) &buf[446];
	PartitionEntry * table_entry_ptr2 = (PartitionEntry *) &buf2[446];

	/*printf("%-10s %5s %8s %8s %8s %9s %6s %6s \n", "Device",
	"Boot", "Start", "End", "Sectors", "Size", "Id", "Type");*/

	//Rewrite the printf to make the table more dynamic
	int shift = 1 + strlen(argv[1]) + 2;
	char dest[] = "%-";
	char shift_c[10];
	sprintf(shift_c, "%i", shift);
	strcat(dest, shift_c);
	strcat(dest, "s %5s %8s %8s %8s %9s %6s %6s \x1B[0m");

	// Make the text Bold
	printf("\x1B[1m");
	printf("Disklabel type: dos\n\n");
	printf(dest, "Device",	"Boot", "Start", "End", "Sectors", "Size", "Id", "Type");
	printf("\n");

	int i = 0;
	int extended_partition_number = -1;
	int offset;
	while (table_entry_ptr[i].lba != 0)
	{
		char unit = 'K';
		char *ptr = &unit;
		int count = 0;
		float size = ((float)(table_entry_ptr[i].sector_count * 512) / 1024);
		size = unit_conversion (&unit, count, size);

		char* partition_type_char;
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
		printf("%6s%d %7c %8u %8u %8u %8.1f%c %6X   %-8s\n",
		argv[1],
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
			table_entry_ptr2 = (PartitionEntry *) &buf2[446];

			char unit = 'K';
			char *ptr = &unit;
			int counting = 0;
			float size = ((float)(table_entry_ptr2[j].sector_count * 512) / 1024);
			size = unit_conversion (&unit, counting, size);

			printf("%6s%d %7c %8u %8u %8u %8.1f%c %6X   %-8s\n",
			argv[1],
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
	return 0;
}

float unit_conversion (char* unit, int count, float size)
{
		while (((int)size/1024) != 0)
		{
			size = size/1024;
			count++;
		}
		if (count == 1)
		{
			*unit = 'M';
		}
		else if (count == 2)
		{
			*unit = 'G';
		}

		return size;
}