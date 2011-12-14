// Copyright 2008-2011 SourceAnalyzer team. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY SOURCEANALYZER TEAM ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SOURCEANALYZER TEAM OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of SourceAnalyzer team.

/*! \class SvgViewer svgviewer.h "../../include/svgviewer.h"
 *  \brief Widget in central area for showing of SVG.
 *
 * Opens the SVG picture, creates object of SvgNativeView class for processing
 * it and shows it.
 */

/// \file
/// \bref Implementation of the \ref SvgViewer class.

#include <QScrollBar>
#include <QMouseEvent>
#include <QSvgWidget>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QSvgRenderer>

#include "svgviewer.h"

SvgViewer::SvgViewer(QWidget* parent)
    : QScrollArea(parent)
{
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    setPalette(palette);
}

////////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////

void SvgViewer::setContent(const QByteArray& content) {
    this->content = content;
    view = new QSvgWidget(this);
    view->load(content);
    setWidget(view);
}

const QByteArray& SvgViewer::getContent() {
    return content;
}

void SvgViewer::print() {
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = view->renderer()->defaultSize();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        view->renderer()->render(&painter);
    }
}

void SvgViewer::find(const QString& funcName) {
    int width = view->width();
    int height = view->height();
    int hValue = horizontalScrollBar()->value();
    int vValue = verticalScrollBar()->value();
    content.replace("red", "black");
    if (funcName != "") {
        QString searchStr = "class=\"node\"><title>" + funcName;
        for (int i = 0; i < content.size(); i++) {
            i = content.indexOf(searchStr, i);
            if (i == -1)
                break;
            content.replace(content.indexOf("black", i), 5, "red");
        }
    }
    setContent(content);
    view->resize(width, height);
    horizontalScrollBar()->setValue(hValue);
    verticalScrollBar()->setValue(vValue);
}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS ///////////////////////////////////////////////////////////

void SvgViewer::mousePressEvent(QMouseEvent* event) {
    mousePressPos = event->pos();
    scrollBarValuesOnMousePress.rx() = horizontalScrollBar()->value();
    scrollBarValuesOnMousePress.ry() = verticalScrollBar()->value();
    event->accept();
}

void SvgViewer::mouseMoveEvent(QMouseEvent* event) {
    if (mousePressPos.isNull()) {
        event->ignore();
        return;
    }
    horizontalScrollBar()->setValue(scrollBarValuesOnMousePress.x() -
                                    event->pos().x() + mousePressPos.x());
    verticalScrollBar()->setValue(scrollBarValuesOnMousePress.y() -
                                  event->pos().y() + mousePressPos.y());
    horizontalScrollBar()->update();
    verticalScrollBar()->update();
    event->accept();
}

void SvgViewer::wheelEvent(QWheelEvent* event) {
    const double diff = 0.1;
    QSize size = view->renderer()->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (event->delta() > 0)
    {
        width = int(view->width()+view->width()*diff);
        height = int(view->height()+view->height()*diff);
    } else
    {
        width  = int(view->width()-view->width()*diff);
        height = int(view->height()-view->height()*diff);
    }
    view->resize(width, height);
}

void SvgViewer::keyPressEvent(QKeyEvent* event) {
    QSize size = view->renderer()->defaultSize();
    int width  = size.width();
    int height = size.height();
    if (event->key() == Qt::Key_D)
    {
        view->resize(width, height);
    }
    event->accept();
}
