#pragma once
#include "./common.h"

#define KB (1<<10)
#define MB (1<<20)


u8 MEM[500*KB];
u32 END = 0;

idx lookup[100] = {0};
u32 count = 0;


typedef struct _key {
	idx index;
	idx shell;
	char label[100];
} key;

key KEYS[1000] = {0};
idx key_count = 0;



// no matter how simple I make a C macro it still manages to fuck me
#define get_code(i) ((Code *) (MEM+i))
#define get_data(T, i) ((T *) (MEM+i+CodeSize))
// #define get_array(T, i) ((T *) (i + get_data(T, get_code(i)->dimens[0] * (IdxSize+1)) ))
// #define get_array(T, i) ((T *) (((get_code(i)->dimens[0]+1) * IdxSize) + (MEM+i+CodeSize)))
// #define get_array(T, i) ((T *) ((   (((Shape *)(get_code(i)->shape))[1]+2) * IdxSize) + (MEM+i+CodeSize)))
// #define get_array(T, i) ((T *) ( (MEM+i+CodeSize) + 1))

#define get_shape(i) ((Shape *) (get_code(i)->shape))
#define get_rank1(i) ((Rank1 *) (get_code(i)->shape))

#define get_array(T, i) ((T *) (get_shape(i)->lens + (get_shape(i)->rank)))
// #define get_graph(T, i) ((T *) (&get_rank1(i)->len + 1))
#define get_graph(i) ((idx *) (get_code(i)->shape + 1))	// get rank1 data array

void panic(char message[]) {
	printf("ERROR: ");
	puts(message);
	printf("\n");
	abort();
}




void add_label (char label[], idx value) {
	// KEYS[key_count] = {
	// 	.index = value,
	// 	.shell = 0
	// };
}

idx key_to_idx(char label[]) { return 0;}
void idx_to_key(idx index, char label[]) {}


void inc_shells(){}