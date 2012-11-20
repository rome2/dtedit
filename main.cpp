////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    main.cpp
///\ingroup dtedit
///\brief   Main application entry point.
///\author  Rolf Meyerhoff (rm@matrix44.de)
///\version 1.0
/// This file is part of the DT editor.
////////////////////////////////////////////////////////////////////////////////
///\par License:
/// This program is free software: you can redistribute it and/or modify it
/// under the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 2 of the License, or (at your option)
/// any later version.
///\par
/// This program is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even  the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///\par
/// You should have received a copy of the GNU General Public License along with
/// this program; see the file COPYING. If not, see http://www.gnu.org/licenses/
/// or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
/// Floor, Boston, MA 02110-1301, USA.
////////////////////////////////////////////////////////////////////////////////
#include <QtGui/QApplication>
#include "dtedit.h"
#include "mainmidiwindow.h"
#include "mainwindow.h"

////////////////////////////////////////////////////////////////////////////////
// main()
////////////////////////////////////////////////////////////////////////////////
///\brief   Main application entry point.
///\param   [in] argc: Number of command line arguments passed to this program.
///\param   [in] argv: Array of command line arguments.
///\return  Returns zero if successfull or an error code on failure.
///\remarks The first argument holds the path to the executable.
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
  // Init the global application object:
  QApplication a(argc, argv);

  // Force plastique style:
  a.setStyle(new QPlastiqueStyle());

  // Set programm properties, used by QSettings and others:
  a.setApplicationName   (QString("DTEdit"));
  a.setApplicationVersion(QString("1.0"));
  a.setOrganizationName  (QString("Rolf Meyerhoff"));
  a.setOrganizationDomain(QString("dtedit.googlecode.com"));

  // Create and show the main application window:
  MainWindow w;
  w.show();

  // Run the application:
  int ret = a.exec();

  // Return to sender:
  return ret;
}

///////////////////////////////// End of File //////////////////////////////////
