#include "CommonHeaders.h"
#include "Wfp_Callout_Functions/Callout_Functions.h"
#include "DriverEntry.h"

//For easier debugging
WDFDRIVER DriverHandle;
WDFDEVICE DeviceHandle;
PDEVICE_CONTEXT DevCtx = nullptr;

EXTERN_C_START
VOID 
WdfEvt_DriverUnload(_In_ WDFDRIVER Driver)
{
    UNREFERENCED_PARAMETER(Driver);
}

VOID
WdfEvt_DriverContextCleanup(_In_ WDFOBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    PAGED_CODE();
}

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
{
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = WdfEvt_DriverContextCleanup;

    //enforce this driver allocated pools should be NonPagedPoolNx automatically. 
    ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

    //to build a KMDF driver as a legacy driver, do it by this way.
    WDF_DRIVER_CONFIG_INIT(&config, WDF_NO_EVENT_CALLBACK);
    config.DriverInitFlags |= WdfDriverInitNonPnpDriver;    //no need to go thru PnpManager. this is for legacy driver only.
    config.EvtDriverUnload = WdfEvt_DriverUnload;

    status = WdfDriverCreate(DriverObject,
        RegistryPath,
        &attributes,
        &config,
        &DriverHandle);

    if (NT_SUCCESS(status))
    {
        PWDFDEVICE_INIT dev_init = nullptr;
        Wdf_CreateDeviceInit(dev_init, DriverHandle);
        if (nullptr == dev_init)
        {
            status = STATUS_MEMORY_NOT_ALLOCATED;
            goto END;
        }

        status = Wdf_CreateDevice(DeviceHandle, dev_init);

        if (!NT_SUCCESS(status))
        {
            //PrintError("[CALLOUT] WfpCallout_CreateDevice() failed(%X)\n", status);
            goto END;
        }
    }
    else
    {
        //PrintError("[CALLOUT] WdfDriverCreate() failed(%X)\n", status);
        goto END;
    }
    
    PDEVICE_CONTEXT devctx = DeviceGetContext(DeviceHandle);
    status = Wfp_SetupCallouts(devctx);
    if (!NT_SUCCESS(status))
        Wfp_TeardownCallouts(devctx);

END:
    return status;
}
EXTERN_C_END
