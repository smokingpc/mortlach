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

NTSTATUS Wdf_QueueInitialize(_In_ WDFDEVICE Device, _Inout_ WDFQUEUE *ioqueue);

NTSTATUS Wdf_CreateDevice(_Out_ WDFDEVICE& device, _In_ PWDFDEVICE_INIT dev_init);
void InitDeviceContext(
    _In_ WDFDEVICE& device,
    _In_ WDFQUEUE& ioqueue,
    _In_ PDEVICE_CONTEXT devctx);

//WDF Power and Pnp functions
void Wdf_SetupPowerAndPnp(_In_ PWDFDEVICE_INIT dev_init);
