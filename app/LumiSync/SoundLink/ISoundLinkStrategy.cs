using LumiSync.Audio;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace LumiSync.SoundLink
{
    public interface ISoundLinkStrategy
    {
        Color Update(SpectrumAnalyzer spectrum);
    }
}
