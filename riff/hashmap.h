#ifndef _HASHMAP_H
#define _HASHMAP_H

#define MAX_MAP_SIZE 10

typedef void* hash_map;

hash_map hashmap_new();

_Bool set(hash_map* map, void* key, void* value);
void* get(hash_map* map, void* key);

#endif