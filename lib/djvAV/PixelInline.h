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
        inline Pixel::Mask::Mask()
        {
            for (int i = 0; i < channelsMax; ++i)
            {
                mask[i] = true;
            }
        }

        inline Pixel::Mask::Mask(bool mask)
        {
            for (int i = 0; i < channelsMax; ++i)
            {
                this->mask[i] = mask;
            }
        }

        inline Pixel::Mask::Mask(bool red, bool green, bool blue, bool alpha)
        {
            mask[0] = red;
            mask[1] = green;
            mask[2] = blue;
            mask[3] = alpha;
        }

        inline Pixel::Mask::Mask(const bool mask[channelsMax])
        {
            for (int i = 0; i < channelsMax; ++i)
            {
                this->mask[i] = mask[i];
            }
        }

        inline Pixel::Mask::Mask(const Mask & mask)
        {
            for (int i = 0; i < channelsMax; ++i)
            {
                this->mask[i] = mask[i];
            }
        }

        inline Pixel::Mask & Pixel::Mask::operator = (const Mask & mask)
        {
            for (int i = 0; i < channelsMax; ++i)
            {
                this->mask[i] = mask[i];
            }
            return *this;
        }

        inline bool Pixel::Mask::operator [] (int index) const
        {
            return mask[index];
        }

        inline bool & Pixel::Mask::operator [] (int index)
        {
            return mask[index];
        }

        inline Pixel::FORMAT Pixel::format(PIXEL in)
        {
            static const FORMAT data[] =
            {
                L,          L,    L,    L,
                LA,         LA,   LA,   LA,
                RGB,  RGB,  RGB,  RGB,  RGB,
                RGBA,       RGBA, RGBA, RGBA
            };
            return data[in];
        }

        inline bool Pixel::format(int channels, FORMAT & format)
        {
            switch (channels)
            {
            case 1: format = L;    return true;
            case 2: format = LA;   return true;
            case 3: format = RGB;  return true;
            case 4: format = RGBA; return true;
            }
            return false;
        }

        inline Pixel::TYPE Pixel::type(PIXEL in)
        {
            static const TYPE data[] =
            {
                U8,      U16, F16, F32,
                U8,      U16, F16, F32,
                U8, U10, U16, F16, F32,
                U8,      U16, F16, F32
            };
            return data[in];
        }

        inline bool Pixel::intType(int bitDepth, TYPE & type)
        {
            switch (bitDepth)
            {
            case  8: type = U8; return true;
            case 10: type = U10; return true;
            case 16: type = U16; return true;
            }
            return false;
        }

        inline bool Pixel::floatType(int bitDepth, TYPE & type)
        {
            switch (bitDepth)
            {
            case 16: type = F16; return true;
            case 32: type = F32; return true;
            }
            return false;
        }

        inline int Pixel::channels(FORMAT in)
        {
            static const int data[] = { 1, 2, 3, 4 };
            return data[in];
        }

        inline int Pixel::channels(PIXEL in)
        {
            static const int data[] =
            {
                1, 1,    1, 1,
                2, 2,    2, 2,
                3, 3, 3, 3, 3,
                4, 4,    4, 4
            };
            return data[in];
        }

        inline int Pixel::channelByteCount(PIXEL in)
        {
            static const int data[] =
            {
                1, 2,    2, 4,
                1, 2,    2, 4,
                1, 0, 2, 2, 4,
                1, 2,    2, 4
            };
            return data[in];
        }

        inline int Pixel::byteCount(PIXEL in)
        {
            static const int data[] =
            {
                1, 2,    2,  4,
                2, 4,    4,  8,
                3, 4, 6, 6, 12,
                4, 8,    8, 16
            };
            return data[in];
        }

        inline int Pixel::bitDepth(TYPE in)
        {
            static const int data[] =
            {
                8, 10, 16, 16, 32
            };
            return data[in];
        }

        inline int Pixel::bitDepth(PIXEL in)
        {
            static const int data[] =
            {
                8,     16, 16, 32,
                8,     16, 16, 32,
                8, 10, 16, 16, 32,
                8,     16, 16, 32
            };
            return data[in];
        }

        inline int Pixel::max(PIXEL in)
        {
            static const int data[] =
            {
                u8Max,         u16Max, 1, 1,
                u8Max,         u16Max, 1, 1,
                u8Max, u10Max, u16Max, 1, 1,
                u8Max,         u16Max, 1, 1
            };
            return data[in];
        }

        inline Pixel::PIXEL Pixel::pixel(FORMAT format, TYPE type)
        {
            PIXEL out = static_cast<PIXEL>(0);
            switch (format)
            {
            case L:
                switch (type)
                {
                case U8:  out = L_U8;  break;
                case U10:
                case U16: out = L_U16; break;
                case F16: out = L_F16; break;
                case F32: out = L_F32; break;
                default: break;
                }
                break;
            case LA:
                switch (type)
                {
                case U8:  out = LA_U8;  break;
                case U10:
                case U16: out = LA_U16; break;
                case F16: out = LA_F16; break;
                case F32: out = LA_F32; break;
                default: break;
                }
                break;
            case RGB:
                switch (type)
                {
                case U8:  out = RGB_U8;  break;
                case U10: out = RGB_U10; break;
                case U16: out = RGB_U16; break;
                case F16: out = RGB_F16; break;
                case F32: out = RGB_F32; break;
                default: break;
                }
                break;
            case RGBA:
                switch (type)
                {
                case U8:  out = RGBA_U8;  break;
                case U10:
                case U16: out = RGBA_U16; break;
                case F16: out = RGBA_F16; break;
                case F32: out = RGBA_F32; break;
                default: break;
                }
                break;
            default: break;
            }
            return out;
        }

        inline bool Pixel::pixel(FORMAT format, TYPE type, PIXEL & out)
        {
            switch (format)
            {
            case L:
                switch (type)
                {
                case U8:  out = L_U8;  break;
                case U16: out = L_U16; break;
                case F16: out = L_F16; break;
                case F32: out = L_F32; break;
                default: return false;
                }
                break;
            case LA:
                switch (type)
                {
                case U8:  out = LA_U8;  break;
                case U16: out = LA_U16; break;
                case F16: out = LA_F16; break;
                case F32: out = LA_F32; break;
                default: return false;
                }
                break;
            case RGB:
                switch (type)
                {
                case U8:  out = RGB_U8;  break;
                case U10: out = RGB_U10; break;
                case U16: out = RGB_U16; break;
                case F16: out = RGB_F16; break;
                case F32: out = RGB_F32; break;
                default: return false;
                }
                break;
            case RGBA:
                switch (type)
                {
                case U8:  out = RGBA_U8;  break;
                case U16: out = RGBA_U16; break;
                case F16: out = RGBA_F16; break;
                case F32: out = RGBA_F32; break;
                default: return false;
                }
                break;
            default:
                return false;
            }
            return true;
        }

        inline bool Pixel::intPixel(int channels, int bitDepth, PIXEL & pixel)
        {
            FORMAT format = static_cast<FORMAT>(0);
            if (!Pixel::format(channels, format))
                return false;
            TYPE type = static_cast<TYPE>(0);
            if (!Pixel::intType(bitDepth, type))
                return false;
            return Pixel::pixel(format, type, pixel);
        }

        inline bool Pixel::floatPixel(int channels, int bitDepth, PIXEL & pixel)
        {
            FORMAT format = static_cast<FORMAT>(0);
            if (!Pixel::format(channels, format))
                return false;
            TYPE type = static_cast<TYPE>(0);
            if (!Pixel::floatType(bitDepth, type))
                return false;
            return Pixel::pixel(format, type, pixel);
        }

        // Note that we use a LUT in some cases because bit shifting doesn't seem to
        // preserve maximum values?
#define _PIXEL_LUT(IN, OUT, IN_MAX, OUT_MAX) \
    static OUT##_T lut [IN_MAX + 1]; \
    static bool init = false; \
    if (! init) \
    { \
        for (int i = 0; i <= IN_MAX; ++i) \
        { \
            lut[i] = OUT##_T(i / static_cast<float>(IN_MAX) * OUT_MAX); \
        } \
        init = true; \
    } \
    return lut[in];

        inline Pixel::U10_T Pixel::u8ToU10(U8_T in)
        {
            //return in << 2;
            _PIXEL_LUT(U8, U10, u8Max, u10Max)
        }

        inline Pixel::U16_T Pixel::u8ToU16(U8_T in)
        {
            //return in << 8;
            _PIXEL_LUT(U8, U16, u8Max, u16Max)
        }

        inline Pixel::F16_T Pixel::u8ToF16(U8_T in)
        {
            //return in / F16(u8Max);
            _PIXEL_LUT(U8, F16, u8Max, 1.f)
        }

        inline Pixel::F32_T Pixel::u8ToF32(U8_T in)
        {
            //return in / F32(u8Max);
            _PIXEL_LUT(U8, F32, u8Max, 1.f)
        }

        inline Pixel::U8_T Pixel::u10ToU8(U10_T in)
        {
            return in >> 2;
        }

        inline Pixel::U16_T Pixel::u10ToU16(U10_T in)
        {
            //return in << 6;
            _PIXEL_LUT(U10, U16, u10Max, u16Max)
        }

        inline Pixel::F16_T Pixel::u10ToF16(U10_T in)
        {
            //return in / F16(u10Max);
            _PIXEL_LUT(U10, F16, u10Max, 1.f)
        }

        inline Pixel::F32_T Pixel::u10ToF32(U10_T in)
        {
            //return in / F32(u10Max);
            _PIXEL_LUT(U10, F32, u10Max, 1.f)
        }

        inline Pixel::U8_T Pixel::u16ToU8(U16_T in)
        {
            return in >> 8;
        }

        inline Pixel::U10_T Pixel::u16ToU10(U16_T in)
        {
            return in >> 6;
        }

        inline Pixel::F16_T Pixel::u16ToF16(U16_T in)
        {
            //return in / F16(u16Max);
            _PIXEL_LUT(U16, F16, u16Max, 1.f);
        }

        inline Pixel::F32_T Pixel::u16ToF32(U16_T in)
        {
            //return in / F32(u16Max);
            _PIXEL_LUT(U16, F32, u16Max, 1.f)
        }

        inline Pixel::U8_T Pixel::f16ToU8(F16_T in)
        {
            return Core::Math::clamp(static_cast<int>(in * u8Max + 0.5), 0, u8Max);
        }

        inline Pixel::U10_T Pixel::f16ToU10(F16_T in)
        {
            return Core::Math::clamp(static_cast<int>(in * u10Max + 0.5), 0, u10Max);
        }

        inline Pixel::U16_T Pixel::f16ToU16(F16_T in)
        {
            return Core::Math::clamp(static_cast<int>(in * u16Max + 0.5), 0, u16Max);
        }

        inline Pixel::F32_T Pixel::f16ToF32(F16_T in)
        {
            return in;
        }

        inline Pixel::U8_T Pixel::f32ToU8(F32_T in)
        {
            //DJV_DEBUG("f32ToU8");
            //DJV_DEBUG_PRINT("in = " << in);

            return Core::Math::clamp(static_cast<int>(in * u8Max + 0.5), 0, u8Max);

            /*// Ill floating point conversion. Assumes IEEE floats: sign (1 bit)
            // + exponent (8 bits) + mantissa (23 bits).
            //
            // References:
            //
            // * http://www.stereopsis.com/FPU.html

            // Transform to get a constant exponent of 127 for 0.0 to 1.0 range.

            const F32_T tmp = in + 1.f;
            const quint32 bits = *(const quint32 *)&tmp;
            //DJV_DEBUG_PRINT(_bits(tmp));
            //DJV_DEBUG_PRINT(_bits(bits));

            // Bail on negative numbers.

            if (bits >> 31) return 0;

            // Exponent:
            //
            // * < 127 - Input <= 0.0.
            // * 127 - Input between 0.0 and 1.0. Mantissa becomes 23 bit integer.
            // * > 127 - Input >= 1.0.

            const quint8 exp = (bits >> 23) & 0xff;
            //DJV_DEBUG_PRINT("exp = " << exp);
            if (127 == exp) return (bits >> 15) & 0xff;
            else if (exp > 127) return u8Max;
            else return 0;*/
        }

        inline Pixel::U10_T Pixel::f32ToU10(F32_T in)
        {
            return Core::Math::clamp(static_cast<int>(in * u10Max + 0.5), 0, u10Max);
        }

        inline Pixel::U16_T Pixel::f32ToU16(F32_T in)
        {
            return Core::Math::clamp(static_cast<int>(in * u16Max + 0.5), 0, u16Max);
        }

        inline Pixel::F16_T Pixel::f32ToF16(F32_T in)
        {
            return in;
        }

    } // namespace AV

    inline bool operator == (const AV::Pixel::Mask & a, const AV::Pixel::Mask & b)
    {
        for (int i = 0; i < AV::Pixel::channelsMax; ++i)
        {
            if (a.mask[i] != b.mask[i])
            {
                return false;
            }
        }
        return true;
    }

    inline bool operator != (const AV::Pixel::Mask & a, const AV::Pixel::Mask & b)
    {
        return !(a == b);
    }

} // namespace djv
