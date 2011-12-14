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

/*! \class CleanCallGraphWidget cleancallgraphwidget.h
 * "./include/cleancallgraphwidget.h"
 *  \brief Window for cleaning of call graph.
 *
 * Clean the given call graph from information that is related to
 * specified files.
 */


/// \file
/// \brief Implementation of the \ref CleanCallGraphWidget class.

#include <QFileDialog>

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
