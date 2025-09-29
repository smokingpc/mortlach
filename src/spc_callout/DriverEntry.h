#pragma once

//DriverEntry.h defines all functions related about WDF only.
// NDIS related functions defined in Callout_Functions.h
// 
// Name Rules of functions =>
// "WdfEvt_" prefix: WDF framework callback functions.
// "Wdf_" prefix: not callback but used to do something in WDF related behavior.

EXTERN_C_START
//
// WDFDRIVER Events
//
DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_UNLOAD WdfEvt_DriverUnload;
EVT_WDF_OBJECT_CONTEXT_CLEANUP WdfEvt_DriverContextCleanup;

EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL WdfEvt_IoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP WdfEvt_IoStop;
EXTERN_C_END

NTSTATUS Wdf_QueueInitialize(_In_ WDFDEVICE Device );

//WDF device create
__inline PWDFDEVICE_INIT
Wdf_CreateDeviceInit(_Out_ PWDFDEVICE_INIT dev_init, _In_ WDFDRIVER driver)
{
    //DeviceInit object is created in AddDevice() event but legacy has no AddDevice event.
    //so we have to create it by ourself.
    dev_init = WdfControlDeviceInitAllocate(driver, &SDDL_DEVOBJ_KERNEL_ONLY);
    return dev_init;
}

NTSTATUS Wdf_CreateDevice(_Out_ WDFDEVICE& device, _In_ PWDFDEVICE_INIT dev_init);
NTSTATUS InitDeviceContext(_Inout_ PDEVICE_CONTEXT devctx);

//WDF Power and Pnp functions
void Wdf_SetupPowerAndPnp(_In_ PWDFDEVICE_INIT dev_init);
