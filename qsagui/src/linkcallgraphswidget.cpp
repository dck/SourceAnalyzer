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
