###############################################################################
## This file is the part of QSAGUI source codes.
## QSAGUI is a graphical user interface for Source Analyzer.
## QSAGUI source codes are distributed with Source Analyzer source codes.
## 
## Source Analyzer is a program that search out a call-graph of
## given source code. See <http://trac-hg.assembla.com/SourceAnalyzer>
## Copyright (C) 2008-2010 BiPro Lab team
## 
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License,
## any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

cmake_minimum_required( VERSION 2.6 )
project( qsagui )

find_package(Qt4 REQUIRED)
set(QT_USE_QTSVG true)
include( ${QT_USE_FILE} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

###############################################################################
## Variables set ##############################################################
set( docs_dir    ${qsagui_SOURCE_DIR}/docs )
set( include_dir ${qsagui_SOURCE_DIR}/include  )
set( src_dir     ${qsagui_SOURCE_DIR}/src  )
set( forms_dir   ${qsagui_SOURCE_DIR}/forms  )
set( res_dir ${qsagui_SOURCE_DIR}/resources )
set( lib_dir ${qsagui_SOURCE_DIR}/lib )

set( SOURCES
${src_dir}/main.cpp
${src_dir}/mainwindow.cpp
${src_dir}/centralwidget.cpp
${src_dir}/settingswidget.cpp
${src_dir}/newcallgraphwidget.cpp
${src_dir}/listwidget.cpp
${src_dir}/aboutwidget.cpp
${src_dir}/helpwidget.cpp
${src_dir}/linkcallgraphswidget.cpp
${src_dir}/cleancallgraphwidget.cpp
${src_dir}/updatecallgraphwidget.cpp
${src_dir}/svgviewer.cpp
${src_dir}/rhwidget.cpp
)

set( MOC_HEADERS
${include_dir}/mainwindow.h
${include_dir}/centralwidget.h
${include_dir}/settingswidget.h
${include_dir}/newcallgraphwidget.h
${include_dir}/listwidget.h
${include_dir}/saguiconstants.h
${include_dir}/aboutwidget.h
${include_dir}/helpwidget.h
${include_dir}/linkcallgraphswidget.h
${include_dir}/cleancallgraphwidget.h
${include_dir}/updatecallgraphwidget.h
${include_dir}/svgviewer.h
${include_dir}/rhwidget.h
)

set( FORMS
${forms_dir}/aboutwidget.ui
${forms_dir}/addnamedialog.ui
${forms_dir}/cleancallgraphwidget.ui
${forms_dir}/helpwidget.ui
${forms_dir}/linkcallgraphswidget.ui
${forms_dir}/listwidget.ui
${forms_dir}/newcallgraphwidget.ui
${forms_dir}/settingswidget.ui
${forms_dir}/updatecallgraphwidget.ui
${forms_dir}/findwidget.ui
${forms_dir}/rhwidget.ui
)

qt4_wrap_ui( UI_HEADERS ${FORMS} )

set( RESOURCES
${res_dir}/resources.qrc
)
###############################################################################

###############################################################################
## Setting the application icon ##############################################################
if( WIN32 )
    if( MINGW )
        add_custom_command( OUTPUT ${CMAKE_BINARY_DIR}/icon_rc.o
        COMMAND windres.exe -I${qsagui_SOURCE_DIR}
        -i${res_dir}/icon.rc
        -o ${CMAKE_BINARY_DIR}/icon_rc.o
	    )
        set( SOURCES ${SOURCES} ${CMAKE_BINARY_DIR}/icon_rc.o )
    else( MINGW )
        set( SOURCES ${SOURCES} ${res_dir}/icon.rc )
    endif( MINGW )
endif( WIN32 )
###############################################################################

qt4_add_resources( QRC_SOURCES ${RESOURCES} )
qt4_wrap_cpp( MOC_SRCS ${MOC_HEADERS} )

###############################################################################
## Path to include ############################################################
include_directories( ${include_dir} )
###############################################################################

add_executable( qsagui WIN32 ${SOURCES} ${MOC_SRCS} ${QRC_SOURCES} ${UI_HEADERS} )
target_link_libraries( qsagui ${QT_LIBRARIES} )

###############################################################################
## Installation packages content ##############################################
install(
    TARGETS
        qsagui
    RUNTIME DESTINATION bin
)

install(
    FILES
        ${docs_dir}/license
        ${docs_dir}/qsagui_usermanual_0.6.pdf
    DESTINATION qsaguidocs
)

if( WIN32 )
    Find_File(qtgui QtGui4.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
    Find_File(qtcore QtCore4.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
	Find_File(qtsvg QtSvg4.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
    Find_File(mingwdll mingwm10.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
	Find_File(libgcc libgcc_s_dw2-1.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH )
    install(
        FILES
		    ${res_dir}/images/icon.ico
            ${qtgui}
			${qtcore}
			${qtsvg}
			${mingwdll}
			${libgcc}
        DESTINATION bin
    )
endif( WIN32 )

###############################################################################
## Installation package settings ##############################################
if( WIN32 )
    set( CPACK_GENERATOR "TGZ;NSIS" )
    set( CPACK_NSIS_COMPRESSOR "/SOLID lzma" )
    set( CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
         createShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\qsagui.lnk\\\" \\\"$INSTDIR\\\\bin\\\\qsagui.exe\\\"
		 CreateShortCut \\\"$DESKTOP\\\\qsagui.lnk\\\" \\\"$INSTDIR\\\\bin\\\\qsagui.exe\\\" \\\"\\\" \\\"$INSTDIR\\\\bin\\\\icon.ico\\\"
")
    set( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
         delete \\\"$SMPROGRAMS\\\\QSAGUI\\\\qsagui.lnk\\\"
		 delete \\\"$DESKTOP\\\\qsagui.lnk\\\"
         RMDir /r \\\"$PROGRAMFILES\\\\QSAGUI\\\"
         RMDir /r \\\"$PROFILE\\\\.qsagui\\\"
		 RMDir /r \\\"$TEMP\\\\qsaguitmpdir\\\"
")
endif(WIN32)

if(UNIX)
    set( CPACK_GENERATOR "TGZ;STGZ;DEB;RPM" )
endif(UNIX)

set(CMAKE_BUILD_TYPE "Release")
set(CPACK_PACKAGE_CONTACT "SourceAnalyzer contributors <sa_team@googlegroups.com>")
set(CPACK_PACKAGE_NAME "qsagui")
set(CPACK_PACKAGE_VENDOR "SourceAnalyzer contributors")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "QSAGUI - graphical user interface for SourceAnalyzer")
set(CPACK_PACKAGE_VERSION "1.1.beta")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "qsagui")
set(CPACK_RESOURCE_FILE_LICENSE "${docs_dir}/licensenote")

include(CPack)
###############################################################################
