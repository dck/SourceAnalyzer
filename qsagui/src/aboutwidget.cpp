///////////////////////////////////////////////////////////////////////////////
// This file is the part of QSAGUI source codes.                             //
// QSAGUI is a graphical user interface for Source Analyzer.                 //
// QSAGUI source codes are distributed with Source Analyzer source codes.    //
//                                                                           //
// Source Analyzer is a program that search out a call-graph of              //
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

/*! \class AboutWidget aboutwidget.h "./include/aboutwidget.h"
 *  \brief Window with information about QSAGUI.
 *
 * Shows the information about QSAGUI and developers in short.
 */

/// \file
/// \brief Implementation of the \ref AboutWidget class.

#include "aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup("About");
        move(settings->value("pos", QPoint(width(), height())).toPoint());
        settings->endGroup();
    settings->endGroup();
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void AboutWidget::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup("About");
        settings->setValue("pos", pos());
        settings->endGroup();
    settings->endGroup();
    QWidget::closeEvent(event);
}

