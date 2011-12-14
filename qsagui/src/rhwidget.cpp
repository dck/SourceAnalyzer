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

