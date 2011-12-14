#ifndef __QSAGUICONSTANTS_H__
#define __QSAGUICONSTANTS_H__

#include <QDir>

class QDir;

    const QString QSAGUIVERSION            = "2.0.beta";
    const int TIME_FOR_SPLASH_SCREEN       = 1100;

//Parameters of configuration file
    const QString VERSION_OF_CONFIG_FILE   = "1.3";
    const QString PATH_TO_SETTINGS         = QDir::homePath() +
                                             QDir::separator() + ".qsagui";
    const QString CONFIG_FILE              = PATH_TO_SETTINGS +
                                             QDir::separator() + "config.ini";
//Saving of last session
    const QString COMMON_GRAPH             = "commongraph.cg";
    const QString SAVED_LIST_OF_FUNCTIONS  = "functions.txt";
    const QString SAVED_LIST_OF_FILES      = "files.txt";
    const QString AREA_CONTENT             = "area_content";
    const QString SAVED_SHELL_OUTPUT_TEXT  = "shellOutput.txt";
    const QString DELIMITER_FOR_ITEMS      = "#";
    const QString SAVED_REQUESTS           = "_requests.txt";

//Binaries info
    const QString CG2TEXT                  = "sacg2text";
    const QString CHILDREN                 = "sachildren";
    const QString LEVCHILDREN              = "salevchildren";
    const QString PARENTS                  = "saparents";
    const QString LEVPARENTS               = "salevparents";
    const QString CLEAN                    = "saclean";
    const QString CPARSE                   = "sacparse";
    const QString ASMPARSE                 = "saasparse";
    const QString FINDFUNC                 = "safindfunc";
    const QString LINK                     = "salink";
    const QString UPDATE                   = "saupdate";
    const QString BINARIES[] = { CG2TEXT,
                                 CHILDREN,
                                 LEVCHILDREN,
                                 PARENTS,
                                 LEVPARENTS,
                                 CLEAN,
                                 CPARSE,
                                 ASMPARSE,
                                 FINDFUNC,
                                 LINK,
                                 UPDATE
                               };
    const int COUNT_OF_SA_BINARIES = 11;
    const QString CG2DOT        = "sacg2dot";
    const QString F90PARSE      = "saf90parse";
    const QString DOT           = "dot";

//Binaries types
    const QString SA_TYPE                  = "sa";
    const QString DOT_TYPE                 = "dot";
    const QString PREPROC_TYPE             = "preproc";

#endif // __QSAGUICONSTANTS_H__
