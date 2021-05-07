using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.Net
{
    public class Device
    {
        public string DeviceId { get; }
        public string Type { get; }
        public string ModelName { get; }
        public string FriendlyName { get; }
        public string Description { get; }

        public Device(string deviceId, string type, string modelName, string friendlyName, string description)
        {
            DeviceId = deviceId;
            Type = type;
            ModelName = modelName;
            FriendlyName = friendlyName;
            Description = description;
        }
    }
}
