/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD WfpCallout_EvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP WfpCallout_EvtDriverContextCleanup;

EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL WfpCallout_EvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP WfpCallout_EvtIoStop;

typedef struct _QUEUE_CONTEXT {
    ULONG PlaceHolder;
} QUEUE_CONTEXT, * PQUEUE_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS WfpCallout_QueueInitialize(_In_ WDFDEVICE Device );
EXTERN_C_END
