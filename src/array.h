#pragma once
#include "common.h"
#include "meta.h"
#include "mem.h"


#define IDX_SENTINEL ((idx) -1)
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define SUM(...)  (sum(NUMARGS(__VA_ARGS__), __VA_ARGS__))




// #define duck(x) (x)
// #define quack(a) (duck(a))



/**
 * Maths should be runnable on desktop computer
 * this will allow the system to reduce and optimize
 * out any constant calculations
 * then output a minimized version for the microcontroller
 */


/**
 * The order idx in a graph is the same as the AST, the order of the memory
 * that does idx's point to is not defined and depends on the C compiler
 * 
 * the order of memory in an array is defined
 */



idx Array(idx i,...) {

	return 0;
}

// idx _Graph(int length, ...) {
//     va_list ptr;
//     va_start(ptr, length);

// 	Code C = {
// 		.variable = false,
// 		.code = GRAPH_OP,
// 	};

// 	idx index = END;
// 	lookup[count++] = index;
	
// 	memcpy(MEM+END, &C, CodeSize);
// 	END += CodeSize;
	
// 	u16 dimens[2] = { 1, length };
// 	u16 dimens_size = sizeof(dimens);
// 	memcpy(MEM+END, &dimens, dimens_size);
// 	END += dimens_size;

// 	bool variable = false;
// 	for (u16 i = 0; i < length; i++) {
// 		// idx p = points[i];
// 		idx p = va_arg(ptr, int);
// 		variable = variable || get_code(p)->variable;
// 		get_code(p)->parent = index;

// 		memcpy(MEM+END, &p, IdxSize);
// 		END += IdxSize;
// 	}
// 	get_code(index)->variable = variable;

// 	return index;
// }
idx _Graph(int length, ...) {
    va_list ptr;
    va_start(ptr, length);

	// Code C = {
	// 	.variable = false,
	// 	.code = GRAPH_OP,
	// };

	idx index = END;
	lookup[count++] = index;
	
	Code * C = (Code *) (MEM+END);
	C->variable = false;
	C->code = GRAPH_OP;
	
	// memcpy(MEM+END, &C, CodeSize);
	END += CodeSize;
	Rank1 * shape = (Rank1 *) (C->shape);
	// Rank1 * shape = (Rank1 *) (MEM+END);

	shape->base = 0;	// graphs should always have a base of zero, because they are nothing more than an array of idx's
	shape->rank = 1;	// graphs are always rank 1
	shape->lens[0] = length;
	END += Rank1Size;

	idx * points = get_array(idx, index);



	bool variable = false;
	for (u16 i = 0; i < length; i++) {
		idx p = va_arg(ptr, int);
		variable = variable || get_code(p)->variable;
		get_code(p)->parent = index;

		// memcpy(MEM+END, &p, IdxSize);
		points[i] = p;
		END += IdxSize;
	}
	C->variable = variable;
	// get_code(index)->variable = variable;

	return index;
}
#define Graph(...)  (_Graph(NUMARGS(__VA_ARGS__), __VA_ARGS__))




/**
 *	Input: lenght, length, length, ..., index-to-graph
 */
idx _Space(int length, ...) {
    va_list ptr;
    va_start(ptr, length);

	// Code C = {
	// 	.variable = false,
	// 	.code = SPACE_OP,
	// };

	idx index = END;
	lookup[count++] = index;


	
	// memcpy(MEM+END, &C, CodeSize);
	
	Code * C = (Code *) (MEM+END);
	C->variable = false;
	C->code = SPACE_OP;
	END += CodeSize;
	
	// u16 dimens[2] = { 1, length };
	// u16 dimens_size = sizeof(dimens);
	// memcpy(MEM+END, &dimens, dimens_size);
	// END += dimens_size;
	
	Rank1 * shape = (Rank1 *) (C->shape);
	shape->base = 0;	// spaces should always have a base of zero, because they are nothing more than an array of scalars of size idx
	shape->rank = 1;	// spaces are always rank 1
	shape->lens[0] = length;
	END += Rank1Size;

	idx * points = get_array(idx, index);
	for (u16 i = 0; i < length; i++) {
		idx p = va_arg(ptr, int);
		// memcpy(MEM+END, &p, IdxSize);

		points[i] = p;
		END += IdxSize;

		if (i == length-1) {
			get_code(p)->parent = index;
		}
	}

	return index;
}
#define Space(...)  (_Space(NUMARGS(__VA_ARGS__), __VA_ARGS__))



idx Cast(idx space_idx, idx graph_idx) {
	// Code * code = get_code(graph);
	// u16 rank = code->dimens[0];
	// u16 * lens = &code->dimens[1];

	// // u16 * data = get_array(u16, graph);
	// idx * data = get_array(idx, graph);

	// printf("rank %u\n", rank);
	// for (int i = 0; i < rank; i++) {
	// 	printf("len %u\n", lens[i]);
	// 	for (int j = 0; j < lens[i]; j++) {
	// 		printf("data @%.4x\n", data[j]);
	// 	}
	// }
	// printf("\n");


	Code * space = get_code(space_idx);
	if (space->code != SPACE_OP) {
		panic("cast given non-space");
	}
	idx rank = get_shape(space_idx)->rank;
	idx*lens = get_shape(space_idx)->lens;
	idx * space_data = get_array(idx, space_idx);




	idx index = END;
	lookup[count++] = index;


	Code * array = (Code *) (MEM+END);
	array->variable = false;
	array->code = ARRAY_OP;
	END += CodeSize;

	// each dimension of Space is expected to be idx in size

	
	Rank1 * shape = (Rank1 *) (array->shape);
	shape->base = 0;
	shape->rank = 0;
	END += ShapeSize;

	u32 array_size = 1;
	u32 total_points = 1;


	// rank should always be 1 for Space object
	for (int i = 0; i < rank; i++) {
		for (int j = 0; j < lens[i]; j++) {
			u16 len = lens[i];
			if (j == len-1) {
				shape->base = space_data[j];
				Code * type = get_code(shape->base);
				printf("test: %s\n", OP_str[type->code]);
				array_size = total_points * OP_size[type->code];
			}
			else {
				total_points *= space_data[j];
				shape->rank += 1;
				shape->lens[j] = space_data[j];
				// memcpy(MEM+END, &space_data[j], CodeSize);
				END += IdxSize;
			}
		}
	}
	printf("point size %u, space size %u\n", total_points, array_size);


	Code * graph = get_code(graph_idx);
	if (graph->code != GRAPH_OP) {
		panic("cast given non-graph");
	}
	// idx rank = get_shape(graph_idx)->rank;
	idx graph_len = get_shape(graph_idx)->lens[0];
	idx * graph_data = get_array(idx, graph_idx);

	idx base_width = OP_size[get_code(shape->base)->code];

	for (int i = 0; i < total_points; i++) {
		if (i < graph_len) {
			idx _idx = graph_data[i];
			u8 * d = get_data(u8, _idx);
			printf("%x %x %x %x\n", d[0], d[1], d[2], d[3]);
			printf("%x\n", get_data(i32, _idx)[0]);
			// memcpy(MEM+END, MEM+_idx+CodeSize, base_width);

			(MEM+END)[0] = get_data(i32, _idx)[0];
			// (MEM+END)[-2] = d[0];
		}
		else {
			(MEM+END)[0] = (i32) 0;

		}

		END += base_width;
	}

	// only allocates space for basic types at the moment, but complex types shouldn't be too hard to add

	// NOTE, the parent of an array is the point that 



	return index;
}



idx I32(i32 val) {
	Code C = {
		.variable = false,
		.code = I32_OP
	};
	
	idx index = END;
	lookup[count++] = index;

	memcpy(MEM+END, &C, CodeSize);
	END += CodeSize;

	u32 val_size = sizeof(val);
	memcpy(MEM+END, &val, val_size);
	END += val_size;


	return index;
}



idx Addi32(idx a, idx b) {
	
	idx index = END;
	get_code(a)->parent = index;
	get_code(b)->parent = index;

	Code C = {
		.variable = get_code(a)->variable || get_code(b)->variable,
		.code = ADDI32_OP
	};
	
	lookup[count++] = index;

	memcpy(MEM+END, &C, CodeSize);
	END += CodeSize;

	i32 val = get_data(i32, a)[0] + get_data(i32, b)[0];
	u32 val_size = sizeof(val);
	memcpy(MEM+END, &val, val_size);
	END += val_size;

	return index;
}


// wraps a point or array and modifies their variable field
idx Variable(idx i) {
	get_code(i)->variable = true;
	return i;
}

// most points only push their parents, but keys or refs may push multiple
// each point only has one parent that is pushed during an update, but a ref is special point the dispatches a point's notification to multiple consumers
idx Ref(idx source, ...) {
	
	return 0;
}


idx F32(f32 val) {
	Code C = {
		.variable = false,
		.code = F32_OP
	};
	
	idx index = END;
	lookup[count++] = index;

	memcpy(MEM+END, &C, CodeSize);
	END += CodeSize;

	u32 val_size = sizeof(val);
	memcpy(MEM+END, &val, val_size);
	END += val_size;


	return index;
}


idx Axis() {
	return 0;
}





void runner() {

	// printf("code size %u\n", CodeSize);
	// printf("Rank1 size %u\n", Rank1Size);

	// idx k = F32(200.4);

	// Graph(		
	// 	F32(200.4),
	// 	F32(435.2),
	// 	F32(23.9),
	// 	Addi32(
	// 		Addi32(
	// 			Variable(I32(10)),
	// 			I32(20)
	// 		),
	// 		Addi32(
	// 			I32(30),
	// 			I32(40)
	// 		)
	// 	),
	// 	Graph(
	// 		F32(45), F32(19.3)
	// 	)
	// );
	// Addi32(
	// 	Addi32(
	// 		Variable(I32(10)),
	// 		I32(20)
	// 	),
	// 	Addi32(
	// 		I32(30),
	// 		I32(40)
	// 	)
	// );
	// I32(40);
	// Graph(I32(10), I32(20), I32(30));
	// idx ID = Graph(
	// 	Graph(I32(1), I32(0), I32(0)),
	// 	Graph(I32(0), I32(1), I32(0)),
	// 	Graph(I32(0), I32(0), I32(1))
	// );

	idx ID3x3 = Cast(
		Space(4, 4, I32(0)),
		Graph(
			I32(1), I32(7), I32(7),
			I32(6), I32(1), I32(6),
			I32(8), I32(8), I32(1)
		));

	idx ID3 = Cast(
		Space(16, I32(0)),
		Graph(
			I32(1), I32(7), I32(7),
			I32(6), I32(1), I32(6),
			I32(8), I32(8), I32(1)
		));
	// Cast(Space(1000, 5, I32(0)), Graph(I32(1), I32(2)));

	// Space(1000, 5, I32(0));

	// print_point(0);
	// print_point(24);
	print_points();
	// Ref(	I32(10),
	// 	Operation(),
	// 	Operation(),
	// 	Operation(),
	// );

// 	Graph(
// 		I32(10),
// 		I32(20),
// 		I32(30)
// 	);

// // [1000, 5, {10,20,30...}]
// 	Array(
// 		Graph(
// 			I32(10),
// 			I32(20),
// 			I32(30)
// 		),
// 		1000, 5
// 	);
// // [1000, 5, i32]
// 	idx table = Array(F32(0), 1000, 5);


// 	// table#[300,1] + table#[300,2]
// 	Add(
// 		Axis(table, 300, 1),
// 		Axis(table, 300, 2)
// 	);


}
