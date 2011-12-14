///////////////////////////////////////////////////////////////////////////////
// This file is the part of QSAGUI source codes.                             //
// QSAGUI is a graphical user interface for Source Analyzer.                 //
// QSAGUI source codes are distributed with Source Analyzer source codes.    //
//                                                                           //
// Source Analyzer is a program that search out a call-graph of              //
// given source code. See <http://trac-hg.assembla.com/SourceAnalyzer>       //
// Copyright (C) 2008 BiPro Lab team                                         //
//                                                                           //
// This program is free software: you can redistribute it and/or modify      //
// it under the terms of the GNU General Public License as published by      //
// the Free Software Foundation, either version 3 of the License,            //
// any later version.                                                        //
//                                                                           //
// This program is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU General Public License for more details.                              //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.     //
///////////////////////////////////////////////////////////////////////////////

/*! \class SvgViewer svgviewer.h "../../include/svgviewer.h"
 *  \brief Widget in central area for showing of SVG.
 *
 * Opens the SVG picture, creates object of SvgNativeView class for processing
 * it and shows it.
 */

/// \file
/// \bref Implementation of the \ref SvgViewer class.

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
