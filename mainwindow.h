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
#include "dtdial.h"
#include "dtslider.h"
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
  // MainWindow::controlChangeReceived()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This is called when a new control change message arrives.
  ///\param   [in] channel:       MIDI channel of this message.
  ///\param   [in] controlNumber: Controller number.
  ///\param   [in] value:         Control value.
  //////////////////////////////////////////////////////////////////////////////
  virtual void controlChangeReceived(unsigned char channel, unsigned char controlNumber, unsigned char value);

private:

  void readFile(const QString& fileName);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::createActions()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Create all required actions.
  //////////////////////////////////////////////////////////////////////////////
  void createActions();

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::createMenu()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Create the main menu.
  //////////////////////////////////////////////////////////////////////////////
  void createMenu();

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
  // MainWindow::dialChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the dial changed event.
  ///\param   [in] controlID: Control channel of the dial.
  ///\param   [in] value:     New value of the dial.
  //////////////////////////////////////////////////////////////////////////////
  void dialChanged(int controlID, int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::sliderChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the slider changed event.
  ///\param   [in] controlID: Control channel of the slider.
  ///\param   [in] value:     New value of the slider.
  //////////////////////////////////////////////////////////////////////////////
  void sliderChanged(int controlID, int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::voiceA1toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice A1 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceA1toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::voiceA2toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice A2 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceA2toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::voiceA3toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice A3 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceA3toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::voiceA4toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice A4 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceA4toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::reverbBypassAChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the reverb A bypass checkbox change event.
  ///\param   [in] value: New state of the box.
  //////////////////////////////////////////////////////////////////////////////
  void reverbBypassAChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::reverbAChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the reverb A combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void reverbAChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolA1toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology A1 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolA1toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolA2toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology A2 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolA2toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolA3toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology A3 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolA3toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolA4toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology A4 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolA4toggled(bool checked);

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
  // MainWindow::voiceB1toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice B1 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceB1toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::voiceB2toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice B2 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceB2toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::voiceB3toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice B3 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceB3toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::voiceB4toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the voice B4 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void voiceB4toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::reverbBypassBChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the reverb B bypass checkbox change event.
  ///\param   [in] value: New state of the box.
  //////////////////////////////////////////////////////////////////////////////
  void reverbBypassBChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::reverbBChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the reverb B combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void reverbBChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolB1toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology B1 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolB1toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolB2toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology B2 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolB2toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolB3toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology B3 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolB3toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::topolB4toggled()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the topology B4 radio button toggle event.
  ///\param   [in] checked: New state of the button.
  //////////////////////////////////////////////////////////////////////////////
  void topolB4toggled(bool checked);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::micChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mic combo box selection changed event.
  ///\param   [in] value: Index of the selected item.
  //////////////////////////////////////////////////////////////////////////////
  void micChanged(int value);

  //////////////////////////////////////////////////////////////////////////////
  // MainWindow::lowVolChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the low volume checkbox change event.
  ///\param   [in] value: New state of the box.
  //////////////////////////////////////////////////////////////////////////////
  void lowVolChanged(int value);

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QComboBox*    ampA;            ///> Channel A amp model selector.
  QComboBox*    cabA;            ///> Channel A cabinet selector.
  DTDial*       gainA;           ///> Channel A gain dial.
  DTDial*       bassA;           ///> Channel A bass dial.
  DTDial*       middleA;         ///> Channel A middle dial.
  DTDial*       trebleA;         ///> Channel A treble dial.
  DTDial*       presenceA;       ///> Channel A presence dial.
  DTDial*       volumeA;         ///> Channel A volume dial.
  QCheckBox*    reverbBypassA;   ///> Reverb A enabled switch.
  QComboBox*    reverbA;         ///> Reverb A type selector.
  DTDial*       reverbDecayA;    ///> Reverb A decay dial.
  DTDial*       reverbPredelayA; ///> Reverb A pre delay dial.
  DTDial*       reverbToneA;     ///> Reverb A tone dial.
  DTDial*       reverbMixA;      ///> Reverb A mix dial.
  QRadioButton* voiceA1;         ///> Channel A voice I select.
  QRadioButton* voiceA2;         ///> Channel A voice II select.
  QRadioButton* voiceA3;         ///> Channel A voice III select.
  QRadioButton* voiceA4;         ///> Channel A voice IV select.
  DTSlider*     classA;          ///> Channel A, Class A/B switch.
  DTSlider*     xtodeA;          ///> Channel A Pentode / triode switch.
  QRadioButton* topolA1;         ///> Channel A topology I select.
  QRadioButton* topolA2;         ///> Channel A topology II select.
  QRadioButton* topolA3;         ///> Channel A topology III select.
  QRadioButton* topolA4;         ///> Channel A topology IV select.
  DTSlider*      boostA;          ///> Channel A tube boost on/off.
  DTSlider*      pivoltA;         ///> Channel A phase inverter switch.
  DTSlider*     capA;            ///> Channel A cap X, cap Y switch.
  QComboBox*    ampB;            ///> Channel B amp model selector.
  QComboBox*    cabB;            ///> Channel B cabinet selector.
  DTDial*       gainB;           ///> Channel B gain dial.
  DTDial*       bassB;           ///> Channel B bass dial.
  DTDial*       middleB;         ///> Channel B middle dial.
  DTDial*       trebleB;         ///> Channel B treble dial.
  DTDial*       presenceB;       ///> Channel B presence dial.
  DTDial*       volumeB;         ///> Channel B volume dial.
  QCheckBox*    reverbBypassB;   ///> Reverb B enabled switch.
  QComboBox*    reverbB;         ///> Reverb B type selector.
  DTDial*       reverbDecayB;    ///> Reverb B decay dial.
  DTDial*       reverbPredelayB; ///> Reverb B pre delay dial.
  DTDial*       reverbToneB;     ///> Reverb B tone dial.
  DTDial*       reverbMixB;      ///> Reverb B mix dial.
  QRadioButton* voiceB1;         ///> Channel B voice I select.
  QRadioButton* voiceB2;         ///> Channel B voice II select.
  QRadioButton* voiceB3;         ///> Channel B voice III select.
  QRadioButton* voiceB4;         ///> Channel B voice IV select.
  DTSlider*     classB;          ///> Channel B, Class A/B switch.
  DTSlider*     xtodeB;          ///> Channel B Pentode / triode switch.
  QRadioButton* topolB1;         ///> Channel B topology I select.
  QRadioButton* topolB2;         ///> Channel B topology II select.
  QRadioButton* topolB3;         ///> Channel B topology III select.
  QRadioButton* topolB4;         ///> Channel B topology IV select.
  DTSlider*     boostB;          ///> Channel B tube boost on/off.
  DTSlider*     pivoltB;         ///> Channel B phase inverter switch.
  DTSlider*     capB;            ///> Channel B cap X, cap Y switch.
  QComboBox*    mic;             ///> Master XLR mic simulation.
  QCheckBox*    lowVol;          ///> Low volume switch.
  DTSlider*     channel;         ///> Channel A/B switch.
  DTDial*       master;          ///> Master volume.
  QAction*      openAction;      ///> File->Open.
  QAction*      saveAction;      ///> File->Save.
  QAction*      saveAsAction;    ///> File->Save as.
  QAction*      quitAction;      ///> File->Quit action.
  QAction*      setupAction;     ///> Options->Setup action.
  QAction*      defaultsAction;  ///> Options->Load defaults on amp change.
  QAction*      aboutAction;     ///> Help->About action.
  QAction*      aboutQtAction;   ///> Help->About Qt action.
  QMenu*        fileMenu;        ///> File main menu item.
  QMenu*        optionsMenu;     ///> Options main menu item.
  QMenu*        helpMenu;        ///> Help main menu item.
  bool          blocked;         ///> UI udate blocking flag.
};

#endif // #ifndef __MAINWINDOW_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
