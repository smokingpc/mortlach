/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_SPC_WDF_CALLOUT,
    0xa4fd72d6,0x8ad8,0x42c9,0xb1,0x1b,0xb4,0xff,0xa8,0x7b,0xd0,0x50);
// {a4fd72d6-8ad8-42c9-b11b-b4ffa87bd050}
