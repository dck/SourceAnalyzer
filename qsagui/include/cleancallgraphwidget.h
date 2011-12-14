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
/** \brief Declaration of the \ref CleanCallGraphWidget class; */

#ifndef __CLEANCALLGRAPHWIDGET_H__
#define __CLEANCALLGRAPHWIDGET_H__

#include <QSettings>
#include <QProcess>
#include <QFileDialog>
#include <QKeyEvent>
#include <QCloseEvent>

#include "saguiconstants.h"
#include "mainwindow.h"
#include "ui_cleancallgraphwidget.h"

class QSettings;
class QProcess;
class QFileDialog;
class QKeyEvent;
class QCloseEvent;

class MainWindow;
class AddNameDialog;

class CleanCallGraphWidget : public QDialog, private Ui::CleanCallGraphWidget
{
Q_OBJECT

public:

    /** \brief Constructor
     * Creates all elements of widget and connects signals with slots
     *  \param[in] parent  main window */
    CleanCallGraphWidget(QWidget *parent);

private:

    MainWindow*     mainWindow;         ///< Main window of application
    QSettings*      settings;           ///< Settings
    AddNameDialog*  addDialog;          /** The dialog for adding file
                                         * to the list */
    QString         qStringInputFile;   ///< Input call graph
    QString         callGraphName;      ///< Output call graph

    /// \brief Shows user manual by pressing "F1"
    virtual void keyPressEvent(QKeyEvent* event);

    /// \brief Saves settings when closing window
    void closeEvent(QCloseEvent* event);

private slots:

    /// \brief Activates the button "Delete" when the list is not empty
    void activateDelButton();

    /// \brief Adds source file to the list for cleaning
    void addFile();

    /// \brief Deletes source file from the list for cleaning
    void delFile();

    /// \brief Opens file dialog and sets input file name to line edit
    void onButtonInputFile();

    /// \brief Opens file dialog and sets output file name to line edit
    void onButtonOutputFile();

    /** \brief Appends to shellOutputBrowser the call line and launch the
     * process */
    void onGenerate();

    /// \brief Calls MainWindow::suggestToOpen()
    void suggestToOpen();
};

#endif // __CLEANCALLGRAPHWIDGET_H__
