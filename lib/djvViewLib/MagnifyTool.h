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

#include <djvViewLib/AbstractTool.h>

#include <djvCore/Vector.h>

#include <memory>

namespace djv
{
    namespace ViewLib
    {
        //! This class provides a magnify tool.
        class MagnifyTool : public AbstractTool
        {
            Q_OBJECT

        public:
            explicit MagnifyTool(
                const QPointer<Session> &,
                const QPointer<ViewContext> &,
                QWidget * parent = nullptr);
            ~MagnifyTool() override;

        protected:
            void showEvent(QShowEvent *) override;
            bool event(QEvent *) override;

        private Q_SLOTS:
            void pickCallback(const glm::ivec2 &);
            void sliderCallback(int);
            void colorProfileCallback(bool);
            void displayProfileCallback(bool);

            void styleUpdate();
            void widgetUpdate();
            void pixelDataUpdate();

        private:
            DJV_PRIVATE_COPY(MagnifyTool);

            struct Private;
            std::unique_ptr<Private> _p;
        };

    } // namespace ViewLib
} // namespace djv
