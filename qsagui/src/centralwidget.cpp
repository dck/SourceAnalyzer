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

/*! \class CentralWidget centralwidget.h "./include/centralwidget.h"
 *  \brief Central area for output of text, HTML and SVG.
 *
 * Shows the children or parents functions, call graph in text or SVG format;
 * includes the find widget.
 */

/// \file
/** \brief Implementation of the \ref CentralWidget and
 * \ref FindWidget classes. */

#include <QtGui>
#include <QtCore>

#include "centralwidget.h"

CentralWidget::CentralWidget(MainWindow* mainWindow) : QWidget()
{
    this->mainWindow = mainWindow;
    settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, this);

    textBrowser = new QPlainTextEdit(this);
    textBrowser->setReadOnly(true);
    htmlBrowser = new QTextBrowser(this);
    htmlBrowser->setReadOnly(true);
    htmlBrowser->hide();
    svgViewer = new SvgViewer(this);
    svgViewer->hide();

    findWidget = new FindWidget(this);
    findWidget->hide();
    isSearch = false;

    browserlayout = new QVBoxLayout;
    browserlayout->addWidget(textBrowser);
    browserlayout->addWidget(htmlBrowser);
    browserlayout->addWidget(svgViewer);
    browserlayout->addWidget(findWidget);
    setLayout(browserlayout);
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////

void CentralWidget::getRelationsList(const QString& binaryName,
                                     QString& functions,
                                     const QString& relationType,
                                     int level) {
    if (binaryName == CG2DOT) {
        if ((relationType == "Child")||((relationType == "Потомок")))
            functions += " -c";
        else if ((relationType == "Parent")||((relationType == "Родитель")))
            functions += " -p";
        else
            functions += " -c -p";
        if (level != 0)
            functions += " -l " + QString::number(level);
        QProcess *getFuncDotProcess = new QProcess(this);
        connect(getFuncDotProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                mainWindow, SLOT(onProcessFinish()));
        QProcess *getFuncSvgProcess = new QProcess(this);
        connect(getFuncSvgProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                SLOT(showRelationsGraph()));
        connect(getFuncSvgProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                mainWindow, SLOT(onProcessFinish()));
        getFuncDotProcess->setStandardOutputProcess(getFuncSvgProcess);
        runProcess(CG2DOT, functions, getFuncDotProcess);
        mainWindow->startProcess(DOT_TYPE, DOT, " -Tsvg", "", "", getFuncSvgProcess);
    }
    else {
        QProcess *getFuncListProcess = new QProcess(this);
        connect(getFuncListProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                SLOT(showRelationsList()));
        connect(getFuncListProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
                mainWindow, SLOT(onProcessFinish()));
        if ( (relationType == "Child") || (relationType == "Потомок") ) {
            if (level != 0)
                runProcess(LEVCHILDREN, functions, getFuncListProcess);
            else
                runProcess(CHILDREN, functions, getFuncListProcess);
        }
        else if ( (relationType == "Parent") || (relationType == "Родитель") ) {
            if (level != 0)
                runProcess(LEVPARENTS, functions, getFuncListProcess);
            else
                runProcess(PARENTS, functions, getFuncListProcess);
        }
    }
}

void CentralWidget::setBrowserHtmlFile(const QString& fileName) {
    showHtmlBrowser();
    htmlBrowser->setSource(QUrl("qrc" + fileName));
    isList=false;
};

void CentralWidget::setBrowserText(const QString& string) {
    showTextBrowser();
    textBrowser->setPlainText(string);
    isList=false;
}

void CentralWidget::rememberLastSession(int position) {
    QString fileName = mainWindow->getSavingDir() + QDir::separator() + AREA_CONTENT + QString::number(position);
    if (svgViewer->isVisible()) {
        fileName += ".svg";
    }
    else if (textBrowser->isVisible()){
        fileName += ".txt";
    }
    else if (htmlBrowser->isVisible()){
        fileName += ".html";
    }
    saveFile(fileName);

    settings->beginGroup("DockWidgets");
        settings->beginGroup("CentralWidgets");
            settings->beginGroup(QString::number(position));
                settings->setValue("isTextBrowserVisible", textBrowser->isVisible());
                settings->setValue("isHtmlBrowserVisible", htmlBrowser->isVisible());
                settings->setValue("isSvgViewerVisible", svgViewer->isVisible());
                settings->setValue("isList", isList);
            settings->endGroup();
        settings->endGroup();
    settings->endGroup();
}

void CentralWidget::restoreLastSession(int position) {
    QString fileName = AREA_CONTENT+QString::number(position);
    settings->beginGroup("DockWidgets");
    settings->beginGroup("CentralWidgets");
    settings->beginGroup(QString::number(position));
    if (settings->value("isSvgViewerVisible").toBool()==true) {
        showSvgViewer();
        fileName += ".svg";
    }
    else if (settings->value("isTextBrowserVisible").toBool()==true){
        showTextBrowser();
        fileName += ".txt";
    }
    else if (settings->value("isHtmlBrowserVisible").toBool()==true){
        showHtmlBrowser();
        fileName += ".html";
    }
    isList = settings->value("isList").toBool();

    QFile file(mainWindow->getSavingDir() + QDir::separator() \
               + fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        settings->endGroup();
        settings->endGroup();
        settings->endGroup();
        return;
    }
    if (settings->value("isSvgViewerVisible").toBool()==true) {
        svgViewer->setContent(file.readAll());
    }
    else if (settings->value("isTextBrowserVisible").toBool()==true) {
        QTextStream stream(&file);
        textBrowser->setPlainText(stream.readAll());
    }
    else if (settings->value("isHtmlBrowserVisible").toBool()==true) {
        QTextStream stream(&file);
        htmlBrowser->setHtml(stream.readAll());
    }
    file.close();
    file.remove();
    settings->endGroup();
    settings->endGroup();
    settings->endGroup();
}

bool CentralWidget::isSearchEnable() {
    return isSearch;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC SLOTS ///////////////////////////////////////////////////////////////

void CentralWidget::getCgAsText() {
    QProcess *getCgAsTextProcess = new QProcess(this);
    connect(getCgAsTextProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(showCgAsText()));
    connect(getCgAsTextProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            mainWindow, SLOT(onProcessFinish()));
    runProcess(CG2TEXT, "", getCgAsTextProcess);
}

void CentralWidget::getCgAsSvg() {
    QProcess *getCgAsDotProcess = new QProcess(this);
    connect(getCgAsDotProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            mainWindow, SLOT(onProcessFinish()));
    QProcess *getCgAsSvgProcess = new QProcess(this);
    connect(getCgAsSvgProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            mainWindow, SLOT(onProcessFinish()));
    connect(getCgAsSvgProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(showCgAsSvg()));
    getCgAsDotProcess->setStandardOutputProcess(getCgAsSvgProcess);
    runProcess(CG2DOT, "", getCgAsDotProcess);
    mainWindow->startProcess(DOT_TYPE, DOT, " -Tsvg", "", "", getCgAsSvgProcess);
}

void CentralWidget::openFindWidget() {
    findWidget->show();
}

void CentralWidget::printFile() {
    if (svgViewer->isVisible()) {
        svgViewer->print();
        return;
    }
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() == QDialog::Accepted) {
        if (textBrowser->isVisible()) {
            textBrowser->document()->print(&printer);
        }
        else if (htmlBrowser->isVisible()) {
            htmlBrowser->document()->print(&printer);
        }
    }
}

void CentralWidget::onSave() {
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,
        tr("Choose a file name"), mainWindow->getWorkingDir());
    saveFile(fileName);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS //////////////////////////////////////////////////////////

void CentralWidget::showTextBrowser() {
    htmlBrowser->hide();
    svgViewer->hide();
    textBrowser->show();
}

void CentralWidget::showHtmlBrowser() {
    textBrowser->hide();
    svgViewer->hide();
    htmlBrowser->show();
}

void CentralWidget::showSvgViewer() {
    textBrowser->hide();
    htmlBrowser->hide();
    svgViewer->show();
}

void CentralWidget::runProcess(const QString& binaryName,
                               const QString& arguments,
                               QProcess* process) {
    QString callGraphName = mainWindow->getCallGraphName();
    if(callGraphName.isEmpty()) {
        return;
    }
    mainWindow->startProcess(SA_TYPE, binaryName, arguments, callGraphName, "", process);
    if (binaryName == CHILDREN || binaryName == PARENTS)
        isList = true;
    else
        isList = false;
}

void CentralWidget::saveFile(const QString& fileName) {
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    QTextStream stream(&file);

    if (svgViewer->isVisible()) {
        stream << svgViewer->getContent();
    }
    else if (textBrowser->isVisible()) {
        stream << textBrowser->toPlainText();
    }
    else if (htmlBrowser->isVisible()) {
        stream << htmlBrowser->toHtml();
    }
    file.close();
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS //////////////////////////////////////////////////////////////

void CentralWidget::showRelationsList() {
    mainWindow->unblockTabs();
    QProcess* process = (QProcess*) sender();
    if (process->exitStatus()==QProcess::CrashExit) {
        return;
    }
    showTextBrowser();
    setBrowserText("");
    // Trouble after merge
    /*QString str = getFuncListProcess->readAllStandardOutput();
    textBrowser->setPlainText(str);
    listOfFunctions.clear();
    listOfFunctions << str.split("\n");*/
    findWidget->hideAdvancedSearchOptions();
}

void CentralWidget::showRelationsGraph() {
    mainWindow->unblockTabs();
    QProcess* process = (QProcess*) sender();
    if (process->exitStatus() == QProcess::CrashExit) {
        return;
    }
    showSvgViewer();
    svgViewer->setContent(process->readAllStandardOutput());
    findWidget->hideAdvancedSearchOptions();
}

void CentralWidget::showCgAsText() {
    mainWindow->unblockTabs();
    QProcess* process = (QProcess*) sender();
    if (process->exitStatus() == QProcess::CrashExit) {
        return;
    }
    showTextBrowser();
    textBrowser->setPlainText(process->readAllStandardOutput());
    findWidget->showAdvancedSearchOptions();
}

void CentralWidget::showCgAsSvg() {
    mainWindow->unblockTabs();
    QProcess* process = (QProcess*) sender();
    if (process->exitStatus() == QProcess::CrashExit) {
        return;
    }
    showSvgViewer();
    svgViewer->setContent(process->readAllStandardOutput());
    findWidget->hideAdvancedSearchOptions();
}

void CentralWidget::findNextString() {
    mainWindow->activateProgressBar();
    isSearch = true;
    QString stringOfSearch = findWidget->getSearchString();
    if (isList==true) {
        textBrowser->setPlainText(listOfFunctions.filter(
                stringOfSearch, Qt::CaseInsensitive ).join("\n"));
    }
    else if (svgViewer->isVisible()) {
        svgViewer->find(stringOfSearch);
    }
    else {
        int flags(findWidget->getFindOptions());
        if (textBrowser->isVisible()) {
            textBrowser->find(stringOfSearch,(QTextDocument::FindFlags)flags);
        }
        else if (htmlBrowser->isVisible()) {
            htmlBrowser->find(stringOfSearch,(QTextDocument::FindFlags)flags);
        }
    }
    isSearch = false;
    mainWindow->stopProgressBar();
}

void CentralWidget::findPreviousString() {
    mainWindow->activateProgressBar();
    isSearch = true;
    QString stringOfSearch = findWidget->getSearchString();
    int flags(findWidget->getFindOptions());
    flags |= QTextDocument::FindBackward;
    if (textBrowser->isVisible()) {
        textBrowser->find(stringOfSearch,(QTextDocument::FindFlags)flags);
    }
    else if (htmlBrowser->isVisible()) {
        htmlBrowser->find(stringOfSearch,(QTextDocument::FindFlags)flags);
    }
    isSearch = false;
    mainWindow->stopProgressBar();
}



/*! \class FindWidget centralwidget.h "./include/centralwidget.h"
 *  \brief Widget for standard words search */

FindWidget::FindWidget(QWidget *parent) : QWidget()
{
    setupUi(this);
    centralWidget = (CentralWidget*) parent;
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(editSearch, SIGNAL(editingFinished()), centralWidget,
            SLOT(findNextString()));
    connect(searchPreviousButton, SIGNAL(clicked()), centralWidget,
            SLOT(findPreviousString()));
    connect(searchNextButton, SIGNAL(clicked()), centralWidget, SLOT(findNextString()));
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS ///////////////////////////////////////////////////////////

int FindWidget::getFindOptions() {
    int findOptions = 0;
    if (caseSensitiveCBox->isChecked()) {
        findOptions |= QTextDocument::FindCaseSensitively;
    }
    if (wholeWordsCBox->isChecked()) {
        findOptions |= QTextDocument::FindWholeWords;
    }
    return findOptions;
}

QString FindWidget::getSearchString() {
    return editSearch->text();
}

void FindWidget::showAdvancedSearchOptions() {
    searchNextButton->setText(tr("Next"));
    wholeWordsCBox->show();
    searchPreviousButton->show();
    caseSensitiveCBox->show();;
}

void FindWidget::hideAdvancedSearchOptions() {
    searchNextButton->setText(tr("Find"));
    wholeWordsCBox->hide();
    searchPreviousButton->hide();
    caseSensitiveCBox->hide();
}

