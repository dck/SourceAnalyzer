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
/// \brief Declaration of the \ref UpdateCallGraphWidget class;

#ifndef __UPDATECALLGRAPHWIDGET_H__
#define __UPDATECALLGRAPHWIDGET_H__

#include <QSettings>
#include <QProcess>
#include <QFileDialog>
#include <QCloseEvent>

#include "mainwindow.h"
#include "ui_updatecallgraphwidget.h"

class QSettings;
class QProcess;
class QFileDialog;
class QCloseEvent;

class MainWindow;

class UpdateCallGraphWidget : public QDialog, private Ui::UpdateCallGraphWidget
{
Q_OBJECT

public:

    /** \brief Constructor
     * Creates all elements of widget and connects signals with slots
     *  \param[in] parent  main window */
    UpdateCallGraphWidget(QWidget *parent);

private:

    MainWindow*   mainWindow;          ///< Main window of application
    QSettings*    settings;            ///< Settings
    QString       callGraphName;       ///< Output call graph

    /// \brief Shows user manual by pressing "F1"
    virtual void keyPressEvent(QKeyEvent* event);

    /// \brief Saves settings when closing window
    void closeEvent(QCloseEvent* event);

private slots:

    /// \brief Activates the button "Delete" when the list is not empty
    void activateDelButton();

    /// \brief Opens file dialog and sets input file name to line edit
    void onButtonInputFile();

    /// \brief Adds call graph to the list for updating
    void addCallGraph();

    /// \brief Deletes call graph from the list for updating
    void delCallGraph();

    /// \brief Opens file dialog and sets output file name to line edit
    void onButtonOutputFile();

    /** \brief Appends to shellOutputBrowser the call line and launch
     * the process */
    void onGenerate();

    /// \brief Calls MainWindow::suggestToOpen()
    void suggestToOpen();
};

#endif // __UPDATECALLGRAPHWIDGET_H__
