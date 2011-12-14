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

/*! \class UpdateCallGraphWidget updatecallgraphwidget.h
 * "./include/updatecallgraphwidget.h"
 *  \brief Window for update of call graph.
 *
 * Updates call graph with other call graphs.
 */

/// \file
/// \brief Implementation of the \ref UpdateCallGraphWidget class.

#include <QFileDialog>

#include "updatecallgraphwidget.h"

UpdateCallGraphWidget::UpdateCallGraphWidget(QWidget* parent) : QDialog(parent)
{
    setupUi(this);

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup("UpdateCallGraph");
        restoreGeometry(settings->value("geometry").toByteArray());
        settings->endGroup();
    settings->endGroup();

    mainWindow = (MainWindow*) parent;

    connect(listOfCallGraphs, SIGNAL(
            currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(activateDelButton()));

    connect(inputFileButton, SIGNAL(clicked()), this,
            SLOT(onButtonInputFile()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addCallGraph()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(delCallGraph()));
    connect(outputFileButton, SIGNAL(clicked()), this,
            SLOT(onButtonOutputFile()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(onGenerate()));
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void UpdateCallGraphWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F1) {
        if(settings->value("language").toString()=="en")mainWindow->showHelp(QUrl("qrc:/html/manual/update.html"));
        else mainWindow->showHelp(QUrl("qrc:/html/manual_ru/update.html"));
    }
}

void UpdateCallGraphWidget::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup("UpdateCallGraph");
        settings->setValue("geometry", saveGeometry());
        settings->endGroup();
    settings->endGroup();

    inputFileEdit->clear();
    listOfCallGraphs->clear();
    outputFileEdit->clear();

    QWidget::closeEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void UpdateCallGraphWidget::activateDelButton() {
    delButton->setDisabled(false);
}

void UpdateCallGraphWidget::onButtonInputFile() {
    QString s = QFileDialog::getOpenFileName(this, tr("Call graph"),
                                             mainWindow->getWorkingDir());
    if (s.isEmpty()) {
        return;
    }
    inputFileEdit->setText(s);
    QFileInfo fi(s);
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void UpdateCallGraphWidget::addCallGraph() {
    QStringList listOfGraphNames = QFileDialog::getOpenFileNames(this,
                                                tr("Call graphs"),
                                                mainWindow->getWorkingDir());
    if (listOfGraphNames.isEmpty()) {
        return;
    }
    for (int i=0; i < listOfGraphNames.count(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(listOfGraphNames.at(i),
                                                    listOfCallGraphs);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    QFileInfo fi(listOfGraphNames.at(0));
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void UpdateCallGraphWidget::delCallGraph() {
    listOfCallGraphs->takeItem(listOfCallGraphs->currentRow());
    if (listOfCallGraphs->count() == 0) {
        delButton->setDisabled(true);
    }
}

void UpdateCallGraphWidget::onButtonOutputFile() {
    callGraphName = QFileDialog::getSaveFileName(this, tr("Call graph"),
                                             mainWindow->getWorkingDir() );
    if (callGraphName.isEmpty()) {
        return;
    }
    outputFileEdit->setText(callGraphName);
    QFileInfo fi(callGraphName);
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void UpdateCallGraphWidget::onGenerate() {
    callGraphName = outputFileEdit->text();
    if((inputFileEdit->text()=="") || (listOfCallGraphs->count() == 0) ||
        callGraphName=="") {
        QMessageBox::critical(this, tr("QSAGUI"),
           tr("Select name of call graph for updating, cpecify 1 call graph \
which was changed at least and enter the name of output call \
graph."));
        return;
    }
    QString params = " \"" + inputFileEdit->text() + "\"";
    for (int i = 0; i < listOfCallGraphs->count(); ++i) {
        params += " \"" + listOfCallGraphs->item(i)->text() + "\"";
    }
    QProcess *process = new QProcess(this);
    connect(process, SIGNAL(finished (int, QProcess::ExitStatus)), mainWindow,
            SLOT(onProcessFinish()));
    connect(process, SIGNAL(finished (int, QProcess::ExitStatus)), this,
            SLOT(suggestToOpen()));
    mainWindow->startProcess(SA_TYPE, UPDATE, params, "", callGraphName, process);
    this->close();
}

void UpdateCallGraphWidget::suggestToOpen() {
    mainWindow->suggestToOpen(callGraphName);
}
