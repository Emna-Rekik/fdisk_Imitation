#ifndef MBR_STRUCT_H_
#define MBR_STRUCT_H_

/** 16 bytes */
typedef struct {	    
	uint8_t status;    
	uint8_t first_chs[3];
	uint8_t partition_type;    
	uint8_t last_chs[3];    
	uint32_t lba;    
	uint32_t sector_count;
}PartitionEntryMBR;

#endif
