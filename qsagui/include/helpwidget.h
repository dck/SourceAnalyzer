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
/// \brief Declaration of the \ref HelpWidget class;

#ifndef __HELPWIDGET_H__
#define __HELPWIDGET_H__

#include <QSettings>
#include <QUrl>
#include <QKeyEvent>
#include <QCloseEvent>

#include "saguiconstants.h"
#include "ui_helpwidget.h"

class QSettings;
class QUrl;
class QKeyEvent;
class QCloseEvent;

class HelpWidget : public QWidget, private Ui::HelpWidget
{
Q_OBJECT

public:

    /** \brief Constructor
     * Creates all elements of widget and connects signals with slots */
    HelpWidget(QWidget *parent);

    /** \brief Sets page with given url in the text browser
     *  \param[in] url  url address */
    void showPage(QUrl url);

private:
    QSettings*    settings;     ///< Settings

    /// \brief
    virtual void keyPressEvent(QKeyEvent* event);

    /// \brief Saves settings when closing window
    void closeEvent(QCloseEvent* event);
};

#endif // __HELPWIDGET_H__
