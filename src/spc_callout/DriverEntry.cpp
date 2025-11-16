#include "CommonHeaders.h"
#include "Wfp_Functions/Wfp_Functions.h"
#include "DriverEntry.h"

//For easier debugging
WDFDRIVER g_DriverHandle = nullptr;
WDFDEVICE g_DeviceHandle = nullptr;
PDEVICE_CONTEXT g_DevCtx = nullptr;
PWDFDEVICE_INIT g_DevInit = nullptr;

EXTERN_C_START
VOID 
WdfEvt_DriverUnload(_In_ WDFDRIVER Driver)
{
    CFuncLog flog(__FUNCTION__);
    UNREFERENCED_PARAMETER(Driver);
}

VOID
WdfEvt_DriverContextCleanup(_In_ WDFOBJECT DriverObject)
{
    CFuncLog flog(__FUNCTION__);
    UNREFERENCED_PARAMETER(DriverObject);
    PAGED_CODE();
}

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    CFuncLog flog(__FUNCTION__);
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;

    if ((nullptr != g_DriverHandle) || (nullptr != g_DeviceHandle))
    {
        PrintError("[Spc Error] Duplicated loading! SPC Callout Driver only allow one instance!!\n");
        status = STATUS_DUPLICATE_NAME;
        goto ERROR;
    }

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
        &g_DriverHandle);

    if (NT_SUCCESS(status))
    {
        //DeviceInit object is created in AddDevice() event but legacy doesn't 
        //have this event. so we have to create it by ourself.
        g_DevInit = WdfControlDeviceInitAllocate(g_DriverHandle, &SDDL_DEVOBJ_KERNEL_ONLY);
        if (nullptr == g_DevInit)
        {
            status = STATUS_MEMORY_NOT_ALLOCATED;
            goto ERROR;
        }

        status = Wdf_CreateDevice(g_DeviceHandle, g_DevInit);

        if (!NT_SUCCESS(status))
            goto ERROR;
    }
    else
    {
        PrintError("[Spc Error] WdfDriverCreate() error=%x\n", status);
        goto ERROR;
    }
    
    PDEVICE_CONTEXT devctx = DeviceGetContext(g_DriverHandle);

    status = Wfp_Setup(devctx);
    if (!NT_SUCCESS(status))
    {
        Wfp_Teardown(devctx);
        goto ERROR;
    }

    return status;
ERROR:
    if (nullptr != g_DevInit)
        WdfDeviceInitFree(g_DevInit);

    return status;
}
EXTERN_C_END
