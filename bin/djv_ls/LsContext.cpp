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

#include <djv_ls/LsContext.h>

#include <djvCore/System.h>

#include <QCoreApplication>

namespace djv
{
    namespace ls
    {
        Context::Context(QObject * parent) :
            Graphics::GraphicsContext(parent),
            _fileInfo(true),
            _filePath(false),
            _sequence(Core::Sequence::COMPRESS_RANGE),
            _recurse(false),
            _hidden(false),
            _columns(Core::System::terminalWidth()),
            _sort(Core::FileInfoUtil::SORT_NAME),
            _reverseSort(false),
            _sortDirsFirst(true)
        {
            //DJV_DEBUG("Context::Context");

            // Load translators.
            loadTranslator("djv_ls");
        }

        Context::~Context()
        {
            //DJV_DEBUG("Context::~Context");
        }

        const QStringList & Context::input() const
        {
            return _input;
        }

        bool Context::hasFileInfo() const
        {
            return _fileInfo;
        }

        bool Context::hasFilePath() const
        {
            return _filePath;
        }

        Core::Sequence::COMPRESS Context::sequence() const
        {
            return _sequence;
        }

        bool Context::hasRecurse() const
        {
            return _recurse;
        }

        bool Context::hasHidden() const
        {
            return _hidden;
        }

        const QStringList & Context::glob() const
        {
            return _glob;
        }

        int Context::columns() const
        {
            return _columns;
        }

        Core::FileInfoUtil::SORT Context::sort() const
        {
            return _sort;
        }

        bool Context::hasReverseSort() const
        {
            return _reverseSort;
        }

        bool Context::hasSortDirsFirst() const
        {
            return _sortDirsFirst;
        }

        bool Context::commandLineParse(QStringList & in)
        {
            //DJV_DEBUG("Context::commandLineParse");
            //DJV_DEBUG_PRINT("in = " << in);

            if (!Graphics::GraphicsContext::commandLineParse(in))
                return false;

            QStringList args;
            QString     arg;
            try
            {
                while (!in.isEmpty())
                {
                    in >> arg;

                    // Parse the options.
                    if (
                        qApp->translate("djv::ls::Context", "-x_info") == arg ||
                        qApp->translate("djv::ls::Context", "-xi") == arg)
                    {
                        _fileInfo = false;
                    }
                    else if (
                        qApp->translate("djv::ls::Context", "-file_path") == arg ||
                        qApp->translate("djv::ls::Context", "-fp") == arg)
                    {
                        _filePath = true;
                    }
                    else if (
                        qApp->translate("djv::ls::Context", "-seq") == arg ||
                        qApp->translate("djv::ls::Context", "-q") == arg)
                    {
                        in >> _sequence;
                    }
                    else if (
                        qApp->translate("djv::ls::Context", "-recurse") == arg ||
                        qApp->translate("djv::ls::Context", "-r") == arg)
                    {
                        _recurse = true;
                    }
                    else if (
                        qApp->translate("djv::ls::Context", "-hidden") == arg)
                    {
                        _hidden = true;
                    }
                    else if (
                        qApp->translate("djv::ls::Context", "-columns") == arg ||
                        qApp->translate("djv::ls::Context", "-c") == arg)
                    {
                        in >> _columns;
                    }

                    // Parse the sorting options.
                    else if (
                        qApp->translate("djv::ls::Context", "-sort") == arg ||
                        qApp->translate("djv::ls::Context", "-s") == arg)
                    {
                        in >> _sort;
                    }
                    else if (
                        qApp->translate("djv::ls::Context", "-reverse_sort") == arg ||
                        qApp->translate("djv::ls::Context", "-rs") == arg)
                    {
                        _reverseSort = true;
                    }
                    else if (
                        qApp->translate("djv::ls::Context", "-x_sort_dirs") == arg ||
                        qApp->translate("djv::ls::Context", "-xsd") == arg)
                    {
                        _sortDirsFirst = false;
                    }

                    // Parse the arguments.
                    else
                    {
                        _input += arg;
                    }
                }
            }
            catch (const QString &)
            {
                throw QString(arg);
            }

            return true;
        }

        QString Context::commandLineHelp() const
        {
            static const QString label = qApp->translate("djv::ls::Context",
                "djv_ls\n"
                "\n"
                "    The djv_ls application is a command line tool for listing directories "
                "with file sequences.\n"
                "\n"
                "    Example output:\n"
                "\n"
                "    el_cerrito_bart.1k.tiff   File 2.23MB darby rw Mon Jun 12 21:21:55 2006\n"
                "    richmond_train.2k.tiff    File 8.86MB darby rw Mon Jun 12 21:21:58 2006\n"
                "    fishpond.1-749.png       Seq 293.17MB darby rw Thu Aug 17 16:47:43 2006\n"
                "\n"
                "    Key:\n"
                "\n"
                "    (name)                        (type) (size) (user) (permissions) (time)\n"
                "\n"
                "    File types:                   Permissions:\n"
                "\n"
                "    File - Regular file           r - Readable\n"
                "    Seq  - File sequence          w - Writable\n"
                "    Dir  - Directory              x - Executable\n"
                "\n"
                "Usage\n"
                "\n"
                "    djv_ls [file|directory]... [option]...\n"
                "\n"
                "    file      - One or more files or image sequences\n"
                "    directory - One or more directories\n"
                "    option    - Additional options (see below)\n"
                "\n"
                "    If no files or directories are given then the current directory will be "
                "used.\n"
                "\n"
                "Options\n"
                "\n"
                "    -x_info, -xi\n"
                "        Don't show information, only file names.\n"
                "    -file_path, -fp\n"
                "        Show file path names.\n"
                "    -seq, -q (value)\n"
                "        Set file sequencing. Options = %1. Default = %2.\n"
                "    -recurse, -r\n"
                "        Descend into sub-directories.\n"
                "    -hidden\n"
                "        Show hidden files.\n"
                "    -columns, -c (value)\n"
                "        Set the number of columns for formatting output. A value of zero "
                "disables formatting.\n"
                "\n"
                "Sorting Options\n"
                "\n"
                "    -sort, -s (value)\n"
                "        Set the sorting. Options = %3. Default = %4.\n"
                "    -reverse_sort, -rs\n"
                "        Reverse the sorting order.\n"
                "    -x_sort_dirs, -xsd\n"
                "        Don't sort directories first.\n"
                "%5"
                "\n"
                "Examples\n"
                "\n"
                "    List the current directory:\n"
                "\n"
                "    > djv_ls\n"
                "\n"
                "    List specific directories:\n"
                "\n"
                "    > djv_ls ~/movies ~/pictures\n"
                "\n"
                "    Sort by time with the most recent first:\n"
                "\n"
                "    > djv_ls -sort time -reverse_sort\n");
            QStringList sequenceLabel;
            sequenceLabel << _sequence;
            QStringList sortLabel;
            sortLabel << _sort;
            return QString(label).
                arg(Core::Sequence::compressLabels().join(", ")).
                arg(sequenceLabel.join(", ")).
                arg(Core::FileInfoUtil::sortLabels().join(", ")).
                arg(sortLabel.join(", ")).
                arg(Graphics::GraphicsContext::commandLineHelp());
        }

    } // namespace ls
} // namespace djv