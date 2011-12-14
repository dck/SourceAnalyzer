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
