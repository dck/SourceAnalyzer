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

/// \file
/// \brief Declaration of the \ref ListWidget class;

#ifndef __LISTWIDGET_H__
#define __LISTWIDGET_H__

#include <QStringList>
#include <QListWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QProcess>

#include "mainwindow.h"
#include "rhwidget.h"
#include "ui_listwidget.h"

class QStringList;
class QListWidgetItem;
class QFile;
class QTextStream;
class QProcess;

class MainWindow;
class RequestHystoryWidget;
class AddNameDialog;

class ListWidget : public QWidget, private Ui::ListWidget
{
Q_OBJECT

public:

    /** \brief Constructor
     * Creates all elements of widget and connects signals with slots
     *  \param[in] mainWindow  main window
     *  \param[in] isFuncList true if it's a functions list */
    ListWidget(MainWindow* mainWindow, bool isFuncList);

    /** \brief Runs process of functions or files list creation */
    void fillList();

    /** \brief Gives number of items in listWidget
     *  \return items count */
    int getItemsCount();

    /// \brief Clears listWidget
    void clear();

    /// \brief Writes content of list of functions or files to temporary file
    void rememberLastSession();

    /// \brief Fills list of functions or files from temporary file
    void restoreLastSession();

    void restoreRequest(QStringList selectedItems,
                        const QString &relationType,
                        const QString &outputFormat,
                        int level);

    const QString& getType();

public slots:

    /** \brief Fills the listWidget and shows the number of items on the
     * label below */
    void insertItems();

    void saveState();

private:

    MainWindow*      mainWindow;         ///< Main window of application
    QSettings*       settings;           ///< Settings
    bool             isFuncList;         /** True if it's list of functions,
                                          * false if it's list of files */
    QStringList      list;               ///< List of functions or files
    QString          listType;
    RequestHystoryWidget *rhWidget;
    AddNameDialog *addRequestDialog;

    /** \brief Insert at the top of listWidget the number of items
     *  \param[in] count  number of items */
    void insertItemsCount(int count);

    QString getSelectedItems();

private slots:

    /** \brief Activates levelSpinBox if svgOutputRButton and levelChBox
    * are enabled */
    void enableLevelSpinBox();

    /** \brief Adds or removes "Children and parents" item,
     *  disables levelSpinBox or calls ListWidget::enableLevelSpinBox()
     *  \param[in] index  index of item */
    void onFormatChanged(int index);

    void addItemForSearch();

    void delItemsForSearch();

    /** \brief Forms list of functions for search of relations in dependence
     * of list type and calls ListWidget::getCurrRelationsFuncs() */
    void getFunctions();

    /// \brief Sorts items in listWidget
    void sortItems();

    /** \brief Forms command to start the process and sends it to the
     * central widget.
     * Central widget shows children or parents functions of selected
     * function(s) or all functions from selected files in text area */
    void getCurrRelationsFuncs();

    void openSaveRequestDialog();

    void saveRequest();
};

#endif // __LISTWIDGET_H__
