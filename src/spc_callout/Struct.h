#pragma once
typedef struct _WFP_KERNEL_CONTEXT {
    HANDLE WfpEngine;
    FWPM_SESSION    Session;
    FWPS_CALLOUT    CalloutFwps[MAX_CALLOUTS];
    FWPM_CALLOUT    CalloutFwpm[MAX_CALLOUTS];

#if 0
    //used by usermode Service
    FWPM_PROVIDER   Provider;
#endif
}WFP_KERNEL_CONTEXT, *PWFP_KERNEL_CONTEXT;
