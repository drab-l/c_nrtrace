#include <vl_buffer_lib.h>
#include "inc/vl_array.h"

struct vl_array { vlb vl; size_t entry_size; struct {void *b;} type[];};

static void get_vl_array_(vl_array **r, size_t entry_size)
{
    *r = malloc(sizeof(**r));
    if (*r) {
        vlb_init(&(*r)->vl, entry_size * 10);
        (*r)->entry_size = entry_size;
    }
}

vl_array *get_vl_array(size_t entry_size)
{
    vl_array *r;
    get_vl_array_(&r, entry_size);
    return r;
}

void put_vl_array(vl_array *p)
{
    vlb_deinit(&p->vl);
    free(p);
}

static void *get_vl_array_entry_unsafe_(vl_array *p, size_t offset)
{
    return (void*)((unsigned char*)GET_HEAD_PTR(p) + offset);
}

static void *for_each_vl_array_entry_(vl_array *p, void **save)
{
    if (!*save) {
        *save = GET_HEAD_PTR(p);
    }
    ptrdiff_t diff = pointer_diff(*save, GET_HEAD_PTR(p));
    if (diff < 0 || (size_t)diff >= GET_USED_SIZE(p)) {
        return NULL;
    }
    void *r = get_vl_array_entry_unsafe_(p, diff);
    *save = get_vl_array_entry_unsafe_(p, diff + p->entry_size);
    return r;
}

void *get_vl_array_entry(vl_array *p, size_t index)
{
    size_t offset = index * p->entry_size;
    if (offset + p->entry_size > GET_REMAIN_SIZE(p)) {
        return NULL;
    }
    return get_vl_array_entry_unsafe_(p, offset);
}

void *set_vl_array_entry(vl_array *p, const void *data, size_t index)
{
    size_t offset = index * p->entry_size;
    if (offset + p->entry_size > GET_USED_SIZE(p)) {
        return NULL;
    }
    void *dst = get_vl_array_entry_unsafe_(p, offset);
    memcpy(dst, data, p->entry_size);
    return dst;
}

void *add_vl_array_entry(vl_array *p, const void *data)
{
    size_t size = p->entry_size;
    EXPAND_BUFFER_IF_NEED(p, size);
    void *dst = GET_TAIL_PTR(p);
    memcpy(dst, data, size);
    ADD_USED_SIZE(p, size);
    return dst;
}

static void *search_vl_array_entry_(vl_array *p, int(search)(const void*, const void*), const void *arg)
{
    for (void *save = NULL, *e; (e = for_each_vl_array_entry_(p, &save)); ) {
        if (search(e, arg)) {
            return e;
        }
    }
    return NULL;
}

void *search_vl_array_entry(vl_array *p, int(search)(const void*, const void*), const void *arg)
{
    return search_vl_array_entry_(p, search, arg);
}

void *searched_or_tail_insert_vl_array_entry(vl_array *p, void *data, int(search)(const void*))
{
    for (void *save = NULL, *e; (e = for_each_vl_array_entry_(p, &save)); ) {
        if (search(e)) {
            memcpy(e, data, p->entry_size);
            return e;
        }
    }
    return add_vl_array_entry(p, data);
}

void for_each_vl_array_entry(vl_array *p, void(*func)(void *,void*), void *arg)
{
    for (void *save = NULL, *e; (e = for_each_vl_array_entry_(p, &save)); ) {
        func(e, arg);
    }
}

