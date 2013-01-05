////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    mainmidiwindow.cpp
///\ingroup dtedit
///\brief   Main window with MIDI class implementation.
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
#include "mainmidiwindow.h"
#include "setupdialog.h"
#include <QThread>

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::MainMIDIWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
MainMIDIWindow::MainMIDIWindow(QWidget *parent) :
  QMainWindow(parent),
  midiInName(""),
  midiOutName(""),
  midiOK(false)
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::~MainMIDIWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Choose! Choose the form of the Destructor!
////////////////////////////////////////////////////////////////////////////////
MainMIDIWindow::~MainMIDIWindow()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::openMIDIPorts()
////////////////////////////////////////////////////////////////////////////////
///\brief   Open the MIDI devices for input/output.
///\return  Returns true if successfull or false otherwise.
///\remarks Always closes the port priot trying to open them again.
////////////////////////////////////////////////////////////////////////////////
bool MainMIDIWindow::openMIDIPorts()
{
  // flag error:
  midiOK = false;

  // Close ports:
  midiIn.closePort();
  midiOut.closePort();

  // Check names:
  if (midiInName.isEmpty() || midiOutName.isEmpty())
    return false;

  // Find MIDI in port number:
  int inPortNo = -1;
  for (int i = 0; i < static_cast<int>(midiIn.getPortCount()); i++)
  {
    if (midiInName.compare(midiIn.getPortName(i).c_str()) == 0)
    {
      inPortNo = i;
      break;
    }
  }
  if (inPortNo < 0)
    return false;

  // Find MIDI out port number:
  int outPortNo = -1;
  for (int i = 0; i < static_cast<int>(midiOut.getPortCount()); i++)
  {
    if (midiOutName.compare(midiOut.getPortName(i).c_str()) == 0)
    {
      outPortNo = i;
      break;
    }
  }
  if (outPortNo < 0)
    return false;

  try
  {
    // Open MIDI in port:
    midiIn.setCallback(onMIDIMessageProxy, this);
    midiIn.openPort(inPortNo);
    midiIn.ignoreTypes(false, true, true);

    // Open MIDI out port:
    midiOut.openPort(outPortNo);

    // Set status:
    midiOK = true;

    // Return success:
    return true;
  }
  catch (...)
  {
    // Close ports:
    midiIn.closePort();
    midiOut.closePort();

    // Error return:
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::showSetupWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Show the configuration dialog.
///\return  Returns true if successfull or false otherwise.
///\remarks A false return value means that the user pressed cancel.
////////////////////////////////////////////////////////////////////////////////
bool MainMIDIWindow::showSetupWindow()
{
  // Create the setup dialog:
  SetupDialog dlg(this);

  // Set properties:
  dlg.setInputName(midiInName);
  dlg.setOutputName(midiOutName);

  // Swow the dialog:
  if (dlg.exec() == QDialog::Rejected)
    return false;

  // Store properties:
  midiInName  = dlg.getInputName();
  midiOutName = dlg.getOutputName();

  // Return success:
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::noteOnReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new note on message arrives.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::noteOnReceived(unsigned char /*channel*/, unsigned char /*noteNumber*/, unsigned char /*velocity*/)
{
  // Log message:
  //QString s;
  //s.sprintf("Note on received (note %d, channel %d, velocity %d)", noteNumber, channel + 1, velocity);
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::noteOffReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new note off message arrives.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::noteOffReceived(unsigned char /*channel*/, unsigned char /*noteNumber*/, unsigned char /*velocity*/)
{
  // Log message:
  //QString s;
  //s.sprintf("Note off received (note %d, channel %d, velocity %d)", noteNumber, channel + 1, velocity);
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::controlChangeReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new control change message arrives.
///\param   [in] channel:       MIDI channel of this message.
///\param   [in] controlNumber: Controller number.
///\param   [in] value:         Control value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::controlChangeReceived(unsigned char /*channel*/, unsigned char /*controlNumber*/, unsigned char /*value*/)
{
  // Log message:
  //QString s;
  //s.sprintf("Control change received (number %d, channel %d, value %d)", controlNumber, channel + 1, value);
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::programChangeReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new program change message arrives.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Program number.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::programChangeReceived(unsigned char /*channel*/, unsigned char /*value*/)
{
  // Log message:
  //QString s;
  //s.sprintf("Program change received (channel %d, value %d)", channel + 1, value);
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::channelAftertouchReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new channel aftertouch message arrives.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::channelAftertouchReceived(unsigned char /*channel*/, unsigned char /*value*/)
{
  // Log message:
  //QString s;
  //s.sprintf("Channel aftertouch received (channel %d, value %d)", channel + 1, value);
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::pitchBendReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new pitch bend message arrives.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pitch bend value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::pitchBendReceived(unsigned char /*channel*/, unsigned short /*value*/)
{
  // Log message:
  //QString s;
  //s.sprintf("Pitch bend received (channel %d, value %d)", channel + 1, value);
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::polyAftertouchReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new poly pressure message arrives.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] value:      Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::polyAftertouchReceived(unsigned char /*channel*/, unsigned char /*noteNumber*/, unsigned char /*value*/)
{
  // Log message:
  //QString s;
  //s.sprintf("Polyphonic aftertouch received (note %d, channel %d, value %d)", noteNumber, channel + 1, value);
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sysExReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new SysEx message arrives.
///\param   [in] buff: The message buffer.
///\param   [in] value:      Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sysExReceived(const std::vector<unsigned char>& /*buff*/)
{
  // Log message:
  //QString s;
  //s.sprintf("SysEx received (size %d bytes)", static_cast<int>(buff.size()));
  //qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sendNoteOn()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a note on message.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sendNoteOn(unsigned char channel, unsigned char noteNumber, unsigned char velocity)
{
  // Environment check:
  if (!midiOK)
    return;

  // Fill buffer with MIDI values:
  std::vector<unsigned char> buff(3);
  buff[0] = 0x90 | (channel & 0x0F);
  buff[1] = noteNumber & 0x7F;
  buff[2] = velocity & 0x7F;

  // Send the message:
  midiOut.sendMessage(&buff);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sendNoteOff()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a note off message.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sendNoteOff(unsigned char channel, unsigned char noteNumber, unsigned char velocity)
{
  // Environment check:
  if (!midiOK)
    return;

  // Fill buffer with MIDI values:
  std::vector<unsigned char> buff(3);
  buff[0] = 0x80 | (channel & 0x0F);
  buff[1] = noteNumber & 0x7F;
  buff[2] = velocity & 0x7F;

  // Send the message:
  midiOut.sendMessage(&buff);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sendControlChange()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a control change message.
///\param   [in] channel:       MIDI channel of this message.
///\param   [in] controlNumber: Control number.
///\param   [in] value:         Control value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sendControlChange(unsigned char channel, unsigned char controlNumber, unsigned char value)
{
  // Environment check:
  if (!midiOK)
    return;

  // Fill buffer with MIDI values:
  std::vector<unsigned char> buff(3);
  buff[0] = 0xB0 | (channel & 0x0F);
  buff[1] = controlNumber & 0x7F;
  buff[2] = value & 0x7F;

  // Send the message:
  midiOut.sendMessage(&buff);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sendProgramChange()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a program change message.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Program number.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sendProgramChange(unsigned char channel, unsigned char value)
{
  // Environment check:
  if (!midiOK)
    return;

  // Fill buffer with MIDI values:
  std::vector<unsigned char> buff(2);
  buff[0] = 0xC0 | (channel & 0x0F);
  buff[1] = value & 0x7F;

  // Send the message:
  midiOut.sendMessage(&buff);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sendChannelAftertouch()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a channel aftertouch message.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sendChannelAftertouch(unsigned char channel, unsigned char value)
{
  // Environment check:
  if (!midiOK)
    return;

  // Fill buffer with MIDI values:
  std::vector<unsigned char> buff(2);
  buff[0] = 0xD0 | (channel & 0x0F);
  buff[1] = value & 0x7F;

  // Send the message:
  midiOut.sendMessage(&buff);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sendPitchBend()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a pitch bend message.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pitch bend value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sendPitchBend(unsigned char channel, unsigned short value)
{
  // Environment check:
  if (!midiOK)
    return;

  // Fill buffer with MIDI values:
  std::vector<unsigned char> buff(3);
  buff[0] = 0xE0 | (channel & 0x0F);
  buff[1] = value & 0x7F;
  buff[2] = (value >> 7) & 0x7F;

  // Send the message:
  midiOut.sendMessage(&buff);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::sendPolyAftertouch()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a poly aftertouch message.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] value:      Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::sendPolyAftertouch(unsigned char channel, unsigned char noteNumber, unsigned char value)
{
  // Environment check:
  if (!midiOK)
    return;

  // Fill buffer with MIDI values:
  std::vector<unsigned char> buff(3);
  buff[0] = 0xA0 | (channel & 0x0F);
  buff[1] = noteNumber & 0x7F;
  buff[2] = value & 0x7F;

  // Send the message:
  midiOut.sendMessage(&buff);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::onMIDIMessage()
////////////////////////////////////////////////////////////////////////////////
///\brief   Callback for incoming MIDI messages.
///\param   [in] timeStamp: Time stamp of the message.
///\param   [in] message:   The raw MIDI message as byte buffer.
///\remarks Updates the surface depending on the MIDI data.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::onMIDIMessage(const double /* timeStamp */, const std::vector<unsigned char>& message)
{
  // Get status:
  unsigned char status  = message.at(0) & 0xF0;
  unsigned char channel = message.at(0) & 0x0F;

  // Note off message?
  if (status == 0x80 || (status ==0x90 && message.at(2) == 0x00))
    noteOffReceived(message.at(0) & 0x0F, message.at(1), message.at(2));

  // Note on message?
  else if (status == 0x90)
    noteOnReceived(channel, message.at(1), message.at(2));

  // Polyphonic aftertouch?
  else if (status == 0xA0)
    polyAftertouchReceived(channel, message.at(1), message.at(2));

  // Control change message?
  else if (status == 0xB0)
    controlChangeReceived(channel, message.at(1), message.at(2));

  // Program change?
  else if (status == 0xC0)
    programChangeReceived(channel, message.at(1));

  // Channel aftertouch?
  else if (status == 0xD0)
    channelAftertouchReceived(channel, message.at(1));

  // Pitch bend?
  else if (status == 0xE0)
    pitchBendReceived(channel, (message.at(1) & 0x7F) | ((message.at(2) & 0x7F) << 7));

  // SysEx?
  else if (status == 0xF0)
    return;
}

////////////////////////////////////////////////////////////////////////////////
///\class tmpSleep
///\brief Helper class to unprotect the sleep function of QThread.
////////////////////////////////////////////////////////////////////////////////
class tmpSleep :
  public QThread
{
public:

  //////////////////////////////////////////////////////////////////////////////
  // tmpSleep::do_sleep()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Helper function to make the current thread sleep a while.
  ///\param   [in] milliSeconds: Number of milliseconds to sleep.
  //////////////////////////////////////////////////////////////////////////////
  static void do_sleep(int ms)
  {
    // Delegate to QThread:
    QThread::msleep(ms);
  }
};

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::Sleep()
////////////////////////////////////////////////////////////////////////////////
///\brief   Helper function to make the current thread sleep a while.
///\param   [in] milliSeconds: Number of milliseconds to sleep.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::Sleep(int milliSeconds)
{
  // Delegate to QThread:
  tmpSleep::do_sleep(milliSeconds);
}

////////////////////////////////////////////////////////////////////////////////
// MainMIDIWindow::onMIDIMessageProxy()
////////////////////////////////////////////////////////////////////////////////
///\brief   Callback for incoming MIDI messages.
///\param   [in] timeStamp: Time stamp of the message.
///\param   [in] message:   The raw MIDI message as byte buffer.
///\param   [in] userData:  User data set when the port was created.
///\remarks The userData holds a pointer to this class so this function
///         delegates the call to the member function of the class.
////////////////////////////////////////////////////////////////////////////////
void MainMIDIWindow::onMIDIMessageProxy(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
  // Delegate to the class function:
  static_cast<MainMIDIWindow*>(userData)->onMIDIMessage(timeStamp, *message);
}

///////////////////////////////// End of File //////////////////////////////////
