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

#include <djvAV/DPXLoad.h>

#include <djvAV/DPXHeader.h>
#include <djvAV/Image.h>
#include <djvAV/PixelDataUtil.h>

#include <djvCore/Assert.h>
#include <djvCore/CoreContext.h>

namespace djv
{
    namespace AV
    {
        DPXLoad::DPXLoad(const Core::FileInfo & fileInfo, const DPX::Options & options, const QPointer<Core::CoreContext> & context) :
            Load(fileInfo, context),
            _options(options)
        {
            Core::FileIO io;
            _open(_fileInfo.fileName(_fileInfo.sequence().start()), _ioInfo, io);
            if (Core::FileInfo::SEQUENCE == _fileInfo.type())
            {
                _ioInfo.sequence.frames = _fileInfo.sequence().frames;
            }
        }

        DPXLoad::~DPXLoad()
        {}

        void DPXLoad::_open(const QString & in, IOInfo & info, Core::FileIO & io)
        {
            //DJV_DEBUG("DPXLoad::_open");
            //DJV_DEBUG_PRINT("in = " << in);
            io.open(in, Core::FileIO::READ);
            info.layers[0].fileName = in;
            _filmPrint = false;
            DPXHeader header;
            header.load(io, info, _filmPrint);
            //DJV_DEBUG_PRINT("info = " << info);
            //DJV_DEBUG_PRINT("film print = " << _filmPrint);
        }

        void DPXLoad::read(Image & image, const ImageIOInfo & frame)
        {
            //DJV_DEBUG("DPXLoad::read");
            //DJV_DEBUG_PRINT("frame = " << frame);

            // Open the file.
            const QString fileName = _fileInfo.fileName(frame.frame != -1 ? frame.frame : _fileInfo.sequence().start());
            //DJV_DEBUG_PRINT("file name = " << fileName);
            IOInfo info;
            QScopedPointer<Core::FileIO> io(new Core::FileIO);
            _open(fileName, info, *io);
            image.tags = info.tags;

            // Set the color profile.
            if ((Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile) ||
                (Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile && _filmPrint))
            {
                //DJV_DEBUG_PRINT("color profile");
                image.colorProfile.type = ColorProfile::LUT;
                if (!_filmPrintLut.isValid())
                {
                    _filmPrintLut = Cineon::filmPrintToLinearLut(_options.inputFilmPrint);
                }
                image.colorProfile.lut = _filmPrintLut;
            }
            else
            {
                image.colorProfile = ColorProfile();
            }

            // Read the file.
            io->readAhead();
            bool mmap = true;
            auto pixelDataInfo = info.layers[0];
            if ((io->size() - io->pos()) < PixelDataUtil::dataByteCount(pixelDataInfo))
            {
                mmap = false;
            }
            //DJV_DEBUG_PRINT("mmap = " << mmap);
            if (mmap)
            {
                if (!frame.proxy)
                {
                    image.set(pixelDataInfo, io->mmapP(), io.data());
                    io.take();
                }
                else
                {
                    _tmp.set(pixelDataInfo, io->mmapP());
                    pixelDataInfo.size = PixelDataUtil::proxyScale(pixelDataInfo.size, frame.proxy);
                    pixelDataInfo.proxy = frame.proxy;
                    image.set(pixelDataInfo);
                    PixelDataUtil::proxyScale(_tmp, image, frame.proxy);
                }
            }
            else
            {
                PixelData * data = frame.proxy ? &_tmp : &image;
                data->set(pixelDataInfo);
                Core::Error error;
                bool errorValid = false;
                try
                {
                    for (int y = 0; y < pixelDataInfo.size.y; ++y)
                    {
                        io->get(
                            data->data(0, y),
                            pixelDataInfo.size.x * Pixel::byteCount(pixelDataInfo.pixel));
                    }
                }
                catch (const Core::Error & otherError)
                {
                    error = otherError;
                    errorValid = true;
                }
                if (frame.proxy)
                {
                    pixelDataInfo.size = PixelDataUtil::proxyScale(pixelDataInfo.size, frame.proxy);
                    pixelDataInfo.proxy = frame.proxy;
                    image.set(pixelDataInfo);
                    PixelDataUtil::proxyScale(_tmp, image, frame.proxy);
                }
                if (errorValid)
                    throw error;
            }
            //DJV_DEBUG_PRINT("image = " << image);
        }

    } // namespace AV
} // namespace djv
