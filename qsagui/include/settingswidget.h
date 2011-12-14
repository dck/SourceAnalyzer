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
/// \brief Declaration of the \ref SettingsWidget class.

#ifndef __SETTINGSWIDGET_H__
#define __SETTINGSWIDGET_H__

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QTextStream>
#include <QCloseEvent>

#include "mainwindow.h"
#include "saguiconstants.h"
#include "ui_settingswidget.h"

class QFileDialog;
class QMessageBox;
class QSettings;
class QProcess;
class QTextStream;
class QCloseEvent;

class MainWindow;

class SettingsWidget : public QDialog, private Ui::SettingsWidget
{
Q_OBJECT

public:

    /** \brief Constructor
     * Sets parent widget widget and connects signals with slots
     *  \param[in] parent  main window */
    SettingsWidget(QWidget *parent);

private:

    MainWindow*  mainWindow;     ///< Main window of application
    QSettings*   settings;       ///< Settings

    /// \brief Shows user manual by pressing "F1"
    virtual void keyPressEvent(QKeyEvent* event);

    /// \brief Saves settings when closing window
    void closeEvent(QCloseEvent* event);

    /// \brief Opens dialog for selection of directory and sets it to line edit
    void chooseDirectory(QLineEdit* editLine);

private slots:

    /// \brief Sets directory with SA binaries to line edit
    void onBrowseSADir();

    /// \brief Sets path to dot utility to line edit
    void onBrowseDotPath();

    /// \brief Sets working directory to line edit
    void onBrowseWorkDir();

    /// \brief Sets directory for saving of temp files to line edit
    void onBrowseSavingDir();

    /** \brief Reads the text in line edit, sets pathes and adds
     * in shell browser message of where configuration file saved to */
    void onOk();
};

#endif // __SETTINGSWIDGET_H__
