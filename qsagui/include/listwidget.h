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
