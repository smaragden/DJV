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

#include <djvUI/Core.h>

#include <djvAV/Pixel.h>

#include <djvCore/Util.h>

#include <QWidget>

#include <memory>

namespace djv
{
    namespace UI
    {
        class UIContext;

        //! This class provides a pixel mask widget.
        class PixelMaskWidget : public QWidget
        {
            Q_OBJECT

            //! This property holds the pixel mask.    
            Q_PROPERTY(
                djv::AV::Pixel::Mask mask
                READ                       mask
                WRITE                      setMask
                NOTIFY                     maskChanged)

        public:
            explicit PixelMaskWidget(const QPointer<UIContext> &, QWidget * parent = nullptr);
            ~PixelMaskWidget() override;

            //! Get the pixel mask.
            const AV::Pixel::Mask & mask() const;

        public Q_SLOTS:
            //! Set the pixel mask.
            void setMask(const djv::AV::Pixel::Mask &);

        Q_SIGNALS:
            //! This signal is emitted when the pixel mask is changed.
            void maskChanged(const djv::AV::Pixel::Mask &);

        protected:
            bool event(QEvent*) override;

        private Q_SLOTS:
            void buttonCallback();
            void soloCallback();
            void toggleCallback(bool);
            void resetCallback();

            void styleUpdate();
            void valueUpdate();

        private:
            DJV_PRIVATE_COPY(PixelMaskWidget);

            struct Private;
            std::unique_ptr<Private> _p;
        };

    } // namespace UI
} // namespace djv


