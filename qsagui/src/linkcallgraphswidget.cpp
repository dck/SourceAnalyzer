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

/*! \class LinkCallGraphsWidget linkcallgraphswidget.h
 * "./include/linkcallgraphswidget.h"
 *  \brief Window for linkage of several call graphs.
 *
 * Links 2 or more call graphs into one common call graph.
 */

/// \file
/// \brief Implementation of the \ref LinkCallGraphsWidget class.

#include "linkcallgraphswidget.h"

LinkCallGraphsWidget::LinkCallGraphsWidget(QWidget* parent) : QDialog(parent)
{
    setupUi(this);

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup("LinkCallGraphs");
        restoreGeometry(settings->value("geometry").toByteArray());
        settings->endGroup();
    settings->endGroup();

    mainWindow = (MainWindow*) parent;

    connect(listOfCallGraphs, SIGNAL(currentItemChanged(QListWidgetItem*,
            QListWidgetItem*)), this, SLOT(activateDelButton()));

    connect(addButton, SIGNAL(clicked()), this, SLOT(addCallGraph()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(delCallGraph()));
    connect(outputFileButton, SIGNAL(clicked()), this,
            SLOT(onButtonOutputFile()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(onGenerate()));
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void LinkCallGraphsWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F1) {
        if(settings->value("language").toString()=="en")mainWindow->showHelp(QUrl("qrc:/html/manual/link.html"));
        else mainWindow->showHelp(QUrl("qrc:/html/manual_ru/link.html"));
    }
}

void LinkCallGraphsWidget::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup("LinkCallGraphs");
        settings->setValue("geometry", saveGeometry());
        settings->endGroup();
    settings->endGroup();

    listOfCallGraphs->clear();
    outputFileEdit->clear();

    QWidget::closeEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void LinkCallGraphsWidget::activateDelButton() {
    delButton->setDisabled(false);
}

void LinkCallGraphsWidget::addCallGraph() {
    QStringList listOfGraphNames = QFileDialog::getOpenFileNames(this,
                                tr("Call graph"), mainWindow->getWorkingDir());
    if (listOfGraphNames.isEmpty()) {
        return;
    }
    for (int i=0; i < listOfGraphNames.count(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(listOfGraphNames.at(i),
                                                    listOfCallGraphs);
        listOfCallGraphs->addItem(item);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    QFileInfo fi(listOfGraphNames.at(0));
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void LinkCallGraphsWidget::delCallGraph() {
    listOfCallGraphs->takeItem(listOfCallGraphs->currentRow());
    if (listOfCallGraphs->count() == 0) {
        delButton->setDisabled(true);
    }
}

void LinkCallGraphsWidget::onButtonOutputFile() {
    callGraphName = QFileDialog::getSaveFileName(this, tr("Call graph"),
                                               mainWindow->getWorkingDir());
    if (callGraphName.isEmpty()) {
        return;
    }
    outputFileEdit->setText(callGraphName);
    QFileInfo fi(callGraphName);
    mainWindow->setLastOpenedDir(fi.absoluteDir().path());
}

void LinkCallGraphsWidget::onGenerate() {
    callGraphName = outputFileEdit->text();
    if((listOfCallGraphs->count() < 2) || (callGraphName=="")) {
        QMessageBox::critical(this, tr("QSAGUI"),
           tr("Select 2 call graphs for linkage at least and name of output \
call graph."));
        return;
    }

    QString params;
    for (int i = 0; i < listOfCallGraphs->count(); ++i) {
        params += " \"" + listOfCallGraphs->item(i)->text() + "\"";
    }
    QProcess *process = new QProcess(this);
    connect(process, SIGNAL(finished (int, QProcess::ExitStatus)), mainWindow,
            SLOT(onProcessFinish()));
    connect(process, SIGNAL(finished (int, QProcess::ExitStatus)), this,
            SLOT(suggestToOpen()));
    mainWindow->startProcess(SA_TYPE, LINK, params, "", callGraphName, process);
    this->close();
}

void LinkCallGraphsWidget::suggestToOpen() {
    mainWindow->suggestToOpen(callGraphName);
}
