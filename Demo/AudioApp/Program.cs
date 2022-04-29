using System;

namespace AudioApp
{
    internal class Program
    {
        static void Main(string[] args)
        {
            AudioEndPointController.Endpoint endpoint = new AudioEndPointController.Endpoint();

            Console.WriteLine("Nuber of audio devices: {0}", endpoint.GetDeviceCount());
            Console.WriteLine("Current audio device name: {0}", endpoint.GetDeviceName());
            Console.WriteLine("Windows version: {0} or {1}", endpoint.ThisWindowsVersion
                , Environment.OSVersion);

            endpoint.SetNextEndpoint();

            Console.ReadLine();
        }
    }
}
