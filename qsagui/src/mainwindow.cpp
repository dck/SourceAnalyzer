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

/*! \class MainWindow mainwindow.h "./include/mainwindow.h"
 *  \brief Main window of application.
 *
 * Forms the main window: creates the widgets, actions, menu, tool bar,
 * progress bar. Manages the processes, settings etc.
 */


/// \file
/** \brief Implementation of the \ref MainWindow, \ref ApplicationOutput
 * \ref and \ref AddNameDialog classes. */
#include <QtGui>
#include <QtCore>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent)
{
    setWindowTitle("QSAGUI");
    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);

    centralWidget = new QTabWidget(this);
    centralWidget->setTabsClosable(true);
    centralWidget->setMovable(true);
    setCentralWidget(centralWidget);
    connect(centralWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    CentralWidget *cw = new CentralWidget(this);

    if(settings->value("language").toString()=="en")cw->setBrowserHtmlFile(":/html/welcome.html");
    else cw->setBrowserHtmlFile(":/html/welcome_ru.html");

    int tabIndex = centralWidget->addTab(cw, tr("New tab"));
    centralWidget->setCurrentIndex(tabIndex);

    addTabDialog = new AddNameDialog(this);
    addTabDialog->setWindowTitle(tr("QSAGUI - tab addition"));
    connect(addTabDialog, SIGNAL(accepted()), this, SLOT(addNewTab()));

    renameTabDialog = new AddNameDialog(this);
    renameTabDialog->setWindowTitle(tr("QSAGUI - tab renaming"));
    connect(renameTabDialog, SIGNAL(accepted()), this, SLOT(renameCurrentTab()));

    createDockWidgets();
    createActions();
    createToolBars();
    createMenus();
    createProgressBar();

    QDir::setCurrent(QDir::tempPath());
    QDir().mkdir("qsaguitmpdir");
    QDir::setCurrent("qsaguitmpdir");

    QString msg = tr("Current work directory for QSAGUI: ") +
                  QDir::currentPath();
    appendToShellBrowser(msg);

    if (settings->value("version").toString() != VERSION_OF_CONFIG_FILE) {
        settings->clear();
        settings->setValue("version", VERSION_OF_CONFIG_FILE);
        resize(WIDTH, HEIGHT);
        setPathToSA("");
        settings->setValue("pathToBinaries", "");
        setPathToDot("");
        settings->setValue("pathToDot", "");
        settings->setValue("checkBinaries", true);
        setWorkingDir("");
        settings->setValue("workDir", "");
        setSavingDir(PATH_TO_SETTINGS);
        settings->setValue("savingDir", PATH_TO_SETTINGS);
        showSettingsWidget();
    }
    else {
        readSettings();
    }
    connect(this, SIGNAL(destroyed()), qApp, SLOT(quit()));
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////

void MainWindow::suggestToOpen(const QString& cgName) {
    int answer = QMessageBox::question(this, this->windowTitle(),
              tr("Call graph ") + cgName +
              tr(" was created.\nWould you like to open it?"),
              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    switch (answer) {
        case QMessageBox::Yes:
            {
                callGraphName = cgName;
                isLinkedGraph = false;
                fillFuncFilesList();
                setWindowTitle("QSAGUI - "+callGraphName);
                QFileInfo fi(callGraphName );
                setLastOpenedDir(fi.absoluteDir().path());
            }
             break;
        case QMessageBox::No:
             break;
        default:
             break;
    }
}

const QString& MainWindow::getWorkingDir() {
    if (workdirPath.isEmpty()) {
        return lastOpenedDir;
    }
    return workdirPath;
}

void MainWindow::appendToShellBrowser(const QString& str) {
    shellOutputDock->append(str);
}

void MainWindow::showInfoMessage() {
    showMenu->setEnabled(true);
    QString infoMessage = tr("Call graph was opened successfully.") +
                          tr("\nNumber of functions - ") +
                          QString::number(funcsListWidget->getItemsCount()) +
                          tr("\nNumber of files - ") +
                          QString::number(filesListWidget->getItemsCount()) +
                          tr("\nName of call graph - ") +
                          QDir().absoluteFilePath(callGraphName);
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->setBrowserText(infoMessage);
}

void MainWindow::readSettings() {
    appendToShellBrowser(tr("Configuration file is saved to: ") + CONFIG_FILE);

    settings->beginGroup("MainWindow");
#ifdef Q_OS_WIN32
    restoreGeometry(settings->value("geometry", saveGeometry()).toByteArray());
#else
    move(settings->value("pos", pos()).toPoint());
    resize(settings->value("size", size()).toSize());
    if (settings->value("maximized", isMaximized()).toBool())
        setWindowState(windowState() | Qt::WindowMaximized);
    if (settings->value("fullscreen", isFullScreen()).toBool())
        setWindowState(windowState() | Qt::WindowFullScreen);
#endif
    restoreState(settings->value("state", saveState()).toByteArray());
    settings->endGroup();

    pathToSA = settings->value("pathToBinaries").toString();
    pathToDot = settings->value("pathToDot").toString();
    workdirPath = settings->value("workDir").toString();
    savingdirPath = settings->value("savingDir").toString();

   if ( settings->contains("call_graph_name") &&
        !settings->value("call_graph_name").toString().isEmpty() ) {
        callGraphName = settings->value("call_graph_name").toString();

        funcsListWidget->restoreLastSession();
        filesListWidget->restoreLastSession();

        centralWidget->clear();
        int count = settings->value("count of tabs").toInt();
        QString title;
        for (int i = 0; i < count; i ++)
        {
            CentralWidget *cw = new CentralWidget(this);
            settings->beginGroup("DockWidgets");
                settings->beginGroup("CentralWidgets");
                    settings->beginGroup(QString::number(i));
                        title = settings->value("title").toString();
                    settings->endGroup();
                settings->endGroup();
            settings->endGroup();
            centralWidget->addTab(cw, title);
            cw->restoreLastSession(i);
        }
        settings->beginGroup("DockWidgets");
        settings->beginGroup("CentralWidgets");
        settings->remove("");
        settings->endGroup();
        settings->endGroup();
        shellOutputDock->restoreLastSession();

        settings->remove("call_graph_name");
    }
    if (settings->value("checkBinaries").toBool() == true)
        QTimer::singleShot(TIME_FOR_SPLASH_SCREEN, this, SLOT(checkBinaries()));
}

QStringList MainWindow::getMissingBinaries() {
    QStringList missingBinaries;
    for (int i = 0; i < COUNT_OF_SA_BINARIES; i++) {
        QProcess testProcess;
        testProcess.start("\"" + getSAPath() + BINARIES[i] + "\" --help");
        testProcess.waitForFinished(3000);  //3 sec.
        QTextStream stream (testProcess.readAllStandardOutput());
        if (!stream.readLine().trimmed().startsWith("Usage:")) {
            missingBinaries << BINARIES[i];
        }
    }
    QProcess testProcess;
    testProcess.start("\"" + getPathToDot() + DOT + "\" -V");
    testProcess.waitForFinished(3000);  //3 sec.
    QTextStream stream (testProcess.readAllStandardError());
    if (!stream.readLine().trimmed().startsWith("dot")) {
        missingBinaries << "Graphviz";
    }
    return missingBinaries;
}

void MainWindow::setBrowserText(const QString& string) {
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->setBrowserText(string);
}

void MainWindow::getRelationsList(const QString& binaryName, QString& functions,
                                  const QString& relationType, int level) {
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->getRelationsList(binaryName, functions, relationType, level);
}

void MainWindow::blockTabs() {
    int count = centralWidget->count();
    int currentIndex = centralWidget->currentIndex();
    for (int i=0; i<count; i++) {
        if (i != currentIndex) {
            centralWidget->setTabEnabled(i, false);
            centralWidget->setTabsClosable(false);
        }
    }
}

void MainWindow::unblockTabs() {
    int count = centralWidget->count();
    for (int i=0; i<count; i++) {
        centralWidget->setTabEnabled(i, true);
        centralWidget->setTabsClosable(true);
    }
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC SLOTS ///////////////////////////////////////////////////////////////

bool MainWindow::startProcess(const QString& type,
                              const QString& binaryName,
                              const QString& params,
                              const QString& inputFileName,
                              const QString& outputFileName,
                              QProcess* process) {
    QString command;
    QProcess testProcess;
    if (type == DOT_TYPE) {
        command = "\"" + getPathToDot() + binaryName + "\"";
        testProcess.start(command + " -V");
        testProcess.waitForFinished(3000);  //3 sec.
        QTextStream stream (testProcess.readAllStandardError());
        if (!stream.readLine().trimmed().startsWith("dot")) {
            if (getPathToDot() == "")
                QMessageBox::critical(this, tr("QSAGUI"),
                                      tr("\"")+binaryName+tr("\" is missed or damaged.\nIt's a  tool for laying out directed graphs into a variety of output formats.\n\nPlease don't use system path to Graphviz in settings."));
            else
                QMessageBox::critical(this, tr("QSAGUI"),
                                  tr("\"")+binaryName+tr("\" is missed or damaged.\nIt's a tool for laying out directed graphs into a variety of output formats.\n\nPlease check it here: ")
                                  +getPathToDot());

            return false;
        }
    }
    else if (type == SA_TYPE) {
        command = "\"" + getSAPath() + binaryName + "\"";
        testProcess.start(command + " --help");
        testProcess.waitForFinished(3000);  //3 sec.
        QTextStream stream (testProcess.readAllStandardOutput());
        if (!stream.readLine().trimmed().startsWith("Usage:")) {
            QMessageBox::critical(this, tr("QSAGUI"),
               binaryName+tr(" is missed."));
            return false;
        }
    }
    else if (type == PREPROC_TYPE) {
        command = binaryName;
    }

    if (!params.isEmpty())
        command+= " " + params;

    if (!inputFileName.isEmpty())
        process->setStandardInputFile(inputFileName);
    if (!outputFileName.isEmpty())
        process->setStandardOutputFile(outputFileName);

    process->start(command);
    if (process->error()==QProcess::FailedToStart) {
        QMessageBox::critical(this, tr("QSAGUI"),
                   tr("The process ") + command + tr(" can't be started."));
        return false;
    }
    if (!inputFileName.isEmpty())
        command += " < \"" + inputFileName + "\"";
    if (!outputFileName.isEmpty())
        command += " > \"" + outputFileName + "\"";
    appendToShellBrowser(command);
    appendToList(process);
    appendToShellBrowser( tr("Wait...") );

    return true;
}

void MainWindow::onProcessFinish() {
    QProcess* process = (QProcess*) sender();
    for (int i = 0; i < listOfCurProcesses.size(); ++i) {
        if (listOfCurProcesses.at(i) == process) {
            listOfCurProcesses.removeAt(i);
        }
    }
    if (listOfCurProcesses.size() == 0) {
        tepminateProcAction->setDisabled(true);
    }
    stopProgressBar();
    QString errorString;
    if (process->exitStatus() == QProcess::CrashExit) {
        errorString += tr("Process crashed\n");
    }
    errorString += process->readAllStandardError();
    if (!errorString.isEmpty()) {
        errorString = errorString.trimmed();
        QMessageBox::critical(this, tr("QSAGUI"), errorString);
        appendToShellBrowser(errorString);
    }
    appendToShellBrowser( tr("Done.") );
}

void MainWindow::activateProgressBar() {
    int count = centralWidget->count();
    for (int i = 0; i < count; i ++)
    {
        CentralWidget* cw = (CentralWidget*) centralWidget->widget(i);
        if (cw->isSearchEnable()==true)
            return;
    }
    if (listOfCurProcesses.size()!=0)
        return;
    progressBar->show();
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
}

void MainWindow::stopProgressBar() {
    int count = centralWidget->count();
    for (int i = 0; i < count; i ++)
    {
        CentralWidget* cw = (CentralWidget*) centralWidget->widget(i);
        if (cw->isSearchEnable()==true)
            return;
    }
    if (listOfCurProcesses.size()!=0)
        return;
    progressBar->reset();
    progressBar->hide();
}

void MainWindow::showSettingsWidget() {
    static bool isSettingsWidgetCreated = false;
    if (isSettingsWidgetCreated==false) {
        settingsWidget = new SettingsWidget(this);
        isSettingsWidgetCreated=true;
    }
    settingsWidget->show();
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void MainWindow::createActions() {
    ////////// fileMenu //////////
    newTabAction = new QAction(tr("&New tab..."), this);
    newTabAction->setShortcut(tr("Ctrl+T"));
    newTabAction->setStatusTip(tr("Add new tab"));
    connect(newTabAction, SIGNAL(triggered()), addTabDialog,
            SLOT(show()));

    closeTabAction = new QAction(tr("&Close tab"), this);
    closeTabAction->setShortcut(tr("Ctrl+F4"));
    closeTabAction->setStatusTip(tr("Close current tab"));
    connect(closeTabAction, SIGNAL(triggered()), this,
            SLOT(closeCurrentTab()));

    renameTabAction = new QAction(tr("&Rename tab..."), this);
    renameTabAction->setShortcut(tr("Ctrl+R"));
    renameTabAction->setStatusTip(tr("Rename current tab"));
    connect(renameTabAction, SIGNAL(triggered()), renameTabDialog,
            SLOT(show()));

    fileSaveAction = new QAction(QIcon(":/images/filesave.png"),
                                 tr("&Save..."), this);
    fileSaveAction->setShortcut(tr("Ctrl+S"));
    fileSaveAction->setStatusTip(tr("Save file..."));
    connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(onSave()));

    filePrintAction = new QAction(QIcon(":/images/fileprint.png"),
                                  tr("&Print..."), this);
    filePrintAction->setShortcut(tr("Ctrl+P"));
    filePrintAction->setStatusTip(tr("Print file..."));
    connect(filePrintAction, SIGNAL(triggered()), this, SLOT(printFile()));

    quitAction = new QAction(QIcon(":/images/quit.png"), tr("&Quit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    quitAction->setStatusTip(tr("Quit"));
    //connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeProgram()));

    ////////// cgMenu //////////
    newCallGraphAction = new QAction(QIcon(":/images/new.png"), tr("&New..."),
                                     this);
    newCallGraphAction->setShortcut(tr("Ctrl+N"));
    newCallGraphAction->setStatusTip(tr("Create new call graph"));
    connect(newCallGraphAction, SIGNAL(triggered()), this,
            SLOT(newCallGraph()));

    loadAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."),
                             this);
    loadAction->setShortcut(tr("Ctrl+O"));
    loadAction->setStatusTip(tr("Open call graph"));
    connect(loadAction, SIGNAL(triggered()), this, SLOT(loadFile()));

    showCgAsTextAction = new QAction(tr("text"), this);
    showCgAsTextAction->setStatusTip(tr("Show call graph in text format"));
    connect(showCgAsTextAction, SIGNAL(triggered()), this, SLOT(getCgAsText()));

    showCgAsSvgAction = new QAction(tr("SVG"), this);
    showCgAsSvgAction->setStatusTip(tr("Show call graph in SVG format"));
    connect(showCgAsSvgAction, SIGNAL(triggered()), this, SLOT(getCgAsSvg()));

    linkAction = new QAction(QIcon(":/images/callgraphslink.png"),
                             tr("&Link..."), this);
    linkAction->setShortcut(tr("Ctrl+L"));
    linkAction->setStatusTip(tr("Link call graphs..."));
    connect(linkAction, SIGNAL(triggered()), this, SLOT(linkCallGraphs()));

    cleanAction = new QAction(QIcon(":/images/callgraphclean.png"),
                              tr("&Clean..."), this);
    cleanAction->setShortcut(tr("Ctrl+Shift+C"));
    cleanAction->setStatusTip(tr("Clean call graph..."));
    connect(cleanAction, SIGNAL(triggered()), this, SLOT(cleanCallGraph()));

    updateAction = new QAction(QIcon(":/images/callgraphupdate.png"),
                               tr("&Update..."), this);
    updateAction->setShortcut(tr("Ctrl+U"));
    updateAction->setStatusTip(tr("Update call graph..."));
    connect(updateAction, SIGNAL(triggered()), this, SLOT(updateCallGraph()));

    ////////// viewMenu //////////
    fullScreenAction = new QAction(tr("&Full screen"), this);
    fullScreenAction->setCheckable(true);
    fullScreenAction->setShortcut(tr("F11"));
    fullScreenAction->setStatusTip(tr("Full screen"));
    connect(fullScreenAction, SIGNAL(triggered(bool)), this,
            SLOT(setFullScreen(bool)));

    ////////// settingsMenu //////////
    settingsAction = new QAction(QIcon(":/images/settings.png"),
                                 tr("&Settings..."), this);
    settingsAction->setStatusTip(tr("Show settings"));
    connect(settingsAction, SIGNAL(triggered()), this,
            SLOT(showSettingsWidget()));

    enSelect=new QAction(tr("English"), this);
    enSelect->setCheckable(true);
    connect(enSelect, SIGNAL(triggered()), this, SLOT(enTranslate()));

    ruSelect=new QAction(tr("Russian"), this);
    ruSelect->setCheckable(true);
    connect(ruSelect, SIGNAL(triggered()), this, SLOT(ruTranslate()));
    if(settings->value("language").toString()=="ru") ruSelect->setChecked(true);
    else enSelect->setChecked(true);

    ////////// editMenu //////////
    findAction = new QAction(QIcon(":/images/find.png"), tr("&Find..."), this);
    findAction->setShortcut(tr("Ctrl+F"));
    findAction->setStatusTip(tr("Find in text"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(openFindWidget()));

    cleanWindowAction = new QAction(QIcon(":/images/clear-all.png"),
                                    tr("&Clean window"), this);
    cleanWindowAction->setStatusTip(tr("Clean window"));
    connect(cleanWindowAction, SIGNAL(triggered()), this, SLOT(cleanWindow()));

    tepminateProcAction = new QAction(QIcon(":/images/kill.png"), tr("&Terminate last process"), this);
    tepminateProcAction->setStatusTip(tr("Terminate last process"));
    tepminateProcAction->setDisabled(true);
    connect(tepminateProcAction, SIGNAL(triggered()), this, SLOT(tepminateLastProcess()));

    ////////// helpMenu //////////
    aboutAction = new QAction(QIcon(":/images/about.png"), tr("&About QSAGUI"), this);
    aboutAction->setStatusTip(tr("About QSAGUI"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutWidget()));

    helpAction = new QAction(QIcon(":/images/help.png"), tr("&Help"), this);
    helpAction->setStatusTip(tr("Show QSAGUI user manual"));
    helpAction->setShortcut(tr("F1"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(showHelp()));
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newTabAction);
    fileMenu->addAction(closeTabAction);
    fileMenu->addAction(renameTabAction);
    fileMenu->addSeparator();
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(filePrintAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    cgMenu = menuBar()->addMenu(tr("&Call graph"));
    cgMenu->addAction(newCallGraphAction);
    cgMenu->addAction(loadAction);
    showMenu = new QMenu(tr("Show as..."), this);
    showMenu->setIcon(QIcon(":/images/callgraph.png"));
    showMenu->addAction(showCgAsTextAction);
    showMenu->addAction(showCgAsSvgAction);
    cgMenu->addMenu(showMenu);
    showMenu->setDisabled(true);
    cgMenu->addSeparator();
    cgMenu->addAction(linkAction);
    cgMenu->addAction(cleanAction);
    cgMenu->addAction(updateAction);

    viewMenu = new QMenu(this);
    viewMenu = createPopupMenu();
    viewMenu->setTitle(tr("&View"));
    viewMenu->addSeparator();
    viewMenu->addAction(fullScreenAction);
    menuBar()->addMenu(viewMenu);

    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(settingsAction);

    language=settingsMenu->addMenu(tr("Language"));
    language->addAction(enSelect);
    language->addAction(ruSelect);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(findAction);
    editMenu->addAction(cleanWindowAction);
    editMenu->addAction(tepminateProcAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(helpAction);
}

void MainWindow::createToolBars() {
    cgToolBar = addToolBar(tr("Open call graph"));
    cgToolBar->setObjectName(tr("openCallGraphToolBar"));
    cgToolBar->addAction(newCallGraphAction);
    cgToolBar->addAction(loadAction);
    cgToolBar->setIconSize(QSize(SIZE_OF_ICON_SIDE, SIZE_OF_ICON_SIDE));

    callGraphToolBar = addToolBar(tr("Call graph"));
    callGraphToolBar->setObjectName(tr("callGraphToolBar"));
    callGraphToolBar->addAction(linkAction);
    callGraphToolBar->addAction(cleanAction);
    callGraphToolBar->addAction(updateAction);
    callGraphToolBar->setIconSize(QSize(SIZE_OF_ICON_SIDE, SIZE_OF_ICON_SIDE));

    settingsToolBar = addToolBar(tr("Settings"));
    settingsToolBar->setObjectName(tr("settingsToolBar"));
    settingsToolBar->addAction(settingsAction);
    settingsToolBar->setIconSize(QSize(SIZE_OF_ICON_SIDE, SIZE_OF_ICON_SIDE));

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName(tr("editToolBar"));
    editToolBar->addAction(findAction);
    editToolBar->addAction(cleanWindowAction);
    editToolBar->addAction(tepminateProcAction);
    editToolBar->setIconSize(QSize(SIZE_OF_ICON_SIDE, SIZE_OF_ICON_SIDE));

    printSaveToolBar = addToolBar(tr("Print and save"));
    printSaveToolBar->setObjectName(tr("printSaveToolBar"));
    printSaveToolBar->addAction(fileSaveAction);
    printSaveToolBar->addAction(filePrintAction);
    printSaveToolBar->setIconSize(QSize(SIZE_OF_ICON_SIDE, SIZE_OF_ICON_SIDE));

    quitToolBar = addToolBar(tr("Quit"));
    quitToolBar->setObjectName(tr("quitToolBar"));
    quitToolBar->addAction(quitAction);
    quitToolBar->setIconSize(QSize(SIZE_OF_ICON_SIDE, SIZE_OF_ICON_SIDE));
}

void MainWindow::createProgressBar() {
    progressBar = new QProgressBar(this);
    progressBar->setFixedWidth(PROGRESS_BAR_WIDTH);
    statusBar()->addPermanentWidget(progressBar);
    progressBar->hide();
}

void MainWindow::createDockWidgets() {
    shellOutputDock = new ApplicationOutput(this);
    shellOutputDock->setObjectName(tr("shellOutputDock"));
    addDockWidget(Qt::BottomDockWidgetArea, shellOutputDock);

    funcsListWidget = new ListWidget(this, true);
    funcsListDock = new QDockWidget(tr("Functions list"), this);
    funcsListDock->setObjectName(tr("funcsListDock"));
    funcsListDock->setWidget(funcsListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, funcsListDock);

    filesListWidget = new ListWidget(this, false);
    filesListDock = new QDockWidget(tr("Source files list"), this);
    filesListDock->setObjectName(tr("filesListDock"));
    filesListDock->setWidget(filesListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, filesListDock);

    tabifyDockWidget(filesListDock, funcsListDock);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!callGraphName.isEmpty()) {
        int answer = QMessageBox::question(this, this->windowTitle(),
                  tr("Do you want to save current work?"),
                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (answer) {
            case QMessageBox::Save:
                 rememberLastSession();
                 break;
            case QMessageBox::Discard:
                 break;
            case QMessageBox::Cancel:
                 event->ignore();
                 break;
            default:
                 rememberLastSession();
                 break;
        }
    }
    for (int i=0; i < listOfCurProcesses.size(); ++i) {
        if (listOfCurProcesses.at(i)->state() ==
             QProcess::Running) {
            listOfCurProcesses.at(i)->kill();
            listOfCurProcesses.removeAt(i);
        }
    }
    QDir directory(QDir::currentPath());
    QStringList list = directory.entryList();
    for (int i = 0; i < list.size(); ++i) {
        directory.remove(list.at(i));
    }
    directory.cdUp();
    directory.rmdir("qsaguitmpdir");

    settings->beginGroup("MainWindow");
    if (!isMinimized() && !isMaximized() && !isFullScreen())
    {
        settings->setValue("pos", pos());
        settings->setValue("size", size());
    }
    settings->setValue("maximized", isMaximized());
    settings->setValue("fullscreen", isFullScreen());
    settings->setValue("geometry", saveGeometry());
    funcsListWidget->saveState();
    filesListWidget->saveState();
    settings->setValue("state", saveState());
    settings->endGroup();
}

void MainWindow::rememberLastSession()  {
    activateProgressBar();
    settings->setValue("call_graph_name", callGraphName);
    statusBar()->showMessage(tr("Saving application output..."));
    shellOutputDock->rememberLastSession();
    statusBar()->showMessage(tr("Saving functions list..."));
    funcsListWidget->rememberLastSession();
    statusBar()->showMessage(tr("Saving files list..."));
    filesListWidget->rememberLastSession();
    statusBar()->showMessage(tr("Saving text area..."));
    int count = centralWidget->count();
    settings->setValue("count of tabs", count);
    for (int i = 0; i < count; i ++)
    {
        centralWidget->setCurrentIndex(i);
        settings->beginGroup("DockWidgets");
            settings->beginGroup("CentralWidgets");
                settings->beginGroup(QString::number(i));
                    settings->setValue("title", centralWidget->tabText(i));
                settings->endGroup();
            settings->endGroup();
        settings->endGroup();
        CentralWidget* cw = (CentralWidget*) centralWidget->widget(i);
        cw->rememberLastSession(i);
    }
    stopProgressBar();
 }

void MainWindow::appendToList(QProcess *process) {
    activateProgressBar();
    listOfCurProcesses.append(process);
    if (tepminateProcAction->isEnabled() == false) {
        tepminateProcAction->setEnabled(true);
    }
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////
void MainWindow::ruTranslate(){
     enSelect->setChecked(false);
     ruSelect->setChecked(true);
     settings->setValue("language","ru");

     QMessageBox::information(this, this->windowTitle(),
     tr("Settings will take effect after restarting QSAGUI"),
     QMessageBox::Ok);
 }

void MainWindow::enTranslate(){
     ruSelect->setChecked(false);
     enSelect->setChecked(true);
     settings->setValue("language","en");

     QMessageBox::information(this, this->windowTitle(),
     tr("Settings will take effect after restarting QSAGUI"),
     QMessageBox::Ok);
}

void MainWindow::addNewTab() {
    CentralWidget *cw = new CentralWidget(this);
    QString name = addTabDialog->getName();
    int tabIndex = centralWidget->addTab(cw, name);
    centralWidget->setCurrentIndex(tabIndex);
}

void MainWindow::closeTab(int i) {
    if (centralWidget->count() == 1)
        return;
    centralWidget->removeTab(i);
}

void MainWindow::closeProgram() {
    bool isCancel = false;
    if (!callGraphName.isEmpty()) {
        int answer = QMessageBox::question(this, this->windowTitle(),
                  tr("Do you want to save current work?"),
                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (answer) {
            case QMessageBox::Save:
                 rememberLastSession();
                 break;
            case QMessageBox::Discard:
                 break;
            case QMessageBox::Cancel:
                 isCancel = true;
                 break;
            default:
                 rememberLastSession();
                 break;
        }
    }
    if(isCancel) return;
    for (int i=0; i < listOfCurProcesses.size(); ++i) {
        if (listOfCurProcesses.at(i)->state() ==
             QProcess::Running) {
            listOfCurProcesses.at(i)->kill();
            listOfCurProcesses.removeAt(i);
        }
    }
    QDir directory(QDir::currentPath());
    QStringList list = directory.entryList();
    for (int i = 0; i < list.size(); ++i) {
        directory.remove(list.at(i));
    }
    directory.cdUp();
    directory.rmdir("qsaguitmpdir");

    settings->beginGroup("MainWindow");
    if (!isMinimized() && !isMaximized() && !isFullScreen())
    {
        settings->setValue("pos", pos());
        settings->setValue("size", size());
    }
    settings->setValue("maximized", isMaximized());
    settings->setValue("fullscreen", isFullScreen());
    settings->setValue("geometry", saveGeometry());
    funcsListWidget->saveState();
    filesListWidget->saveState();
    settings->setValue("state", saveState());
    settings->endGroup();
    qApp->quit();
}

void MainWindow::renameCurrentTab() {
    int i = centralWidget->currentIndex();
    QString name = renameTabDialog->getName();
    centralWidget->setTabText(i, name);
}

void MainWindow::closeCurrentTab() {
    int i = centralWidget->currentIndex();
    closeTab(i);
}

void MainWindow::onSave() {
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->onSave();
}

void MainWindow::printFile() {
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->printFile();
}

void MainWindow::openFindWidget() {
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->openFindWidget();
}

void MainWindow::getCgAsText() {
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->getCgAsText();
}

void MainWindow::getCgAsSvg() {
    CentralWidget* cw = (CentralWidget*) centralWidget->currentWidget();
    cw->getCgAsSvg();
}

void MainWindow::checkBinaries() {
    QStringList missingBinaries = getMissingBinaries();
    if (missingBinaries.isEmpty()) {
        return;
    }
    else {
        QString bins = missingBinaries.join("\n");
        int answer = QMessageBox::warning(this, tr("QSAGUI"),
                             tr("Some binaries are missed:\n")+bins+"\n"+
                             tr("Display settings?"),
                             QMessageBox::Yes | QMessageBox::No);
         switch (answer) {
             case QMessageBox::Yes:
                  showSettingsWidget();
                  break;
             case QMessageBox::No:
                  break;
             default:
                  showSettingsWidget();
                  break;
        }
    }
}

void MainWindow::loadFile() {
    QStringList listOfFileNames = QFileDialog::getOpenFileNames(this,
                                  tr("Load file"), getWorkingDir());
    if (listOfFileNames.isEmpty()) {
        return;
    }
    MainWindow::cleanWindow();
    if (centralWidget->count() > 1) {
        int answer = QMessageBox::warning(this, tr("QSAGUI"),
                             tr("All tabs will be closed.\nDo you wish to continue?"),
                             QMessageBox::Yes | QMessageBox::No);
         switch (answer) {
             case QMessageBox::Yes:
                  break;
             case QMessageBox::No:
                  return;
                  break;
             default:
                  return;
                  break;
        }
    }
//    for (int i=0; i < listOfCurProcesses.size(); ++i) {
//        if (listOfCurProcesses.at(i)->state() ==
//             QProcess::Running) {
//            listOfCurProcesses.at(i)->kill();
//            listOfCurProcesses.removeAt(i);
//        }
//    }
    QString title = centralWidget->tabText(centralWidget->currentIndex());
    centralWidget->clear();
    CentralWidget *cw = new CentralWidget(this);
    int tabIndex = centralWidget->addTab(cw, title);
    centralWidget->setCurrentIndex(tabIndex);
    newTabAction->setDisabled(true);
    if (listOfFileNames.size() == 1) {
        callGraphName = listOfFileNames.at(0);
        isLinkedGraph = false;
        fillFuncFilesList();
    }
    else {
        callGraphName = getSavingDir()+QDir::separator()+COMMON_GRAPH;
        QString params;
        for (int i = 0; i < listOfFileNames.size(); ++i) {
            params += " \"" + listOfFileNames.at(i) + "\"";
        }
        isLinkedGraph = true;
        QProcess *linkageProcess = new QProcess(this);
        connect(linkageProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(onProcessFinish()));
        connect(linkageProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(fillFuncFilesList()));
        startProcess(SA_TYPE, LINK, params, "", callGraphName, linkageProcess);
    }
    setWindowTitle("QSAGUI - "+callGraphName);
    QFileInfo fi(listOfFileNames.at(0));
    setLastOpenedDir(fi.absoluteDir().path());
}

void MainWindow::fillFuncFilesList() {
    if (isLinkedGraph) {
        QProcess* process = (QProcess*) sender();
        if (process->exitStatus() == QProcess::CrashExit) {
            unblockTabs();
            return;
        }
    }
    //cw->fillCallGraphStruct();  // Trouble after merge
    filesListWidget->fillList();
    newTabAction->setDisabled(false);
}

void MainWindow::newCallGraph() {
    static bool isNewWidgetCreated = false;
    if (isNewWidgetCreated==false) {
        newCallGraphWidget = new NewCallGraphWidget(this);
        isNewWidgetCreated=true;
    }

    newCallGraphWidget->show();
}

void MainWindow::linkCallGraphs() {
    static bool isLinkWidgetCreated = false;
    if (isLinkWidgetCreated==false) {
        linkCallGraphsWidget = new LinkCallGraphsWidget(this);
        isLinkWidgetCreated=true;
    }
    linkCallGraphsWidget->show();
}

void MainWindow::cleanCallGraph() {
    static bool isCleanWidgetCreated = false;
    if (isCleanWidgetCreated==false) {
        cleanCallGraphWidget = new CleanCallGraphWidget(this);
        isCleanWidgetCreated=true;
    }
    cleanCallGraphWidget->show();
}

void MainWindow::updateCallGraph() {
    static bool isUpdateWidgetCreated = false;
    if (isUpdateWidgetCreated==false) {
        updateCallGraphWidget = new UpdateCallGraphWidget(this);
        isUpdateWidgetCreated=true;
    }
    updateCallGraphWidget->show();
}

void MainWindow::showAboutWidget() {
    static bool isAboutWidgetCreated = false;
    if (isAboutWidgetCreated==false) {
        aboutWidget = new AboutWidget (this);
        isAboutWidgetCreated=true;
    }
    aboutWidget->show();
}

void MainWindow::showHelp() {
    if(settings->value("language").toString()=="en")showHelp(QUrl("qrc:/html/manual/getting_started.html"));
    else showHelp(QUrl("qrc:/html/manual_ru/getting_started.html"));
}

void MainWindow::showHelp(QUrl url) {
    static bool isHelpWidgetCreated = false;
    if (isHelpWidgetCreated==false) {
        helpWidget = new HelpWidget(this);
        isHelpWidgetCreated=true;
    }
    helpWidget->showPage(url);
    helpWidget->show();
}

void MainWindow::cleanWindow() {
    centralWidget->clear();

    CentralWidget *cw = new CentralWidget(this);
    int tabIndex = centralWidget->addTab(cw, tr("New tab"));
    centralWidget->setCurrentIndex(tabIndex);

    shellOutputDock->cleanShellOutput();
    callGraphName = "";
    showMenu->setDisabled(true);
    funcsListWidget->clear();
    filesListWidget->clear();
    //ListWidget::selectedItemsList
    //kfkf->cleanShellOutput();

}

void MainWindow::tepminateLastProcess() {
    if (listOfCurProcesses.last()->state() == QProcess::Running) {
        listOfCurProcesses.last()->kill();
        listOfCurProcesses.removeLast();
    }
}

void MainWindow::setFullScreen(bool on) {
    if (bool(windowState() & Qt::WindowFullScreen) == on)
        return;
    if (on) {
        setWindowState(windowState() | Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    }
}


/*! \class ApplicationOutput mainwindow.h "./include/mainwindow.h"
 *  \brief Bottom area in which commands and information messages are displayed. */

ApplicationOutput::ApplicationOutput(QWidget* parent)
    : QDockWidget(parent)
{
    setWindowTitle(tr("Application output"));
    mainWindow = (MainWindow*) parent;
    shellOutputBrowser = new QTextBrowser(this);
    setWidget(shellOutputBrowser);

    cleanOutputButton = new QPushButton(QIcon(":/images/edit-clear.png"),
                                        tr(""), this);
    cleanOutputButton->setFixedSize(SIZE_OF_CLEAN_OUTPUT_BUTTON_SIDE,
                                    SIZE_OF_CLEAN_OUTPUT_BUTTON_SIDE);
    cleanOutputButton->setFlat(true);
    connect(cleanOutputButton, SIGNAL(clicked()), this,
            SLOT(cleanShellOutput()));

    closeOutputButton = new QPushButton(this);
    closeOutputButton->setIcon(style()->standardIcon(
            QStyle::SP_TitleBarCloseButton));
    closeOutputButton->setFixedSize(SIZE_OF_CLEAN_OUTPUT_BUTTON_SIDE,
                                    SIZE_OF_CLEAN_OUTPUT_BUTTON_SIDE);
    closeOutputButton->setFlat(true);
    connect(closeOutputButton, SIGNAL(clicked()), this, SLOT(hide()));

    titleOutputLabel = new QLabel(tr("Application output"), this);
    titleOutputLayout = new QHBoxLayout;
    titleOutputLayout->setContentsMargins(LR_MARGIN, TB_MARGIN, LR_MARGIN,
                                            TB_MARGIN);
    titleOutputLayout->addWidget(titleOutputLabel);
    titleOutputLayout->addWidget(cleanOutputButton);
    titleOutputLayout->addStretch();
    titleOutputLayout->addWidget(closeOutputButton);
    titleOutputWidget = new QWidget(this);
    titleOutputWidget->setLayout(titleOutputLayout);
    setTitleBarWidget(titleOutputWidget);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS //////////////////////////////////////////////////////////

void ApplicationOutput::append( const QString& str ) {
    shellOutputBrowser->append(str);
}

void ApplicationOutput::rememberLastSession() {
    QFile shellOutput(mainWindow->getSavingDir() + QDir::separator() +
                       SAVED_SHELL_OUTPUT_TEXT);
    if (!shellOutput.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream out(&shellOutput);
    out << shellOutputBrowser->toPlainText();
    shellOutput.close();
}

void ApplicationOutput::restoreLastSession() {
    QFile shellOutput(mainWindow->getSavingDir() + QDir::separator() +
                       SAVED_SHELL_OUTPUT_TEXT);
    if (!shellOutput.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream text(&shellOutput);
    shellOutputBrowser->append(text.readAll());
    shellOutput.close();
    shellOutput.remove();
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC SLOTS ///////////////////////////////////////////////////////////////

void ApplicationOutput::cleanShellOutput() {
    shellOutputBrowser->clear();
}


/*! \class AddNameDialog mainwindow.h "./include/mainwindow.h"
 *  \brief Small window for addition of the file name or tab name. */

AddNameDialog::AddNameDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUi(this);

    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);
    settings->beginGroup("Windows");
        settings->beginGroup("AddNameDialog");
        restoreGeometry(settings->value("geometry").toByteArray());
        settings->endGroup();
    settings->endGroup();

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOk()));
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void AddNameDialog::closeEvent(QCloseEvent *event) {
    settings->beginGroup("Windows");
        settings->beginGroup("AddNameDialog");
        settings->setValue("geometry", saveGeometry());
        settings->endGroup();
    settings->endGroup();
    QWidget::closeEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void AddNameDialog::onOk() {
    name = inputFileEdit->text();
    this->accept();
}
