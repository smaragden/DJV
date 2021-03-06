//------------------------------------------------------------------------------
// Copyright (c) 2004-2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvCore/Math.h>

namespace djv
{
    namespace AV
    {
        inline void ColorUtil::rgbToHsv(const float in[3], float out[3])
        {
            const float & min = Core::Math::min(in[0], Core::Math::min(in[1], in[2]));
            const float & max = Core::Math::max(in[0], Core::Math::max(in[1], in[2]));
            out[2] = max;
            out[1] = max != 0.f ? (1.f - min / max) : 0.f;
            const float v = (max - min) * 6.f;
            if (max == min)
            {
                out[0] = 0.f;
            }
            else if (in[0] == max)
            {
                if (in[1] >= in[2])
                {
                    out[0] = (in[1] - in[2]) / v;
                }
                else
                {
                    out[0] = 1.f + (in[1] - in[2]) / v;
                }
            }
            else if (in[1] == max)
            {
                out[0] = 1.f / 3.f + (in[2] - in[0]) / v;
            }
            else if (in[2] == max)
            {
                out[0] = 2.f / 3.f + (in[0] - in[1]) / v;
            }
        }

        inline void ColorUtil::hsvToRgb(const float in[3], float out[3])
        {
            const float h = Core::Math::mod(in[0] * 6.f, 6.f);
            const int   i = Core::Math::floor(h);
            const float f = h - i;
            const float p = in[2] * (1.f - in[1]);
            const float q = in[2] * (1.f - (in[1] * f));
            const float t = in[2] * (1.f - (in[1] * (1.f - f)));
            switch (i)
            {
            case 0:
                out[0] = in[2];
                out[1] = t;
                out[2] = p;
                break;
            case 1:
                out[0] = q;
                out[1] = in[2];
                out[2] = p;
                break;
            case 2:
                out[0] = p;
                out[1] = in[2];
                out[2] = t;
                break;
            case 3:
                out[0] = p;
                out[1] = q;
                out[2] = in[2];
                break;
            case 4:
                out[0] = t;
                out[1] = p;
                out[2] = in[2];
                break;
            case 5:
                out[0] = in[2];
                out[1] = p;
                out[2] = q;
                break;
            }
        }

    } // namespace AV
} // namespace djv
