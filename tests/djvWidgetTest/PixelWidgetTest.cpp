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

#include <djvWidgetTest/PixelWidgetTest.h>

#include <djvUI/PixelWidget.h>

#include <djvCore/System.h>

#include <QVBoxLayout>

namespace djv
{
    namespace WidgetTest
    {
        PixelWidgetTest::PixelWidgetTest(const QPointer<UI::UIContext> & context) :
            AbstractWidgetTest(context)
        {}

        QString PixelWidgetTest::name()
        {
            return "PixelWidgetTest";
        }

        void PixelWidgetTest::run(const QStringList & args)
        {
            QWidget * window = new QWidget;

            UI::PixelWidget * widget = new UI::PixelWidget;

            QVBoxLayout * layout = new QVBoxLayout(window);
            layout->addWidget(widget);

            connect(
                widget,
                SIGNAL(pixelChanged(djv::AV::Pixel::PIXEL)),
                SLOT(callback(djv::AV::Pixel::PIXEL)));

            window->show();
        }

        void PixelWidgetTest::callback(AV::Pixel::PIXEL value)
        {
            QStringList tmp;
            tmp << value;
            Core::System::print(QString("%1").arg(tmp.join(", ")));
        }

    } // namespace WidgetTest
} // namespace djv
