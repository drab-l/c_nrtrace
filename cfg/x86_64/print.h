#pragma once
#include <stdint.h>
#include <inttypes.h>

#include <printer.h>

typedef struct {
    uint64_t st_dev;
    uint64_t st_ino;
    uint64_t st_nlink;
    uint32_t st_mode;
    uint32_t st_uid;
    uint32_t st_gid;
    int32_t pad0;
    uint64_t st_rdev;
    int64_t st_size;
    int64_t st_blksize;
    int64_t st_blocks;
    struct { int64_t tv_sec; int64_t tv_nsec; }
    st_atim, st_mtim, st_ctim;
    int64_t reserved[3];
} bit64_stat;
#define bit64_stat bit64_stat

typedef union {
    bit64_ptr_t ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} bit64_epoll_data_t;
struct bit64_epoll_event {
  uint32_t events;
  bit64_epoll_data_t data;
} __attribute__ ((__packed__));
typedef struct bit64_epoll_event bit64_epoll_event;
unused_ static void sb_print_bit64_epoll_event(printer *pr, const bit64_epoll_event *s, size_t)
{
    string_buffer *sb = pr->sb;
    string_buffer_strcat(sb, ".events = ");
    string_buffer_dec(sb, s->events);
    string_buffer_strcat(sb, ", .data = ");
    string_buffer_hex(sb, s->data.u64);
}
#define bit64_epoll_event bit64_epoll_event
#define sb_print_bit64_epoll_event sb_print_bit64_epoll_event
