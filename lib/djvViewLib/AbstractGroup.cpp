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

#include <djvViewLib/AbstractGroup.h>

#include <djvViewLib/Session.h>
#include <djvViewLib/ViewContext.h>

#include <QMenu>
#include <QToolBar>

namespace djv
{
    namespace ViewLib
    {
        struct AbstractGroup::Private
        {
            Private(
                const QPointer<Session> & session,
                const QPointer<ViewContext> & context) :
                session(session),
                context(context)
            {}

            QPointer<Session> session;
            QPointer<ViewContext> context;
        };

        AbstractGroup::AbstractGroup(
            const QPointer<Session> & session,
            const QPointer<ViewContext> & context) :
            QObject(session),
            _p(new Private(session, context))
        {}

        AbstractGroup::~AbstractGroup()
        {}

        QPointer<QMenu> AbstractGroup::createMenu() const
        {
            return nullptr;
        }

        QPointer<QToolBar> AbstractGroup::createToolBar() const
        {
            return nullptr;
        }

        const QPointer<Session> & AbstractGroup::session() const
        {
            return _p->session;
        }

        const QPointer<ViewContext> & AbstractGroup::context() const
        {
            return _p->context;
        }

    } // namespace ViewLib
} // namespace djv
