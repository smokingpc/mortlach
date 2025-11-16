#pragma once

NTSTATUS Wfp_Setup(PDEVICE_CONTEXT devctx);
void Wfp_Teardown(PDEVICE_CONTEXT devctx);

void InitWfpSessionContext(PWFP_KERNEL_CONTEXT ctx);
NTSTATUS CreateWfpEngineSession(PDEVICE_CONTEXT devctx);
void CloseWfpEngineSession(PDEVICE_CONTEXT devctx);
