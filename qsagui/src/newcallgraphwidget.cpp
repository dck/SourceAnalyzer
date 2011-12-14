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

/*! \class NewCallGraphWidget newcallgraphwidget.h
 * "./include/newcallgraphwidget.h"
 *  \brief Window for creation of new call graph.
 *
 * Creates file in internal format for further work with it in this
 * application.
 */

/// \file
/** \brief Implementation of the \ref NewCallGraphWidget class. */

#include "newcallgraphwidget.h"
#include <QtCore>
#include <QtGui>

NewCallGraphWidget::NewCallGraphWidget(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup("NewCallGraph");
        restoreGeometry(settings->value("geometry").toByteArray());
        settings->endGroup();
    settings->endGroup();

    mainWindow = (MainWindow*) parent;

    numOfPathesOfGraph = 0;

    connect(baseDirButton, SIGNAL(clicked()), this, SLOT(chooseBaseDirectory()));

    connect(addCButton,   SIGNAL(clicked()), this, SLOT(addFilesToList()));
    connect(delCButton,   SIGNAL(clicked()), this, SLOT(delFiles()));
    connect(addAsmButton, SIGNAL(clicked()), this, SLOT(addFilesToList()));
    connect(delAsmButton, SIGNAL(clicked()), this, SLOT(delFiles()));
    connect(addF90Button, SIGNAL(clicked()), this, SLOT(addFilesToList()));
    connect(delF90Button, SIGNAL(clicked()), this, SLOT(delFiles()));

    connect(outputFileButton, SIGNAL(clicked()), this,
            SLOT(chooseFileToSave()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(onGenerate()));
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void NewCallGraphWidget::generate(const QString& baseDir,
                                  const QString& binaryName,
                                  const QString& outputFileName,
                                  QListWidget*  listOfFiles) {
    numberOfItems = listOfFiles->count();
    if (numberOfItems == 1) {
        QProcess *createNewCGProcess = new QProcess(this);
        connect(createNewCGProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
                mainWindow, SLOT(onProcessFinish()));
        connect(createNewCGProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
                this, SLOT(suggestToOpen()));
        mainWindow->startProcess(SA_TYPE, binaryName, "", listOfFiles->item(0)->text(), outputFileName, createNewCGProcess);
    }
     else {
        QDir directory (QDir::currentPath());
        QStringList list = directory.entryList();
        for (int i = 0; i < list.size(); ++i) {
            if (list.at(i).contains(binaryName+"graph")) {
                directory.remove(list.at(i));
            }
        }
        QString params;
        if (baseDir != "") {
            params += "\"" + baseDir + "\"";
        }
        for (int i = 0; i < numberOfItems; ++i) {
            QString inputFile = listOfFiles->item(i)->text();
            QString outputFile = binaryName + "graph" + QString::number(i) + ".cg";
            QProcess *createPartOfCGProcess = new QProcess(this);
            connect(createPartOfCGProcess,
                    SIGNAL(finished (int, QProcess::ExitStatus)), this,
                    SLOT(linkPartsOfGraph()));
            connect(createPartOfCGProcess,
                    SIGNAL(finished (int, QProcess::ExitStatus)),
                    mainWindow, SLOT(onProcessFinish()));
            mainWindow->startProcess(SA_TYPE, binaryName, params, inputFile, outputFile, createPartOfCGProcess);
        }
    }
}

void NewCallGraphWidget::linkPartsOfGraph() {
    QString binaryName;
    switch (tabWidget->currentIndex()) {
        case 0:
            binaryName = CPARSE;
            break;
        case 1:
            binaryName = ASMPARSE;
            break;
        case 2:
            binaryName = F90PARSE;
            break;
    }

    numOfPathesOfGraph++;

    if(numOfPathesOfGraph!=numberOfItems) {
        return;
    }
    numOfPathesOfGraph=0;
    QString params;
    for (int i = 0; i < numberOfItems; ++i) {
        params += " " + binaryName + "graph" + QString::number(i) + ".cg";
    }
    QProcess *linkageProcess = new QProcess(this);
    connect(linkageProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            mainWindow, SLOT(onProcessFinish()));
    connect(linkageProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
            this, SLOT(suggestToOpen()));
    mainWindow->startProcess(SA_TYPE, LINK, params, "", callGraphName, linkageProcess);
}

void NewCallGraphWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F1) {
        if(settings->value("language").toString()=="en")mainWindow->showHelp(QUrl("qrc:/html/manual/create.html"));
        else mainWindow->showHelp(QUrl("qrc:/html/manual_ru/create.html"));
    }
}

void NewCallGraphWidget::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup("NewCallGraph");
        settings->setValue("geometry", saveGeometry());
        settings->endGroup();
    settings->endGroup();

    baseDirCBox->setChecked(false);
    baseDirEdit->clear();
    baseDirEdit->setDisabled(true);
    baseDirButton->setDisabled(true);
    preprocessedFileRButton->setChecked(true);
    listOfCFiles->clear();
    sourceFileRButton->setChecked(false);
    sourceFileEdit->setText("gcc <input_file_name> -E -o <output_file_name>");
    sourceFileEdit->setDisabled(true);

    listOfAsmFiles->clear();
    listOfF90Files->clear();

    outputFileEdit->clear();

    QWidget::closeEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void NewCallGraphWidget::chooseBaseDirectory()
{
    QString path = QFileDialog::getExistingDirectory(this,
            tr("Open directory"), mainWindow->getWorkingDir(),
            QFileDialog::DontUseNativeDialog |
            QFileDialog::ShowDirsOnly);
    if (path.isEmpty()) {
        return;
    }
    baseDirEdit->setText(path);
    QFileInfo fi(path);
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void NewCallGraphWidget::addFilesToList()
{
    QListWidget* listOfFiles = NULL;
    switch (tabWidget->currentIndex()) {
        case 0:
            listOfFiles = listOfCFiles;
            break;
        case 1:
            listOfFiles = listOfAsmFiles;
            break;
        case 2:
            listOfFiles = listOfF90Files;
            break;
    }

    QStringList listOfFileNames = QFileDialog::getOpenFileNames(this,
                    ("Choose file(s)"), mainWindow->getWorkingDir());
    if (listOfFileNames.isEmpty()) {
        return;
    }
    for (int i=0; i < listOfFileNames.count(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(listOfFileNames.at(i),
                                                    listOfFiles);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    QFileInfo fi(listOfFileNames.at(0));
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void NewCallGraphWidget::delFiles() {
    QListWidget* listOfFiles = NULL;
    switch (tabWidget->currentIndex()) {
        case 0:
            listOfFiles = listOfCFiles;
            break;
        case 1:
            listOfFiles = listOfAsmFiles;
            break;
        case 2:
            listOfFiles = listOfF90Files;
            break;
    }

    //do in for, add multy selection in lists
    listOfFiles->takeItem(listOfFiles->currentRow());
}

void NewCallGraphWidget::chooseFileToSave()
{
    callGraphName = QFileDialog::getSaveFileName(this, tr("Call graph"),
                                              mainWindow->getWorkingDir());
    if (callGraphName.isEmpty()) {
        return;
    }
    outputFileEdit->setText(callGraphName);
    QFileInfo fi(callGraphName);
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void NewCallGraphWidget::onGenerate() {
    callGraphName = outputFileEdit->text();
    switch (tabWidget->currentIndex()) {
        case 0:
            if(((preprocessedFileRButton->isChecked() && listOfCFiles->count() == 0)
                 || (sourceFileRButton->isChecked() && sourceFileEdit->text()==""))
                 || (callGraphName=="")) {
                QMessageBox::critical(this, tr("QSAGUI"),
                   tr("Each of the margins \"Preprocessed code \" \
(or \"Source file \") and \"call graph name\" must be filled"));
                return;
            }
            if (preprocessedFileRButton->isChecked()) {
                QString baseDir;
                if (baseDirCBox->isChecked() && baseDirEdit->text() != "") {
                    baseDir=baseDirEdit->text();
                }
                else
                    baseDir="";
                    generate(baseDir, CPARSE, callGraphName,
                             listOfCFiles);
            } else if (sourceFileRButton->isChecked()) {
                QString prepCommand = sourceFileEdit->text();
                int lastSpace = prepCommand.length() - prepCommand.lastIndexOf(" ")
                                - 1;
                preprocInputFile = prepCommand.right(lastSpace);
                if (preprocInputFile.startsWith("\"")) {
                    preprocInputFile.remove(0, 1);
                }
                if (preprocInputFile.endsWith("\"")) {
                    preprocInputFile.remove(-1, 1);
                }
                QProcess *preprocessing = new QProcess(this);
                connect(preprocessing, SIGNAL(finished (int, QProcess::ExitStatus)),
                        mainWindow, SLOT(onProcessFinish()));
                connect(preprocessing, SIGNAL(finished (int, QProcess::ExitStatus)),
                        this, SLOT(onPreprocFinished()));
                mainWindow->startProcess(PREPROC_TYPE, prepCommand, "", "", "", preprocessing);
            }
            break;
        case 1:
            if((listOfAsmFiles->count() == 0) || (callGraphName=="")) {
                QMessageBox::critical(this, tr("QSAGUI"),
                   tr("Choose one source file at least and enter the name of output \
call graph."));
                return;
            }
            generate("", ASMPARSE, callGraphName,
                     listOfAsmFiles);
            break;
        case 2:
            if((listOfF90Files->count() == 0) || (callGraphName=="")) {
                QMessageBox::critical(this, tr("QSAGUI"),
                   tr("Choose one source file at least and enter the name of output \
call graph."));
                return;
            }
            generate("", F90PARSE, callGraphName,
                     listOfF90Files);
            break;
    }
    this->close();
}

void NewCallGraphWidget::onPreprocFinished() {
    QProcess* process = (QProcess*) sender();
    if (process->exitStatus() == QProcess::CrashExit) {
        return;
    }
    QProcess *createNewCGProcess = new QProcess(this);
    connect(createNewCGProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
            mainWindow, SLOT(onProcessFinish()));
    connect(createNewCGProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
            this, SLOT(suggestToOpen()));
    mainWindow->startProcess(SA_TYPE, CPARSE, "", preprocInputFile, callGraphName, createNewCGProcess);
}

void NewCallGraphWidget::suggestToOpen() {
    mainWindow->suggestToOpen(callGraphName);
}
