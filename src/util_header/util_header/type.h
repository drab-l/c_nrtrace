#pragma once

#define member_typeof(t_,m_) typeof(((t_*)0)->m_)
#define member_sizeof(t_,m_) sizeof(((t_*)0)->m_)
#define member_offset(t_,m_) ((size_t)&(((t_*)0)->m_))

typedef unsigned long long ullong;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef signed long long sllong;
typedef signed long slong;
typedef signed int sint;
typedef signed short sshort;
typedef signed char schar;

