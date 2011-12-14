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

/*! \class ListWidget listwidget.h "./include/listwidget.h"
 *  \brief Left area for viewing of functions or files list.
 *
 * Creates list of functions or files and process it, gets children or parents
 * functions of choosen functions or of all functions from selected files.
 */

/// \file
/// \brief Implementation of the \ref ListWidget class.

#include "listwidget.h"


ListWidget::ListWidget(MainWindow* mainWindow, bool isFuncList)
          : QWidget()
{
    setupUi(this);

    this->mainWindow = mainWindow;
    this->isFuncList = isFuncList;

    if(isFuncList) {
        listType = "Functions";
        searcTypeLabel->setText(tr("Functions for relations search:"));
    }
    else {
        listType = "Files";
        searcTypeLabel->setText(tr("Files for relations search:"));
    }

    rhWidget = new RequestHystoryWidget(this);

    addRequestDialog = new AddNameDialog(this);
    addRequestDialog->setWindowTitle(tr("QSAGUI - save request"));
    connect(addRequestDialog, SIGNAL(accepted()), this, SLOT(saveRequest()));

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup(listType+"List");
    splitter->restoreState(settings->value("splitterSizes").toByteArray());
    settings->endGroup();

    connect(openRequestButton, SIGNAL(clicked()), rhWidget, SLOT(show()));
    connect(saveRequestButton, SIGNAL(clicked()), this, SLOT(openSaveRequestDialog()));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(sortItems()));
    connect(searchEdit, SIGNAL(editingFinished()), this, SLOT(sortItems()));
    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(addItemForSearch()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(delItemsForSearch()));
    connect(viewButton, SIGNAL(clicked()), this, SLOT(getFunctions()));
    connect(formatBox, SIGNAL(activated(int)), this, SLOT(onFormatChanged(int)));
    connect(levelChBox, SIGNAL(toggled(bool)), this, SLOT(enableLevelSpinBox()));
    connect(cleanButton, SIGNAL(clicked()), selectedItemsList, SLOT(clear()));
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////

int ListWidget::getItemsCount() {
    return listWidget->count();
}

void ListWidget::clear() {
    listWidget->clear();
    selectedItemsList->clear();
}

void ListWidget::rememberLastSession() {
    QString pathToFile = mainWindow->getSavingDir() + QDir::separator();
    if (isFuncList == true) {
        pathToFile += SAVED_LIST_OF_FUNCTIONS;
    }
    else {
        pathToFile += SAVED_LIST_OF_FILES;
    }
    QFile file(pathToFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    out << list.join(DELIMITER_FOR_ITEMS);
    file.close();
}

void ListWidget::restoreLastSession() {
    QString pathToFile = mainWindow->getSavingDir() + QDir::separator();
    if (isFuncList == true) {
        pathToFile += SAVED_LIST_OF_FUNCTIONS;
    }
    else {
        pathToFile += SAVED_LIST_OF_FILES;
    }
    QFile file(pathToFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&file);
    list << stream.readAll().split(DELIMITER_FOR_ITEMS);
    file.close();
    file.remove();
    list.sort();
    listWidget->insertItems(0, list);
    insertItemsCount(list.size());
    mainWindow->showInfoMessage();
}

void ListWidget::fillList() {
    QString params = "\"" + listType + " list\"";
    QProcess *createListProcess = new QProcess(this);
    connect(createListProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
            mainWindow, SLOT(onProcessFinish()));
    connect(createListProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
            this, SLOT(insertItems()));
    mainWindow->startProcess(SA_TYPE, CG2TEXT, params, mainWindow->getCallGraphName(), "", createListProcess);
}

void ListWidget::restoreRequest(QStringList selectedItems,
                                const QString &relationType,
                                const QString &outputFormat,
                                int level) {
    selectedItemsList->clear();
    selectedItemsList->insertItems(0, selectedItems);
    relationTypeBox->setCurrentIndex(relationTypeBox->findText(relationType));
    int index = formatBox->findText(outputFormat);
    formatBox->setCurrentIndex(index);
    onFormatChanged(index);
    if (level != 0 ) {
        levelChBox->setChecked(true);
        if (formatBox->currentIndex() == 1) // SVG
            levelSpinBox->setValue(level);
    }
}

const QString& ListWidget::getType() {
    return listType;
}


///////////////////////////////////////////////////////////////////////////////
// PUBLIC SLOTS ///////////////////////////////////////////////////////////////

void ListWidget::insertItems() {
    QProcess* process = (QProcess*) sender();
    if (process->exitStatus() == QProcess::CrashExit) {
        return;
    }
    QTextStream stream(process->readAllStandardOutput());
    list.clear();
    listWidget->clear();
    while ( !stream.atEnd() ) {
        list << stream.readLine();
    }
    if (list.isEmpty()) {
        return;
    }
    list.last() = list.last().trimmed();
    list.sort();
    listWidget->insertItems(0, list);
    insertItemsCount(list.size());
    mainWindow->showInfoMessage();
}

void ListWidget::saveState()
{
    settings->beginGroup(listType+"List");
    settings->setValue("splitterSizes", splitter->saveState());
    settings->endGroup();

    rhWidget->saveRequests();
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void ListWidget::insertItemsCount(int count) {
    listWidget->insertItem(0, tr("Items count: ") + QString::number(count));
    listWidget->item(0)->setFlags(Qt::ItemIsEnabled);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void ListWidget::enableLevelSpinBox() {
    if(formatBox->currentIndex() == 1) {
        if (levelChBox->isChecked())
            levelSpinBox->setEnabled(true);
        else
            levelSpinBox->setDisabled(true);
    }
}

void ListWidget::onFormatChanged(int index) {
    if (index == 0) {
        if (relationTypeBox->count() == 3)
            relationTypeBox->removeItem(2);
        levelSpinBox->setDisabled(true);
    }
    else if (index == 1) {
        if (relationTypeBox->count() == 2)
            relationTypeBox->addItem(tr("Child and parent"));
        enableLevelSpinBox();
    }
}

void ListWidget::addItemForSearch() {
    QList<QListWidgetItem *> itemsForSearch = listWidget->selectedItems();
    if (itemsForSearch.count() == 0)
        return;
    for (int i=0; i < itemsForSearch.count(); i++) {
        QString itemText = itemsForSearch.at(i)->text();
        bool exists = false;
        for (int j = 0; j < selectedItemsList->count(); j++) {
            if (selectedItemsList->item(j)->text() == itemText)
                exists = true;
        }
        if (!exists)
            selectedItemsList->addItem(itemText);
    }
}

void ListWidget::delItemsForSearch() {
    QList<QListWidgetItem *> selectedItems = selectedItemsList->selectedItems();
    if (selectedItems.count() > 0) {
        for (int i=0; i<selectedItems.count(); i++) {
            selectedItemsList->takeItem(selectedItemsList->row(selectedItems.at(i)));
        }
    }
}

void ListWidget::sortItems() {
    QStringList filteredList;
    foreach (QString str, list) {
        if (str.indexOf(searchEdit->text())>=0)
            filteredList += str;
    }
    listWidget->clear();
    listWidget->insertItems(0, filteredList);
    insertItemsCount(filteredList.size());
}

void ListWidget::getFunctions() {
    if (selectedItemsList->count()==0) {
        return;
    }
    mainWindow->blockTabs();
    if (isFuncList == false) {
        QString params;
        params = getSelectedItems();
        QProcess *getFuncsProcess = new QProcess(this);
        connect(getFuncsProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
                mainWindow, SLOT(onProcessFinish()));
        connect(getFuncsProcess, SIGNAL(finished (int, QProcess::ExitStatus)),
                this, SLOT(getCurrRelationsFuncs()));
        mainWindow->startProcess(SA_TYPE, FINDFUNC, params, mainWindow->getCallGraphName(), "", getFuncsProcess);
    }
    else {
        getCurrRelationsFuncs();
    }
}

void ListWidget::getCurrRelationsFuncs() {
    QProcess* process = (QProcess*) sender();
    if (process->exitStatus() == QProcess::CrashExit) {
        return;
        mainWindow->unblockTabs();
    }
    QString functions;
    if (isFuncList == false) {
        functions = process->readAllStandardOutput();
        functions = functions.trimmed();
        if (!functions.contains( QRegExp("[a-z]"))) {
            mainWindow->setBrowserText("No functions in selected file(s).");
            mainWindow->unblockTabs();
            return;
        }
    }
    else {
        functions = getSelectedItems();
    }
    int level = 0;
    QString binaryName;
    if (formatBox->currentIndex() == 0) { // "Text"
        binaryName = CG2TEXT;
        if (levelChBox->isChecked())
            level = 1;
        else
            level = 0;
    }
    else if (formatBox->currentIndex() == 1) { // "SVG"
        binaryName = CG2DOT;
        if (levelChBox->isChecked())
            level = levelSpinBox->value();
        else
            level = 0;
    }
    mainWindow->getRelationsList(binaryName, functions, relationTypeBox->currentText(), level);
}

QString ListWidget::getSelectedItems() {
    QString selectedItems;
    if (isFuncList == false) {
        for (int i=0; i < selectedItemsList->count(); ++i) {
            selectedItems += " \"" + selectedItemsList->item(i)->text()+ "\"";
        }
    }
    else {
        for (int i=0; i < selectedItemsList->count(); ++i) {
            selectedItems += " " + selectedItemsList->item(i)->text();
        }
    }
    return selectedItems;
}

void ListWidget::openSaveRequestDialog() {
    addRequestDialog->show();
}

void ListWidget::saveRequest() {
    QString name = addRequestDialog->getName();
    if (name.isEmpty()) {
        QMessageBox::critical(this, tr("QSAGUI"),
           tr("The name can't be empty"));
        addRequestDialog->show();
    }
    if (rhWidget->contains(name)) {
        QMessageBox::critical(this, tr("QSAGUI"),
           tr("Item with such name already exists"));
        addRequestDialog->show();
    }
    else {
        int level = 0;
        if (levelChBox->isChecked())
            level = levelSpinBox->value();
        rhWidget->insertItem(name,
                             getSelectedItems(),
                             relationTypeBox->currentText(),
                             formatBox->currentText(),
                             level);
    }
}
