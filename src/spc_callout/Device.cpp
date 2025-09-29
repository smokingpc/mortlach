#include "CommonHeaders.h"
#include "DriverEntry.h"
extern PDEVICE_CONTEXT DevCtx;

NTSTATUS
Wdf_CreateDevice(_Out_ WDFDEVICE &device, _In_ PWDFDEVICE_INIT dev_init)
{
    WDF_OBJECT_ATTRIBUTES attribute;
    PDEVICE_CONTEXT devctx = nullptr;
    NTSTATUS status = STATUS_SUCCESS;

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attribute, DEVICE_CONTEXT);

    WdfDeviceInitSetDeviceType(dev_init, FILE_DEVICE_NETWORK);
    status = WdfDeviceCreate(&dev_init, &attribute, &device);

    if (!NT_SUCCESS(status))
        goto END;

    devctx = DeviceGetContext(device);
    status = InitDeviceContext(devctx);
    if (!NT_SUCCESS(status))
        goto END;

    devctx->Device = device;
    DevCtx = devctx;    //for debugging.

    status = WdfDeviceCreateDeviceInterface(
        device,
        &GUID_DEVINTERFACE_SPC_WDF_CALLOUT,
        NULL // ReferenceString
    );

    if (!NT_SUCCESS(status))
        goto END;

    // Initialize the I/O Package and any Queues
    status = Wdf_QueueInitialize(device);

END:
    return status;
}

NTSTATUS InitDeviceContext(_Inout_ PDEVICE_CONTEXT devctx)
{
    RtlZeroMemory(devctx, sizeof(DEVICE_CONTEXT));

    //WFP provider information, used for FwpmProviderAdd0()
    devctx->Provider.serviceName = PROVIDER_SVCNAME;
    devctx->Provider.providerKey = GUID_WFP_PROVIDER_GUID;
    devctx->Provider.displayData.name = PROVIDER_NAME;
    devctx->Provider.displayData.description = PROVIDER_DESCRIPTION;



    return STATUS_SUCCESS;
}
