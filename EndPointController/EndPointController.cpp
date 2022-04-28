// EndPointController.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <wchar.h>
#include <tchar.h>
#include "windows.h"
#include "Mmdeviceapi.h"
#include "IPolicyConfig.h"
#include "Propidl.h"
#include "Functiondiscoverykeys_devpkey.h"

#include <SDKDDKVer.h>

#pragma once
#define DllExport   __declspec( dllexport )

using namespace std;

class CoreAudioController
{
	// To be called from C# project with https://github.com/ppdac/Helpers.WinNT/blob/main/KernelNames.cs
	void SetDefaultAudioPlaybackDevice(LPCWSTR devID, int NTx)
	{
		ERole reserved = eConsole;

		if (NTx != 0)
		{
			IPolicyConfig* pPolicyConfig;
			HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_ALL, __uuidof(IPolicyConfig), (LPVOID*)&pPolicyConfig);
			if (SUCCEEDED(hr))
			{
				hr = pPolicyConfig->SetDefaultEndpoint(devID, reserved);
				pPolicyConfig->Release();
			}
		}
		else if (NTx == 0)
		{
			IPolicyConfigVista* pPolicyConfigVista;
			HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_ALL, __uuidof(IPolicyConfig), (LPVOID*)&pPolicyConfigVista);
			if (SUCCEEDED(hr))
			{
				hr = pPolicyConfigVista->SetDefaultEndpoint(devID, reserved);
				pPolicyConfigVista->Release();
			}
		}
	}

	DllExport void CommitEndpoint(INT deviceID, int NTx)
	{
		IMMDeviceEnumerator* pEnum = NULL;
		// Create a multimedia device enumerator.
		HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		if (SUCCEEDED(hr))
		{
			IMMDeviceCollection* pDevices;
			// Enumerate the output devices.
			hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
			if (SUCCEEDED(hr))
			{
				UINT count;
				pDevices->GetCount(&count);
				if (SUCCEEDED(hr))
				{
					for (UINT i = 0; i < count; i++)
					{
						IMMDevice* pDevice;
						hr = pDevices->Item(i, &pDevice);
						if (SUCCEEDED(hr))
						{
							LPWSTR wstrID = NULL;
							hr = pDevice->GetId(&wstrID);
							if (SUCCEEDED(hr))
							{
								IPropertyStore* pStore;
								hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
								if (SUCCEEDED(hr))
								{
									PROPVARIANT friendlyName;
									PropVariantInit(&friendlyName);
									hr = pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
									if (SUCCEEDED(hr))
									{
										if (i == deviceID)
											SetDefaultAudioPlaybackDevice(wstrID, NTx);

										PropVariantClear(&friendlyName);
									}
									pStore->Release();
								}
							}
							pDevice->Release();
						}
					}
				}
				pDevices->Release();
			}
			pEnum->Release();
		}
	}

	DllExport INT CountAudioPlaybackDevices()
	{
		UINT count = 0;
		IMMDeviceEnumerator* pEnum = NULL;

		HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		if (SUCCEEDED(hr))
		{
			IMMDeviceCollection* pDevices;
			hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);

			if (SUCCEEDED(hr))
			{
				hr = pDevices->GetCount(&count);
				pDevices->Release();
			}

			pEnum->Release();
		}

		return count;
	}

	DllExport BSTR GetEndpointName(int deviceID)
	{
		BSTR bstrFriendlyName = NULL;
		IMMDeviceEnumerator* pEnum = NULL;
		// Create a multimedia device enumerator.
		HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		if (SUCCEEDED(hr))
		{
			IMMDeviceCollection* pDevices;
			// Enumerate the output devices.
			hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
			if (SUCCEEDED(hr))
			{
				UINT count;
				pDevices->GetCount(&count);
				if (SUCCEEDED(hr))
				{
					for (UINT i = 0; i < count; i++)
					{
						IMMDevice* pDevice;
						hr = pDevices->Item(i, &pDevice);
						if (SUCCEEDED(hr))
						{
							LPWSTR wstrID = NULL;
							hr = pDevice->GetId(&wstrID);
							if (SUCCEEDED(hr))
							{
								IPropertyStore* pStore;
								hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
								if (SUCCEEDED(hr))
								{
									PROPVARIANT friendlyName;
									PropVariantInit(&friendlyName);
									hr = pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
									if (SUCCEEDED(hr))
									{
										if (i == deviceID)
										{
											BSTR pBstr = SysAllocString(friendlyName.pwszVal);
											bstrFriendlyName = pBstr;
											SysFreeString(pBstr);
										}

										PropVariantClear(&friendlyName);
									}
									pStore->Release();
								}
							}
							pDevice->Release();
						}
					}
				}
				pDevices->Release();
			}
			pEnum->Release();
		}

		return bstrFriendlyName;
	};
};