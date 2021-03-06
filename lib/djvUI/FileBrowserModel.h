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

#include <djvCore/Util.h>
#include <djvCore/FileInfo.h>

#include <QAbstractItemModel>

#include <memory>

namespace djv
{
    namespace UI
    {
        class FileBrowserItem;
        class UIContext;

        //! This class provides a file browser model.
        class FileBrowserModel : public QAbstractItemModel
        {
            Q_OBJECT

            //! This property holds the path.
            Q_PROPERTY(
                QString path
                READ    path
                WRITE   setPath
                NOTIFY  pathChanged)

            //! This property holds the file sequence formatting.
            Q_PROPERTY(
                djv::Core::Sequence::FORMAT sequence
                READ                        sequence
                WRITE                       setSequence
                NOTIFY                      sequenceChanged)

            //! This property holds the filter text.
            Q_PROPERTY(
                QString filterText
                READ    filterText
                WRITE   setFilterText
                NOTIFY  filterTextChanged)

            //! This property holds whether hidden files are shown.
            Q_PROPERTY(
                bool   showHidden
                READ   hasShowHidden
                WRITE  setShowHidden
                NOTIFY showHiddenChanged)

            //! This property holds the sorting.
            Q_PROPERTY(
                FileBrowserModel::COLUMNS columnsSort
                READ                      columnsSort
                WRITE                     setColumnsSort
                NOTIFY                    columnsSortChanged)

            //! This property holds whether sorting is reversed.
            Q_PROPERTY(
                bool   reverseSort
                READ   hasReverseSort
                WRITE  setReverseSort
                NOTIFY reverseSortChanged)

            //! This property holds whether directories are sorted first.
            Q_PROPERTY(
                bool   sortDirsFirst
                READ   hasSortDirsFirst
                WRITE  setSortDirsFirst
                NOTIFY sortDirsFirstChanged)

            //! This property holds the image thumbnail mode.
            Q_PROPERTY(
                FileBrowserModel::THUMBNAIL_MODE thumbnailMode
                READ                             thumbnailMode
                WRITE                            setThumbnailMode
                NOTIFY                           thumbnailModeChanged)

            //! This property holds the image thumbnail size.
            Q_PROPERTY(
                FileBrowserModel::THUMBNAIL_SIZE thumbnailSize
                READ                             thumbnailSize
                WRITE                            setThumbnailSize
                NOTIFY                           thumbnailSizeChanged)

        public:

            //! This enumeration provides the columns.
            enum COLUMNS
            {
                NAME,
                SIZE,
#if ! defined(DJV_WINDOWS)
                USER,
#endif
                PERMISSIONS,
                TIME,

                COLUMNS_COUNT
            };
            Q_ENUM(COLUMNS);

            //! Get the column labels.
            static const QStringList & columnsLabels();

            explicit FileBrowserModel(const QPointer<UIContext> &, QObject * parent = nullptr);
            ~FileBrowserModel() override;

            //! Get the path.
            const QString & path() const;

            //! Get the list of files.
            const Core::FileInfoList & contents() const;

            //! Convert a model index to file information.
            Core::FileInfo fileInfo(const QModelIndex &) const;

            //! Get the file sequence formatting.
            Core::Sequence::FORMAT sequence() const;

            //! Get the filter text.
            const QString & filterText() const;

            //! Get whether hidden files are shown.
            bool hasShowHidden() const;

            //! Get the sorting.
            COLUMNS columnsSort() const;

            //! Get whether sorting is reversed.
            bool hasReverseSort() const;

            //! Get whether directories are sorted first.
            bool hasSortDirsFirst() const;

            //! This enumeration provides the image thumbnail mode.
            enum THUMBNAIL_MODE
            {
                THUMBNAIL_MODE_OFF,
                THUMBNAIL_MODE_LOW,
                THUMBNAIL_MODE_HIGH,

                THUMBNAIL_MODE_COUNT
            };
            Q_ENUM(THUMBNAIL_MODE);

            //! Get the image thumbnail mode labels.
            static const QStringList & thumbnailModeLabels();

            //! Get the image thumbnail mode.
            THUMBNAIL_MODE thumbnailMode() const;

            //! This enumeration provides the image thumbnail size.
            enum THUMBNAIL_SIZE
            {
                THUMBNAIL_SMALL,
                THUMBNAIL_MEDIUM,
                THUMBNAIL_LARGE,

                THUMBNAIL_SIZE_COUNT
            };
            Q_ENUM(THUMBNAIL_SIZE);

            //! Get the image thumbnail size labels.
            static const QStringList & thumbnailSizeLabels();

            //! Get the image thumbnail size value.
            static int thumbnailSizeValue(THUMBNAIL_SIZE);

            //! Get the image thumbnail size.
            THUMBNAIL_SIZE thumbnailSize() const;

            QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;
            QModelIndex	parent(const QModelIndex & = QModelIndex()) const override;
            QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
            QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
            Qt::ItemFlags flags(const QModelIndex &) const override;
            int rowCount(const QModelIndex & parent = QModelIndex()) const override;
            int columnCount(const QModelIndex & parent = QModelIndex()) const override;
            QStringList mimeTypes() const override;
            QMimeData * mimeData(const QModelIndexList &) const override;

        public Q_SLOTS:
            //! Set the path.
            void setPath(const QString &);

            //! Reload the files.
            void reload();

            //! Set the file sequence formatting.
            void setSequence(djv::Core::Sequence::FORMAT);

            //! Set the filter text.
            void setFilterText(const QString &);

            //! Set whether hidden files are shown.
            void setShowHidden(bool);

            //! Set the sorting.
            void setColumnsSort(djv::UI::FileBrowserModel::COLUMNS);

            //! Set whether sorting is reversed.
            void setReverseSort(bool);

            //! Set whether directories are sorted first.
            void setSortDirsFirst(bool);

            //! Set the image thumbnail mode.
            void setThumbnailMode(djv::UI::FileBrowserModel::THUMBNAIL_MODE);

            //! Set the image thumbnail size.
            void setThumbnailSize(djv::UI::FileBrowserModel::THUMBNAIL_SIZE);

        Q_SIGNALS:
            //! This signal is emitted when the path is changed.
            void pathChanged(const QString &);

            //! This signal is emitted when the file sequence formatting is changed.
            void sequenceChanged(djv::Core::Sequence::FORMAT);

            //! This signal is emitted when the filter text is changed.
            void filterTextChanged(const QString &);

            //! This signal is emitted when the hidden files are changed.
            void showHiddenChanged(bool);

            //! This signal is emitted when the sorting is changed.    
            void columnsSortChanged(djv::UI::FileBrowserModel::COLUMNS);

            //! This signal is emitted when reverse sorting is changed.
            void reverseSortChanged(bool);

            //! This signal is emitted when sort directories first is changed.
            void sortDirsFirstChanged(bool);

            //! This signal is emitted when the thumbnail mode is changed.
            void thumbnailModeChanged(djv::UI::FileBrowserModel::THUMBNAIL_MODE);

            //! This signal is emitted when the thumbnail size is changed.
            void thumbnailSizeChanged(djv::UI::FileBrowserModel::THUMBNAIL_SIZE);

            //! This signal is emitted when an option is changed.
            void optionChanged();

        private Q_SLOTS:
            void ioInfoCallback();
            void thumbnailCallback();
            void sequencePrefsCallback();

            void dirUpdate();
            void modelUpdate();

        private:
            DJV_PRIVATE_COPY(FileBrowserModel);

            struct Private;
            std::unique_ptr<Private> _p;
        };

    } // namespace UI

    DJV_STRING_OPERATOR(UI::FileBrowserModel::COLUMNS);
    DJV_STRING_OPERATOR(UI::FileBrowserModel::THUMBNAIL_MODE);
    DJV_STRING_OPERATOR(UI::FileBrowserModel::THUMBNAIL_SIZE);

} // namespace djv


