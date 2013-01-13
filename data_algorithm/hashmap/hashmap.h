#ifndef _HASHMAP_H_
#define _HASHMAP_H_

struct hashmap;

struct hashmap * map_new(int max);
void map_delete(struct hashmap * m);
int map_search(struct hashmap *m , int num);
void map_insert(struct hashmap * m, int num);
void map_erase(struct hashmap * m, int num);
void map_dump(struct hashmap * m);

#endif
