using Microsoft.Win32;

namespace AudioEndPointController.Helpers
{
    public class RegistryStore
    {
        public RegistryKey HKEY { get; set; }
        public string SubKeyPath { get; set; }

        public RegistryStore(RegistryKey hKey, string subKeyPath)
        {
            HKEY = hKey;
            SubKeyPath = subKeyPath;
        }

        public string ReadKey(string keyName)
        {
            RegistryKey registryKey = HKEY;
            RegistryKey subKey = registryKey.OpenSubKey(SubKeyPath);
            if (subKey != null)
                if (subKey.GetValue(keyName) != null)
                    return subKey.GetValue(keyName).ToString();

            return null;
        }

        public void WriteKey(string keyName, object value, RegistryValueKind valueType)
        {
            RegistryKey registryKey = HKEY;
            RegistryKey subKey = registryKey.CreateSubKey(SubKeyPath);

            subKey.SetValue(keyName, value, valueType);
        }
    }
}
