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
#include "qimagedial.h"
#include "qimagetoggle.h"
#include "qimagebutton.h"
#include "qimagetoggle4.h"
#include "qimageled.h"
#include "dtedit.h"
#include "mainmidiwindow.h"

////////////////////////////////////////////////////////////////////////////////
///\class MainWindow mainwindow.h
///\brief Main window class.
/// This is the main widget of the application.
////////////////////////////////////////////////////////////////////////////////
class MainWindow :
  public MainMIDIWindow
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
  // MainWindow::paintEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Message handler for the paint event.
  ///\param   [in] e: Description of the event.
  //////////////////////////////////////////////////////////////////////////////
  void paintEvent(QPaintEvent* e);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::controlChangeReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new control change message arrives.
  ///\param   [in] channel:       MIDI channel of this message.
  ///\param   [in] controlNumber: Controller number.
  ///\param   [in] value:         Control value.
  //////////////////////////////////////////////////////////////////////////////
  virtual void controlChangeReceived(unsigned char channel, unsigned char controlNumber, unsigned char value);

private:

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::createEditArea()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Create the main edit area.
  //////////////////////////////////////////////////////////////////////////////
  void createEditArea();

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::getValuesFromDT()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Sync UI with the values from the actual DT.
  ///\param   [in] async: Is this called asyncally?
  ///\remarks This functions sends value request CCs to the DT. The UI is then
  ///         updated by the CC receive function.
  //////////////////////////////////////////////////////////////////////////////
  void getValuesFromDT(bool async = false);

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

private slots:

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::about()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the about button signal.
  ///\param   [in] sender: The sending control.
  ///\remarks Shows an about box with informations about this application.
  //////////////////////////////////////////////////////////////////////////////
  void about(QImageButton* sender);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::setupMIDI()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the File->Setup signal.
  ///\param   [in] sender: The sending control.
  ///\remarks Shows the MIDI setup dialog.
  //////////////////////////////////////////////////////////////////////////////
  void setupMIDI(QImageButton* sender);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::rotaryChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the dial changed event.
  ///\param   [in] sender: The sending control.
  ///\param   [in] newVal: The new value.
  //////////////////////////////////////////////////////////////////////////////
  void rotaryChanged(QImageDial* sender, double newVal);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::rotaryReleased()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the dial released event.
  ///\param   [in] sender: The sending control.
  //////////////////////////////////////////////////////////////////////////////
  void rotaryReleased(QImageDial* sender);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::toggleChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the toggle changed event.
  ///\param   [in] sender: The sending control.
  ///\param   [in] newVal: The new value.
  //////////////////////////////////////////////////////////////////////////////
  void toggleChanged(QImageToggle* sender, bool newVal);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::toggle4Changed()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the toggle changed event.
  ///\param   [in] sender: The sending control.
  ///\param   [in] newVal: The new value.
  //////////////////////////////////////////////////////////////////////////////
  void toggle4Changed(QImageToggle4* sender, int newVal);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::ampAChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the amp A combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void ampAChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::cabAChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the cab A combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void cabAChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::reverbAChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the reverb A combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void reverbAChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::ampBChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the amp B combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void ampBChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::cabBChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the cab B combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void cabBChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::reverbBChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the reverb B combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void reverbBChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::micChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mic combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void micChanged(int value);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QComboBox*     ampA;            ///\> Channel A amp model selector.
  QComboBox*     cabA;            ///\> Channel A cabinet selector.
  QImageDial*    gainA;           ///\> Channel A gain dial.
  QImageDial*    bassA;           ///\> Channel A bass dial.
  QImageDial*    middleA;         ///\> Channel A middle dial.
  QImageDial*    trebleA;         ///\> Channel A treble dial.
  QImageDial*    presenceA;       ///\> Channel A presence dial.
  QImageDial*    volumeA;         ///\> Channel A volume dial.
  QImageLED*     reverbLedA;      ///\> Channel A reverb LED.
  QImageToggle*  reverbBypassA;   ///\> Reverb A enabled switch.
  QComboBox*     reverbA;         ///\> Reverb A type selector.
  QImageDial*    reverbDecayA;    ///\> Reverb A decay dial.
  QImageDial*    reverbPredelayA; ///\> Reverb A pre delay dial.
  QImageDial*    reverbToneA;     ///\> Reverb A tone dial.
  QImageDial*    reverbMixA;      ///\> Reverb A mix dial.
  QImageToggle4* voiceA;          ///\> Channel A voice selector.
  QImageToggle*  classA;          ///\> Channel A, Class A/B switch.
  QImageToggle*  xtodeA;          ///\> Channel A Pentode / triode switch.
  QImageToggle4* topolA;          ///\> Channel A topology selector.
  QImageToggle*  boostA;          ///\> Channel A tube boost on/off.
  QImageToggle*  pivoltA;         ///\> Channel A phase inverter switch.
  QImageToggle*  capA;            ///\> Channel A cap X, cap Y switch.
  QComboBox*     ampB;            ///\> Channel B amp model selector.
  QComboBox*     cabB;            ///\> Channel B cabinet selector.
  QImageDial*    gainB;           ///\> Channel B gain dial.
  QImageDial*    bassB;           ///\> Channel B bass dial.
  QImageDial*    middleB;         ///\> Channel B middle dial.
  QImageDial*    trebleB;         ///\> Channel B treble dial.
  QImageDial*    presenceB;       ///\> Channel B presence dial.
  QImageDial*    volumeB;         ///\> Channel B volume dial.
  QImageLED*     reverbLedB;      ///\> Channel B reverb LED.
  QImageToggle*  reverbBypassB;   ///\> Reverb B enabled switch.
  QComboBox*     reverbB;         ///\> Reverb B type selector.
  QImageDial*    reverbDecayB;    ///\> Reverb B decay dial.
  QImageDial*    reverbPredelayB; ///\> Reverb B pre delay dial.
  QImageDial*    reverbToneB;     ///\> Reverb B tone dial.
  QImageDial*    reverbMixB;      ///\> Reverb B mix dial.
  QImageToggle4* voiceB;          ///\> Channel B voice selector.
  QImageToggle*  classB;          ///\> Channel B, Class A/B switch.
  QImageToggle*  xtodeB;          ///\> Channel B Pentode / triode switch.
  QImageToggle4* topolB;          ///\> Channel B topology selector.
  QImageToggle*  boostB;          ///\> Channel B tube boost on/off.
  QImageToggle*  pivoltB;         ///\> Channel B phase inverter switch.
  QImageToggle*  capB;            ///\> Channel B cap X, cap Y switch.
  QComboBox*     mic;             ///\> Master XLR mic simulation.
  QImageLED*     lowVolLed;       ///\> Low Volume LED.
  QImageToggle*  lowVol;          ///\> Low volume switch.
  QImageToggle*  channel;         ///\> Channel A/B switch.
  QImageDial*    master;          ///\> Master volume.
  QImage         backPic;         ///\> Main background image.
  bool           blocked;         ///\> UI udate blocking flag.
  QMutex         receiveMutex;    ///\> Mutex to avoid endless recursion.
};

#endif // #ifndef __MAINWINDOW_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
