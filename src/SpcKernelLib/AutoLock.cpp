#include <wdm.h>
#include "AutoLock.h"
#include "AutoPtr.hpp"

CSpinLock::CSpinLock(KSPIN_LOCK* lock, bool do_lock)
{
    //No checking and no protect. Let it crash if wrong argument assigned.
    //If I protect it here, driver could have timing issue because SpinLock not work.
    //It is more difficult to debug.
    this->Lock = lock;
    if (do_lock)
        Acquire();
}
CSpinLock::~CSpinLock()
{
    Release();
}
void CSpinLock::Acquire()
{
    KeAcquireSpinLock(this->Lock, &this->OldIrql);
}
void CSpinLock::Release()
{
    KeReleaseSpinLock(this->Lock, this->OldIrql);
    //don't cleanup this->Lock, leave some debug information....
    //    this->Lock = nullptr;
}

CQueuedSpinLock::CQueuedSpinLock(KSPIN_LOCK* lock, bool do_lock)
{
    //No checking and no protect. Let it crash if wrong argument assigned.
    //If I protect it here, driver could have timing issue because SpinLock not work.
    //It is more difficult to debug.
    this->Lock = lock;
    if (do_lock)
        Acquire();
}
CQueuedSpinLock::~CQueuedSpinLock()
{
    Release();
}
void CQueuedSpinLock::Acquire()
{
    OldIrql = KeGetCurrentIrql();
    KeAcquireInStackQueuedSpinLock(this->Lock, &this->QueueHandle);
}
void CQueuedSpinLock::Release()
{
    KeReleaseInStackQueuedSpinLock(&this->QueueHandle);
}

CSpinLockAtDpc::CSpinLockAtDpc(KSPIN_LOCK* lock, bool do_lock)
{
    //No checking and no protect. Let it crash if wrong argument assigned.
    //If I protect it here, driver could have timing issue because SpinLock not work.
    //It is more difficult to debug.
    this->Lock = lock;
    if (do_lock)
        Acquire();
}
CSpinLockAtDpc::~CSpinLockAtDpc()
{
    Release();
}
void CSpinLockAtDpc::Acquire()
{
    KeAcquireSpinLockAtDpcLevel(this->Lock);
}
void CSpinLockAtDpc::Release()
{
    KeReleaseSpinLockFromDpcLevel(this->Lock);
    //don't cleanup this->Lock, leave some debug information....
    //    this->Lock = nullptr;
}
