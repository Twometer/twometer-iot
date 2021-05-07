using System;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.Net
{
    public class BridgeDiscovery
    {

        public static async Task<Bridge> Discover()
        {
            var udpClient = new UdpClient();

            var endpoint = new IPEndPoint(GetBroadcastIp(), 11210);
            var message = Encoding.UTF8.GetBytes("DISCOV:LumiIoT.Bridge");
            await udpClient.SendAsync(message, message.Length, endpoint);
            var ip = (await ReadBridgeResult(udpClient)).RemoteEndPoint.Address;
            return new Bridge(ip);
        }

        private static async Task<UdpReceiveResult> ReadBridgeResult(UdpClient client)
        {
            while (true)
            {
                var packet = await client.ReceiveAsync();
                var data = Encoding.UTF8.GetString(packet.Buffer);
                if (data.StartsWith("BHELLO:"))
                {
                    return packet;
                }
            }
        }

        private static IPAddress GetBroadcastIp()
        {
            var endpoint = NetworkInterface.GetAllNetworkInterfaces()
                .OrderByDescending(iface => iface.GetIPStatistics().BytesReceived + iface.GetIPStatistics().BytesSent)
                .Where(iface => iface.OperationalStatus == OperationalStatus.Up)
                .Select(iface => iface.GetIPProperties().UnicastAddresses
                    .Where(u => u.Address.AddressFamily == AddressFamily.InterNetwork)
                    .FirstOrDefault())
                .FirstOrDefault();

            if (endpoint == null)
                throw new IOException("Failed to find broadcast IP. Make sure that you are connected to the internet.");
            else
                return CalculateBroadcastIp(endpoint.Address, endpoint.IPv4Mask);
        }

        private static IPAddress CalculateBroadcastIp(IPAddress address, IPAddress mask)
        {
            uint ipAddress = BitConverter.ToUInt32(address.GetAddressBytes(), 0);
            uint ipMaskV4 = BitConverter.ToUInt32(mask.GetAddressBytes(), 0);
            uint broadCastIpAddress = ipAddress | ~ipMaskV4;
            return new IPAddress(BitConverter.GetBytes(broadCastIpAddress));
        }

    }
}
