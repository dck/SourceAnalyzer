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
/// \brief Declaration of the \ref MainWindow class;

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QProcess>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QProgressBar>
#include <QDockWidget>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QCloseEvent>
#include <QSettings>
#include <QTimer>
#include <QTextStream>
#include <QTabWidget>
//#include <QActionGroup>

#include "settingswidget.h"
#include "newcallgraphwidget.h"
#include "linkcallgraphswidget.h"
#include "cleancallgraphwidget.h"
#include "updatecallgraphwidget.h"
#include "listwidget.h"
#include "centralwidget.h"
#include "aboutwidget.h"
#include "helpwidget.h"
#include "saguiconstants.h"
#include "ui_addnamedialog.h"

//class QActionGroup;
class QApplication;
class QMainWindow;
class QStackedWidget;
class QTextBrowser;
class QProcess;
class QAction;
class QMenu;
class QMenuBar;
class QToolBar;
class QStatusBar;
class QProgressBar;
class QDockWidget;
class QMessageBox;
class QDir;
class QFile;
class QCloseEvent;
class QSettings;
class QTimer;
class QTextStream;
class QTabWidget;

class SettingsWidget;
class NewCallGraphWidget;
class LinkCallGraphsWidget;
class CleanCallGraphWidget;
class UpdateCallGraphWidget;
class ListWidget;
class CentralWidget;
class AboutWidget;
class HelpWidget;
class ApplicationOutput;
class MainWindow;
class AddNameDialog;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:

    /** \brief Constructor
     * Calls all the functions to create elements of main window
     *  \param[in] parent  parent widget */
    MainWindow(QWidget * parent = 0);

    /** \brief Sets given string to local variable
     * (for setting path to SA binaries)
     *  \param[in] pathToSA  path to SA binaries */
    inline void setPathToSA(const QString& pathToSA)
                             { this->pathToSA = pathToSA; };

    /** \brief Gets path to SA
     *  \return path to SA binaries */
    inline const QString& getSAPath() { return pathToSA; };

    /** \brief Sets given string to local variable
     * (for setting path to dot utility)
     *  \param[in] pathToDot  path to dot utility */
    inline void setPathToDot(const QString& pathToDot)
                             { this->pathToDot = pathToDot; };

    /** \brief Gets path to dot utility
     *  \return path to dot utility */
    inline const QString& getPathToDot() { return pathToDot; };

    /** \brief Sets string with wogring directory to local variable
     * of main window
     *  \param[in] workdirPath path to working directory */
    inline void setWorkingDir(const QString& workdirPath)
    { this->workdirPath = workdirPath; };

    /** \brief Sets string with last opened directory to local variable
     * of main window
     *  \param[in] str path to last opened directory */
    inline void setLastOpenedDir(const QString& lastOpenedDir)
    { this->lastOpenedDir = lastOpenedDir; };

    /** \brief Sets string with directory for saving temp files to local
     * variable of main window
     *  \param[in] savingdirPat path to directory for saving */
    inline void setSavingDir(const QString& savingdirPath)
    { this->savingdirPath = savingdirPath; };

    /** \brief Gets string with directory for saving temp files
     *  \return path to directory for saving */
    inline const QString& getSavingDir() { return savingdirPath; };

    /** \brief Gets name of current call graph
     *  \return  call graph name*/
    inline const QString& getCallGraphName() { return callGraphName; };

    /** \brief Suggests to open call graph after cpecified action
     *  \param[in] cgName  name of call graph */
    void suggestToOpen(const QString& cgName);

    /** \brief Gets working directory
     *  \return path to working directory */
    const QString& getWorkingDir();

    /** \brief Appends given string to shellOutputBrowser
     *  \param[in] str  given string */
    void appendToShellBrowser(const QString& str);

    /// \brief Shows the message after loading of file
    void showInfoMessage();

    /// \brief Reads settings from configuration file
    void readSettings();

    /** \brief Checks all binaries
     *  \return  list of missing binaries */
    QStringList getMissingBinaries();

    /** \brief Show user manual with given page
     *  \param[in] url  url address */
    void showHelp(QUrl url);

    /// \brief Calls CentralWidget::setBrowserText(const QString& string)
    void setBrowserText(const QString& string);

    /** \brief Calls CentralWidget::getRelationsList(const QString& binaryName,
     * QString& functions, const QString& relationType, int level) */
    void getRelationsList(const QString& binaryName, QString& functions, const QString& relationType,
                          int level);

    /// \brief Blocks all tabs except  current
    void blockTabs();

    /// \brief Unblocks all tabs
    void unblockTabs();

public slots:

    /** \brief Checks binary on validness, runs the process
     *  and appends to shell browser string with command
     *  \param[in] type  binary type (sa, dot or preprocessing)
     *  \param[in] binaryName  name of binary
     *  \param[in] params  params for process
     *  \param[in] inputFileName  name of input file for process
     *  \param[in] outputFileName  name of output file for process
     *  \param[in] process  process to start
     *  \return true if the process have been started */
    bool startProcess(const QString& type,
                      const QString& binaryName,
                      const QString& params,
                      const QString& inputFileName,
                      const QString& outputFileName,
                      QProcess* process);

    /** \brief Removes list of related processes from list of running processes
     * if all of related processes are finished.
     * Makes "terminate last process" action inaccessible if no processes are in list of running
     * processes. Stops progress bar if it's necessary.
     * Appends error string to application output if process is crashed */
    void onProcessFinish();

    /// \brief Activates progress bar
    void activateProgressBar();

    /// \brief Stops progress bar
    void stopProgressBar();

    /// \brief Creates SettingsWidget
    void showSettingsWidget();

private:

enum
{
    WIDTH                  = 800,                 ///< Default window width
    HEIGHT                 = 600,                 ///< Default window height
    SIZE_OF_ICON_SIDE      = 25,                  /** Size for side of tool
                                                   * bar icon */
    PROGRESS_BAR_WIDTH     = 250,                 ///< Width of progress bar
};

    QSettings*             settings;               ///< Settings
    SettingsWidget*        settingsWidget;         ///< Widget with settings
    NewCallGraphWidget*    newCallGraphWidget;     /** Widget for selection of
                                                    * new call graph parameters */
    LinkCallGraphsWidget*  linkCallGraphsWidget;   /** Widget for linkage of
                                                    * call graphs */
    CleanCallGraphWidget*  cleanCallGraphWidget;   /** Widget for cleaning of
                                                    * call graph */
    UpdateCallGraphWidget* updateCallGraphWidget;  /** Widget for updating of
                                                    * call graph */
    ListWidget*            funcsListWidget;        /** Widget for viewing
                                                    * funcsList */
    ListWidget*            filesListWidget;        /** Widget for viewing
                                                    * filesList */
    QTabWidget*            centralWidget;          /** Central widget for
                                                    * viewing text */
    AddNameDialog*         addTabDialog;           ///< The dialog for adding tab
    AddNameDialog*         renameTabDialog;        ///< The dialog for renaming tab
    AboutWidget*           aboutWidget;            /** Widget for showining
                                                    * information about QSAGUI
                                                    */
    HelpWidget*            helpWidget;             /** Widget for
                                                    * representation of user
                                                    * manual */

    ApplicationOutput*     shellOutputDock;        /** Dock widget for
                                                    * application output */

    QString                callGraphName;          /** Name of current call
                                                    * graph */
    bool                   isLinkedGraph;         /** True if current call graph
                                                    * is linked from several */
    QString                pathToSA;               /** Path to SourceAnalyzer
                                                    * binaries */
    QString                pathToDot;              ///< Path to dot utility
    QString                workdirPath;            /** Path to working
                                                    * directory */
    QString                lastOpenedDir;          /** Path to last opened
                                                    * directory */
    QString                savingdirPath;          /** Path to directory for
                                                    * saving temp files */
    QList<QProcess *>      listOfCurProcesses;     /**  List of processes
                                                    * that currently are
                                                    * running. */
    QAction*               enSelect;
    QAction*               ruSelect;

    QAction*               newTabAction;           ///< New tab addition
    QAction*               closeTabAction;         ///< Closing of current tab
    QAction*               renameTabAction;        ///< Renaming of current tab
    QAction*               newCallGraphAction;     ///< New call-graph creation
    QAction*               loadAction;             ///< Ffile loading
    QAction*               linkAction;             ///< Call graphs linkage
    QAction*               cleanAction;            ///< Call graph cleaning
    QAction*               updateAction;           ///< Call graph updating
    QAction*               fileSaveAction;         ///< Text area saving
    QAction*               filePrintAction;        ///< Text area printing
    QAction*               settingsAction;         ///< Opens settings window
    QAction*               quitAction;             ///< Quit from program
    QAction*               aboutAction;            /** Showing of information
                                                    * about QSAGUI programmers
                                                    */
    QAction*               helpAction;             /** Showing of QSAGUI user
                                                    * manual  */
    QAction*               findAction;             ///< Search in text area
    QAction*               showCgAsTextAction;       /** Showing of call graph in
                                                    * text format in the
                                                    * central widget */
    QAction*               showCgAsSvgAction;        /** Showing of call graph in
                                                    * SVG format in the
                                                    * central widget */
    QAction*               cleanWindowAction;      /** Cleaning of window's
                                                    * fields */
    QAction*               tepminateProcAction;    ///< Terminatess last process
    QAction*               fullScreenAction;       ///< Full screen mode

    QMenu*                 language;

    QMenu*                 fileMenu;               /** Menu of actions with
                                                    * tabs */
    QMenu*                 cgMenu;                 /** Menu of actions with
                                                    * call graph */
    QMenu*                 showMenu;               /** Menu for selection of
                                                    * call graph output*/
    QMenu*                 editMenu;               ///< Edit menu
    QMenu*                 viewMenu;               /** Menu of dockwidget and
                                                    * toolbars */
    QMenu*                 settingsMenu;           /** Menu, includes item
                                                    * "Settings" */
    QMenu*                 helpMenu;               /** Menu, includes item
                                                    * "About" */
    QToolBar*              cgToolBar;              /** Tool bar with actions of
                                                    * creation, opening and
                                                    * showing of call graph */
    QToolBar*              callGraphToolBar;       /** Tool bar with actions of
                                                    * linkage, cleaning and
                                                    * updating of call
                                                    * graph(s) */
    QToolBar*              printSaveToolBar;       /** Tool bar with actions of
                                                    * saving and printing of
                                                    * text area */
    QToolBar*              settingsToolBar;        /** Tool bar, includes item
                                                    * "Path" */
    QToolBar*              editToolBar;            /** Tool bar, includes item
                                                    * "Find" and
                                                    * "Clean window" */
    QToolBar*              quitToolBar;            /** Tool bar, includes item
                                                    * "Quit" */
    QProgressBar*          progressBar;            ///< Progress bar

    QDockWidget*           funcsListDock;          /** This is the QDockWidget
                                                    * for funcsListWidget */
    QDockWidget*           filesListDock;          /** This is the QDockWidget
                                                    * for filesListWidget */

    /// \brief Creates all actions and connects it with slots
    void createActions();

    /// \brief Creates all menus and adds its to menu bar
    void createMenus();

    /// \brief Creates all tool bars
    void createToolBars();

    /// \brief Creates progress bar
    void createProgressBar();

    /// \brief Creates all QDockWidget-s and sets its to main window
    void createDockWidgets();

    /** \brief Writes window properties to configuration file,
     * saves settings when closing window */
    void closeEvent(QCloseEvent* event);

    /** \brief Writes call graph name to configuration file and creates
     * temporary files */
    void rememberLastSession();

    /** \brief Adds process to list
     *  \param[in] process  process to add */
    void appendToList(QProcess *process);

private slots:

    void enTranslate();

    void ruTranslate();

    /// \brief Adds new tab to centralWidget
    void addNewTab();

    /** \brief Close tab
     *  \param[in] i  tab index */
    void closeTab(int i);

    /// \brief Renames current tab
    void renameCurrentTab();

    /// \brief Closes current tab
    void closeCurrentTab();

    void closeProgram();

    /// \brief Calls CentralWidget::onSave()
    void onSave();

    /// \brief Calls CentralWidget::printFile()
    void printFile();

    /// \brief Calls CentralWidget::openFindWidget()
    void openFindWidget();

    /// \brief Calls CentralWidget::getCgAsText()
    void getCgAsText();

    /// \brief Calls CentralWidget::getCgAsSvg()
    void getCgAsSvg();

    /// \brief Shows warning if some of binaries are missed
    void checkBinaries();

    /** \brief Loads call graph. If there is a several call graphs, it runs the
     * process of linkage */
    void loadFile();

    /// \brief Runst the processes of functions and files lists creation
    void fillFuncFilesList();

    /// \brief Shows the newCallGraphWidget
    void newCallGraph();

    /// \brief Shows the linkCallGraphsWidget
    void linkCallGraphs();

    /// \brief Shows the cleanCallGraphWidget
    void cleanCallGraph();

    /// \brief Shows the updateCallGraphWidget
    void updateCallGraph();

    /// \brief Shows the aboutWidget
    void showAboutWidget();

    /// \brief Shows user manual with start page
    void showHelp();

    /// \brief Cleans window's fields
    void cleanWindow();

    /// \brief Terminates last process
    void tepminateLastProcess();

    /// \brief Set full screen mode
    void setFullScreen(bool on);
};

class ApplicationOutput : public QDockWidget
{
Q_OBJECT

public:

    /** \brief Creates all elements of the widget and connects signals with
     * slots. */
    ApplicationOutput(QWidget *parent);

    /** \brief Appends given string to shellOutputBrowser
     *  \param[in] str  given string */
    void append(const QString& str);

    /// \brief Writes content of shellOutputBrowser to temporary file
    void rememberLastSession();

    /// \brief Fills shellOutputBrowser from temporary file
    void restoreLastSession();

public slots:

    /// \brief Cleans shellOutputBrowser
    void cleanShellOutput();

private:

enum
{
    WIDTH = 900,                                   ///< Default widget width
    HEIGHT = 200,                                  ///< Default widget height
    SIZE_OF_CLEAN_OUTPUT_BUTTON_SIDE = 21,         /** Size of "clean output"
                                                    * button side */
    LR_MARGIN = 5,                                 /** Left or right margin for
                                                    * layout of title */
    TB_MARGIN = 0,                                 /** Top or bottom margin for
                                                    * layout of title */
};

    MainWindow*            mainWindow;             /** Main window of
                                                    * application */
    QTextBrowser*          shellOutputBrowser;     /** Browser for output
                                                    * messages of current
                                                    * status of process */
    QPushButton*           cleanOutputButton;      /** Button for cleaning
                                                    * shellOutputBrowser */
    QPushButton*           closeOutputButton;      /** Button for closing of
                                                    * shellOutputBrowser */
    QLabel*                titleOutputLabel;       /** Title of
                                                    * shellOutputBrowser */
    QHBoxLayout*           titleOutputLayout;      /** Layout for location of
                                                    * titleOutputLabel and
                                                    * cleanOutputButton */
    QWidget*               titleOutputWidget;      /** Title widget for
                                                    * shellOutputBrowser */
};

class AddNameDialog : public QDialog, private Ui::AddNameDialog
{
Q_OBJECT

public:
    /** \brief Creates all elements of the window and connects signals with slots.
     * Restores size and position from settings.
     *  \param parent  parent widget */
    AddNameDialog(QWidget *parent);

    /** \brief Gets name entered in the line edit
     *  \return input file or tab name */
    inline const QString &getName() { return name; };

private:

    QSettings*    settings;         ///< Settings
    QString       name;    ///< Input file name

    /// \brief Saves settings when closing window
    void closeEvent(QCloseEvent *event);

private slots:

    /// \brief Initializes name and closes the dialog
    void onOk();
};

#endif // __MAINWINDOW_H__
