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

#include <djvUI/FileBrowserPrefsWidget.h>

#include <djvUI/FileBrowserPrefs.h>
#include <djvUI/UIContext.h>
#include <djvUI/IconLibrary.h>
#include <djvUI/IntEdit.h>
#include <djvUI/InputDialog.h>
#include <djvUI/PrefsGroupBox.h>
#include <djvUI/ShortcutsWidget.h>
#include <djvUI/ToolButton.h>

#include <djvCore/Memory.h>
#include <djvCore/SignalBlocker.h>

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QListWidget>
#include <QPointer>
#include <QVBoxLayout>

namespace djv
{
    namespace UI
    {
        namespace
        {

            class SmallListWidget : public QListWidget
            {
            public:
                QSize sizeHint() const
                {
                    const QSize size = QListWidget::sizeHint();
                    return QSize(size.width(), size.height() / 2);
                }
            };

        } // namespace

        struct FileBrowserPrefsWidget::Private
        {
            QPointer<QCheckBox> showHiddenWidget;
            QPointer<QComboBox> sortWidget;
            QPointer<QCheckBox> reverseSortWidget;
            QPointer<QCheckBox> sortDirsFirstWidget;
            QPointer<QComboBox> thumbnailModeWidget;
            QPointer<QComboBox> thumbnailSizeWidget;
            QPointer<IntEdit> thumbnailCacheWidget;
            QPointer<QListWidget> bookmarksWidget;
            QPointer<ToolButton> addBookmarkButton;
            QPointer<ToolButton> removeBookmarkButton;
            QPointer<ToolButton> moveBookmarkUpButton;
            QPointer<ToolButton> moveBookmarkDownButton;
            QPointer<ShortcutsWidget> shortcutsWidget;
            QPointer<QVBoxLayout> layout;
        };

        FileBrowserPrefsWidget::FileBrowserPrefsWidget(
            const QPointer<UIContext> & context,
            QWidget * parent) :
            AbstractPrefsWidget(qApp->translate("djv::UI::FileBrowserPrefsWidget", "File Browser"), context, parent),
            _p(new Private)
        {
            // Create the widgets.
            _p->showHiddenWidget = new QCheckBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Show hidden files"));

            _p->sortWidget = new QComboBox;
            _p->sortWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            _p->sortWidget->addItems(FileBrowserModel::columnsLabels());

            _p->reverseSortWidget = new QCheckBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Reverse the sort direction"));

            _p->sortDirsFirstWidget = new QCheckBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Sort directories first"));

            _p->thumbnailModeWidget = new QComboBox;
            _p->thumbnailModeWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            _p->thumbnailModeWidget->addItems(FileBrowserModel::thumbnailModeLabels());

            _p->thumbnailSizeWidget = new QComboBox;
            _p->thumbnailSizeWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            _p->thumbnailSizeWidget->addItems(FileBrowserModel::thumbnailSizeLabels());

            _p->thumbnailCacheWidget = new IntEdit;
            _p->thumbnailCacheWidget->setRange(0, 4096);
            _p->thumbnailCacheWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            _p->bookmarksWidget = new SmallListWidget;

            _p->addBookmarkButton = new ToolButton(context);
            _p->addBookmarkButton->setToolTip(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Add a new bookmark"));
            _p->removeBookmarkButton = new ToolButton(context);
            _p->removeBookmarkButton->setAutoRepeat(true);
            _p->removeBookmarkButton->setToolTip(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Remove the selected bookmark"));
            _p->moveBookmarkUpButton = new ToolButton(context);
            _p->moveBookmarkUpButton->setAutoRepeat(true);
            _p->moveBookmarkUpButton->setToolTip(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Move the selected bookmark up"));
            _p->moveBookmarkDownButton = new ToolButton(context);
            _p->moveBookmarkDownButton->setAutoRepeat(true);
            _p->moveBookmarkDownButton->setToolTip(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Move the selected bookmark down"));

            _p->shortcutsWidget = new ShortcutsWidget(context);

            // Layout the widgets.
            _p->layout = new QVBoxLayout(this);

            PrefsGroupBox * prefsGroupBox = new PrefsGroupBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Files"), context);
            QFormLayout * formLayout = prefsGroupBox->createLayout();
            formLayout->addRow(_p->showHiddenWidget);
            _p->layout->addWidget(prefsGroupBox);

            prefsGroupBox = new PrefsGroupBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Sorting"), context);
            formLayout = prefsGroupBox->createLayout();
            formLayout->addRow(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Sort by:"),
                _p->sortWidget);
            formLayout->addRow(_p->reverseSortWidget);
            formLayout->addRow(_p->sortDirsFirstWidget);
            _p->layout->addWidget(prefsGroupBox);

            prefsGroupBox = new PrefsGroupBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Thumbnails"), context);
            formLayout = prefsGroupBox->createLayout();
            formLayout->addRow(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Mode:"),
                _p->thumbnailModeWidget);
            formLayout->addRow(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Size:"),
                _p->thumbnailSizeWidget);
            QHBoxLayout * hLayout = new QHBoxLayout;
            hLayout->addWidget(_p->thumbnailCacheWidget);
            hLayout->addWidget(
                new QLabel(qApp->translate("djv::UI::FileBrowserPrefsWidget", "(MB)")));
            formLayout->addRow(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Cache size:"),
                hLayout);
            _p->layout->addWidget(prefsGroupBox);

            prefsGroupBox = new PrefsGroupBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Bookmarks"), context);
            formLayout = prefsGroupBox->createLayout();
            formLayout->addRow(_p->bookmarksWidget);
            hLayout = new QHBoxLayout;
            hLayout->addStretch();
            hLayout->addWidget(_p->addBookmarkButton);
            hLayout->addWidget(_p->removeBookmarkButton);
            hLayout->addWidget(_p->moveBookmarkUpButton);
            hLayout->addWidget(_p->moveBookmarkDownButton);
            formLayout->addRow(hLayout);
            _p->layout->addWidget(prefsGroupBox);

            prefsGroupBox = new PrefsGroupBox(
                qApp->translate("djv::UI::FileBrowserPrefsWidget", "Keyboard Shortcuts"), context);
            formLayout = prefsGroupBox->createLayout();
            formLayout->addRow(_p->shortcutsWidget);
            _p->layout->addWidget(prefsGroupBox);

            // Initialize.
            styleUpdate();
            widgetUpdate();

            // Setup the callbacks.
            connect(
                _p->showHiddenWidget,
                SIGNAL(toggled(bool)),
                context->fileBrowserPrefs(),
                SLOT(setShowHidden(bool)));
            connect(
                _p->sortWidget,
                SIGNAL(activated(int)),
                SLOT(sortCallback(int)));
            connect(
                _p->reverseSortWidget,
                SIGNAL(toggled(bool)),
                context->fileBrowserPrefs(),
                SLOT(setReverseSort(bool)));
            connect(
                _p->sortDirsFirstWidget,
                SIGNAL(toggled(bool)),
                context->fileBrowserPrefs(),
                SLOT(setSortDirsFirst(bool)));
            connect(
                _p->thumbnailModeWidget,
                SIGNAL(activated(int)),
                SLOT(thumbnailModeCallback(int)));
            connect(
                _p->thumbnailSizeWidget,
                SIGNAL(activated(int)),
                SLOT(thumbnailSizeCallback(int)));
            connect(
                _p->thumbnailCacheWidget,
                SIGNAL(valueChanged(int)),
                SLOT(thumbnailCacheCallback(int)));
            connect(
                _p->bookmarksWidget,
                SIGNAL(itemChanged(QListWidgetItem *)),
                SLOT(bookmarkCallback(QListWidgetItem *)));
            connect(
                _p->addBookmarkButton,
                SIGNAL(clicked()),
                SLOT(addBookmarkCallback()));
            connect(
                _p->removeBookmarkButton,
                SIGNAL(clicked()),
                SLOT(removeBookmarkCallback()));
            connect(
                _p->moveBookmarkUpButton,
                SIGNAL(clicked()),
                SLOT(moveBookmarkUpCallback()));
            connect(
                _p->moveBookmarkDownButton,
                SIGNAL(clicked()),
                SLOT(moveBookmarkDownCallback()));
            connect(
                _p->shortcutsWidget,
                SIGNAL(shortcutsChanged(const QVector<djv::UI::Shortcut> &)),
                SLOT(shortcutsCallback(const QVector<djv::UI::Shortcut> &)));
            connect(
                context->fileBrowserPrefs(),
                SIGNAL(prefChanged()),
                SLOT(widgetUpdate()));
        }
        
        FileBrowserPrefsWidget::~FileBrowserPrefsWidget()
        {}

        void FileBrowserPrefsWidget::resetPreferences()
        {
            context()->fileBrowserPrefs()->setShowHidden(FileBrowserPrefs::showHiddenDefault());
            context()->fileBrowserPrefs()->setColumnsSort(FileBrowserPrefs::columnsSortDefault());
            context()->fileBrowserPrefs()->setReverseSort(FileBrowserPrefs::reverseSortDefault());
            context()->fileBrowserPrefs()->setSortDirsFirst(FileBrowserPrefs::sortDirsFirstDefault());
            context()->fileBrowserPrefs()->setThumbnailMode(FileBrowserPrefs::thumbnailModeDefault());
            context()->fileBrowserPrefs()->setThumbnailSize(FileBrowserPrefs::thumbnailSizeDefault());
            context()->fileBrowserPrefs()->setThumbnailCache(FileBrowserPrefs::thumbnailCacheDefault());
            context()->fileBrowserPrefs()->setShortcuts(FileBrowserPrefs::shortcutsDefault());
        }

        void FileBrowserPrefsWidget::sortCallback(int index)
        {
            context()->fileBrowserPrefs()->setColumnsSort(static_cast<FileBrowserModel::COLUMNS>(index));
        }

        void FileBrowserPrefsWidget::thumbnailModeCallback(int index)
        {
            context()->fileBrowserPrefs()->setThumbnailMode(static_cast<FileBrowserModel::THUMBNAIL_MODE>(index));
        }

        void FileBrowserPrefsWidget::thumbnailSizeCallback(int index)
        {
            context()->fileBrowserPrefs()->setThumbnailSize(static_cast<FileBrowserModel::THUMBNAIL_SIZE>(index));
        }

        void FileBrowserPrefsWidget::thumbnailCacheCallback(int value)
        {
            context()->fileBrowserPrefs()->setThumbnailCache(value * Core::Memory::megabyte);
        }

        void FileBrowserPrefsWidget::bookmarkCallback(QListWidgetItem * item)
        {
            QStringList bookmarks = context()->fileBrowserPrefs()->bookmarks();
            bookmarks[_p->bookmarksWidget->row(item)] = item->data(Qt::EditRole).toString();
            context()->fileBrowserPrefs()->setBookmarks(bookmarks);
        }

        void FileBrowserPrefsWidget::addBookmarkCallback()
        {
            InputDialog dialog(qApp->translate("djv::UI::FileBrowserPrefsWidget", "Add bookmark:"));
            if (QDialog::Accepted == dialog.exec())
            {
                context()->fileBrowserPrefs()->addBookmark(dialog.text());
            }
        }

        void FileBrowserPrefsWidget::removeBookmarkCallback()
        {
            int index = _p->bookmarksWidget->currentRow();
            QStringList bookmarks = context()->fileBrowserPrefs()->bookmarks();
            if (-1 == index)
                index = bookmarks.count() - 1;
            if (index != -1)
            {
                bookmarks.removeAt(index);
                context()->fileBrowserPrefs()->setBookmarks(bookmarks);
                _p->bookmarksWidget->setCurrentRow(index >= bookmarks.count() ? (index - 1) : index);
            }
        }

        void FileBrowserPrefsWidget::moveBookmarkUpCallback()
        {
            int index = _p->bookmarksWidget->currentRow();
            if (index != -1)
            {
                QStringList bookmarks = context()->fileBrowserPrefs()->bookmarks();
                QString bookmark = bookmarks[index];
                bookmarks.removeAt(index);
                if (index > 0)
                    --index;
                bookmarks.insert(index, bookmark);
                context()->fileBrowserPrefs()->setBookmarks(bookmarks);
                _p->bookmarksWidget->setCurrentRow(index);
            }
        }

        void FileBrowserPrefsWidget::moveBookmarkDownCallback()
        {
            int index = _p->bookmarksWidget->currentRow();
            if (index != -1)
            {
                QStringList bookmarks = context()->fileBrowserPrefs()->bookmarks();
                QString bookmark = bookmarks[index];
                bookmarks.removeAt(index);
                if (index < bookmarks.count())
                    ++index;
                bookmarks.insert(index, bookmark);
                context()->fileBrowserPrefs()->setBookmarks(bookmarks);
                _p->bookmarksWidget->setCurrentRow(index);
            }
        }

        void FileBrowserPrefsWidget::shortcutsCallback(const QVector<Shortcut> & in)
        {
            context()->fileBrowserPrefs()->setShortcuts(in);
        }

        void FileBrowserPrefsWidget::styleUpdate()
        {
            _p->addBookmarkButton->setIcon(context()->iconLibrary()->icon("djv/UI/AddIcon"));
            _p->removeBookmarkButton->setIcon(context()->iconLibrary()->icon("djv/UI/RemoveIcon"));
            _p->moveBookmarkUpButton->setIcon(context()->iconLibrary()->icon("djv/UI/UpIcon"));
            _p->moveBookmarkDownButton->setIcon(context()->iconLibrary()->icon("djv/UI/DownIcon"));
            updateGeometry();
        }
        
        void FileBrowserPrefsWidget::widgetUpdate()
        {
            Core::SignalBlocker signalBlocker(QObjectList() <<
                _p->showHiddenWidget <<
                _p->sortWidget <<
                _p->reverseSortWidget <<
                _p->sortDirsFirstWidget <<
                _p->thumbnailModeWidget <<
                _p->thumbnailSizeWidget <<
                _p->thumbnailCacheWidget <<
                _p->bookmarksWidget <<
                _p->shortcutsWidget);
            _p->showHiddenWidget->setChecked(context()->fileBrowserPrefs()->hasShowHidden());
            _p->sortWidget->setCurrentIndex(context()->fileBrowserPrefs()->columnsSort());
            _p->reverseSortWidget->setChecked(context()->fileBrowserPrefs()->hasReverseSort());
            _p->sortDirsFirstWidget->setChecked(context()->fileBrowserPrefs()->hasSortDirsFirst());
            _p->thumbnailModeWidget->setCurrentIndex(context()->fileBrowserPrefs()->thumbnailMode());
            _p->thumbnailSizeWidget->setCurrentIndex(context()->fileBrowserPrefs()->thumbnailSize());
            _p->thumbnailCacheWidget->setValue(context()->fileBrowserPrefs()->thumbnailCache() / Core::Memory::megabyte);
            _p->bookmarksWidget->clear();
            const QStringList & bookmarks = context()->fileBrowserPrefs()->bookmarks();
            for (int i = 0; i < bookmarks.count(); ++i)
            {
                QListWidgetItem * item = new QListWidgetItem(_p->bookmarksWidget);
                item->setFlags(
                    Qt::ItemIsSelectable |
                    Qt::ItemIsEditable |
                    Qt::ItemIsEnabled);
                item->setData(Qt::DisplayRole, bookmarks[i]);
                item->setData(Qt::EditRole, bookmarks[i]);
            }
            _p->shortcutsWidget->setShortcuts(context()->fileBrowserPrefs()->shortcuts());
        }

        bool FileBrowserPrefsWidget::event(QEvent * event)
        {
            if (QEvent::StyleChange == event->type())
            {
                styleUpdate();
            }
            return AbstractPrefsWidget::event(event);
        }

    } // namespace UI
} // namespace djv
