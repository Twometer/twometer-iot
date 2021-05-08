using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.Audio
{
    public class PeakDetector
    {
        private RollingAverage sampleAvg = new RollingAverage(5);
        private RollingAverage diffAvg = new RollingAverage(2);

        private float lastValue;
        private float lastSaturated;

        private float lastMinimum;

        public float Detect(float sample)
        {
            var value = (float)Math.Pow(sampleAvg.AverageWith(sample), 2.2f);
            var diff = diffAvg.AverageWith(value - lastValue);
            lastValue = value;

            var saturated = Saturate(diff, -1, 1);
            var diffSt = lastSaturated - saturated;
            lastSaturated = saturated;

            var peakHeight = 0f;
            if (diffSt != 0)
            {
                // Maximum or Minimum detected

                if (diffSt < 0)
                {
                    // MIN
                    lastMinimum = value;
                }
                else
                {
                    // MAX (a peak)
                    peakHeight = value - lastMinimum;

                }


            }

            return peakHeight;
        }


        private float Saturate(float v, float min, float max)
        {
            if (v < 0)
                v = min;
            else if (v > 0)
                v = max;
            else
                v = 0;

            return v;
        }


    }
}
