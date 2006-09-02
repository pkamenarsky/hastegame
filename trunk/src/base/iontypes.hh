#ifndef ION_BASE_ION_TYPES_HH_INCLUDED
#define ION_BASE_ION_TYPES_HH_INCLUDED

// Visual C types

#ifdef _MSC_VER

// For intptr_t
#include <stddef.h>

typedef signed __int8 ion_int8;
typedef signed __int16 ion_int16;
typedef signed __int32 ion_int32;
typedef signed __int64 ion_int64;

typedef unsigned __int8 ion_uint8;
typedef unsigned __int16 ion_uint16;
typedef unsigned __int32 ion_uint32;
typedef unsigned __int64 ion_uint64;

typedef signed __int64 ion_longlong;
typedef unsigned __int64 ion_ulonglong;

typedef intptr_t ion_intptr;

#define ITYPES_DEFINED	1
#endif

// GNU gcc types

#ifdef __GNUC__

#include <inttypes.h>

typedef int8_t ion_int8;
typedef int16_t ion_int16;
typedef int32_t ion_int32;
typedef int64_t ion_int64;

typedef uint8_t ion_uint8;
typedef uint16_t ion_uint16;
typedef uint32_t ion_uint32;
typedef uint64_t ion_uint64;

// TODO: check if the gcc version supports C99 long long's
typedef signed long long ion_longlong;
typedef unsigned long long ion_ulonglong;

typedef intptr_t ion_intptr;

#define ITYPES_DEFINED	1
#endif

// Unknown platform - break

#ifndef ITYPES_DEFINED
#error Sorry, your platform is unknown. Define the correct types in include/itypes.h
#endif

#endif /*__ION_CORE_INTTYPES_H*/
