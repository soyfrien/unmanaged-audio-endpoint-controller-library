using AudioEndPointController.Helpers;
using Microsoft.Win32;
using System;
using System.Runtime.InteropServices;

namespace AudioEndPointController
{
    public class Endpoint : WindowsNTVersions
    {
        public int CurrentEndpoint { get; set; }
        public int DeviceCount { get; set; }
        public KernelName ThisWindowsVersion { get; set; }

        private RegistryKey hKey = Registry.CurrentUser;
        private string subKeyPath = "Software\\PPDAC\\AudioEndPointController";

        public Endpoint(bool setNextEndpoint = false)
        {
            ThisWindowsVersion = GetWindowsNTVersion();
            GetCurrentEndpoint();
            SetDeviceCount();

            if (setNextEndpoint)
                SetNextEndpoint();
        }

        public int GetCurrentEndpoint()
        {
            RegistryStore registry = new RegistryStore(hKey, subKeyPath);
            CurrentEndpoint = Convert.ToInt32(registry.ReadKey("Current Endpoint"));

            return CurrentEndpoint;
        }
        public int GetDeviceCount()
        {
            return CountAudioPlaybackDevices();
        }
        public string GetDeviceName()
        {
            return GetEndpointName(CurrentEndpoint);
        }
        public void SetDeviceCount()
        {
            DeviceCount = GetDeviceCount();

            RegistryStore registry = new RegistryStore(hKey, subKeyPath);
            registry.WriteKey("Device Count", DeviceCount, RegistryValueKind.DWord);
        }
        public void SetNextEndpoint()
        {
            PrepareNextEndpoint();
            SetEndpoint(CurrentEndpoint, ThisWindowsVersion);
        }
        private void PrepareNextEndpoint()
        {
            RegistryStore registry = new RegistryStore(hKey, subKeyPath);
            CurrentEndpoint = Convert.ToInt32(registry.ReadKey("Current Endpoint"));

            if (CurrentEndpoint == DeviceCount - 1)
            {
                CurrentEndpoint = 0;
                registry.WriteKey("Current Endpoint", CurrentEndpoint, RegistryValueKind.DWord);
            }
            else if (CurrentEndpoint < DeviceCount - 1)
            {
                CurrentEndpoint += 1;
                registry.WriteKey("Current Endpoint", CurrentEndpoint, RegistryValueKind.DWord);
            }
        }

        private void SetEndpoint(int deviceID, KernelName NTx)
        {
            if (NTx != KernelName.WinOutdated)
                CommitEndpoint(deviceID, (int)NTx);
        }

        [DllImport(@"EndpointController.dll", EntryPoint = "#1")]
        private static extern void CommitEndpoint(int deviceID, int NTx);

        [DllImport(@"EndpointController.dll", EntryPoint = "#2")]
        private static extern int CountAudioPlaybackDevices();

        [DllImport(@"EndpointController.dll", EntryPoint = "#3")]
        [return: MarshalAs(UnmanagedType.BStr)]
        private static extern string GetEndpointName(int deviceID);
    }
}
