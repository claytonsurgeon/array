#pragma once
#include "common.h"
#include "array.h"
#include "mem.h"



// void print_point(idx i) {
// 	printf(
// 		"%u: %s %s\n\tlength:%u\n\tvolume:%u\n\tparent:%u\n\t"
// 		,	i
// 		,	OP_str[get_code(i)->code]
// 		,	get_code(i)->variable ? "variable" : "constant"
// 		,	get_code(i)->point.length
// 		,	get_code(i)->point.volume
// 		,	get_code(i)->point.parent
// 	);
// }

void print_point(idx i) {
	OP code = get_code(i)->code;
	printf(
		"@%.4x: %8s @%.4x %s:"//length:%u\n\tvolume:%u\n\tparent:%u\n\t"
		,	i
		,	OP_str[code]
		,	get_code(i)->parent
		,	get_code(i)->variable ? "variable" : "constant"
	);
	switch (code) {
		case F32_OP: {
			printf(" %f", get_data(f32, i)[0]);
			break;
		}
		case I32_OP: {
			printf(" %d", get_data(i32, i)[0]);
			break;
		}

		case ADDI32_OP: {
			printf(" %d", get_data(i32, i)[0]);
			break;
		}

		case GRAPH_OP: {
			// u16 rank = get_data(u16, i)[0];
			// u16 len = get_data(u16, i)[1];

			Rank1 * shape = get_rank1(i);
			// idx base = shape->base;
			// idx rank = shape->rank;
			idx len = shape->len;

			
			// printf(" base: %s, rank: %u, shape:", OP_str[0], rank);
			// for (u16 j = 0; j < rank; j++) {
			// 	printf(" %u", lens[j]);
			// }
			// printf("\n\t    {");

			printf(" { ");
			// for (u16 j = 0; j < rank; j++) {
				for (u16 k = 0; k < len; k++) {
					printf(" @%.4x ", get_graph(i)[k]);
				}
			// }
			// printf(" rank: %u, shape: %u, {", rank, len);
			// for (u16 j = 0; j < len; j++) {
			// 	printf(" @%.4x ", get_data(idx, i)[2 + j]);
			// }
			printf("}");
			break;
		}

		case ARRAY_OP: {
			// this currently only prints 4 byte data points
			// needs to be generalized
			
			Shape * shape = get_shape(i);
			idx base = shape->base;
			idx rank = shape->rank;
			idx*lens = shape->lens;


			idx total_points = 1;
			for (u16 j = 0; j < rank; j++) {
				total_points *= lens[j];
			}
		

			printf(" base: @%.4x, rank: %u, shape:", base, rank);
			for (u16 j = 0; j < rank; j++) {
				printf(" %u", lens[j]);
			}
			printf("\n\t    {");
			for (u16 j = 0; j < total_points; j++) {
				printf(" %d ", get_array(i32, i)[j]);
			}
			printf("}");
			break;
		}

		case SPACE_OP: {
			// u16 rank = get_data(u16, i)[0];
			// u16 len = get_data(u16, i)[1];
			
			Rank1 * shape = get_rank1(i);
			// idx rank = shape->rank;
			// idx*lens = shape->lens;
			idx len = shape->len;

			// printf(" rank: %u, shape:", rank);
			// for (u16 j = 0; j < rank; j++) {
			// 	printf(" %u", lens[j]);
			// }
			printf(" [");
			// for (u16 j = 0; j < rank; j++) {
			// 	for (u16 k = 0; k < lens[j]; k++) {
			// 		if (k == lens[j]-1) {
			// 			printf("@%.4x", get_array(idx, i)[k]);
			// 		}
			// 		else {
			// 			printf("%u, ", get_array(idx, i)[k]);
			// 		}
			// 	}
			// }
			// for (u16 j = 0; j < rank; j++) {
				for (u16 k = 0; k < len; k++) {
					if (k == len-1) {
						printf("@%.4x", get_graph(i)[k]);
					}
					else {
						printf("%u, ", get_graph(i)[k]);
					}
				}
			// }



			// printf(" rank: %u, shape: %u, [", rank, len);
			// for (u16 j = 0; j < len; j++) {
			// 	if (j == len-1) {
			// 		printf("@%.4x", get_data(idx, i)[2 + j]);
			// 	}
			// 	else {
			// 		printf("%u, ", get_data(idx, i)[2 + j]);
			// 	}
			// }
			printf("]");
			break;
		}

		default: {
			printf("Unrecognized OP code: %u", code);
			break;
		}
	}
}

// print_data()

void print_points() {
	for (u32 i = 0; i < count; i++) {
		print_point(lookup[i]);
		// printf("%u ", i);
		printf("\n");
	}
}