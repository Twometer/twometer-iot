using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LumiSync.Audio
{
    public class RollingAverage
    {
        private float[] data;
        private int idx;

        public RollingAverage(int size)
        {
            data = new float[size];
        }

        public void Add(float d)
        {
            data[idx] = d;
            idx = (idx + 1) % data.Length;
        }

        public float AverageWith(float d)
        {
            Add(d);
            return Value;
        }

        public float Value => data.Average();

    }
}
