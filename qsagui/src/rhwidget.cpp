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

/*! \class RequestHystoryWidget rhwidget.h "./include/rhwidget.h"
 *  \brief Widget dysplays saved requests of relations search. */

/// \file
/// \brief Implementation of the \ref RequestHystoryWidget class.

#include "rhwidget.h"
#include <QtGui>
#include <QtCore>

RequestHystoryWidget::RequestHystoryWidget(ListWidget* parent)
    : QDialog(parent)
{
    setupUi(this);
    this->listWidget = parent;

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup(listWidget->getType() + "RequestHystoryWidget");
        restoreGeometry(settings->value("geometry").toByteArray());
        settings->endGroup();
    settings->endGroup();

    pathToRequestsFile = settings->value("savingDir").toString()+ QDir::separator() +
                         listWidget->getType() + SAVED_REQUESTS;
    QFile file(pathToRequestsFile);
    if (file.open(QIODevice::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QStringList cols = stream.readLine().split(DELIMITER_FOR_ITEMS);
            insertItem(cols.at(0),cols.at(1),cols.at(2),cols.at(3),cols.at(4).toInt());
        }
        file.close();
        file.remove();
    }

    connect(okButton, SIGNAL(clicked()), this, SLOT(restoreRequest()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(deleteItem()));
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////

bool RequestHystoryWidget::contains(const QString &itemName) {
    QList<QTableWidgetItem *> items = requestsWidget->findItems(itemName, Qt::MatchExactly);
    if (items.count() == 0)
        return false;
    for (int i=0; i<items.count(); i++) {
        if (items.at(i)->column() == 0)
            return true;
    }
    return false;
}

void RequestHystoryWidget::insertItem(const QString &itemName,
                                      const QString& selectedItems,
                                      const QString& relationType,
                                      const QString& outputFormat,
                                      int level) {
    int row = requestsWidget->rowCount();
    requestsWidget->setRowCount(row+1);
    QTableWidgetItem *nameItem = new QTableWidgetItem(itemName);
    QTableWidgetItem *itemsForSearch = new QTableWidgetItem(selectedItems.trimmed());
    QTableWidgetItem *relationTypeItem = new QTableWidgetItem(relationType);
    QTableWidgetItem *outputFormatItem = new QTableWidgetItem(outputFormat);
    QTableWidgetItem *levelItem = new QTableWidgetItem(QString::number(level));
    requestsWidget->setItem(row, 0, nameItem);
    requestsWidget->setItem(row, 1, itemsForSearch);
    requestsWidget->setItem(row, 2, relationTypeItem);
    requestsWidget->setItem(row, 3, outputFormatItem);
    requestsWidget->setItem(row, 4, levelItem);
}

void RequestHystoryWidget::saveRequests() {
    QFile file(pathToRequestsFile);
    if (file.open(QIODevice::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        for (int i=0; i<requestsWidget->rowCount(); i++) {
            out << requestsWidget->item(i, 0)->text() << DELIMITER_FOR_ITEMS
                << requestsWidget->item(i, 1)->text() << DELIMITER_FOR_ITEMS
                << requestsWidget->item(i, 2)->text() << DELIMITER_FOR_ITEMS
                << requestsWidget->item(i, 3)->text() << DELIMITER_FOR_ITEMS
                << requestsWidget->item(i, 4)->text() << DELIMITER_FOR_ITEMS << "\n";
        }
        file.close();
    }
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void RequestHystoryWidget::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup(listWidget->getType() + "RequestHystoryWidget");
        settings->setValue("geometry", saveGeometry());
        settings->endGroup();
    settings->endGroup();

    QWidget::closeEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void RequestHystoryWidget::restoreRequest() {
    if (requestsWidget->rowCount() > 0) {
        int row = requestsWidget->currentRow();
        if (row >= 0) {
            QStringList selectedItems = requestsWidget->item(row, 1)->text().split(" ");
            QString relationType = requestsWidget->item(row, 2)->text();
            QString outputFormat = requestsWidget->item(row, 3)->text();
            int level = requestsWidget->item(row, 4)->text().toInt();
            listWidget->restoreRequest(selectedItems, relationType, outputFormat, level);
            this->close();
        }
    }
    this->accept();
}

void RequestHystoryWidget::deleteItem(){
    QList<QTableWidgetItem *> selectedItems = requestsWidget->selectedItems();
    if (selectedItems.count() == 0)
        return;
    for (int i=0; i<selectedItems.count(); i++) {
        requestsWidget->removeRow(selectedItems.at(i)->row());
    }
}

