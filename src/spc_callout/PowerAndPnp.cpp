#include "CommonHeaders.h"

static NTSTATUS WdfEvt_DeviceD0Entry(
    WDFDEVICE Device,
    WDF_POWER_DEVICE_STATE PreviousState)
{
    CFuncLog flog(__FUNCTION__);
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(PreviousState);

    return STATUS_SUCCESS;
}
static NTSTATUS WdfEvt_DeviceD0Exit(
    WDFDEVICE Device,
    WDF_POWER_DEVICE_STATE PreviousState)
{
    CFuncLog flog(__FUNCTION__);
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(PreviousState);
    return STATUS_SUCCESS;
}

void Wdf_SetupPowerAndPnp(_In_ PWDFDEVICE_INIT dev_init)
{
    CFuncLog flog(__FUNCTION__);
    WDF_PNPPOWER_EVENT_CALLBACKS  pnp_cb = { 0 };

    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnp_cb);
    pnp_cb.EvtDeviceD0Entry = WdfEvt_DeviceD0Entry;
    pnp_cb.EvtDeviceD0Exit = WdfEvt_DeviceD0Exit;
    //pnp_cb.EvtDevicePrepareHardware = HandleDevicePrepareHardware;
    //pnp_cb.EvtDeviceReleaseHardware = HandleDeviceReleaseHardware;
    //pnp_cb.EvtDeviceSurpriseRemoval = HandleDeviceSurpriseRemoval;
    //pnp_cb.EvtDeviceQueryRemove = HandleDeviceQueryRemove;
    //pnp_cb.EvtDeviceQueryStop = HandleDeviceQueryStop;

    WdfDeviceInitSetPnpPowerEventCallbacks(dev_init, &pnp_cb);
}
