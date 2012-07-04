/*

Copyright 2012 Adam Reichold

This file is part of qpdfview.

qpdfview is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

qpdfview is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with qpdfview.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
    m_settings = new QSettings(this);

    m_dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(m_dialogButtonBox, SIGNAL(accepted()), SLOT(accept()));
    connect(m_dialogButtonBox, SIGNAL(rejected()), SLOT(reject()));

    m_defaultsButton = m_dialogButtonBox->addButton(tr("Defaults"), QDialogButtonBox::ResetRole);
    connect(m_defaultsButton, SIGNAL(clicked()), SLOT(on_defaults_clicked()));

    m_formLayout = new QFormLayout(this);
    setLayout(m_formLayout);

    // tab position

    m_tabPositionComboBox = new QComboBox(this);
    m_tabPositionComboBox->addItem(tr("Top"), static_cast< uint >(QTabWidget::North));
    m_tabPositionComboBox->addItem(tr("Bottom"), static_cast< uint >(QTabWidget::South));
    m_tabPositionComboBox->addItem(tr("Left"), static_cast< uint >(QTabWidget::West));
    m_tabPositionComboBox->addItem(tr("Right"), static_cast< uint >(QTabWidget::East));

    uint tabPosition = static_cast< uint >(m_settings->value("mainWindow/tabPosition", 0).toUInt());

    for(int index = 0; index < m_tabPositionComboBox->count(); index++)
    {
        if(m_tabPositionComboBox->itemData(index).toUInt() == tabPosition)
        {
            m_tabPositionComboBox->setCurrentIndex(index);
        }
    }

    // tab visibility

    m_tabVisibilityComboBox = new QComboBox(this);
    m_tabVisibilityComboBox->addItem(tr("As needed"), static_cast< uint >(TabWidget::TabBarAsNeeded));
    m_tabVisibilityComboBox->addItem(tr("Always"), static_cast< uint >(TabWidget::TabBarAlwaysOn));
    m_tabVisibilityComboBox->addItem(tr("Never"), static_cast< uint >(TabWidget::TabBarAlwaysOff));

    uint tabBarPolicy = static_cast< uint >(m_settings->value("mainWindow/tabVisibility", 0).toUInt());

    for(int index = 0; index < m_tabVisibilityComboBox->count(); index++)
    {
        if(m_tabVisibilityComboBox->itemData(index).toUInt() == tabBarPolicy)
        {
            m_tabVisibilityComboBox->setCurrentIndex(index);
        }
    }

    // open URL

    m_openUrlCheckBox = new QCheckBox(this);
    m_openUrlCheckBox->setChecked(m_settings->value("documentView/openUrl", false).toBool());

    // auto-refresh

    m_autoRefreshCheckBox = new QCheckBox(this);
    m_autoRefreshCheckBox->setChecked(m_settings->value("documentView/autoRefresh", false).toBool());

    // restore tabs

    m_restoreTabsCheckBox = new QCheckBox(this);
    m_restoreTabsCheckBox->setChecked(m_settings->value("mainWindow/restoreTabs", false).toBool());

    // restore bookmarks

    m_restoreBookmarksCheckBox = new QCheckBox(this);
    m_restoreBookmarksCheckBox->setChecked(m_settings->value("mainWindow/restoreBookmarks", false).toBool());

    // decorate pages

    m_decoratePagesCheckBox = new QCheckBox(this);
    m_decoratePagesCheckBox->setChecked(m_settings->value("pageItem/decoratePages", true).toBool());

    // decorate links

    m_decorateLinksCheckBox = new QCheckBox(this);
    m_decorateLinksCheckBox->setChecked(m_settings->value("pageItem/decorateLinks", true).toBool());

    // page spacing

    m_pageSpacingSpinBox = new QDoubleSpinBox(this);
    m_pageSpacingSpinBox->setSuffix(" px");
    m_pageSpacingSpinBox->setRange(0.0, 25.0);
    m_pageSpacingSpinBox->setSingleStep(0.25);
    m_pageSpacingSpinBox->setValue(m_settings->value("documentView/pageSpacing", 5.0).toDouble());

    // thumbnail spacing

    m_thumbnailSpacingSpinBox = new QDoubleSpinBox(this);
    m_thumbnailSpacingSpinBox->setSuffix(" px");
    m_thumbnailSpacingSpinBox->setRange(0.0, 25.0);
    m_thumbnailSpacingSpinBox->setSingleStep(0.25);
    m_thumbnailSpacingSpinBox->setValue(m_settings->value("documentView/thumbnailSpacing", 3.0).toDouble());

    // thumbnail size

    m_thumbnailSizeSpinBox = new QDoubleSpinBox(this);
    m_thumbnailSizeSpinBox->setSuffix(" px");
    m_thumbnailSizeSpinBox->setRange(30.0, 300.0);
    m_thumbnailSizeSpinBox->setSingleStep(10.0);
    m_thumbnailSizeSpinBox->setValue(m_settings->value("documentView/thumbnailSize", 150.0).toDouble());

    // antialiasing

    m_antialiasingCheckBox = new QCheckBox(this);
    m_antialiasingCheckBox->setChecked(m_settings->value("documentView/antialiasing", true).toBool());

    // text antialising

    m_textAntialiasingCheckBox = new QCheckBox(this);
    m_textAntialiasingCheckBox->setChecked(m_settings->value("documentView/textAntialiasing", true).toBool());

    // text hinting

    m_textHintingCheckBox = new QCheckBox(this);
    m_textHintingCheckBox->setChecked(m_settings->value("documentView/textHinting", false).toBool());

    // cache size

    m_cacheSizeComboBox = new QComboBox(this);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(0), 0);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(8), 8 * 1024 * 1024);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(16), 16 * 1024 * 1024);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(32), 32 * 1024 * 1024);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(64), 64 * 1024 * 1024);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(128), 128 * 1024 * 1024);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(256), 256 * 1024 * 1024);
    m_cacheSizeComboBox->addItem(tr("%1 MB").arg(512), 512 * 1024 * 1024);

    int cacheSize = m_settings->value("pageItem/cacheSize", 32 * 1024 * 1024).toInt();

    for(int index = 0; index < m_cacheSizeComboBox->count(); index++)
    {
        if(m_cacheSizeComboBox->itemData(index).toInt() == cacheSize)
        {
            m_cacheSizeComboBox->setCurrentIndex(index);
        }
    }

    // prefetch

    m_prefetchCheckBox = new QCheckBox(this);
    m_prefetchCheckBox->setChecked(m_settings->value("documentView/prefetch", false).toBool());

    // file tool bar

    m_fileToolBarLineEdit = new QLineEdit(this);
    m_fileToolBarLineEdit->setText(m_settings->value("mainWindow/fileToolBar", QStringList() << "openInNewTab" << "refresh").toStringList().join(","));
    m_fileToolBarLineEdit->setToolTip(tr("Effective after restart."));

    // edit tool bar

    m_editToolBarLineEdit = new QLineEdit(this);
    m_editToolBarLineEdit->setText(m_settings->value("mainWindow/editToolBar", QStringList() << "currentPage" << "numberOfPages" << "previousPage" << "nextPage").toStringList().join(","));
    m_editToolBarLineEdit->setToolTip(tr("Effective after restart."));

    // view tool bar

    m_viewToolBarLineEdit = new QLineEdit(this);
    m_viewToolBarLineEdit->setText(m_settings->value("mainWindow/viewToolBar", QStringList() << "scaleFactor" << "zoomIn" << "zoomOut").toStringList().join(","));
    m_viewToolBarLineEdit->setToolTip(tr("Effective after restart."));

    m_formLayout->addRow(tr("Tab position:"), m_tabPositionComboBox);
    m_formLayout->addRow(tr("Tab visibility:"), m_tabVisibilityComboBox);

    m_formLayout->addRow(tr("Open URL:"), m_openUrlCheckBox);

    m_formLayout->addRow(tr("Auto-refresh:"), m_autoRefreshCheckBox);

    m_formLayout->addRow(tr("Restore tabs:"), m_restoreTabsCheckBox);
    m_formLayout->addRow(tr("Restore bookmarks:"), m_restoreBookmarksCheckBox);

    m_formLayout->addRow(tr("Decorate pages:"), m_decoratePagesCheckBox);
    m_formLayout->addRow(tr("Decorate links:"), m_decorateLinksCheckBox);

    m_formLayout->addRow(tr("Page spacing:"), m_pageSpacingSpinBox);
    m_formLayout->addRow(tr("Thumbnail spacing:"), m_thumbnailSpacingSpinBox);

    m_formLayout->addRow(tr("Thumbnail size:"), m_thumbnailSizeSpinBox);

    m_formLayout->addRow(tr("Antialiasing:"), m_antialiasingCheckBox);
    m_formLayout->addRow(tr("Text antialiasing:"), m_textAntialiasingCheckBox);
    m_formLayout->addRow(tr("Text hinting:"), m_textHintingCheckBox);

    m_formLayout->addRow(tr("Cache size:"), m_cacheSizeComboBox);

    m_formLayout->addRow(tr("Prefetch:"), m_prefetchCheckBox);

    m_formLayout->addRow(tr("File tool bar:"), m_fileToolBarLineEdit);
    m_formLayout->addRow(tr("Edit tool bar:"), m_editToolBarLineEdit);
    m_formLayout->addRow(tr("View tool bar:"), m_viewToolBarLineEdit);

    m_formLayout->addRow(m_dialogButtonBox);
}

void SettingsDialog::accept()
{
    m_settings->setValue("mainWindow/tabPosition", m_tabPositionComboBox->itemData(m_tabPositionComboBox->currentIndex()));
    m_settings->setValue("mainWindow/tabVisibility", m_tabVisibilityComboBox->itemData(m_tabVisibilityComboBox->currentIndex()));

    m_settings->setValue("documentView/openUrl", m_openUrlCheckBox->isChecked());

    m_settings->setValue("documentView/autoRefresh", m_autoRefreshCheckBox->isChecked());

    m_settings->setValue("mainWindow/restoreTabs", m_restoreTabsCheckBox->isChecked());
    m_settings->setValue("mainWindow/restoreBookmarks", m_restoreBookmarksCheckBox->isChecked());

    m_settings->setValue("pageItem/decoratePages", m_decoratePagesCheckBox->isChecked());
    m_settings->setValue("pageItem/decorateLinks", m_decorateLinksCheckBox->isChecked());

    m_settings->setValue("documentView/pageSpacing", m_pageSpacingSpinBox->value());
    m_settings->setValue("documentView/thumbnailSpacing", m_thumbnailSpacingSpinBox->value());

    m_settings->setValue("documentView/thumbnailSize", m_thumbnailSizeSpinBox->value());

    m_settings->setValue("documentView/antialiasing", m_antialiasingCheckBox->isChecked());
    m_settings->setValue("documentView/textAntialiasing", m_textAntialiasingCheckBox->isChecked());
    m_settings->setValue("documentView/textHinting", m_textHintingCheckBox->isChecked());

    m_settings->setValue("pageItem/cacheSize", m_cacheSizeComboBox->itemData(m_cacheSizeComboBox->currentIndex()));

    m_settings->setValue("documentView/prefetch", m_prefetchCheckBox->isChecked());

    m_settings->setValue("mainWindow/fileToolBar", m_fileToolBarLineEdit->text().split(","));
    m_settings->setValue("mainWindow/editToolBar", m_editToolBarLineEdit->text().split(","));
    m_settings->setValue("mainWindow/viewToolBar", m_viewToolBarLineEdit->text().split(","));

    QDialog::accept();
}

void SettingsDialog::on_defaults_clicked()
{
    m_tabPositionComboBox->setCurrentIndex(0);
    m_tabVisibilityComboBox->setCurrentIndex(0);

    m_openUrlCheckBox->setChecked(false);

    m_autoRefreshCheckBox->setChecked(false);

    m_restoreTabsCheckBox->setChecked(false);
    m_restoreBookmarksCheckBox->setChecked(false);

    m_decoratePagesCheckBox->setChecked(true);
    m_decorateLinksCheckBox->setChecked(true);

    m_pageSpacingSpinBox->setValue(5.0);
    m_thumbnailSpacingSpinBox->setValue(3.0);

    m_thumbnailSizeSpinBox->setValue(150.0);

    m_antialiasingCheckBox->setChecked(true);
    m_textAntialiasingCheckBox->setChecked(true);
    m_textHintingCheckBox->setChecked(false);

    m_cacheSizeComboBox->setCurrentIndex(3);

    m_prefetchCheckBox->setChecked(false);

    m_fileToolBarLineEdit->setText("openInNewTab,refresh");
    m_editToolBarLineEdit->setText("currentPage,numberOfPages,previousPage,nextPage");
    m_viewToolBarLineEdit->setText("scaleFactor,zoomIn,zoomOut");
}
