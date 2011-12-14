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
