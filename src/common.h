#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef volatile u32 r32;
typedef volatile u32 reg32;

typedef volatile u64 r64;
typedef volatile u64 reg64;

typedef float f32;
typedef double f64;


typedef u16 idx;

#define IdxSize sizeof(idx)

// fighting with poor syntax highlighting on VSCode
typedef enum {
	IDX_OP,
	I32_OP,
	F32_OP,
	ADDI32_OP,
	GRAPH_OP,
	SPACE_OP,
	ARRAY_OP,
	CAST_OP
} __attribute__((packed)) OP_;
typedef OP_ OP;

static char OP_str[10][10] = {
	"idx",	// the base type of graphs which are arrays of idx's
	"i32",
	"f32",
	"addi32",
	"graph",
	"space",
	"array",
	"cast"
};

// size only includes space occupied by data, NOT code
// zero size implies complex size, e.g. an array has a variadic length
static u8 OP_size[10] = {
	IdxSize,
	4,
	4,
	4,
	0,
	0,
	0,
	0
};




// typedef struct _Code {
// 	bool	variable;	// variable or constant?
// 	u8		code;		// joining operation
// 	union { duck
// 	union { duck
// 		struct {
// 			// u8		word;		// 1,2,4,8,16 bytes = 8,16,32,64,128 bits
// 			// u32	volume;	// physical capacity
// 			// u32 	length;	// logical length
// 			idx 	parent;	// index of parent
// 		} point;
// 		struct {
// 			u32	word;		// any size, for example elements may be structs of 387 byte size
// 			u32	volume;	// length * word * fold(*, dimensions)
// 			u32 	length;	// logical length
// 			idx 	parent;	// index of parent
// 			u8		dimens[];	// object dimensions	
// 		} array;
// 	};
// } Code;


typedef struct _Shape {
	idx	base;		// index to point that is being used as the base element
	idx	rank;
	idx	lens[];	
} Shape;

typedef struct _Rank1 {
	// idx	base;
	// idx	rank;
	// idx	lens[1];
	idx len;
} Rank1;

#define Rank1Size (sizeof(Rank1))
#define ShapeSize (sizeof(Shape))

// typedef struct _Code {
// 	bool	variable;	// variable or constant?
// 	OP		code;			// joining operation
// 	idx 	parent;		// index of parent
// 	u16	dimens[];	// array rank dimension lengths, non arrays have an implied 0 rank
// } Code;

typedef struct _Code {
	bool	variable;	// variable or constant?
	OP		code;			// joining operation
	idx 	parent;		// index of parent
	idx	shape[];	// array rank dimension lengths, non arrays have an implied 0 rank
} Code;



u32 CodeSize = sizeof(Code);