#include "mapstr.h"

#include <memory.h>
#include <stdlib.h>

void mapstr_make(mapstr_t* map, usize val_size) {
	map->val_size = val_size;

	for (usize i = 0; i < HASHMAP_CAPACITY; i++) {
		VEC_MAKE(&map->table[i], mapstr_item_t);
	}
}

// not my code
static uint32_t murmur_hash(const char *key, uint32_t len, uint32_t seed) {
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m = 5;
    static const uint32_t n = 0xe6546b64;

    uint32_t hash = seed;

    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *) key;
    int i;
    for (i = 0; i < nblocks; i++) {
        uint32_t k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = (const uint8_t *) (key + nblocks * 4);
    uint32_t k1 = 0;

    switch (len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];

        k1 *= c1;
        k1 = (k1 << r1) | (k1 >> (32 - r1));
        k1 *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

static inline u32 get_index_by_key(const char* key) {
	return murmur_hash(key, strlen(key), HASHMAP_SEED) % HASHMAP_SEED;
}

void mapstr_add(mapstr_t* map, const char* key, void* value) {
	printf("add index -> %d\n", get_index_by_key(key));

	mapstr_item_t item = {
		.key = key,
		.val = malloc(map->val_size)
	};
	ASSERT(item.val != NULL, DEF_ALLOC_ERRMSG);
	memcpy(item.val, value, map->val_size);

	vec_push(&map->table[get_index_by_key(key)], &item);
}

void* mapstr_find(mapstr_t* map, const char* key) {
	printf("find index -> %d\n", get_index_by_key(key));

	vec_t* bucket = &map->table[get_index_by_key(key)];

	if (vec_size(bucket) == 0) {
		return NULL;
	}

	// find correct key (need this because hash collision)
	for (usize i = 0; i < vec_size(bucket); i++) {
		mapstr_item_t* item = VEC_GET(bucket, mapstr_item_t, i);

		if (strcmp(item->key, key) == 0) {
			return item->val;
		}
	}

	return NULL;
}
