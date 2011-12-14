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

/*! \class CleanCallGraphWidget cleancallgraphwidget.h
 * "./include/cleancallgraphwidget.h"
 *  \brief Window for cleaning of call graph.
 *
 * Clean the given call graph from information that is related to
 * specified files.
 */


/// \file
/// \brief Implementation of the \ref CleanCallGraphWidget class.

#include "cleancallgraphwidget.h"
CleanCallGraphWidget::CleanCallGraphWidget(QWidget* parent) : QDialog(parent)
{
    setupUi(this);

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup("CleanCallGraphs");
        restoreGeometry(settings->value("geometry").toByteArray());
        settings->endGroup();
    settings->endGroup();

    mainWindow = (MainWindow*) parent;

    addDialog = new AddNameDialog(this);
    addDialog->setWindowTitle(tr("QSAGUI - source file name"));

    connect(listOfCFiles, SIGNAL(currentItemChanged(QListWidgetItem*,
                                                    QListWidgetItem*)),
                                                    this,
                                                    SLOT(activateDelButton()));

    connect(inputFileButton, SIGNAL(clicked()), this,
            SLOT(onButtonInputFile()));
    connect(addButton, SIGNAL(clicked()), addDialog, SLOT(show()));
    connect(addDialog, SIGNAL(accepted()), this, SLOT(addFile()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(delFile()));
    connect(outputFileButton, SIGNAL(clicked()), this,
            SLOT(onButtonOutputFile()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(onGenerate()));
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void CleanCallGraphWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F1) {
       if(settings->value("language").toString()=="en") mainWindow->showHelp(QUrl("qrc:/html/manual/clean.html") );
       else mainWindow->showHelp(QUrl("qrc:/html/manual_ru/clean.html") );
    }
}

void CleanCallGraphWidget::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup("CleanCallGraphs");
        settings->setValue("geometry", saveGeometry());
        settings->endGroup();
    settings->endGroup();

    inputFileEdit->clear();
    listOfCFiles->clear();
    outputFileEdit->clear();

    QWidget::closeEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void CleanCallGraphWidget::activateDelButton() {
    delButton->setDisabled(false);
}

void CleanCallGraphWidget::addFile() {
    if (addDialog->getName().isEmpty()) {
        return;
    }
    QListWidgetItem* item = new QListWidgetItem(addDialog->getName());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    listOfCFiles->addItem(item);
}

void CleanCallGraphWidget::delFile() {
    listOfCFiles->takeItem(listOfCFiles->currentRow());
    if (listOfCFiles->count() == 0) {
        delButton->setDisabled(true);
    }
}

void CleanCallGraphWidget::onButtonInputFile() {
    qStringInputFile = QFileDialog::getOpenFileName(this, tr("Call graph"),
                                              mainWindow->getWorkingDir());
    if (qStringInputFile.isEmpty()) {
        return;
    }
    inputFileEdit->setText(qStringInputFile);
    QFileInfo fi(qStringInputFile);
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void CleanCallGraphWidget::onButtonOutputFile() {
    callGraphName = QFileDialog::getSaveFileName(this, tr("Call graph"),
                                               mainWindow->getWorkingDir());
    if (callGraphName.isEmpty()) {
        return;
    }
    outputFileEdit->setText(callGraphName);
    QFileInfo fi(callGraphName);
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void CleanCallGraphWidget::onGenerate() {
    callGraphName = outputFileEdit->text();
    if((inputFileEdit->text()=="") || (listOfCFiles->count() == 0) ||
       (callGraphName=="")) {
        QMessageBox::critical(this, tr("QSAGUI"),
           tr("Select name of call graph for cleaning, cpecify 1 file for \
deleting at least and enter name of output call graph."));
        return;
    }

    QString params;
    for (int i = 0; i < listOfCFiles->count(); ++i) {
        params += " \"" + listOfCFiles->item(i)->text() + "\"";
    }
    QProcess *process = new QProcess(this);
    connect(process, SIGNAL(finished (int, QProcess::ExitStatus)), mainWindow,
            SLOT(onProcessFinish()));
    connect(process, SIGNAL(finished (int, QProcess::ExitStatus)), this,
            SLOT(suggestToOpen()));
    mainWindow->startProcess(SA_TYPE, CLEAN, params, inputFileEdit->text(), callGraphName, process);
    this->close();
}

void CleanCallGraphWidget::suggestToOpen() {
    mainWindow->suggestToOpen(callGraphName);
}
