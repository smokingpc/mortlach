// SpcKernelLib.cpp : Defines the functions for the static library.
//

#include <wdm.h>
#include "SpcKernelLib.h"

#define OVERRUN_TAG     0xDEADDEAD
#define CPP_TAG         ((ULONG)' PPC')

#if NTDDI_VERSION <= NTDDI_WIN10_19H1
    #define EX_ALLOC_POOL       ExAllocatePoolWithTag
#else
    #define EX_ALLOC_POOL       ExAllocatePoolUninitialized
#endif


static void* CppCreateBuffer(size_t size, POOL_TYPE type, ULONG tag, bool clear = true)
{
    //ExAllocatePoolUninitialized create pool with 16bytes alignment in x64,
    // 8bytes alignment in 32bit....
    PVOID ptr = EX_ALLOC_POOL(type, size, tag);
    if (ptr != nullptr && true == clear)
    {
        RtlZeroMemory(ptr, size);
    }
    return ptr;
}

void* __cdecl operator new (size_t size)
{
    return CppCreateBuffer(size, PagedPool, CPP_TAG, false);
}
void* operator new (size_t size, POOL_TYPE type, ULONG tag, bool clear)
{
    if (tag == 0)
        tag = CPP_TAG;

    return CppCreateBuffer(size, type, tag, clear);
}

void* __cdecl operator new[](size_t size)
{
    return CppCreateBuffer(size, PagedPool, CPP_TAG);
}

void* operator new[](size_t size, POOL_TYPE type, ULONG tag, bool clear)
{
    if (tag == 0)
        tag = CPP_TAG;

    return CppCreateBuffer(size, type, tag, clear);
}

void __cdecl operator delete (void* ptr, size_t size)
{
    //don't check ptr nullptr. let it crash if I set wrong argument.
    UNREFERENCED_PARAMETER(size);
    ExFreePool(ptr);
}
void __cdecl operator delete[](void* ptr)
{
    //don't check ptr nullptr. let it crash if I set wrong argument.
    ExFreePool(ptr);
}
void __cdecl operator delete[](void* ptr, size_t size)
{
    //don't check ptr nullptr. let it crash if I set wrong argument.
    UNREFERENCED_PARAMETER(size);
    ExFreePool(ptr);
}

void operator delete(void* ptr, ULONG tag)
{
    ExFreePoolWithTag(ptr, tag);
}
