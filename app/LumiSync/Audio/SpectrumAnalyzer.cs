using CSCore;
using CSCore.DSP;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.Audio
{
    public class SpectrumAnalyzer
    {
        private const FftSize size = FftSize.Fft4096;
        private const int sizeInt = (int)size;

        private FftProvider fft = new FftProvider(2, size);
        private float[] sampleBuffer = new float[8192];
        private float[] fftBuffer = new float[sizeInt];
        private ISampleSource source;

        public SpectrumAnalyzer(ISampleSource source)
        {
            this.source = source;
        }

        public void Update()
        {
            int read = source.Read(sampleBuffer, 0, sampleBuffer.Length);
            fft.Add(sampleBuffer, read);
            fft.GetFftData(fftBuffer);
        }

        public float[] BandFilter(int bands)
        {
            float[] data = new float[bands];
            int sizeHalf = sizeInt / 2;

            int bandSize = sizeHalf / bands;
            for (int i = 0; i < bands; i++)
            {
                float av = 0;
                int baseIdx = i * bandSize;
                for (int j = 0; j < bandSize; j++)
                {
                    av += fftBuffer[baseIdx + j];
                    av += fftBuffer[fftBuffer.Length - 1 - (baseIdx + j)];
                }
                av /= 2 * bandSize;
                data[i] = LinearScale(av);
            }
            return data;
        }

        private float LinearScale(float val)
        {
            return val * 100000;
        }

    }
}
