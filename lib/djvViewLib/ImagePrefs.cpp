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

#include <djvViewLib/ImagePrefs.h>

#include <djvViewLib/Util.h>
#include <djvViewLib/ViewContext.h>

#include <djvUI/Prefs.h>

#include <djvAV/Image.h>

#include <QPointer>

namespace djv
{
    namespace ViewLib
    {
        ImagePrefs::ImagePrefs(const QPointer<ViewContext> & context, QObject * parent) :
            AbstractPrefs(context, parent),
            _frameStoreFileReload(frameStoreFileReloadDefault()),
            _mirror(mirrorDefault()),
            _scale(scaleDefault()),
            _rotate(rotateDefault()),
            _premultipliedAlpha(premultipliedAlphaDefault()),
            _colorProfile(colorProfileDefault()),
            _displayProfileIndex(displayProfileIndexDefault()),
            _channel(channelDefault())
        {
            UI::Prefs prefs("djv::ViewLib::ImagePrefs");
            prefs.get("frameStoreFileReload", _frameStoreFileReload);
            prefs.get("mirror", _mirror);
            prefs.get("scale", _scale);
            prefs.get("rotate", _rotate);
            prefs.get("premultipliedAlpha", _premultipliedAlpha);
            prefs.get("colorProfile", _colorProfile);
            UI::Prefs displayProfilePrefs("djv::ViewLib::ImagePrefs.displayProfile");
            displayProfilePrefs.get("index", _displayProfileIndex);
            int displayProfilesCount = 0;
            displayProfilePrefs.get("size", displayProfilesCount);
            for (int i = 0; i < displayProfilesCount; ++i)
            {
                DisplayProfile value;
                displayProfilePrefs.get(QString("%1").arg(i), value);
                try
                {
                    Util::loadLut(value.lutFile, value.lut, context);
                }
                catch (const Core::Error & error)
                {
                    context->printError(error);
                }
                _displayProfiles += value;
            }
            prefs.get("channel", _channel);
        }

        ImagePrefs::~ImagePrefs()
        {
            UI::Prefs prefs("djv::ViewLib::ImagePrefs");
            prefs.set("frameStoreFileReload", _frameStoreFileReload);
            prefs.set("mirror", _mirror);
            prefs.set("scale", _scale);
            prefs.set("rotate", _rotate);
            prefs.set("premultipliedAlpha", _premultipliedAlpha);
            prefs.set("colorProfile", _colorProfile);
            UI::Prefs displayProfilePrefs("djv::ViewLib::ImagePrefs.displayProfile");
            displayProfilePrefs.set("index", _displayProfileIndex);
            displayProfilePrefs.set("size", _displayProfiles.count());
            for (int i = 0; i < _displayProfiles.count(); ++i)
            {
                displayProfilePrefs.set(QString("%1").arg(i), _displayProfiles[i]);
            }
            prefs.set("channel", _channel);
        }

        bool ImagePrefs::frameStoreFileReloadDefault()
        {
            return false;
        }

        bool ImagePrefs::hasFrameStoreFileReload() const
        {
            return _frameStoreFileReload;
        }

        AV::PixelDataInfo::Mirror ImagePrefs::mirrorDefault()
        {
            return AV::PixelDataInfo::Mirror();
        }

        const AV::PixelDataInfo::Mirror & ImagePrefs::mirror() const
        {
            return _mirror;
        }

        Enum::IMAGE_SCALE ImagePrefs::scaleDefault()
        {
            return static_cast<Enum::IMAGE_SCALE>(0);
        }

        Enum::IMAGE_SCALE ImagePrefs::scale() const
        {
            return _scale;
        }

        Enum::IMAGE_ROTATE ImagePrefs::rotateDefault()
        {
            return static_cast<Enum::IMAGE_ROTATE>(0);
        }

        Enum::IMAGE_ROTATE ImagePrefs::rotate() const
        {
            return _rotate;
        }

        bool ImagePrefs::premultipliedAlphaDefault()
        {
            return true;
        }

        bool ImagePrefs::hasPremultipliedAlpha() const
        {
            return _premultipliedAlpha;
        }

        bool ImagePrefs::colorProfileDefault()
        {
            return true;
        }

        bool ImagePrefs::hasColorProfile() const
        {
            return _colorProfile;
        }

        int ImagePrefs::displayProfileIndexDefault()
        {
            return -1;
        }

        int ImagePrefs::displayProfileIndex() const
        {
            return _displayProfileIndex;
        }

        DisplayProfile ImagePrefs::displayProfile() const
        {
            return
                (_displayProfileIndex >= 0 &&
                    _displayProfileIndex < _displayProfiles.count()) ?
                _displayProfiles[_displayProfileIndex] :
                DisplayProfile();
        }

        const QVector<DisplayProfile> & ImagePrefs::displayProfiles() const
        {
            return _displayProfiles;
        }

        QStringList ImagePrefs::displayProfileNames() const
        {
            QStringList out;
            for (int i = 0; i < _displayProfiles.count(); ++i)
            {
                out += _displayProfiles[i].name;
            }
            return out;
        }

        AV::OpenGLImageOptions::CHANNEL ImagePrefs::channelDefault()
        {
            return static_cast<AV::OpenGLImageOptions::CHANNEL>(0);
        }

        AV::OpenGLImageOptions::CHANNEL ImagePrefs::channel() const
        {
            return _channel;
        }

        void ImagePrefs::setFrameStoreFileReload(bool in)
        {
            _frameStoreFileReload = in;
        }

        void ImagePrefs::setMirror(const AV::PixelDataInfo::Mirror & mirror)
        {
            if (mirror == _mirror)
                return;
            _mirror = mirror;
            Q_EMIT mirrorChanged(_mirror);
            Q_EMIT prefChanged();
        }

        void ImagePrefs::setScale(Enum::IMAGE_SCALE in)
        {
            if (in == _scale)
                return;
            _scale = in;
            Q_EMIT scaleChanged(_scale);
            Q_EMIT prefChanged();
        }

        void ImagePrefs::setRotate(Enum::IMAGE_ROTATE in)
        {
            if (in == _rotate)
                return;
            _rotate = in;
            Q_EMIT rotateChanged(_rotate);
            Q_EMIT prefChanged();
        }

        void ImagePrefs::setPremultipliedAlpha(bool in)
        {
            if (in == _premultipliedAlpha)
                return;
            _premultipliedAlpha = in;
            Q_EMIT premultipliedAlphaChanged(_premultipliedAlpha);
            Q_EMIT prefChanged();
        }

        void ImagePrefs::setColorProfile(bool in)
        {
            if (in == _colorProfile)
                return;
            _colorProfile = in;
            Q_EMIT colorProfileChanged(_colorProfile);
            Q_EMIT prefChanged();
        }

        void ImagePrefs::setDisplayProfileIndex(int index)
        {
            if (index == _displayProfileIndex)
                return;
            //DJV_DEBUG("setDisplayProfileIndex");
            //DJV_DEBUG_PRINT("index = " << index);
            _displayProfileIndex = index;
            DisplayProfile tmp;
            Q_EMIT displayProfileChanged(displayProfile());
            Q_EMIT prefChanged();
        }

        void ImagePrefs::setDisplayProfiles(const QVector<DisplayProfile> & in)
        {
            //! \bug Error C2893 on Windows?
            //if (in == _displayProfiles)
            //    return;
            if (in.size() == _displayProfiles.size())
            {
                int i = 0;
                for (; i < in.size(); ++i)
                {
                    if (in[i] != _displayProfiles[i])
                        break;
                }
                if (i == in.size())
                    return;
            }

            //DJV_DEBUG("setDisplayProfiles");
            //DJV_DEBUG_PRINT("in = " << in);
            _displayProfiles = in;
            setDisplayProfileIndex(Core::Math::min(
                _displayProfileIndex,
                _displayProfiles.count() - 1));
            Q_EMIT displayProfilesChanged(_displayProfiles);
            Q_EMIT prefChanged();
        }

        void ImagePrefs::setChannel(AV::OpenGLImageOptions::CHANNEL in)
        {
            if (in == _channel)
                return;
            _channel = in;
            Q_EMIT channelChanged(_channel);
            Q_EMIT prefChanged();
        }

    } // namespace ViewLib
} // namespace djv
