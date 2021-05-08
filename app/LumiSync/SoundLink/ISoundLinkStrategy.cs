using LumiSync.Audio;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.SoundLink
{
    public interface ISoundLinkStrategy
    {
        Color Update(SpectrumAnalyzer spectrum);
    }
}
