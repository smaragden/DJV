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

#include <djv_info/InfoApplication.h>

#include <djv_info/InfoContext.h>

#include <djvAV/IO.h>

#include <djvCore/DebugLog.h>
#include <djvCore/FileInfoUtil.h>
#include <djvCore/Time.h>
#include <djvCore/VectorUtil.h>

#include <QDir>
#include <QTimer>

namespace djv
{
    namespace info
    {
        Application::Application(int & argc, char ** argv) :
            QGuiApplication(argc, argv)
        {
            //DJV_DEBUG("Application::Application");

            setOrganizationName("djv.sourceforge.net");
            setApplicationName("djv_info");

            // Create the context.
            _context.reset(new Context(argc, argv));

            // Parse the command line.
            if (!_context->commandLine(argc, argv))
            {
                QTimer::singleShot(0, this, SLOT(commandLineExit()));
            }
            else
            {
                QTimer::singleShot(0, this, SLOT(work()));
            }
        }

        Application::~Application()
        {
            //DJV_DEBUG("Application::~Application");
        }

        void Application::commandLineExit()
        {
            exit(1);
        }

        void Application::work()
        {
            //DJV_DEBUG("Application::work");

            int r = 0;

            // Convert the command line inputs:
            //
            // * Match wildcards
            // * Expand sequences
            Core::FileInfoList list;
            const QStringList & input = _context->input();
            for (int i = 0; i < input.count(); ++i)
            {
                // Parse the input.
                Core::FileInfo fileInfo = Core::FileInfoUtil::parse(input[i], Core::Sequence::format());
                //DJV_DEBUG_PRINT("input = " << fileInfo);
                DJV_LOG(_context->debugLog(), "djv_info", QString("Input = \"%1\"").arg(fileInfo));

                // Expand the sequence.
                QStringList tmp = Core::FileInfoUtil::expandSequence(fileInfo);
                for (int j = 0; j < tmp.count(); ++j)
                {
                    fileInfo = Core::FileInfo(tmp[j]);
                    if (Core::Sequence::format() && fileInfo.isSequenceValid())
                    {
                        fileInfo.setType(Core::FileInfo::SEQUENCE);
                    }
                    list += fileInfo;
                }
            }
            //DJV_DEBUG_PRINT("list = " << list);
            DJV_LOG(_context->debugLog(), "djv_info",
                QString("Input count = %1").arg(list.count()));

            // Convert files into sequences.
            Core::FileInfoUtil::sequence(list, Core::Sequence::format());
            //DJV_DEBUG_PRINT("list = " << list);
            DJV_LOG(_context->debugLog(), "djv_info",
                QString("Processed count = %1").arg(list.count()));

            // If there are no inputs use the current directory.
            if (!list.count() && 0 == r)
            {
                list += Core::FileInfo(".");
            }

            // Print the files.
            for (int i = 0; i < list.count(); ++i)
            {
                if (Core::FileInfo::DIRECTORY == list[i].type())
                {
                    printDirectory(
                        list[i],
                        ((list.count() > 1) || _context->hasRecurse()) && !_context->hasFilePath());
                }
                else
                {
                    try
                    {
                        printItem(list[i], _context->hasFilePath(), _context->hasInfo());
                    }
                    catch (const Core::Error & error)
                    {
                        _context->printError(error);
                        r = 1;
                    }
                }
            }

            exit(r);
        }

        void Application::printItem(const Core::FileInfo & in, bool path, bool info)
        {
            //DJV_DEBUG("Application::printItem");
            //DJV_DEBUG_PRINT("in = " << in);

            // Open the file.
            AV::IOInfo _info;
            std::unique_ptr<AV::Load> load;
            try
            {
                load = _context->ioFactory()->load(in, _info);
            }
            catch (Core::Error error)
            {
                error.add(
                    qApp->translate("djv::info::Application", "Cannot open file: \"%1\"").
                    arg(QDir::toNativeSeparators(in)));
                throw error;
            }

            // Print the file.
            const QString name = in.fileName(-1, path);
            const bool verbose = _context->hasVerbose();
            const int columns = _context->columns();
            if (info && !verbose)
            {
                if (1 == _info.layers.size())
                {
                    // Print single layer information.
                    QStringList info;
                    if (_info.layers[0].isValid())
                    {
                        QStringList pixelLabel;
                        pixelLabel << _info.layers[0].pixel;
                        info += qApp->translate("djv::info::Application", "I[%1x%2:%3 %4]").
                            arg(_info.layers[0].size.x).
                            arg(_info.layers[0].size.y).
                            arg(Core::VectorUtil::aspect(_info.layers[0].size), 0, 'f', 2).
                            arg(pixelLabel.join(", "));
                    }
                    if (_info.audio.isValid())
                    {
                        info += qApp->translate("djv::info::Application", "A[%1:%2@%3]").
                            arg(_info.audio.channels).
                            arg(AV::Audio::typeLabels()[_info.audio.type]).
                            arg(_info.audio.sampleRate);
                    }
                    if (_info.sequence.frames.size())
                    {
                        info += qApp->translate("djv::info::Application", "T[%1@%2]").
                            arg(Core::Time::frameToString(
                                _info.sequence.frames.count(),
                                _info.sequence.speed)).
                            arg(Core::Speed::speedToFloat(_info.sequence.speed), 0, 'f', 2);
                    }
                    _context->print(qApp->translate("djv::info::Application", "%1 %2").
                        arg(QDir::toNativeSeparators(name)).
                        arg(info.join(' '), columns - name.length() - 2));
                }
                else
                {
                    // Print multi-layer information.
                    QStringList info;
                    if (_info.audio.isValid())
                    {
                        info += qApp->translate("djv::info::Application", "A[%1:%2@%3]").
                            arg(_info.audio.channels).
                            arg(AV::Audio::typeLabels()[_info.audio.type]).
                            arg(_info.audio.sampleRate);
                    }
                    if (_info.sequence.frames.size())
                    {
                        info += qApp->translate("djv::info::Application", "T[%1@%2]").
                            arg(Core::Time::frameToString(
                                _info.sequence.frames.count(),
                                _info.sequence.speed)).
                            arg(Core::Speed::speedToFloat(_info.sequence.speed), 0, 'f', 2);
                    }
                    _context->print(qApp->translate("djv::info::Application", "%1 %2").
                        arg(QDir::toNativeSeparators(name)).
                        arg(info.join(' '), columns - name.length() - 2));
                    for (size_t i = 0; i < _info.layers.size(); ++i)
                    {
                        const QString nameString = qApp->translate("djv::info::Application", "    %1").
                            arg(_info.layers[i].layerName);
                        QStringList pixelLabel;
                        pixelLabel << _info.layers[i].pixel;
                        const QString infoString = qApp->translate("djv::info::Application","I[%1x%2:%3 %4]").
                            arg(_info.layers[i].size.x).
                            arg(_info.layers[i].size.y).
                            arg(Core::VectorUtil::aspect(_info.layers[i].size), 0, 'f', 2).
                            arg(pixelLabel.join(", "));
                        _context->print(qApp->translate("djv::info::Application", "%1 %2").
                            arg(nameString).
                            arg(infoString, columns - nameString.length() - 2));
                    }
                }
            }
            else if (info && verbose)
            {
                // Print verbose informaton.
                _context->print(QDir::toNativeSeparators(name));
                if (1 == _info.layers.size())
                {
                    if (_info.layers[0].isValid())
                    {
                        _context->print(qApp->translate("djv::info::Application", "    Image width = %1").
                            arg(_info.layers[0].size.x));
                        _context->print(qApp->translate("djv::info::Application", "    Image height = %1").
                            arg(_info.layers[0].size.y));
                        _context->print(qApp->translate("djv::info::Application", "    Image aspect ratio = %1").
                            arg(Core::VectorUtil::aspect(_info.layers[0].size), 0, 'f', 2));
                        QStringList pixelLabel;
                        pixelLabel << _info.layers[0].pixel;
                        _context->print(qApp->translate("djv::info::Application", "    Image pixel = %1").
                            arg(pixelLabel.join(", ")));
                    }
                }
                else
                {
                    for (size_t i = 0; i < _info.layers.size(); ++i)
                    {
                        if (_info.layers[i].isValid())
                        {
                            _context->print(qApp->translate("djv::info::Application", "    %1").
                                arg(_info.layers[i].layerName));
                            _context->print(qApp->translate("djv::info::Application", "        Image width = %1").
                                arg(_info.layers[i].size.x));
                            _context->print(qApp->translate("djv::info::Application", "        Image height = %1").
                                arg(_info.layers[i].size.y));
                            _context->print(qApp->translate("djv::info::Application", "        Image aspect ratio = %1").
                                arg(Core::VectorUtil::aspect(_info.layers[i].size), 0, 'f', 2));
                            QStringList pixelLabel;
                            pixelLabel << _info.layers[i].pixel;
                            _context->print(qApp->translate("djv::info::Application", "        Image pixel = %1").
                                arg(pixelLabel.join(", ")));
                        }
                    }
                }
                if (_info.audio.isValid())
                {
                    _context->print(qApp->translate("djv::info::Application", "    Audio channels = %1").
                        arg(_info.audio.channels));
                    _context->print(qApp->translate("djv::info::Application", "    Audio bit depth = %1").
                        arg(AV::Audio::typeLabels()[_info.audio.type]));
                    _context->print(qApp->translate("djv::info::Application", "    Audio sample rate = %1").
                        arg(_info.audio.sampleRate));
                    _context->print(qApp->translate("djv::info::Application", "    Audio sample Count = %1").
                        arg(_info.audio.sampleCount));
                }
                if (_info.sequence.frames.size())
                {
                    _context->print(qApp->translate("djv::info::Application", "    Time start = %1").
                        arg(Core::Time::frameToString(
                            _info.sequence.start(),
                            _info.sequence.speed)));
                    _context->print(qApp->translate("djv::info::Application", "    Time end = %1").
                        arg(Core::Time::frameToString(
                            _info.sequence.end(),
                            _info.sequence.speed)));
                    _context->print(qApp->translate("djv::info::Application", "    Time duration = %1").
                        arg(Core::Time::frameToString(
                            _info.sequence.frames.count(),
                            _info.sequence.speed)));
                    _context->print(qApp->translate("djv::info::Application", "    Speed = %1").
                        arg(Core::Speed::speedToFloat(_info.sequence.speed)));
                }
                const QStringList keys = _info.tags.keys();
                for (int i = 0; i < keys.count(); ++i)
                {
                    _context->print(qApp->translate("djv::info::Application", "    Tag %1 = %2").
                        arg(keys[i]).
                        arg(_info.tags[keys[i]]));
                }
                _context->printSeparator();
            }
            else
            {
                _context->print(name);
            }
        }

        void Application::printDirectory(const Core::FileInfo & in, bool label)
        {
            //DJV_DEBUG("Application::printDirectory");
            //DJV_DEBUG_PRINT("in = " << in);

            // Read the directory contents.
            Core::FileInfoList items;
            items = Core::FileInfoUtil::list(in, Core::Sequence::format());

            // Process the items.
            Core::FileInfoUtil::filter(items, Core::FileInfoUtil::FILTER_DIRECTORIES);

            // Print the items.
            if (label)
            {
                _context->print(qApp->translate("djv::info::Application", "%1:").
                    arg(QDir::toNativeSeparators(in)));
                if (_context->hasVerbose())
                {
                    _context->printSeparator();
                }
            }
            for (int i = 0; i < items.count(); ++i)
            {
                try
                {
                    printItem(items[i], _context->hasFilePath(), _context->hasInfo());
                }
                catch (const Core::Error &)
                {
                }
            }
            if (label)
            {
                _context->printSeparator();
            }

            // Recurse.
            if (_context->hasRecurse())
            {
                Core::FileInfoList list = Core::FileInfoUtil::list(in, Core::Sequence::format());
                Core::FileInfoUtil::filter(
                    list,
                    Core::FileInfoUtil::FILTER_FILES | Core::FileInfoUtil::FILTER_HIDDEN);
                for (int i = 0; i < list.count(); ++i)
                {
                    printDirectory(list[i], label);
                }
            }
        }

    } // namespace info
} // namespace djv
