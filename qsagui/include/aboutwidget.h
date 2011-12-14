///////////////////////////////////////////////////////////////////////////////
// This file is the part of QSAGUI source codes.                             //
// QSAGUI is a graphical user interface for SourceAnalyzer.                  //
// QSAGUI source codes are distributed with SourceAnalyzer source codes.     //
//                                                                           //
// SourceAnalyzer is a program that search out a call-graph of               //
// given source code. See <http://trac-hg.assembla.com/SourceAnalyzer>       //
// Copyright (C) 2008-2010 BiPro Lab team                                    //
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
/// \brief Declaration of the \ref AboutWidget class;

#ifndef __ABOUTWIDGET_H__
#define __ABOUTWIDGET_H__

#include <QSettings>
#include <QCloseEvent>

#include "saguiconstants.h"
#include "ui_aboutwidget.h"

class QSettings;
class QCloseEvent;

class AboutWidget : public QDialog, private Ui::AboutWidget
{
Q_OBJECT

public:

    /** \brief Creates all elements of the window and connects signals with slots.
     * Restores size and position from settings, sets color of background,
     * sets text.
     * \param parent  parent widget */
    AboutWidget(QWidget *parent);

private:
    
    QSettings*   settings;     ///< Settings

    /// \brief Saves settings when closing window
    void closeEvent(QCloseEvent* event);
};

#endif // __ABOUTWIDGET_H__
