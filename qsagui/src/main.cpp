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
