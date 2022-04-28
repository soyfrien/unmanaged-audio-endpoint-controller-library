#  Unmanaged Windows Audio Endpoint Controller Library
See `PolicyConfig.h` and the endpoint controller here: https://github.com/davuxcom/audio-endpoint-controller.

# Entrypoints implemented
```
>dumpbin /exports EndPointController.dll
Microsoft (R) COFF/PE Dumper Version 14.32.31328.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file EndPointController.dll

File Type: DLL

  Section contains the following exports for EndPointController.dll

    00000000 characteristics
    FFFFFFFF time date stamp
        0.00 version
           1 ordinal base
           3 number of functions
           3 number of names

    ordinal hint RVA      name

          1    0 00001000 ?CommitEndpoint@CoreAudioController@@AAEXHH@Z = ?CommitEndpoint@CoreAudioController@@AAEXHH@Z (private: void __thiscall CoreAudioController::CommitEndpoint(int,int))
          2    1 00001170 ?CountAudioPlaybackDevices@CoreAudioController@@AAEHXZ = ?CountAudioPlaybackDevices@CoreAudioController@@AAEHXZ (private: int __thiscall CoreAudioController::CountAudioPlaybackDevices(void))
          3    2 000011E0 ?GetEndpointName@CoreAudioController@@AAEPA_WH@Z = ?GetEndpointName@CoreAudioController@@AAEPA_WH@Z (private: wchar_t * __thiscall CoreAudioController::GetEndpointName(int))

  Summary

        1000 .data
        1000 .rdata
        1000 .reloc
        1000 .rsrc
        1000 .text
```
