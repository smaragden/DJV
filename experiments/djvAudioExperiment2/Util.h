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

#pragma once

#include <djvAV/AudioData.h>
#include <djvAV/PixelData.h>

#include <djvCore/Speed.h>

#include <QMetaType>

#include <list>
#include <memory>
#include <mutex>

namespace djv
{
    namespace AudioExperiment2
    {
        class Context;

        class Util
        {
            Q_GADGET

        public:
            virtual ~Util() = 0;

            enum PLAYBACK
            {
                PLAYBACK_STOP,
                PLAYBACK_FORWARD,
                PLAYBACK_REVERSE,

                PLAYBACK_COUNT
            };
            Q_ENUM(PLAYBACK);

            struct VideoInfo
            {
                AV::PixelDataInfo info;
                Core::Speed       speed;
                int64_t           duration = 0;

                bool operator == (const VideoInfo &) const;
            };

            struct AudioInfo
            {
                size_t          channels   = 0;
                AV::Audio::TYPE type       = AV::Audio::TYPE_NONE;
                size_t          sampleRate = 0;
                int64_t         duration   = 0;

                bool operator == (const AudioInfo &) const;
            };

            typedef std::pair<int64_t, std::shared_ptr<AV::PixelData> > VideoFrame;
            typedef std::pair<int64_t, std::shared_ptr<AV::AudioData> > AudioFrame;

            struct AVQueue
            {
                std::list<VideoFrame> video;
                std::list<AudioFrame> audio;
                std::mutex mutex;
            };

            static double timeToSeconds(int64_t);
            static int64_t secondsToTime(double);
        };

    } // namespace AudioExperiment2
} // namespace djv
