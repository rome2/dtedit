////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    mainwindow.h
///\ingroup dtedit
///\brief   Main window class definition.
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
#ifndef __MAINWINDOW_H_INCLUDED__
#define __MAINWINDOW_H_INCLUDED__

#include <QtGui>
#include "dtedit.h"

////////////////////////////////////////////////////////////////////////////////
///\class MainWindow mainwindow.h
///\brief Main window class.
/// This is the main widget of the application.
////////////////////////////////////////////////////////////////////////////////
class MainWindow :
  public QMainWindow
{
  Q_OBJECT // Qt magic...

public:
  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::MainWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Initialization constructor of this window.
  ///\param   [in] parent: Parent window for this window.
  ///\remarks Basically initializes the entire gui.
  //////////////////////////////////////////////////////////////////////////////
  MainWindow(QWidget* parent = 0);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::~MainWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Choose! Choose the form of the Destructor!
  //////////////////////////////////////////////////////////////////////////////
  ~MainWindow();

protected:
  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::closeEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Message handler for the window close event.
  ///\param   [in] e: Description of the event.
  ///\remarks To allow the window to close, call e->accept().
  //////////////////////////////////////////////////////////////////////////////
  void closeEvent(QCloseEvent* e);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::showEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Message handler for the window show event.
  ///\param   [in] e: Description of the event.
  //////////////////////////////////////////////////////////////////////////////
  void showEvent(QShowEvent* e);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::noteOnReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new note on message arrives.
  ///\param   [in] channel:    MIDI channel of this message.
  ///\param   [in] noteNumber: Note number.
  ///\param   [in] velocity:   Note velocity.
  //////////////////////////////////////////////////////////////////////////////
  void noteOnReceived(unsigned char channel, unsigned char noteNumber, unsigned char velocity);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::noteOffReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new note off message arrives.
  ///\param   [in] channel:    MIDI channel of this message.
  ///\param   [in] noteNumber: Note number.
  ///\param   [in] velocity:   Note velocity.
  //////////////////////////////////////////////////////////////////////////////
  void noteOffReceived(unsigned char channel, unsigned char noteNumber, unsigned char velocity);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::controlChangeReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new control change message arrives.
  ///\param   [in] channel:       MIDI channel of this message.
  ///\param   [in] controlNumber: Controller number.
  ///\param   [in] value:         Control value.
  //////////////////////////////////////////////////////////////////////////////
  void controlChangeReceived(unsigned char channel, unsigned char controlNumber, unsigned char value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::programChangeReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new program change message arrives.
  ///\param   [in] channel: MIDI channel of this message.
  ///\param   [in] value:   Program number.
  //////////////////////////////////////////////////////////////////////////////
  void programChangeReceived(unsigned char channel, unsigned char value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::channelAftertouchReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new channel aftertouch message arrives.
  ///\param   [in] channel: MIDI channel of this message.
  ///\param   [in] value:   Pressure value.
  //////////////////////////////////////////////////////////////////////////////
  void channelAftertouchReceived(unsigned char channel, unsigned char value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::pitchBendReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new pitch bend message arrives.
  ///\param   [in] channel: MIDI channel of this message.
  ///\param   [in] value:   Pitch bend value.
  //////////////////////////////////////////////////////////////////////////////
  void pitchBendReceived(unsigned char channel, unsigned short value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::polyAftertouchReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new poly pressure message arrives.
  ///\param   [in] channel:    MIDI channel of this message.
  ///\param   [in] noteNumber: Note number.
  ///\param   [in] value:      Pressure value.
  //////////////////////////////////////////////////////////////////////////////
  void polyAftertouchReceived(unsigned char channel, unsigned char noteNumber, unsigned char value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sysExReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new SysEx message arrives.
  ///\param   [in] buff: The message buffer.
  ///\param   [in] value:      Pressure value.
  //////////////////////////////////////////////////////////////////////////////
  void sysExReceived(const std::vector<unsigned char>& buff);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendNoteOn()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send a note on message.
  ///\param   [in] channel:    MIDI channel of this message.
  ///\param   [in] noteNumber: Note number.
  ///\param   [in] velocity:   Note velocity.
  //////////////////////////////////////////////////////////////////////////////
  void sendNoteOn(unsigned char channel, unsigned char noteNumber, unsigned char velocity);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendNoteOff()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send a note off message.
  ///\param   [in] channel:    MIDI channel of this message.
  ///\param   [in] noteNumber: Note number.
  ///\param   [in] velocity:   Note velocity.
  //////////////////////////////////////////////////////////////////////////////
  void sendNoteOff(unsigned char channel, unsigned char noteNumber, unsigned char velocity);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendControlChange()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send a control change message.
  ///\param   [in] channel:       MIDI channel of this message.
  ///\param   [in] controlNumber: Control number.
  ///\param   [in] value:         Control value.
  //////////////////////////////////////////////////////////////////////////////
  void sendControlChange(unsigned char channel, unsigned char controlNumber, unsigned char value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendProgramChange()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send a program change message.
  ///\param   [in] channel: MIDI channel of this message.
  ///\param   [in] value:   Program number.
  //////////////////////////////////////////////////////////////////////////////
  void sendProgramChange(unsigned char channel, unsigned char value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendChannelAftertouch()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send a channel aftertouch message.
  ///\param   [in] channel: MIDI channel of this message.
  ///\param   [in] value:   Pressure value.
  //////////////////////////////////////////////////////////////////////////////
  void sendChannelAftertouch(unsigned char channel, unsigned char value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendPitchBend()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send a pitch bend message.
  ///\param   [in] channel: MIDI channel of this message.
  ///\param   [in] value:   Pitch bend value.
  //////////////////////////////////////////////////////////////////////////////
  void sendPitchBend(unsigned char channel, unsigned short value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendPolyAftertouch()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send a poly aftertouch message.
  ///\param   [in] channel:    MIDI channel of this message.
  ///\param   [in] noteNumber: Note number.
  ///\param   [in] value:      Pressure value.
  //////////////////////////////////////////////////////////////////////////////
  void sendPolyAftertouch(unsigned char channel, unsigned char noteNumber, unsigned char value);

private:

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::getValuesFromDT()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Sync UI with the values from the actual DT.
  ///\remarks This functions sends value request CCs to the DT. The UI is then
  ///         updated by the CC receive function.
  //////////////////////////////////////////////////////////////////////////////
  void getValuesFromDT();

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sendBlockMessage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Send ui block CCs to the DT.
  ///\param   [in] block: Block the UI?
  ///\remarks The DT just reflects everything that it receives at the input to
  ///         the output. So every clean to the DT is guarded by an unsed CC
  ///         with a boolean flag to either block or unblock the UI to avoid
  ///         MIDI feedback loops.
  //////////////////////////////////////////////////////////////////////////////
  void sendBlockMessage(bool block);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::openMIDIPorts()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Open the MIDI devices for input/output.
  ///\return  Returns true if successfull or false otherwise.
  ///\remarks Always closes the port priot trying to open them again.
  //////////////////////////////////////////////////////////////////////////////
  bool openMIDIPorts();

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::showSetupWindow()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Show the configuration dialog.
  ///\return  Returns true if successfull or false otherwise.
  ///\remarks A false return value means that the user pressed cancel.
  //////////////////////////////////////////////////////////////////////////////
  bool showSetupWindow();

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::onMIDIMessage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Callback for incoming MIDI messages.
  ///\param   [in] timeStamp: Time stamp of the message.
  ///\param   [in] message:   The raw MIDI message as byte buffer.
  ///\remarks Updates the surface depending on the MIDI data.
  //////////////////////////////////////////////////////////////////////////////
  void onMIDIMessage(const double timeStamp, const std::vector<unsigned char>& message);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::onMIDIMessageProxy()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Callback for incoming MIDI messages.
  ///\param   [in] timeStamp: Time stamp of the message.
  ///\param   [in] message:   The raw MIDI message as byte buffer.
  ///\param   [in] userData:  User data set when the port was created.
  ///\remarks The userData holds a pointer to this class so this function
  ///         delegates the call to the member function of the class.
  //////////////////////////////////////////////////////////////////////////////
  static void onMIDIMessageProxy(double timeStamp, std::vector<unsigned char>* message, void* userData);

private slots:
  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::exitApplication()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the Exit-Application signal.
  ///\remarks Closes the window and quits the application.
  //////////////////////////////////////////////////////////////////////////////
  void exitApplication();

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::about()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the Help->About signal.
  ///\remarks Shows an about box with informations about this application.
  //////////////////////////////////////////////////////////////////////////////
  void about();

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::setupMIDI()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the File->Setup signal.
  ///\remarks Shows the MIDI setup dialog.
  //////////////////////////////////////////////////////////////////////////////
  void setupMIDI();

  void ampAChanged(int value);
  void cabAChanged(int value);
  void gainAChanged(int value);
  void bassAChanged(int value);
  void middleAChanged(int value);
  void trebleAChanged(int value);
  void presenceAChanged(int value);
  void volumeAChanged(int value);
  void voiceA1toggled(bool checked);
  void voiceA2toggled(bool checked);
  void voiceA3toggled(bool checked);
  void voiceA4toggled(bool checked);
  void reverbBypassAChanged(int value);
  void reverbAChanged(int value);
  void reverbDecayAChanged(int value);
  void reverbPredelayAChanged(int value);
  void reverbToneAChanged(int value);
  void reverbMixAChanged(int value);
  void classAChanged(int value);
  void xtodeAChanged(int value);
  void boostAChanged(int value);
  void pivoltAChanged(int value);
  void capAChanged(int value);
  void topolA1toggled(bool checked);
  void topolA2toggled(bool checked);
  void topolA3toggled(bool checked);
  void topolA4toggled(bool checked);
  void ampBChanged(int value);
  void cabBChanged(int value);
  void gainBChanged(int value);
  void bassBChanged(int value);
  void middleBChanged(int value);
  void trebleBChanged(int value);
  void presenceBChanged(int value);
  void volumeBChanged(int value);
  void voiceB1toggled(bool checked);
  void voiceB2toggled(bool checked);
  void voiceB3toggled(bool checked);
  void voiceB4toggled(bool checked);
  void reverbBypassBChanged(int value);
  void reverbBChanged(int value);
  void reverbDecayBChanged(int value);
  void reverbPredelayBChanged(int value);
  void reverbToneBChanged(int value);
  void reverbMixBChanged(int value);
  void classBChanged(int value);
  void xtodeBChanged(int value);
  void boostBChanged(int value);
  void pivoltBChanged(int value);
  void capBChanged(int value);
  void topolB1toggled(bool checked);
  void topolB2toggled(bool checked);
  void topolB3toggled(bool checked);
  void topolB4toggled(bool checked);
  void micChanged(int value);
  void lowVolChanged(int value);
  void channelChanged(int value);
  void masterChanged(int value);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QComboBox*    ampA;            ///> Channel A amp model selector.
  QComboBox*    cabA;            ///> Channel A cabinet selector.
  QDial*        gainA;           ///> Channel A gain dial.
  QDial*        bassA;           ///> Channel A bass dial.
  QDial*        middleA;         ///> Channel A middle dial.
  QDial*        trebleA;         ///> Channel A treble dial.
  QDial*        presenceA;       ///> Channel A presence dial.
  QDial*        volumeA;         ///> Channel A volume dial.
  QCheckBox*    reverbBypassA;   ///> Reverb A enabled switch.
  QComboBox*    reverbA;         ///> Reverb A type selector.
  QDial*        reverbDecayA;    ///> Reverb A decay dial.
  QDial*        reverbPredelayA; ///> Reverb A pre delay dial.
  QDial*        reverbToneA;     ///> Reverb A tone dial.
  QDial*        reverbMixA;      ///> Reverb A mix dial.
  QRadioButton* voiceA1;         ///> Channel A voice I select.
  QRadioButton* voiceA2;         ///> Channel A voice II select.
  QRadioButton* voiceA3;         ///> Channel A voice III select.
  QRadioButton* voiceA4;         ///> Channel A voice IV select.
  QSlider*      classA;          ///> Channel A, Class A/B switch.
  QSlider*      xtodeA;          ///> Channel A Pentode / triode switch.
  QRadioButton* topolA1;         ///> Channel A topology I select.
  QRadioButton* topolA2;         ///> Channel A topology II select.
  QRadioButton* topolA3;         ///> Channel A topology III select.
  QRadioButton* topolA4;         ///> Channel A topology IV select.
  QSlider*      boostA;          ///> Channel A tube boost on/off.
  QSlider*      pivoltA;         ///> Channel A phase inverter switch.
  QSlider*      capA;            ///> Channel A cap X, cap Y switch.
  QComboBox*    ampB;            ///> Channel B amp model selector.
  QComboBox*    cabB;            ///> Channel B cabinet selector.
  QDial*        gainB;           ///> Channel B gain dial.
  QDial*        bassB;           ///> Channel B bass dial.
  QDial*        middleB;         ///> Channel B middle dial.
  QDial*        trebleB;         ///> Channel B treble dial.
  QDial*        presenceB;       ///> Channel B presence dial.
  QDial*        volumeB;         ///> Channel B volume dial.
  QCheckBox*    reverbBypassB;   ///> Reverb B enabled switch.
  QComboBox*    reverbB;         ///> Reverb B type selector.
  QDial*        reverbDecayB;    ///> Reverb B decay dial.
  QDial*        reverbPredelayB; ///> Reverb B pre delay dial.
  QDial*        reverbToneB;     ///> Reverb B tone dial.
  QDial*        reverbMixB;      ///> Reverb B mix dial.
  QRadioButton* voiceB1;         ///> Channel B voice I select.
  QRadioButton* voiceB2;         ///> Channel B voice II select.
  QRadioButton* voiceB3;         ///> Channel B voice III select.
  QRadioButton* voiceB4;         ///> Channel B voice IV select.
  QSlider*      classB;          ///> Channel B, Class A/B switch.
  QSlider*      xtodeB;          ///> Channel B Pentode / triode switch.
  QRadioButton* topolB1;         ///> Channel B topology I select.
  QRadioButton* topolB2;         ///> Channel B topology II select.
  QRadioButton* topolB3;         ///> Channel B topology III select.
  QRadioButton* topolB4;         ///> Channel B topology IV select.
  QSlider*      boostB;          ///> Channel B tube boost on/off.
  QSlider*      pivoltB;         ///> Channel B phase inverter switch.
  QSlider*      capB;            ///> Channel B cap X, cap Y switch.
  QComboBox*    mic;             ///> Master XLR mic simulation.
  QCheckBox*    lowVol;          ///> Low volume switch.
  QSlider*      channel;         ///> Channel A/B switch.
  QDial*        master;          ///> Master volume.
  QAction*      quitAction;      ///> File->Quit action.
  QAction*      setupAction;     ///> Options->Setup action.
  QAction*      defaultsAction;  ///> Options->Load defaults on amp change.
  QAction*      aboutAction;     ///> Help->About action.
  QAction*      aboutQtAction;   ///> Help->About Qt action.
  QMenu*        fileMenu;        ///> File main menu item.
  QMenu*        optionsMenu;     ///> Options main menu item.
  QMenu*        helpMenu;        ///> Help main menu item.
  QString       midiInName;      ///> Name of the active MIDI input.
  QString       midiOutName;     ///> Name of the active MIDI output.
  bool          midiOK;          ///> Is the MIDI system up and running?
  RtMidiIn      midiIn;          ///> The MIDI input used.
  RtMidiOut     midiOut;         ///> The MIDI output used.
  bool          blocked;         ///> UI udate blocking flag.
};

#endif // #ifndef __MAINWINDOW_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
