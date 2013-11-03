/*

Copyright 2012-2013 Adam Reichold

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

#include "annotationwidgets.h"

#include <QAction>
#include <QDesktopServices>
#include <QFileDialog>
#include <QGraphicsProxyWidget>
#include <QMenu>
#include <QMessageBox>
#include <QMutex>
#include <QUrl>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

#include <poppler-qt5.h>

#else

#include <poppler-qt4.h>

#endif // QT_VERSION

#include <poppler-annotation.h>

static bool hideOnEscape(QWidget* widget, QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
    {
        widget->hide();

        event->accept();
        return true;
    }

    return false;
}

AnnotationWidget::AnnotationWidget(QMutex* mutex, Poppler::Annotation* annotation, QWidget* parent) : QPlainTextEdit(parent),
    m_mutex(mutex),
    m_annotation(annotation)
{
#ifndef HAS_POPPLER_24

    QMutexLocker mutexLocker(m_mutex);

#endif // HAS_POPPLER_24

    setPlainText(m_annotation->contents());

    connect(this, SIGNAL(textChanged()), SLOT(on_textChanged()));
    connect(this, SIGNAL(textChanged()), SIGNAL(wasModified()));

    moveCursor(QTextCursor::End);
}

void AnnotationWidget::keyPressEvent(QKeyEvent* event)
{
    if(!hideOnEscape(this, event))
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}

void AnnotationWidget::on_textChanged()
{
#ifndef HAS_POPPLER_24

    QMutexLocker mutexLocker(m_mutex);

#endif // HAS_POPPLER_24

    m_annotation->setContents(toPlainText());
}


FileAttachmentAnnotationWidget::FileAttachmentAnnotationWidget(QMutex* mutex, Poppler::FileAttachmentAnnotation* annotation, QWidget* parent) : QToolButton(parent),
    m_mutex(mutex),
    m_annotation(annotation)
{
    m_menu = new QMenu(this);
    m_saveAction = new QAction(tr("Save..."), this);
    m_saveAndOpenAction = new QAction(tr("Save and open..."), this);

    setMenu(m_menu);
    setPopupMode(QToolButton::InstantPopup);
    setIcon(QIcon::fromTheme(QLatin1String("mail-attachment"), QIcon(QLatin1String(":icons/mail-attachment.svg"))));

    connect(m_menu, SIGNAL(aboutToShow()), SLOT(on_aboutToShow()));
    connect(m_menu, SIGNAL(aboutToHide()), SLOT(on_aboutToHide()));

    connect(m_saveAction, SIGNAL(triggered()), SLOT(on_save_triggered()));
    connect(m_saveAndOpenAction, SIGNAL(triggered()), SLOT(on_saveAndOpen_triggered()));
}

void FileAttachmentAnnotationWidget::keyPressEvent(QKeyEvent *event)
{
    if(!hideOnEscape(this, event))
    {
        QToolButton::keyPressEvent(event);
    }
}

void FileAttachmentAnnotationWidget::on_aboutToShow()
{
    graphicsProxyWidget()->setZValue(1.0);
}

void FileAttachmentAnnotationWidget::on_aboutToHide()
{
    graphicsProxyWidget()->setZValue(0.0);
}

void FileAttachmentAnnotationWidget::on_save_triggered()
{
    save(false);
}

void FileAttachmentAnnotationWidget::on_saveAndOpen_triggered()
{
    save(true);
}

void FileAttachmentAnnotationWidget::save(bool open)
{
#ifndef HAS_POPPLER_24

    QMutexLocker mutexLocker(m_mutex);

#endif // HAS_POPPLER_24

    Poppler::EmbeddedFile* embeddedFile = m_annotation->embeddedFile();

    QString filePath = QFileDialog::getSaveFileName(0, tr("Save file attachment"), embeddedFile->name());

    if(!filePath.isEmpty())
    {
        QFile file(filePath);

        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            file.write(embeddedFile->data());

            file.close();

            if(open)
            {
                QDesktopServices::openUrl(QUrl(filePath));
            }
        }
        else
        {
            QMessageBox::warning(0, tr("Warning"), tr("Could not save file attachment to '%1'.").arg(filePath));
        }
    }
}
