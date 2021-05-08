using LumiSync.Audio;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.SoundLink
{
    public class ColorCycleStrategy : ISoundLinkStrategy
    {
        private readonly PeakDetector detector = new PeakDetector();
        private const int Bands = 64;
        private const int Threshold = 1000;

        private readonly Color[] colors =
        {
            Color.Red,
            Color.Lime,
            Color.Blue,
            Color.Aqua,
            Color.Magenta
        };

        private int colorIdx = 0;

        public override string ToString()
        {
            return "Color cycle";
        }

        public Color Update(SpectrumAnalyzer spectrum)
        {
            var bands = spectrum.BandFilter(Bands);
            var peak = detector.Detect(bands[0]);

            if (peak > Threshold)
            {
                colorIdx = (colorIdx + 1) % colors.Length;
            }

            return colors[colorIdx];
        }
    }
}
