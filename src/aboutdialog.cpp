////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    aboutdialog.cpp
///\ingroup dtedit
///\brief   About window class implementation.
///\author  Rolf Meyerhoff (badlantic@gmail.com)
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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

////////////////////////////////////////////////////////////////////////////////
// AboutDialog::AboutDialog()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
AboutDialog::AboutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AboutDialog)
{
  // Init user interface:
  ui->setupUi(this);
}

////////////////////////////////////////////////////////////////////////////////
// AboutDialog::~AboutDialog()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Does the cleanup.
////////////////////////////////////////////////////////////////////////////////
AboutDialog::~AboutDialog()
{
  // Delete user interface:
  delete ui;
}

///////////////////////////////// End of File //////////////////////////////////
