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
/// \brief Declaration of the \ref NewCallGraphWidget class;

#ifndef __NEWCALLGRAPHWIDGET_H__
#define __NEWCALLGRAPHWIDGET_H__

#include <QSettings>
#include <QProcess>
#include <QFileDialog>
#include <QCloseEvent>

#include "mainwindow.h"
#include "ui_newcallgraphwidget.h"

class QSettings;
class QProcess;
class QFileDialog;
class QCloseEvent;

class MainWindow;

class NewCallGraphWidget : public QDialog, private Ui::NewCallGraphWidget
{
Q_OBJECT

public:

    /** \brief Constructor
     * Creates all elements of widget
     *  \param[in] parent  main window */
    NewCallGraphWidget(QWidget *parent);

    /** \brief Runs necessary process subject to number of source files
     *  \param[in] baseDir  base directory (only for c source code)
     *  \param[in] binaryName  name of binary file
     *  \param[in] outputFileName  name of output call graph
     *  \param[in] listOfFiles  list of source files */
    void generate(const QString& baseDir, const QString& binaryName,
                  const QString& outputFileName, QListWidget*  listOfFiles);

private:
    MainWindow*        mainWindow;            ///< Main window of application
    QSettings*         settings;              ///< Settings
    QString            preprocInputFile;      ///< Name of input file
    QString            callGraphName;         ///< Name of call graph
    int                numOfPathesOfGraph;    /** Number of call graphs for
                                               * linkage
                                               * (if several files is selected)
                                               */
    int                numberOfItems;         ///< Number of items in files list
    /// \brief Shows user manual by pressing "F1"
    virtual void keyPressEvent(QKeyEvent* event);

    /// \brief Saves settings when closing window
    void closeEvent(QCloseEvent* event);

private slots:

    /// \brief
    void chooseBaseDirectory();

    /** \brief Opens dialog for selection of files and sets its to list */
    void addFilesToList();

    /// \brief Deletes files from the list for linkage
    void delFiles();

    void chooseFileToSave();

    /** \brief Calls function of call graph creation or
     * launch the process of preprocessing */
    void onGenerate();

    /// \brief Links graphs if several files are in the list
    void linkPartsOfGraph();

    /** \brief Checks exit status of preprocessing and starts process
     * of new call graph creation */
    void onPreprocFinished();

    /// \brief Calls MainWindow::suggestToOpen()
    void suggestToOpen();
};

#endif // __NEWCALLGRAPHWIDGET_H__
