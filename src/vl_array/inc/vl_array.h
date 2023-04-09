#pragma once

#include <stddef.h>

struct vl_array;
typedef struct vl_array vl_array;
vl_array *get_vl_array(size_t entry_size);
void *searched_or_tail_insert_vl_array_entry(vl_array *p, void *data, int(search)(const void*));
void *search_vl_array_entry(vl_array *p, int(search)(const void*, const void*), const void *arg);
void for_each_vl_array_entry(vl_array *p, void(*func)(void *,void*), void *arg);

