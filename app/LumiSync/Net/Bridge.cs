using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Drawing;
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

        public async Task SetColor(string device, string property, Color color)
        {
            try
            {
                var request = WebRequest.CreateHttp($"{baseUrl}devices/{device}/{property}");
                request.Method = "PUT";
                request.ContentType = "application/json";

                var payload = new
                {
                    value = new
                    {
                        hue = color.GetHue(),
                        saturation = color.GetSaturation(),
                        brightness = color.GetBrightness()
                    }
                };

                //Console.WriteLine(JsonConvert.SerializeObject(payload));

                var requestStream = await request.GetRequestStreamAsync();
                var writer = new StreamWriter(requestStream);
                writer.Write(JsonConvert.SerializeObject(payload));
                writer.Flush();

                await request.GetResponseAsync();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }

    }
}
