#include "../CommonHeaders.h"
#include "BFE_Functions.h"
#include "Callout_Functions.h"

NTSTATUS
Wfp_SetupCallouts(PDEVICE_CONTEXT devctx)
{
    CFuncLog flog(__FUNCTION__);
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    UNREFERENCED_PARAMETER(status);
    UNREFERENCED_PARAMETER(devctx);

    return status;
}

void Wfp_TeardownCallouts(PDEVICE_CONTEXT devctx)
{
    CFuncLog flog(__FUNCTION__);
    UNREFERENCED_PARAMETER(devctx);
}
