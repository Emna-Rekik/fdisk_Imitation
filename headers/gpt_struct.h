#ifndef GPT_STRUCT_H_
#define GPT_STRUCT_H_

/** 128 bytes */
typedef struct {	    
	uint64_t partition_type[2];    
	uint64_t unique_partition[2];
	uint64_t lba;    
	uint64_t last_lba;    
	uint64_t flag;    
	uint8_t partition_name[9];
}PartitionEntryGPT;

#endif
