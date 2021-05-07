using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.Net
{
    public class Bridge
    {
        public IPAddress IpAddress { get; } 
        private string baseUrl;

        public Bridge(IPAddress address)
        {
            IpAddress = address;
            baseUrl = $"http://{address}/";
        }

    }
}
