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
