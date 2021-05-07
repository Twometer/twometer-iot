using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
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

        public async Task<List<Device>> GetDevices()
        {
            return JsonConvert.DeserializeObject<List<Device>>(await Get("devices"));
        }

        private async Task<string> Get(string url)
        {
            var request = WebRequest.CreateHttp(baseUrl + url);
            request.Method = "GET";
            using (var response = await request.GetResponseAsync())
            {
                using (var reader = new StreamReader(response.GetResponseStream()))
                {
                    return reader.ReadToEnd();
                }
            }
        }


    }
}
