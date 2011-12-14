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

/// \file
/// \bref Declaration of the \ref SvgViewer class;

#ifndef __SVGVIEWER_H__
#define __SVGVIEWER_H__

#include <QScrollArea>
#include <QScrollBar>
#include <QSvgWidget>
#include <QPaintEvent>
#include <QSvgRenderer>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>

class QScrollArea;
class QScrollBar;
class QSvgWidget;
class QPaintEvent;
class QSvgRenderer;
class QPainter;
class QPrinter;
class QPrintDialog;

class SvgViewer : public QScrollArea
{
Q_OBJECT

public:

    /** \brief Constructor
     * Sets white background
     *  \param[in] parent  parent widget */
    SvgViewer(QWidget* parent = 0);

    /** \brief Sets the widget for working with SVG content
     *  \param[in] content  the specified SVG format content */
    void setContent(const QByteArray& content);

    /** \brief Gets content of SVG viewer
     *  \return SVG content */
    const QByteArray& getContent();

    /// \brief Prints the SVG content
    void print();

    /** \brief Changes color of functions with the same names
     * from black to red.
     *  \param[in] funcName  name of function */
    void find(const QString& funcName);

private:
    QSvgWidget*  view;                        /** Widget for working with SVG
                                               * content */
    QByteArray   content;                     ///< SVG content
    QPoint       mousePressPos;               ///< The position of mouse cursor
    QPoint       scrollBarValuesOnMousePress; /** The position of scroll bar
                                               * when mouse is pressed */

    /** \brief Accepts the position of mouse cursor
     *  \param[in] event  mouse event */
    void mousePressEvent(QMouseEvent* event);

    /** \brief Changes the position of scroll bar
     *  \param[in] event  mouse event */
    void mouseMoveEvent(QMouseEvent* event);

    /** \brief Resizes SVG subject to scrolling of wheel
     *  \param[in] event  wheel event */
    virtual void wheelEvent(QWheelEvent* event);

    /** \brief Processes the event of key pressing
     * if key is "D" sets the default size of image
     *  \param[in] event  key event */
    void keyPressEvent(QKeyEvent* event);
};

#endif // __SVGVIEWER_H__
