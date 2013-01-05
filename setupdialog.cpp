////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    mainwindow.cpp
///\ingroup dtedit
///\brief   Main window class implementation.
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
#include "setupdialog.h"
#include "ui_setupdialog.h"
#include "dtedit.h"

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::SetupDialog()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
SetupDialog::SetupDialog(QWidget* parent) :
  QDialog(parent),
  ui(new Ui::SetupDialog),
  inputName(""),
  outputName(""),
  blocked(false)
{
  // Init user interface:
  ui->setupUi(this);
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::~SetupDialog()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Does the cleanup.
////////////////////////////////////////////////////////////////////////////////
SetupDialog::~SetupDialog()
{
  // Delete user interface:
  delete ui;
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::setInputName()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the InputName property.
///\param   [in] name: The new name of the input.
////////////////////////////////////////////////////////////////////////////////
void SetupDialog::setInputName(const QString& name)
{
  // Store name:
  inputName = name;
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::getInputName()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the InputName property.
///\return  The current name of the input.
////////////////////////////////////////////////////////////////////////////////
const QString& SetupDialog::getInputName() const
{
  // Return the name:
  return inputName;
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::setOutputName()
////////////////////////////////////////////////////////////////////////////////
///\brief   Set accessor for the OutputName property.
///\param   [in] name: The new name of the output.
////////////////////////////////////////////////////////////////////////////////
void SetupDialog::setOutputName(const QString& name)
{
  // Store name:
  outputName = name;
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::getOutputName()
////////////////////////////////////////////////////////////////////////////////
///\brief   Get accessor for the OutputName property.
///\return  The current name of the output.
////////////////////////////////////////////////////////////////////////////////
const QString& SetupDialog::getOutputName() const
{
  // Return the name:
  return outputName;
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::showEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the window show event.
///\param   [in] e: Description of the event.
////////////////////////////////////////////////////////////////////////////////
void SetupDialog::showEvent(QShowEvent* /*e*/)
{
  // Lock UI:
  blocked = true;

  // Get MIDI in properties:
  RtMidiIn midiIn;
  unsigned int portCnt = midiIn.getPortCount();
  if (portCnt > 0)
  {
    int selItem = -1;

    // Loop through MIDI ports:
    for (unsigned int i = 0; i < portCnt; i++)
    {
      // Get name of the port:
      QString name(midiIn.getPortName(i).c_str());

      // Does this match the currently selected option?
      if (name == inputName)
        selItem = i;

      // Add item to the combo box:
      ui->inputComboBox->addItem(name);
    }

    // Select current item, if any:
    ui->inputComboBox->setCurrentIndex(selItem);
  }

  // Get MIDI out properties:
  RtMidiOut midiOut;
  portCnt = midiOut.getPortCount();
  if (portCnt > 0)
  {
    int selItem = -1;

    // Loop through MIDI ports:
    for (unsigned int i = 0; i < portCnt; i++)
    {
      // Get name of the port:
      QString name(midiOut.getPortName(i).c_str());

      // Does this match the currently selected option?
      if (name == outputName)
        selItem = i;

      // Add item to the combo box:
      ui->outputComboBox->addItem(name);
    }

    // Select current item, if any:
    ui->outputComboBox->setCurrentIndex(selItem);
  }

  // Unlock UI:
  blocked = false;
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::on_inputComboBox_currentIndexChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the input combo box selection changed signal.
///\remarks Updates the inputName member.
////////////////////////////////////////////////////////////////////////////////
void SetupDialog::on_inputComboBox_currentIndexChanged(const QString& arg1)
{
  // Update allowed?
  if (blocked)
    return;

  // Save name:
  inputName = arg1;
}

////////////////////////////////////////////////////////////////////////////////
// SetupDialog::on_outputComboBox_currentIndexChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the output combo box selection changed signal.
///\remarks Updates the outputName member.
////////////////////////////////////////////////////////////////////////////////
void SetupDialog::on_outputComboBox_currentIndexChanged(const QString& arg1)
{
  // Update allowed?
  if (blocked)
    return;

  // Save name:
  outputName = arg1;
}

///////////////////////////////// End of File //////////////////////////////////
