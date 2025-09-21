#pragma once

typedef struct _DEVICE_CONTEXT
{
    ULONG PlaceHolder;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

NTSTATUS CreateWdfDevice(_Inout_ PWDFDEVICE_INIT DeviceInit);
NTSTATUS InitDeviceContext(_Inout_ PDEVICE_CONTEXT devctx);