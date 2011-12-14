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
/// \brief Declaration of the \ref NewCallGraphWidget class;

#ifndef __NEWCALLGRAPHWIDGET_H__
#define __NEWCALLGRAPHWIDGET_H__

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
