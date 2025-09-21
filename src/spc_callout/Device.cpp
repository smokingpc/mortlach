#include "CommonHeaders.h"

NTSTATUS
CreateWdfDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    WDF_OBJECT_ATTRIBUTES attribute;
    PDEVICE_CONTEXT devctx;
    WDFDEVICE device;
    NTSTATUS status = STATUS_UNSUCCESSFUL;

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attribute, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &attribute, &device);

    if (NT_SUCCESS(status))
    {
        devctx = DeviceGetContext(device);
        devctx->PlaceHolder = 0;
        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_SPC_WDF_CALLOUT,
            NULL // ReferenceString
        );

        if (NT_SUCCESS(status)) {
            //
            // Initialize the I/O Package and any Queues
            //
            status = WfpCallout_QueueInitialize(device);
        }
    }

    return status;
}

NTSTATUS InitDeviceContext(_Inout_ PDEVICE_CONTEXT devctx)
{
    devctx->PlaceHolder = 0;
    return STATUS_SUCCESS;
}
