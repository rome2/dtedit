////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    aboutdialog.h
///\ingroup dtedit
///\brief   About window class definition.
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
#ifndef __ABOUTDIALOG_H_INCLUDED__
#define __ABOUTDIALOG_H_INCLUDED__

#include <QDialog>

////////////////////////////////////////////////////////////////////////////////
// Forwards:
namespace Ui { class AboutDialog; }

////////////////////////////////////////////////////////////////////////////////
///\class AboutDialog aboutdialog.h
///\brief About dialog class.
/// This is the about box of the application.
////////////////////////////////////////////////////////////////////////////////
class AboutDialog : public QDialog
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // AboutDialog::AboutDialog()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this window.
  ///\param   [in] parent: Parent window for this window.
  ///\remarks Basically initializes the entire gui.
  //////////////////////////////////////////////////////////////////////////////
  explicit AboutDialog(QWidget *parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // AboutDialog::~AboutDialog()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Does the cleanup.
  //////////////////////////////////////////////////////////////////////////////
  ~AboutDialog();

private:
  //////////////////////////////////////////////////////////////////////////////
  // Member:
  Ui::AboutDialog *ui; ///> Design class.
};

#endif // __ABOUTDIALOG_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
