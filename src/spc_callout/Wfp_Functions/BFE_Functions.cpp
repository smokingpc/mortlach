#include "../CommonHeaders.h"
#include "Callout_Functions.h"

void CALLBACK BFE_StateChangeCallback(
    _Inout_ VOID* Context,
    _In_ FWPM_SERVICE_STATE BfeState)
{
    CFuncLog flog(__FUNCTION__);
    UNREFERENCED_PARAMETER(Context);
//    NTSTATUS status = STATUS_UNSUCCESSFUL;
//    PDEVICE_CONTEXT ctx = (PDEVICE_CONTEXT)Context;

    switch (BfeState)
    {
    case FWPM_SERVICE_RUNNING:
    {
        //if (pDeviceData->pEngineHandle == 0)
        //{
        //    status = KrnlHlprFwpmSessionCreateEngineHandle(&(pDeviceData->pEngineHandle));
        //    HLPR_BAIL_ON_FAILURE(status);
        //}

        break;
    }
    case FWPM_SERVICE_STOP_PENDING:
    {
        //KrnlHlprFwpmSessionDestroyEngineHandle(&(pDeviceData->pEngineHandle));

        break;
    }
    }

    return;
}
