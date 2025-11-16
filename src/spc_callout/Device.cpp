#include "CommonHeaders.h"
#include "DriverEntry.h"
#include "Wfp_Functions/Wfp_Functions.h"

extern PDEVICE_CONTEXT g_DevCtx;

VOID WdfEvt_CleanupDeviceObject(_In_ WDFOBJECT DeviceObject)
{
    //close WfpEngine session when this device object reference to Zero.
    //This device is going down...
    CFuncLog flog(__FUNCTION__);
    WDFDEVICE device = (WDFDEVICE)DeviceObject;
    PDEVICE_CONTEXT devctx = DeviceGetContext(device);
    CloseWfpEngineSession(devctx);
}

NTSTATUS
Wdf_CreateDevice(_Out_ WDFDEVICE &device, _In_ PWDFDEVICE_INIT dev_init)
{
    CFuncLog flog(__FUNCTION__);
    WDF_OBJECT_ATTRIBUTES attribute;
    PDEVICE_CONTEXT devctx = nullptr;
    WDFQUEUE io_queue = nullptr;
    NTSTATUS status = STATUS_SUCCESS;

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attribute, DEVICE_CONTEXT);
    attribute.EvtCleanupCallback = WdfEvt_CleanupDeviceObject;

    WdfDeviceInitSetDeviceType(dev_init, FILE_DEVICE_NETWORK);
    status = WdfDeviceCreate(&dev_init, &attribute, &device);
    if (!NT_SUCCESS(status)) {
        PrintError("[Spc Error] WdfDeviceCreate() error=%x\n", status);
        goto END;
    }

    status = WdfDeviceCreateDeviceInterface(
        device,
        &GUID_DEVINTERFACE_SPC_WDF_CALLOUT,
        NULL // ReferenceString
    );
    if (!NT_SUCCESS(status)) {
        PrintError("[Spc Error] WdfDeviceCreateDeviceInterface() error=%x\n", status);
        goto END;
    }

    // Initialize the I/O Package and any Queues
    status = Wdf_QueueInitialize(device, &io_queue);
    if (!NT_SUCCESS(status)) {
        PrintError("[Spc Error] Wdf_QueueInitialize() error=%x\n", status);
        goto END;
    }

    devctx = DeviceGetContext(device);
    InitDeviceContext(device, io_queue, devctx);
    g_DevCtx = devctx;    //for debugging.

END:
    return status;
}

void InitDeviceContext(
    _In_ WDFDEVICE& device, 
    _In_ WDFQUEUE &io_queue, 
    _In_ PDEVICE_CONTEXT devctx)
{
    CFuncLog flog(__FUNCTION__);
    RtlZeroMemory(devctx, sizeof(DEVICE_CONTEXT));
    devctx->WdfDevice = device;
    devctx->IoQueue = io_queue;
    devctx->WdmDevObj = WdfDeviceWdmGetDeviceObject(devctx->WdfDevice);
    InitWfpSessionContext(&devctx->WfpCtx);
}
