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

#ifndef __RHWIDGET_H__
#define __RHWIDGET_H__

#include "listwidget.h"
#include "ui_rhwidget.h"

class ListWidget;

class RequestHystoryWidget : public QDialog, private Ui::RequestHystoryWidget
{
Q_OBJECT

public:
    /** \brief Creates all elements of the window and connects signals with slots.
     * Restores size and position from settings.
     *  \param[in] parent  parent widget */
    RequestHystoryWidget(ListWidget *parent);

    bool contains(const QString &itemName);

    void insertItem(const QString &itemName,
                    const QString& selectedItems,
                    const QString& relationType,
                    const QString& outputFormat,
                    int level);

    void saveRequests();

private:
    QSettings*    settings;           ///< Settings
    ListWidget*   listWidget;
    QString       pathToRequestsFile;

    void closeEvent(QCloseEvent* event);

private slots:

    void restoreRequest();

    void deleteItem();
};

#endif // __RHWIDGET_H__
