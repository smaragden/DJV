//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
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

#include <djvUI/SearchBox.h>

#include <djvUI/Border.h>
#include <djvUI/Icon.h>
#include <djvUI/LineEdit.h>
#include <djvUI/RowLayout.h>
#include <djvUI/SoloLayout.h>
#include <djvUI/ToolButton.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        struct SearchBox::Private
        {
            std::shared_ptr<LineEdit> lineEdit;
            std::shared_ptr<Layout::Solo> soloLayout;
            std::shared_ptr<Layout::Border> border;
        };

        void SearchBox::_init(Context * context)
        {
            Widget::_init(context);

            setClassName("djv::UI::SearchBox");
            setMargin(Style::MetricsRole::MarginSmall);

            _p->lineEdit = LineEdit::create(context);
            _p->lineEdit->setBorder(false);
            _p->lineEdit->setMargin(Style::MetricsRole::None);
            _p->lineEdit->setBackgroundRole(Style::ColorRole::None);
            
            auto searchIcon = Icon::create(context);
            searchIcon->setIcon("djvIconSearch");
            
            auto clearButton = Button::Tool::create(context);
            clearButton->setIcon("djvIconClear");
            clearButton->setInsideMargin(Style::MetricsRole::None);
            clearButton->setBackgroundRole(Style::ColorRole::None);
            
            auto layout = Layout::Horizontal::create(context);
            layout->setSpacing(Style::MetricsRole::None);
            layout->addWidget(_p->lineEdit, Layout::RowStretch::Expand);
            _p->soloLayout = Layout::Solo::create(context);
            _p->soloLayout->addWidget(searchIcon);
            _p->soloLayout->addWidget(clearButton);
            layout->addWidget(_p->soloLayout);
            
            _p->border = Layout::Border::create(context);
            _p->border->setBackgroundRole(Style::ColorRole::BackgroundText);
            _p->border->addWidget(layout);
            _p->border->setParent(shared_from_this());
        }

        SearchBox::SearchBox() :
            _p(new Private)
        {}

        SearchBox::~SearchBox()
        {}

        std::shared_ptr<SearchBox> SearchBox::create(Context * context)
        {
            auto out = std::shared_ptr<SearchBox>(new SearchBox);
            out->_init(context);
            return out;
        }

        float SearchBox::getHeightForWidth(float value) const
        {
            float out = 0.f;
            if (auto style = _getStyle().lock())
            {
                out = _p->border->getHeightForWidth(value) +
                    getMargin().getHeight(style);
            }
            return out;
        }

        void SearchBox::_preLayoutEvent(Event::PreLayout& event)
        {
            if (auto style = _getStyle().lock())
            {
                _setMinimumSize(_p->border->getMinimumSize() + getMargin().getSize(style));
            }
        }

        void SearchBox::_layoutEvent(Event::Layout& event)
        {
            if (auto style = _getStyle().lock())
            {
                const BBox2f g = getGeometry();
                _p->border->setGeometry(getMargin().bbox(g, style));
            }
        }

    } // namespace UI
} // namespace djv