#pragma once

#include "type.h"
#include "vector.h"

#ifndef HASHMAP_CAPACITY
	#define HASHMAP_CAPACITY 16
#endif // #ifdef HASHMAP_CAPACITY

#ifndef HASHMAP_SEED 
	#define HASHMAP_SEED 123
#endif // #ifndef HASHMAP_SEED

typedef struct mapstr_item {
	const char* key;
	void* val;
} mapstr_item_t;

typedef struct {
	usize val_size;
	vec_t table[HASHMAP_CAPACITY];
} mapstr_t;

void mapstr_make(mapstr_t* map, usize val_size);
void mapstr_destroy(mapstr_t* map);

void mapstr_add(mapstr_t* map, const char* key, void* value);
void* mapstr_find(mapstr_t* map, const char* key);
