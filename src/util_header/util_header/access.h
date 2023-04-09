#pragma once

#include <stddef.h>

#define can_access_member(type_, member_, size_) ((offsetof(type_, member_) + sizeof(((type_*)0)->member_)) <= (size_))
#define pointer_diff(pos_, head_) ((ptrdiff_t)((unsigned char*)(pos_) - (unsigned char*)(head_)))
#define pointer_add(ptr_, add_) ((typeof(ptr_))((unsigned char*)(ptr_) + (ptrdiff_t)(add_)))

#define array_count(a_) (sizeof(a_)/sizeof(a_[0]))

#define for_each(e_, a_) for (typeof(*a_) *e_ = (a_); e_ < (&(a_)[array_count(a_)]); e_++)
