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

#include <QApplication>
#include <QTextCodec>
#include <QIcon>
#include <QPixmap>
#include <QSplashScreen>
#include <QTimer>
#include <QLabel>

#include <QtGui>
#include <QtCore>

#include "mainwindow.h"
#include "saguiconstants.h"

class QApplication;
class QTextCodec;
class QIcon;
class QPixmap;
class QSplashScreen;
class QTimer;

class MainWindow;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/icon.ico"));

    QTranslator SATranslator;
    SATranslator.load(":/translation/rutranslation");

    QTranslator QT_Ru;
    QT_Ru.load(":/translation/qt_ru");

    QSettings* settings = new QSettings(CONFIG_FILE, QSettings::IniFormat, qApp);
    if(settings->value("language").toString()=="ru")
    {
        app.installTranslator(&SATranslator);
        app.installTranslator(&QT_Ru);
    }

    QTextCodec* textCodec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(textCodec);
    QTextCodec::setCodecForTr(textCodec);
    QTextCodec::setCodecForCStrings(textCodec);

    QSplashScreen splash(QPixmap(":/images/splashscreen.png"));
    splash.show();
    app.processEvents();
    QString splashMessage = "(c) SourceAnalyzer Team, 2008-2010\nVersion "
 + QSAGUIVERSION;

    splash.showMessage(splashMessage);
    MainWindow mw;

    QTimer::singleShot(TIME_FOR_SPLASH_SCREEN, &mw, SLOT(show()));
    QTimer::singleShot(TIME_FOR_SPLASH_SCREEN, &splash, SLOT(close()));

    return app.exec();
}
