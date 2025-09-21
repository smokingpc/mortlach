#pragma once

//DO NOT use virtual function in kernel!
class CAutoSpinLockBase
{
protected:
    KSPIN_LOCK* Lock = nullptr;
    KIRQL OldIrql = PASSIVE_LEVEL;
};

//Generic SpinLock
class CSpinLock : CAutoSpinLockBase
{
public:
    CSpinLock(KSPIN_LOCK* lock, bool do_lock = true);
    CSpinLock(CSpinLock& old) = delete;
    ~CSpinLock();

    void Acquire();
    void Release();
};

//SpinLock for Threaded DPC
class CQueuedSpinLock : public CAutoSpinLockBase
{
public:
    CQueuedSpinLock(KSPIN_LOCK* lock, bool do_lock = true);
    CQueuedSpinLock(CQueuedSpinLock& old) = delete;
    ~CQueuedSpinLock();
    void Acquire();
    void Release();
protected:
    KLOCK_QUEUE_HANDLE QueueHandle;
};

//SpinLock at DPC LEVEL or above
class CSpinLockAtDpc : public CAutoSpinLockBase
{
public:
    CSpinLockAtDpc(KSPIN_LOCK* lock, bool do_lock = true);
    CSpinLockAtDpc(CSpinLockAtDpc& old) = delete;
    ~CSpinLockAtDpc();
    void Acquire();
    void Release();
};
