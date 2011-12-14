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
/// \brief Declaration of the \ref CentralWidget class;

#ifndef __CENTRALWIDGET_H__
#define __CENTRALWIDGET_H__

#include "mainwindow.h"
#include "saguiconstants.h"
#include "svgviewer.h"
#include "ui_findwidget.h"

class QSettings;
class QPlainTextEdit;
class QTextBrowser;
class QUrl;
class QPushButton;
class QLineEdit;
class QCheckBox;
class QVBoxLayout;
class QHBoxLayout;
class QProcess;
class QFile;
class QTextStream;
class QPrinter;
class QPrintDialog;
class QFileDialog;

class SvgViewer;
class MainWindow;
class FindWidget;

class CentralWidget : public QWidget/*, private Ui::CentralWidget*/
{
Q_OBJECT

public:

    /** \brief Constructor
     * Creates all elements of widget and connects signals with slots
     *  \param[in] mainWindow  main window */
    CentralWidget(MainWindow* mainWindow);

    /** \brief Runs the process that creates list of relations
     *  \param[in] functions  names of functions separated by spaces
     *  \param[in] relationType  type of relation - children or parents
     *  \param[in] level  nesting level (for svg)
     * type */
    void getRelationsList(const QString& binaryName, QString& functions, const QString& relationType,
                          int level);

    /** \brief Sets content of html file to text area
     *  \param[in] fileName  name of html file */
    void setBrowserHtmlFile(const QString& fileName);

    /** \brief Sets given string to text area
     *  \param[in] string  given string */
    void setBrowserText(const QString& string);

    /** \brief Writes content of text area to temporary file and saves
     * the state
     *  \param[in] position  tab position in settings */
    void rememberLastSession(int position);

    /** \brief Fills text area from file and restores the state
     *  \param[in] position  tab position in settings */
    void restoreLastSession(int position);

    /** \brief Checks is text search is running
     *  \return  true if search is running, otherwise false */
    bool isSearchEnable();

public slots:

    /// \brief Runs the process that creates call graph in text format
    void getCgAsText();

    /// \brief Runs the process that creates call graph in SVG format
    void getCgAsSvg();

    /// \brief Shows the findWidget
    void openFindWidget();

    /// \brief Prints the content of text area
    void printFile();

    ///< Opens file dialog and calls CentralWidget::saveFile(const QString& fileName)
    void onSave();

private:

enum
{
    CLOSE_BUTTON_SIDE = 20                    ///< Size of closeButton side
};

    MainWindow*         mainWindow;           ///< Main window of application
    QSettings*          settings;             ///< Settings
    QPlainTextEdit*     textBrowser;          ///< Browser for text output
    QTextBrowser*       htmlBrowser;          ///< Browser for html output
    SvgViewer*          svgViewer;            ///< Window for work with svg
    QStringList         listOfFunctions;      /** List of parents/children
                                               * functions */
    FindWidget*         findWidget;           /** Widget for word search
                                               * in the text */
    QString*            stringOfSearch;       ///< String of search
    QVBoxLayout*        browserlayout;        /** Layout for location of
                                               * textBrowser and findWidget */
    bool                isSearch;             /** This flag indicates whether
                                               * the search is running */
    bool                isList;               /** This flag indicates what is
                                               * in the text area now */

    /// \brief Shows text browser and hides others viewers
    void showTextBrowser();

    /// \brief Shows html browser and hides others viewers
    void showHtmlBrowser();

    /// \brief Shows SVG viewer and hides others viewers
    void showSvgViewer();

    /// \brief Common actions for each process
    void runProcess(const QString& binaryName, const QString& arguments,
                    QProcess* process);

    /// \brief Saves the content of text area in file
    void saveFile(const QString& fileName);

private slots:

    /// \brief Shows relation functions as text
    void showRelationsList();

    /// \brief Shows relation functions as SVG subgraph
    void showRelationsGraph();

    /// \brief Shows call graph in text format
    void showCgAsText();

     /// \brief Shows call graph in SVG format
    void showCgAsSvg();

    /// \brief Finds next string in the text area
    void findNextString();

    /// \brief Finds previous string in the text area
    void findPreviousString();
};



class FindWidget : public QWidget, private Ui::FindWidget
{
Q_OBJECT

public:

    /** \brief Constructor
     * Creates all elements of widget and connects signals with slots */
    FindWidget(QWidget *parent);

    /** \brief Returns parameters of search
     *  \return  search flags */
    int getFindOptions();

    /** \brief Returns search string
     *  \return  word or phrase for search */
    QString getSearchString();

    /** \brief Shows wholeWordsCBox, searchPreviousButton and
     * caseSensitiveCBox */
    void showAdvancedSearchOptions();

    /** \brief Hides wholeWordsCBox, searchPreviousButton and
     * caseSensitiveCBox */
    void hideAdvancedSearchOptions();

private:
    CentralWidget*        centralWidget;     ///< Central area of main window
};

#endif // __CENTRALWIDGET_H__
