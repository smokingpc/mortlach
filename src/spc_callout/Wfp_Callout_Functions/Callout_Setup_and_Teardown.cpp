#include "../CommonHeaders.h"
#include "Callout_Functions.h"

static NTSTATUS Wfp_RegisterProvider(PDEVICE_CONTEXT devctx)
{
    UNREFERENCED_PARAMETER(devctx);
    return STATUS_SUCCESS;
}

NTSTATUS
Wfp_SetupCallouts(PDEVICE_CONTEXT devctx)
{
    NTSTATUS status = STATUS_UNSUCCESSFUL;

    status = Wfp_RegisterProvider(devctx);
    if (!NT_SUCCESS(status))
        goto END;

    //callouts should be register and add together.
    //"register" first, then "add"
    //if no register, FWP_ACTION_CALLOUT_INSPECTION type of callout will be ignored.
END:
    return status;
}

void
Wfp_TeardownCallouts(PDEVICE_CONTEXT devctx)
{
    UNREFERENCED_PARAMETER(devctx);
}
