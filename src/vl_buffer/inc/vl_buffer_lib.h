#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <util_header_all.h>

typedef struct {
    void *buffer;
    size_t size;
    size_t max;
} vlb;

static const size_t DEFAULT_VL_BUFFER_SIZE  = 1024;
static const size_t MIN_VL_EXPAND_SIZE = DEFAULT_VL_BUFFER_SIZE;

unused_ static int resize_heap(void **old, size_t size)
{
    void *new = realloc(*old, size);
    if (!new) {
        return -1;
    }
    *old = new;
    return 0;
}

unused_ static int vlb_expand(vlb *p, size_t expand_size)
{
    expand_size = (expand_size < MIN_VL_EXPAND_SIZE) ? MIN_VL_EXPAND_SIZE : expand_size;
    if (resize_heap(&p->buffer, p->max + expand_size)) {
        return -1;
    }
    p->max += expand_size;
    return 0;
}

unused_ static void vlb_init(vlb *r, size_t size)
{
    *r = (typeof(*r)){};
    vlb_expand(r, size);
}

unused_ static void vlb_alloc(vlb **r)
{
    *r = malloc(sizeof(**r));
    if (*r) {
        vlb_init(*r, 0);
    }
}

unused_ static void vlb_deinit(vlb *p)
{
    free(p->buffer);
}

unused_ static void vlb_dealloc(vlb *p)
{
    vlb_deinit(p);
    free(p);
}

unused_ static void vlb_clear_used_size(vlb *r)
{
    r->size = 0;
}

unused_ static size_t vlb_get_remain_size(const vlb *p)
{
    return p->max - p->size;
}

unused_ static size_t vlb_get_used_size(const vlb *p)
{
    return p->size;
}

unused_ static void *vlb_get_tail_ptr(vlb *p)
{
    return (unsigned char*)p->buffer + vlb_get_used_size(p);
}

unused_ static void *vlb_get_head_ptr(vlb *p)
{
    return (unsigned char*)p->buffer;
}

unused_ static void vlb_add_used_size(vlb *p, size_t size)
{
    if (vlb_get_remain_size(p) < size) {
        size = vlb_get_remain_size(p);
    }
    p->size += size;
}

unused_ static int vlb_expand_if_need(vlb *p, size_t need_size)
{
    if (need_size >= vlb_get_remain_size(p)) {
        vlb_expand(p, need_size - vlb_get_remain_size(p));
        return 1;
    }
    return 0;
}

#define GET_HEAD_PTR(p_) ((typeof(p_->type[0].b))vlb_get_head_ptr((vlb*)(p_)))
#define GET_TAIL_PTR(p_) ((typeof(p_->type[0].b))vlb_get_tail_ptr((vlb*)(p_)))
#define GET_REMAIN_SIZE(p_) (vlb_get_remain_size((vlb*)(p_)))
#define GET_USED_SIZE(p_) (vlb_get_used_size((vlb*)(p_)))
#define ADD_USED_SIZE(p_, s_) (vlb_add_used_size((vlb*)(p_), (s_)))
#define EXPAND_BUFFER_IF_NEED(p_, s_) (vlb_expand_if_need((vlb*)(p_), (s_)))

