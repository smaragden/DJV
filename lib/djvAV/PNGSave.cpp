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

#include <djvAV/PNGSave.h>

#include <djvAV/OpenGLImage.h>

#include <djvCore/CoreContext.h>
#include <djvCore/Error.h>
#include <djvCore/Memory.h>
#include <djvCore/StringUtil.h>

namespace djv
{
    namespace AV
    {
        PNGSave::PNGSave(const Core::FileInfo & fileInfo, const IOInfo & ioInfo, const QPointer<Core::CoreContext> & context) :
            Save(fileInfo, ioInfo, context)
        {
            //DJV_DEBUG("PNGSave::PNGSave");
            //DJV_DEBUG_PRINT("fileInfo = " << fileInfo);
            _pngError.context = context;
            if (_ioInfo.sequence.frames.count() > 1)
            {
                _fileInfo.setType(Core::FileInfo::SEQUENCE);
            }
            _info = PixelDataInfo();
            _info.size = _ioInfo.layers[0].size;
            Pixel::TYPE type = Pixel::type(_ioInfo.layers[0].pixel);
            switch (type)
            {
            case Pixel::U10:
            case Pixel::F16:
            case Pixel::F32: type = Pixel::U16; break;
            default: break;
            }
            _info.pixel = Pixel::pixel(Pixel::format(_ioInfo.layers[0].pixel), type);
            //DJV_DEBUG_PRINT("info = " << _info);
            _image.set(_info);
        }

        PNGSave::~PNGSave()
        {
            _close();
        }

        namespace
        {
            bool pngScanline(png_structp png, const quint8 * in)
            {
                if (setjmp(png_jmpbuf(png)))
                    return false;
                png_write_row(png, (png_byte *)in);
                return true;
            }

            bool pngEnd(png_structp png, png_infop pngInfo)
            {
                if (setjmp(png_jmpbuf(png)))
                    return false;
                png_write_end(png, pngInfo);
                return true;
            }

        } // namespace

        void PNGSave::write(const Image & in, const ImageIOInfo & frame)
        {
            //DJV_DEBUG("PNGSave::write");
            //DJV_DEBUG_PRINT("in = " << in);

            // Open the file.
            const QString fileName = _fileInfo.fileName(frame.frame);
            //DJV_DEBUG_PRINT("file name = " << fileName);
            IOInfo info(_info);
            info.tags = in.tags;
            _open(fileName, info);

            // Convert the image.
            const PixelData * p = &in;
            if (in.info() != _image.info())
            {
                _image.zero();
                OpenGLImage().copy(in, _image);
                p = &_image;
            }

            // Write the file.
            const int h = p->h();
            for (int y = 0; y < h; ++y)
            {
                if (!pngScanline(_png, p->data(0, h - 1 - y)))
                {
                    throw Core::Error(PNG::staticName, _pngError.msg);
                }
            }
            if (!pngEnd(_png, _pngInfo))
            {
                throw Core::Error(PNG::staticName, _pngError.msg);
            }

            _close();
        }

        namespace
        {
            bool pngOpen(
                FILE *         f,
                png_structp    png,
                png_infop *    pngInfo,
                const IOInfo & info)
            {
                if (setjmp(png_jmpbuf(png)))
                {
                    return false;
                }
                *pngInfo = png_create_info_struct(png);
                if (!*pngInfo)
                {
                    return false;
                }
                png_init_io(png, f);
                int colorType = 0;
                switch (info.layers[0].pixel)
                {
                case Pixel::L_U8:
                case Pixel::L_U16:
                    colorType = PNG_COLOR_TYPE_GRAY;
                    break;
                case Pixel::LA_U8:
                case Pixel::LA_U16:
                    colorType = PNG_COLOR_TYPE_GRAY_ALPHA;
                    break;
                case Pixel::RGB_U8:
                case Pixel::RGB_U16:
                    colorType = PNG_COLOR_TYPE_RGB;
                    break;
                case Pixel::RGBA_U8:
                case Pixel::RGBA_U16:
                    colorType = PNG_COLOR_TYPE_RGB_ALPHA;
                    break;
                default: break;
                }
                png_set_IHDR(
                    png,
                    *pngInfo,
                    info.layers[0].size.x,
                    info.layers[0].size.y,
                    Pixel::bitDepth(info.layers[0].pixel),
                    colorType,
                    PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_DEFAULT,
                    PNG_FILTER_TYPE_DEFAULT);
                png_write_info(png, *pngInfo);
                return true;
            }

        } // namespace

        void PNGSave::_open(const QString & in, const IOInfo & info)
        {
            //DJV_DEBUG("PNGSave::_open");
            //DJV_DEBUG_PRINT("in = " << in);

            // Initialize libpng.
            _png = png_create_write_struct(
                PNG_LIBPNG_VER_STRING,
                &_pngError,
                djvPNGError,
                djvPNGWarning);
            if (!_png)
            {
                throw Core::Error(
                    PNG::staticName,
                    IOPlugin::errorLabels()[IOPlugin::ERROR_OPEN]);
            }

            // Open the file.
#if defined(DJV_WINDOWS)
            ::_wfopen_s(&_f, Core::StringUtil::qToStdWString(in).data(), L"wb");
#else
            _f = ::fopen(in.toUtf8().data(), "wb");
#endif
            if (!_f)
            {
                throw Core::Error(
                    PNG::staticName,
                    IOPlugin::errorLabels()[IOPlugin::ERROR_OPEN]);
            }
            if (!pngOpen(_f, _png, &_pngInfo, info))
            {
                throw Core::Error(PNG::staticName, _pngError.msg);
            }

            // Set the endian.
            if (Pixel::bitDepth(info.layers[0].pixel) >= 16 &&
                Core::Memory::LSB == Core::Memory::endian())
            {
                png_set_swap(_png);
            }
        }

        void PNGSave::_close()
        {
            if (_png || _pngInfo)
            {
                png_destroy_write_struct(
                    _png ? &_png : nullptr,
                    _pngInfo ? &_pngInfo : nullptr);
                _png = nullptr;
                _pngInfo = nullptr;
            }
            if (_f)
            {
                ::fclose(_f);
                _f = nullptr;
            }
        }

    } // namespace AV
} // namespace djv
