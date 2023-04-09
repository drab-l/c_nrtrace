#pragma once
#include <stdint.h>
#include <inttypes.h>

#include <printer.h>
#include <util_header/stdc.h>

typedef struct {
    uint32_t st_dev;
    uint32_t st_ino;
    uint16_t st_mode;
    uint16_t st_nlink;
    uint16_t st_uid;
    uint16_t st_gid;
    uint32_t st_rdev;
    uint32_t st_size;
    uint32_t st_blksize;
    uint32_t st_blocks;
    struct { uint32_t tv_sec; uint32_t tv_nsec; } st_atim, st_mtim, st_ctim;
} bit32_stat;
#define bit32_stat bit32_stat

typedef struct {
    uint64_t st_dev;
    uint32_t pad1;
    uint32_t st_ino__;
    uint32_t st_mode;
    uint32_t st_nlink;
    uint32_t st_uid;
    uint32_t st_gid;
    uint64_t st_rdev;
    uint32_t pad2;
    int64_t st_size;
    int32_t st_blksize;
    int64_t st_blocks;
    struct { int32_t tv_sec; int32_t tv_nsec; }
    st_atim, st_mtim, st_ctim;
    uint64_t st_ino;
} __attribute__((packed)) bit32_stat64;
unused_ static void sb_print_bit32_stat64(printer *pr, const bit32_stat64 *s, size_t)
{
    string_buffer *sb = pr->sb;
    string_buffer_strcat(sb, ".st_dev = ");
    string_buffer_hex(sb, s->st_dev);
    string_buffer_strcat(sb, ", .st_ino = ");
    string_buffer_dec(sb, s->st_ino);
    string_buffer_strcat(sb, ", . = st_mode");
    string_buffer_hex(sb, s->st_mode);
    string_buffer_strcat(sb, ", . = st_nlink");
    string_buffer_dec(sb, s->st_nlink);
    string_buffer_strcat(sb, ", . = st_uid");
    string_buffer_dec(sb, s->st_uid);
    string_buffer_strcat(sb, ", . = st_gid");
    string_buffer_dec(sb, s->st_gid);
    string_buffer_strcat(sb, ", . = st_rdev");
    string_buffer_hex(sb, s->st_rdev);
    string_buffer_strcat(sb, ", . = st_size");
    string_buffer_dec(sb, s->st_size);
    string_buffer_strcat(sb, ", . = st_blksize");
    string_buffer_dec(sb, s->st_blksize);
    string_buffer_strcat(sb, ", . = st_blocks");
    string_buffer_dec(sb, s->st_blocks);
    string_buffer_strcat(sb, ", .st_atim = {.tv_sec = ");
    string_buffer_dec(sb, s->st_atim.tv_sec);
    string_buffer_strcat(sb, ", .tv_nsec = ");
    string_buffer_dec(sb, s->st_atim.tv_nsec);
    string_buffer_strcat(sb, "}, st_mtim  {.tv_sec = ");
    string_buffer_dec(sb, s->st_mtim.tv_sec);
    string_buffer_strcat(sb, ", .tv_nsec = ");
    string_buffer_dec(sb, s->st_mtim.tv_nsec);
    string_buffer_strcat(sb, "}, st_ctim  {.tv_sec = ");
    string_buffer_dec(sb, s->st_ctim.tv_sec);
    string_buffer_strcat(sb, ", .tv_nsec = ");
    string_buffer_dec(sb, s->st_ctim.tv_nsec);
    string_buffer_charcat(sb, '}');
}
#define bit32_stat64 bit32_stat64
#define sb_print_bit32_stat64 sb_print_bit32_stat64

typedef union {
    bit32_ptr_t ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} bit32_epoll_data_t;
struct bit32_epoll_event {
  uint32_t events;
  bit32_epoll_data_t data;
} __attribute__ ((__packed__));
typedef struct bit32_epoll_event bit32_epoll_event;
unused_ static void sb_print_bit32_epoll_event(printer *pr, const bit32_epoll_event *s, size_t)
{
    string_buffer *sb = pr->sb;
    string_buffer_strcat(sb, ".events = ");
    string_buffer_dec(sb, s->events);
    string_buffer_strcat(sb, ", .data = ");
    string_buffer_hex(sb, s->data.u64);
}
#define bit32_epoll_event bit32_epoll_event
#define sb_print_bit32_epoll_event sb_print_bit32_epoll_event
