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
