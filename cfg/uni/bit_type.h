#pragma once

#include <stdint.h>

// for LP64,ILP32
typedef char bit64_char;
typedef char bit32_char;
typedef signed char bit64_schar;
typedef signed char bit32_schar;
typedef unsigned char bit64_uchar;
typedef unsigned char bit32_uchar;

#ifndef bit64_sshort
typedef int16_t bit64_sshort;
#endif
#ifndef bit32_sshort
typedef int16_t bit32_sshort;
#endif
#ifndef bit64_ushort
typedef int16_t bit64_ushort;
#endif
#ifndef bit32_ushort
typedef int16_t bit32_ushort;
#endif

#ifndef bit64_sint
typedef int32_t bit64_sint;
#endif
#ifndef bit32_sint
typedef int32_t bit32_sint;
#endif
#ifndef bit64_uint
typedef uint32_t bit64_uint;
#endif
#ifndef bit32_uint
typedef uint32_t bit32_uint;
#endif

#ifndef bit64_slong
typedef int64_t bit64_slong;
#endif
#ifndef bit32_slong
typedef int32_t bit32_slong;
#endif
#ifndef bit64_ulong
typedef uint64_t bit64_ulong;
#endif
#ifndef bit32_ulong
typedef uint32_t bit32_ulong;
#endif

#ifndef bit64_sllong
typedef int64_t bit64_sllong;
#endif
#ifndef bit32_sllong
typedef int64_t bit32_sllong;
#endif
#ifndef bit64_ullong
typedef uint64_t bit64_ullong;
#endif
#ifndef bit32_ullong
typedef uint64_t bit32_ullong;
#endif

#ifndef bit64_size_t
typedef uint64_t bit64_size_t;
#endif
#ifndef bit32_size_t
typedef uint32_t bit32_size_t;
#endif
typedef bit64_size_t bit_size_t;

#ifndef bit64_ptr_t
typedef uint64_t bit64_ptr_t;
#endif
#ifndef bit32_ptr_t
typedef uint32_t bit32_ptr_t;
#endif
typedef bit64_ptr_t bit_ptr_t;

#ifndef bit64_time_t
typedef int64_t bit64_time_t;
#endif
#ifndef bit32_time_t
typedef int32_t bit32_time_t;
#endif

typedef bit64_reg_t bit_reg_t;
typedef bit64_sreg_t bit_sreg_t;

#ifndef bit64_off_t
typedef int64_t bit64_off_t;
#endif
#ifndef bit32_off_t
typedef int32_t bit32_off_t;
#endif
#ifndef bit64_dev_t
typedef uint64_t bit64_dev_t;
#endif
#ifndef bit32_dev_t
typedef uint64_t bit32_dev_t;
#endif
#ifndef bit64_ino_t
typedef uint64_t bit64_ino_t;
#endif
#ifndef bit32_ino_t
typedef uint32_t bit32_ino_t;
#endif
#ifndef bit64_mode_t
typedef uint32_t bit64_mode_t;
#endif
#ifndef bit32_mode_t
typedef uint32_t bit32_mode_t;
#endif
#ifndef bit64_nlink_t
typedef uint32_t bit64_nlink_t;
#endif
#ifndef bit32_nlink_t
typedef uint32_t bit32_nlink_t;
#endif
#ifndef bit64_uid_t
typedef uint32_t bit64_uid_t;
#endif
#ifndef bit32_uid_t
typedef uint32_t bit32_uid_t;
#endif
#ifndef bit64_gid_t
typedef uint32_t bit64_gid_t;
#endif
#ifndef bit32_gid_t
typedef uint32_t bit32_gid_t;
#endif

#ifndef bit64_socklen_t
typedef uint32_t bit64_socklen_t;
#endif
#ifndef bit32_socklen_t
typedef uint32_t bit32_socklen_t;
#endif

#ifndef bit64_sa_family_t
typedef unsigned short bit64_sa_family_t;
#endif
#ifndef bit32_sa_family_t
typedef unsigned short bit32_sa_family_t;
#endif

