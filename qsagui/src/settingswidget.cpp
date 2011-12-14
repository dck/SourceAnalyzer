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

/*! \class SettingsWidget settingswidget.h "./include/settingswidget.h"
 *  \brief Settings window.
 *
 * Choice of the directory with executable file of SourceAnalyzer and working
 * directory.
 */

/// \file
/// \brief Implementation of the \ref SettingsWidget class.

#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    mainWindow = (MainWindow*) parent;

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup("Settings");
        restoreGeometry(settings->value("geometry").toByteArray());
        settings->endGroup();
    settings->endGroup();

    if (settings->value("pathToBinaries").toString().isEmpty()) {
        pathToSAEdit->setDisabled(true);
        pathToSAchBox->setChecked(true);
        pathToSAButton->setDisabled(true);
    }
    else {
        pathToSAEdit->setText(settings->value("pathToBinaries").toString());
    }

    if (settings->value("pathToDot").toString().isEmpty()) {
        pathDotEdit->setDisabled(true);
        pathDotChBox->setChecked(true);
        pathDotButton->setDisabled(true);
    }
    else {
        pathDotEdit->setText(settings->value("pathToDot").toString());
    }
    if (settings->value("checkBinaries").toBool()==true) {
        checkChBox->setChecked(true);
    }
    else {
        checkChBox->setChecked(false);
    }

    workDirEdit->setText(settings->value("workDir").toString());
    savingDirEdit->setText(settings->value("savingDir",
                                           PATH_TO_SETTINGS).toString());

    connect(pathToSAButton, SIGNAL(clicked()), this, SLOT(onBrowseSADir()));
    connect(pathDotButton, SIGNAL(clicked()), this, SLOT(onBrowseDotPath()));
    connect(workDirButton, SIGNAL(clicked()), this, SLOT(onBrowseWorkDir()));
    connect(savingDirButton, SIGNAL(clicked()), this,
            SLOT(onBrowseSavingDir()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOk()));
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void SettingsWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F1) {
        mainWindow->showHelp(QUrl("qrc:/html/manual/settings.html"));
    }
}

void SettingsWidget::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup("Settings");
        settings->setValue("geometry", saveGeometry());
        settings->endGroup();
    settings->endGroup();
    QWidget::closeEvent(event);
}

void SettingsWidget::chooseDirectory(QLineEdit* editLine)
{
    QString path = QFileDialog::getExistingDirectory(this,
                        tr("Open Directory"),
                        mainWindow->getWorkingDir(),
                        QFileDialog::DontUseNativeDialog |
                        QFileDialog::ShowDirsOnly);
    if (path.isEmpty()) {
        return;
    }
    editLine->setText(path);
    mainWindow->setLastOpenedDir(path);

}

////////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS ///////////////////////////////////////////////////////////////

void SettingsWidget::onBrowseSADir()
{
    chooseDirectory(pathToSAEdit);
}

void SettingsWidget::onBrowseDotPath()
{
    chooseDirectory(pathDotEdit);
}

void SettingsWidget::onBrowseWorkDir()
{
    chooseDirectory(workDirEdit);
}

void SettingsWidget::onBrowseSavingDir()
{
    chooseDirectory(savingDirEdit);
}

void SettingsWidget::onOk()
{
    QString pathToSA;
    if (pathToSAchBox->isChecked()) {
        pathToSA = "";
    } else if (!pathToSAchBox->isChecked() && pathToSAEdit->text().isEmpty()) {
        QMessageBox::critical(this, tr("QSAGUI"),
            tr("The path to binaries can't be empty."));
        return;
    } else if (!pathToSAchBox->isChecked() &&
               !pathToSAEdit->text().isEmpty()) {
        pathToSA = pathToSAEdit->text();
        if (!pathToSA.endsWith("/")) {
            pathToSA.append("/");
        }
    }
    settings->setValue("pathToBinaries", pathToSA);
    mainWindow->setPathToSA(pathToSA);

    QString pathToDot;
    if (pathDotChBox->isChecked()) {
        pathToDot = "";
    } else if (!pathDotChBox->isChecked() && pathDotEdit->text().isEmpty()) {
        QMessageBox::critical(this, tr("QSAGUI"),
            tr("The path to dot can't be empty."));
        return;
    } else if (!pathDotChBox->isChecked() && !pathDotEdit->text().isEmpty()) {
        pathToDot = pathDotEdit->text();
        if (!pathToDot.endsWith("/")) {
            pathToDot.append("/");
        }
    }
    settings->setValue("pathToDot", pathToDot);
    mainWindow->setPathToDot(pathToDot);

    settings->setValue("workDir", workDirEdit->text());
    mainWindow->setWorkingDir(workDirEdit->text());
    if (!savingDirEdit->text().isEmpty()) {
        settings->setValue("savingDir", savingDirEdit->text());
        mainWindow->setSavingDir(savingDirEdit->text());
    }
    else {
        settings->setValue("savingDir", PATH_TO_SETTINGS);
        mainWindow->setSavingDir(PATH_TO_SETTINGS);
    }
    mainWindow->appendToShellBrowser("Configuration file is saved to: " +
                                     CONFIG_FILE);

    if (checkChBox->isChecked()) {
        settings->setValue("checkBinaries", true);
        QStringList missingBinaries = mainWindow->getMissingBinaries();
        if (!missingBinaries.isEmpty()) {
                QString bins = missingBinaries.join("\n");
                int answer = QMessageBox::warning(this, tr("QSAGUI"),
                                     tr("Some binaries are missed:\n")+bins+"\n"+
                                     tr("Return to the settings?"),
                                     QMessageBox::Yes | QMessageBox::No);
             switch (answer) {
                     case QMessageBox::Yes:
                          return;
                     case QMessageBox::No:
                          close();
                          break;
                     default:
                          return;
            }
        }
    }
    else {
        settings->setValue("checkBinaries", false);
    }
    close();
}
