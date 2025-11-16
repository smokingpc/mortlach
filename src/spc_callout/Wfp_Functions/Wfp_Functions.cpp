#include "../CommonHeaders.h"
#include "BFE_Functions.h"
#include "Callout_Functions.h"
#include "Wfp_Functions.h"

static NTSTATUS BFE_SubscribeStateChange(PDEVICE_CONTEXT devctx)
{
    if (nullptr == devctx->WdmDevObj)
        return STATUS_INVALID_DEVICE_OBJECT_PARAMETER;
    //BFE service is important component for WFP running.
    //if BFE stopped we have to stop filtering.
    NTSTATUS status = FwpmBfeStateSubscribeChanges(devctx->WdmDevObj,
        BFE_StateChangeCallback,
        devctx,
        &devctx->BfeStateHandle);
    if (!NT_SUCCESS(status))
        PrintError("[Spc Error] FwpmBfeStateSubscribeChanges() error=%x\n", status);

    return status;
}

NTSTATUS CreateWfpEngineSession(PDEVICE_CONTEXT devctx)
{
    //GUID_WFP_SESSION_KERNEL
    NTSTATUS status = FwpmEngineOpen(0,
                                    RPC_C_AUTHN_WINNT,
                                    0,
                                    &devctx->WfpCtx.Session,
                                    &devctx->WfpCtx.WfpEngine);
    if (!NT_SUCCESS(status))
        PrintError("[Spc Error] FwpmEngineOpen() error=%x\n", status);

    return status;
}
void CloseWfpEngineSession(PDEVICE_CONTEXT devctx)
{
    NTSTATUS status = FwpmEngineClose(devctx->WfpCtx.WfpEngine);
    if (!NT_SUCCESS(status))
        PrintError("[Spc Error] FwpmEngineClose() error=%X\n", status);
}
void BFE_UnsubscribeStateChange(PDEVICE_CONTEXT devctx)
{
    if (nullptr == devctx->BfeStateHandle)
        return;
    NTSTATUS status = FwpmBfeStateUnsubscribeChanges0(devctx->BfeStateHandle);
    if (NT_SUCCESS(status))
        devctx->BfeStateHandle = nullptr;
    else
        PrintError("[Spc Error] FwpmBfeStateUnsubscribeChanges0() error=%x\n", status);
}


//==================================================================
void InitWfpSessionContext(PWFP_KERNEL_CONTEXT ctx)
{
    ctx->Session.displayData.name = WFP_SESSION_NAME;
    ctx->Session.displayData.description = WFP_SESSION_DESCRIPTION;
    ctx->Session.sessionKey = GUID_WFP_SESSION_KERNEL;
    ctx->Session.flags = FWPM_SESSION_FLAG_DYNAMIC;    // any objects added during the session are automatically deleted when the session ends.
    ctx->Session.kernelMode = TRUE;
#if 0
    //BFE provider information, used for FwpmProviderAdd0()
    ctx->Provider.providerKey = GUID_WFP_PROVIDER_SPC;
    ctx->Provider.serviceName = PROVIDER_SVCNAME;
    ctx->Provider.displayData.name = PROVIDER_DISPLAY_NAME;
    ctx->Provider.displayData.description = PROVIDER_DESCRIPTION;
#endif
}

NTSTATUS Wfp_Setup(PDEVICE_CONTEXT devctx)
{
    CFuncLog flog(__FUNCTION__);
    NTSTATUS status = STATUS_UNSUCCESSFUL;

    //1. BFE STATE SUBSCRIPTION.
    status = BFE_SubscribeStateChange(devctx);
    if (!NT_SUCCESS(status))
        goto ERROR;

    if (FWPM_SERVICE_RUNNING != FwpmBfeStateGet()) {
        goto ERROR;
    }
    //2. OPEN WFP Engine Session
    status = CreateWfpEngineSession(devctx);
    if (!NT_SUCCESS(status))
        goto ERROR;

    //3. Create injection hadnles in different directions
    //4. Create redirection handles



    return status;
ERROR:
    if (nullptr != devctx->BfeStateHandle) {
        BFE_UnsubscribeStateChange(devctx);
    }
    return status;
}
void Wfp_Teardown(PDEVICE_CONTEXT devctx)
{
    CFuncLog flog(__FUNCTION__);

    BFE_UnsubscribeStateChange(devctx);
}
