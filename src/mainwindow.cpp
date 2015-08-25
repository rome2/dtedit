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
#include "dtedit.h"
#include "mainmidiwindow.h"
#include "mainwindow.h"
#include "aboutdialog.h"

////////////////////////////////////////////////////////////////////////////////
// MainWindow::MainWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
  MainMIDIWindow(parent),
  blocked(false)
{
  // Init title:
  setWindowTitle("DT Edit");
  setWindowIcon(QIcon(":/images/dtedit.png"));

  // Hide status bar and menu:
  statusBar()->setVisible(false);
  menuBar()->setVisible(false);

  // Load background:
  backPic.load(":/images/back.png");

  // Create the main edit area:
  createEditArea();

  // Init size and position (screen center):
  int w = backPic.width();
  int h = backPic.height();
  int x = (QApplication::desktop()->width()  / 2) - (w / 2);
  int y = (QApplication::desktop()->height() / 2) - (h / 2);

  // Load settings:
  QSettings settings;
  x = settings.value("mainwindow/x", QVariant(x)).toInt();
  y = settings.value("mainwindow/y", QVariant(y)).toInt();
  midiInName  = settings.value("MIDI/inputName",  QVariant("")).toString();
  midiOutName = settings.value("MIDI/outputName", QVariant("")).toString();

  // Place window:
  setFixedSize(w, h);
  setGeometry(x, y, width(), height());
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::~MainWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Destructor of this window.
///\remarks Choose! Choose the form of the Destructor!
////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
  // Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::closeEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the window close event.
///\param   [in] e: Description of the event.
///\remarks To allow the window to close, call e->accept().
////////////////////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent* e)
{
  // Save window position:
  QSettings settings;
  QRect rc = geometry();
  settings.setValue("mainwindow/x", rc.left());
  settings.setValue("mainwindow/y", rc.top());

  // Save MIDI state:
  settings.setValue("MIDI/inputName",  midiInName);
  settings.setValue("MIDI/outputName", midiOutName);

  // allow closing:
  e->accept();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::showEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the window show event.
///\param   [in] e: Description of the event.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::showEvent(QShowEvent* /*e*/)
{
  // Open the MIDI ports and if the opening fails, ask user what to do:
  while (!openMIDIPorts())
  {
    // Get user wish:
    if (QMessageBox::question(this, tr("MIDI error"), tr("There was an error while establishing the MIDI connection to the device.\n\nWould you like to check the configuration?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
      return;

    // Show the setup dialog:
    if (!showSetupWindow())
      return;
  }

  // Get current state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::paintEvent()
////////////////////////////////////////////////////////////////////////////////
///\brief   Message handler for the paint event.
///\param   [in] e: Description of the event.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::paintEvent(QPaintEvent* e)
{
  // Update title:
  if (!versionString.isEmpty())
    setWindowTitle("DT Edit (connected to " + versionString + ")");
  else
    setWindowTitle("DT Edit (not connected)");

  // Do we have a background image?
  if (backPic.isNull())
  {
    // Let the base class do the painting:
    QWidget::paintEvent(e);
    return;
  }

  // Draw the back pic:
  QPainter qp(this);
  qp.drawImage(0, 0, backPic);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::openMIDIPorts()
////////////////////////////////////////////////////////////////////////////////
///\brief   Open the MIDI devices for input/output.
///\return  Returns true if successfull or false otherwise.
///\remarks Always closes the port priot trying to open them again.
////////////////////////////////////////////////////////////////////////////////
bool MainWindow::openMIDIPorts()
{
  // Reset version display:
  versionString = "";

  // Base class handling:
  if (!MainMIDIWindow::openMIDIPorts())
    return false;

  // Send "identify yourself!" string:
  std::vector<unsigned char> buff;
  buff.reserve(6);
  buff.push_back(0xF0);
  buff.push_back(0x7E);
  buff.push_back(0x7F);
  buff.push_back(0x06);
  buff.push_back(0x01);
  buff.push_back(0xF7);
  midiOut.sendMessage(&buff);

  // Return success:
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::controlChangeReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new control change message arrives.
///\param   [in] channel:       MIDI channel of this message.
///\param   [in] controlNumber: Controller number.
///\param   [in] value:         Control value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::controlChangeReceived(unsigned char channel, unsigned char controlNumber, unsigned char value)
{
  // Are we ment?
  if (channel != DT_MIDI_CHANNEL)
    return;

  // Check for block messages:
  if (controlNumber == 127)
    blocked = value >= 64;

  // Get receiving state. If this is true then we are currently be called by a
  // getValuesFromDT() request.
  static bool receiving = false;
  if (controlNumber == 126)
    receiving = value >= 64;

  // UI locked?
  if (blocked)
    return;

  bool oldState;
  switch (controlNumber)
  {
  case CC_AMP_A:
    oldState = ampA->blockSignals(true);
    ampA->setCurrentIndex(value);
    ampA->blockSignals(oldState);
    break;
  case CC_CAB_A:
    oldState = cabA->blockSignals(true);
    cabA->setCurrentIndex(value);
    cabA->blockSignals(oldState);
    break;
  case CC_GAIN_A:
    oldState = gainA->blockSignals(true);
    gainA->setValue(value / 127.0);
    gainA->blockSignals(oldState);
    break;
  case CC_BASS_A:
    oldState = bassA->blockSignals(true);
    bassA->setValue(value / 127.0);
    bassA->blockSignals(oldState);
    break;
  case CC_MIDDLE_A:
    oldState = middleA->blockSignals(true);
    middleA->setValue(value / 127.0);
    middleA->blockSignals(oldState);
    break;
  case CC_TREBLE_A:
    oldState = trebleA->blockSignals(true);
    trebleA->setValue(value / 127.0);
    trebleA->blockSignals(oldState);
    break;
  case CC_PRESENCE_A:
    oldState = presenceA->blockSignals(true);
    presenceA->setValue(value / 127.0);
    presenceA->blockSignals(oldState);
    break;
  case CC_VOLUME_A:
    oldState = volumeA->blockSignals(true);
    volumeA->setValue(value / 127.0);
    volumeA->blockSignals(oldState);
    break;
  case CC_VOICE_A:
    if (voiceA->value() != value)
    {
      oldState = voiceA->blockSignals(true);
      voiceA->setValue(value);
      voiceA->blockSignals(oldState);
      if (!receiving)
        getValuesFromDT(true);
    }
    break;
  case CC_REV_BYPASS_A:
    oldState = reverbBypassA->blockSignals(true);
    reverbBypassA->setValue(value >= 64);
    reverbBypassA->blockSignals(oldState);
    oldState = reverbLedA->blockSignals(true);
    reverbLedA->setValue(reverbBypassA->value());
    reverbLedA->blockSignals(oldState);
    break;
  case CC_REV_TYPE_A:
    oldState = reverbA->blockSignals(true);
    reverbA->setCurrentIndex(value);
    reverbA->blockSignals(oldState);
    break;
  case CC_REV_DECAY_A:
    oldState = reverbDecayA->blockSignals(true);
    reverbDecayA->setValue(value / 127.0);
    reverbDecayA->blockSignals(oldState);
    break;
  case CC_REV_PREDELAY_A:
    oldState = reverbPredelayA->blockSignals(true);
    reverbPredelayA->setValue(value / 127.0);
    reverbPredelayA->blockSignals(oldState);
    break;
  case CC_REV_TONE_A:
    oldState = reverbToneA->blockSignals(true);
    reverbToneA->setValue(value / 127.0);
    reverbToneA->blockSignals(oldState);
    break;
  case CC_REV_MIX_A:
    oldState = reverbMixA->blockSignals(true);
    reverbMixA->setValue(value / 127.0);
    reverbMixA->blockSignals(oldState);
    break;
  case CC_CLASS_A:
    oldState = classA->blockSignals(true);
    classA->setValue(value >= 64 ? 1 : 0);
    classA->blockSignals(oldState);
    break;
  case CC_TOPOL_A:
    oldState = topolA->blockSignals(true);
    topolA->setValue(value);
    topolA->blockSignals(oldState);
    break;
  case CC_XTODE_A:
    oldState = xtodeA->blockSignals(true);
    xtodeA->setValue(value >= 64 ? 1 : 0);
    xtodeA->blockSignals(oldState);
    break;
  case CC_BOOST_A:
    oldState = boostA->blockSignals(true);
    boostA->setValue(value >= 64 ? 1 : 0);
    boostA->blockSignals(oldState);
    break;
  case CC_PI_VOLTAGE_A:
    oldState = pivoltA->blockSignals(true);
    pivoltA->setValue(value >= 64 ? 1 : 0);
    pivoltA->blockSignals(oldState);
    break;
  case CC_CAP_TYPE_A:
    oldState = capA->blockSignals(true);
    capA->setValue(value >= 64 ? 1 : 0);
    capA->blockSignals(oldState);
    break;
  case CC_AMP_B:
    oldState = ampB->blockSignals(true);
    ampB->setCurrentIndex(value);
    ampB->blockSignals(oldState);
    break;
  case CC_CAB_B:
    oldState = cabB->blockSignals(true);
    cabB->setCurrentIndex(value);
    cabB->blockSignals(oldState);
    break;
  case CC_GAIN_B:
    oldState = gainB->blockSignals(true);
    gainB->setValue(value / 127.0);
    gainB->blockSignals(oldState);
    break;
  case CC_BASS_B:
    oldState = bassB->blockSignals(true);
    bassB->setValue(value / 127.0);
    bassB->blockSignals(oldState);
    break;
  case CC_MIDDLE_B:
    oldState = middleB->blockSignals(true);
    middleB->setValue(value / 127.0);
    middleB->blockSignals(oldState);
    break;
  case CC_TREBLE_B:
    oldState = trebleB->blockSignals(true);
    trebleB->setValue(value / 127.0);
    trebleB->blockSignals(oldState);
    break;
  case CC_PRESENCE_B:
    oldState = presenceB->blockSignals(true);
    presenceB->setValue(value / 127.0);
    presenceB->blockSignals(oldState);
    break;
  case CC_VOLUME_B:
    oldState = volumeB->blockSignals(true);
    volumeB->setValue(value / 127.0);
    volumeB->blockSignals(oldState);
    break;
  case CC_VOICE_B:
    if (voiceB->value() != value)
    {
      oldState = voiceB->blockSignals(true);
      voiceB->setValue(value);
      voiceB->blockSignals(oldState);
      if (!receiving)
        getValuesFromDT(true);
    }
    break;
  case CC_REV_BYPASS_B:
    oldState = reverbBypassB->blockSignals(true);
    reverbBypassB->setValue(value >= 64);
    reverbBypassB->blockSignals(oldState);
    oldState = reverbLedB->blockSignals(true);
    reverbLedB->setValue(reverbBypassB->value());
    reverbLedB->blockSignals(oldState);
    break;
  case CC_REV_TYPE_B:
    oldState = reverbB->blockSignals(true);
    reverbB->setCurrentIndex(value);
    reverbB->blockSignals(oldState);
    break;
  case CC_REV_DECAY_B:
    oldState = reverbDecayB->blockSignals(true);
    reverbDecayB->setValue(value / 127.0);
    reverbDecayB->blockSignals(oldState);
    break;
  case CC_REV_PREDELAY_B:
    oldState = reverbPredelayB->blockSignals(true);
    reverbPredelayB->setValue(value / 127.0);
    reverbPredelayB->blockSignals(oldState);
    break;
  case CC_REV_TONE_B:
    oldState = reverbToneB->blockSignals(true);
    reverbToneB->setValue(value / 127.0);
    reverbToneB->blockSignals(oldState);
    break;
  case CC_REV_MIX_B:
    oldState = reverbMixB->blockSignals(true);
    reverbMixB->setValue(value / 127.0);
    reverbMixB->blockSignals(oldState);
    break;
  case CC_CLASS_B:
    oldState = classB->blockSignals(true);
    classB->setValue(value >= 64 ? 1 : 0);
    classB->blockSignals(oldState);
    break;
  case CC_TOPOL_B:
    oldState = topolB->blockSignals(true);
    topolB->setValue(value);
    topolB->blockSignals(oldState);
    break;
  case CC_XTODE_B:
    oldState = xtodeB->blockSignals(true);
    xtodeB->setValue(value >= 64 ? 1 : 0);
    xtodeB->blockSignals(oldState);
    break;
  case CC_BOOST_B:
    oldState = boostB->blockSignals(true);
    boostB->setValue(value >= 64 ? 1 : 0);
    boostB->blockSignals(oldState);
    break;
  case CC_PI_VOLTAGE_B:
    oldState = pivoltB->blockSignals(true);
    pivoltB->setValue(value >= 64 ? 1 : 0);
    pivoltB->blockSignals(oldState);
    break;
  case CC_CAP_TYPE_B:
    oldState = capB->blockSignals(true);
    capB->setValue(value >= 64 ? 1 : 0);
    capB->blockSignals(oldState);
    break;
  case CC_XLR_MIC:
    oldState = mic->blockSignals(true);
    mic->setCurrentIndex(value);
    mic->blockSignals(oldState);
    break;
  case CC_LOWVOLUME:
    oldState = lowVol->blockSignals(true);
    lowVol->setValue(value < 64);
    lowVol->blockSignals(oldState);
    oldState = lowVolLed->blockSignals(true);
    lowVolLed->setValue(!lowVol->value());
    lowVolLed->blockSignals(oldState);
    break;
  case CC_CHANNEL:
    oldState = this->channel->blockSignals(true);
    this->channel->setValue(value >= 64 ? 0 : 1);
    this->channel->blockSignals(oldState);
    break;
  case CC_MASTER_VOL:
    oldState = master->blockSignals(true);
    master->setValue(value / 127.0);
    master->blockSignals(oldState);
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::sysExReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new SysEx message arrives.
///\param   [in] buff: The message buffer.
///\param   [in] value:      Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sysExReceived(const std::vector<unsigned char>& buff)
{
  // Check size and type:
  if (buff.size() == 17 && buff[0] == 0xF0)
  {
    // Check header
    if (buff[1] != 0x7E || buff[2] != 0x7F || buff[3] != 0x06 || buff[4] != 0x02)
      return;
    if (buff[5] != 0x00 || buff[6] != 0x01 || buff[7] != 0x0C)
      return;

    // Check device:
    if (buff[8] != 0x15 && buff[9] != 0x00)
      return;

    // Get model:
    int model = buff[10];
    switch (model)
    {
    case 0:
      versionString = "DT50 1x12 Combo";
      break;
    case 1:
      versionString = "DT50 212 Combo";
      break;
    case 2:
      versionString = "DT50 Head";
      break;
    case 3:
      versionString = "DT25 1x12 Combo";
      break;
    case 4:
      versionString = "DT25 Head";
      break;
    default:
      versionString = "Unknown DT model";
      break;
    }

    // Add version:
    versionString += " v";
    //versionString += (char)buff[12]; // Leading space. Add again if firmware version reaches 10.00
    versionString += (char)buff[13];
    versionString += '.';
    versionString += (char)buff[14];
    versionString += (char)buff[15];

    // Force a redraw:
    update();
  }
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::createEditArea()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create the main edit area.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::createEditArea()
{
  int x0 = 0;
  int y0 = 22;
  #ifdef __LINUX_ALSA__
  QString comboStyle("QComboBox { border: 1px solid black; }");
  #else
  QString comboStyle("QComboBox { background: #202020; color: white; border: 1px solid #606060; }");
  #endif

  QPixmap I_II_III_IV(":/images/I_II_III_IV.png");
  QPixmap I_II_III_IV_disabled(":/images/I_II_III_IV_disabled.png");
  QPixmap knob_movie(":/images/knob_movie.png");
  QPixmap knob_disabled(":/images/knob_disabled.png");
  QPixmap led_yellow(":/images/led_yellow.png");
  QPixmap led_yellow_disabled(":/images/led_yellow_disabled.png");
  QPixmap onoff(":/images/onoff.png");
  QPixmap onoff_disabled(":/images/onoff_disabled.png");
  QPixmap classAB(":/images/class.png");
  QPixmap class_disabled(":/images/class_disabled.png");
  QPixmap xtode(":/images/xtode.png");
  QPixmap xtode_disabled(":/images/xtode_disabled.png");
  QPixmap boost(":/images/boost.png");
  QPixmap boost_disabled(":/images/boost_disabled.png");
  QPixmap piv(":/images/piv.png");
  QPixmap piv_disabled(":/images/piv_disabled.png");
  QPixmap cap(":/images/cap.png");
  QPixmap cap_disabled(":/images/cap_disabled.png");
  QPixmap channelImg(":/images/channel.png");
  QPixmap channel_disabled(":/images/channel_disabled.png");
  QPixmap led_red(":/images/led_red.png");
  QPixmap led_red_disabled(":/images/led_red_disabled.png");
  QPixmap midi(":/images/midi.png");
  QPixmap midi_disabled(":/images/midi_disabled.png");
  QPixmap about(":/images/about.png");
  QPixmap about_disabled(":/images/about_disabled.png");

  voiceA = new QImageToggle4(this);
  voiceA->setGeometry(x0 + 34, y0 + 30, 48, 48);
  voiceA->setImage(I_II_III_IV);
  voiceA->setDisabledImage(I_II_III_IV_disabled);
  voiceA->setEnabled(true);
  voiceA->setTag(CC_VOICE_A);
  connect(voiceA, SIGNAL(valueChanged()), this, SLOT(toggle4Changed()));

  ampA = new QComboBox(this);
  ampA->addItem("None");
  ampA->addItem("Blackface Double Normal");
  ampA->addItem("Blackface Double Vib");
  ampA->addItem("Hiway 100");
  ampA->addItem("Super O");
  ampA->addItem("Gibtone 185");
  ampA->addItem("Tweed B-Man Normal");
  ampA->addItem("Tweed B-Man Bright");
  ampA->addItem("Blackface 'Lux Normal");
  ampA->addItem("Blackface 'Lux Vib");
  ampA->addItem("Divide 9/15");
  ampA->addItem("Phd Motorway");
  ampA->addItem("Class A-15");
  ampA->addItem("Class A-30");
  ampA->addItem("Brit J-45 Normal");
  ampA->addItem("Brit J-45 Bright");
  ampA->addItem("Brit Plexi 100 Normal");
  ampA->addItem("Brit Plexi 100 Bright");
  ampA->addItem("Brit P-75 Normal");
  ampA->addItem("Brit P-75 Bright");
  ampA->addItem("Brit J-800");
  ampA->addItem("Bomber Uber");
  ampA->addItem("Treadplate");
  ampA->addItem("Angel F-Ball");
  ampA->addItem("Line 6 Elektrik");
  ampA->addItem("Flip Top (Bass)");
  ampA->addItem("Solo 100 Clean");
  ampA->addItem("Solo 100 Crunch");
  ampA->addItem("Solo 100 Overdrive");
  ampA->addItem("Line 6 Doom");
  ampA->addItem("Line 6 Epic");
  ampA->setGeometry(x0 + 142, y0 + 34, 134, 22);
  ampA->setStyleSheet(comboStyle);
  connect(ampA, SIGNAL(currentIndexChanged(int)), this, SLOT(ampAChanged(int)));

  cabA = new QComboBox(this);
  cabA->addItem("None");
  cabA->addItem("2x12 Blackface Double");
  cabA->addItem("4x12 Hiway");
  cabA->addItem("1x(6x9) Super O");
  cabA->addItem("1x12 Gibtone F-Coil");
  cabA->addItem("4x10 Tweed B-Man");
  cabA->addItem("1x12 Blackface â€˜Lux");
  cabA->addItem("1x12 Brit 12-H");
  cabA->addItem("2x12 PhD Ported");
  cabA->addItem("1x12 Blue Bell");
  cabA->addItem("2x12 Silver Bell");
  cabA->addItem("4x12 Greenback 25");
  cabA->addItem("4x12 Blackback 30");
  cabA->addItem("4x12 Brit T-75");
  cabA->addItem("4x12 Uber");
  cabA->addItem("4x12 Tread V-30");
  cabA->addItem("4x12 XXL V-30");
  cabA->addItem("1x15 Flip Top (Bass)");
  cabA->setGeometry(x0 + 142, y0 + 64, 134, 22);
  cabA->setStyleSheet(comboStyle);
  connect(cabA, SIGNAL(currentIndexChanged(int)), this, SLOT(cabAChanged(int)));

  gainA = new QImageDial(this);
  gainA->setImage(knob_movie);
  gainA->setDisabledImage(knob_disabled);
  gainA->setFrameCount(61);
  gainA->setEnabled(true);
  gainA->setGeometry(x0 + 303, y0 + 30, 48, 48);
  gainA->setTag(CC_GAIN_A);
  connect(gainA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(gainA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  bassA = new QImageDial(this);
  bassA->setImage(knob_movie);
  bassA->setDisabledImage(knob_disabled);
  bassA->setFrameCount(61);
  bassA->setEnabled(true);
  bassA->setGeometry(x0 + 367, y0 + 30, 48, 48);
  bassA->setTag(CC_BASS_A);
  connect(bassA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(bassA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  middleA = new QImageDial(this);
  middleA->setImage(knob_movie);
  middleA->setDisabledImage(knob_disabled);
  middleA->setFrameCount(61);
  middleA->setEnabled(true);
  middleA->setGeometry(x0 + 431, y0 + 30, 48, 48);
  middleA->setTag(CC_MIDDLE_A);
  connect(middleA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(middleA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  trebleA = new QImageDial(this);
  trebleA->setImage(knob_movie);
  trebleA->setDisabledImage(knob_disabled);
  trebleA->setFrameCount(61);
  trebleA->setEnabled(true);
  trebleA->setGeometry(x0 + 495, y0 + 30, 48, 48);
  trebleA->setTag(CC_TREBLE_A);
  connect(trebleA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(trebleA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  presenceA = new QImageDial(this);
  presenceA->setImage(knob_movie);
  presenceA->setDisabledImage(knob_disabled);
  presenceA->setFrameCount(61);
  presenceA->setEnabled(true);
  presenceA->setGeometry(x0 + 559, y0 + 30, 48, 48);
  presenceA->setTag(CC_PRESENCE_A);
  connect(presenceA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(presenceA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  volumeA = new QImageDial(this);
  volumeA->setImage(knob_movie);
  volumeA->setDisabledImage(knob_disabled);
  volumeA->setFrameCount(61);
  volumeA->setEnabled(true);
  volumeA->setGeometry(x0 + 623, y0 + 30, 48, 48);
  volumeA->setTag(CC_VOLUME_A);
  connect(volumeA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(volumeA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbA = new QComboBox(this);
  reverbA->addItem("None");
  reverbA->addItem("Spring");
  reverbA->addItem("'63 Spring");
  reverbA->addItem("Plate");
  reverbA->addItem("Room");
  reverbA->addItem("Chamber");
  reverbA->addItem("Hall");
  reverbA->addItem("Cave");
  reverbA->addItem("Ducking");
  reverbA->addItem("Octo");
  reverbA->addItem("Tile");
  reverbA->addItem("Echo");
  reverbA->addItem("Particle Verb");
  reverbA->setGeometry(x0 + 280, y0 + 168, 134, 22);
  reverbA->setStyleSheet(comboStyle);
  connect(reverbA, SIGNAL(currentIndexChanged(int)), this, SLOT(reverbAChanged(int)));

  reverbLedA = new QImageLED(this);
  reverbLedA->setImage(led_yellow);
  reverbLedA->setDisabledImage(led_yellow_disabled);
  reverbLedA->setGeometry(x0 + 244, y0 + 130, 31, 31);
  reverbLedA->setEnabled(true);
  reverbLedA->setValue(true);

  reverbBypassA = new QImageToggle(this);
  reverbBypassA->setGeometry(x0 + 275, y0 + 130, 44, 31);
  reverbBypassA->setImage(onoff);
  reverbBypassA->setDisabledImage(onoff_disabled);
  reverbBypassA->setEnabled(true);
  reverbBypassA->setTag(CC_REV_BYPASS_A);
  reverbBypassA->setLeftRight(true);
  reverbBypassA->setValue(true);
  connect(reverbBypassA, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  reverbDecayA = new QImageDial(this);
  reverbDecayA->setImage(knob_movie);
  reverbDecayA->setDisabledImage(knob_disabled);
  reverbDecayA->setFrameCount(61);
  reverbDecayA->setEnabled(true);
  reverbDecayA->setGeometry(x0 + 431, y0 + 133, 48, 48);
  reverbDecayA->setTag(CC_REV_DECAY_A);
  connect(reverbDecayA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbDecayA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbPredelayA = new QImageDial(this);
  reverbPredelayA->setImage(knob_movie);
  reverbPredelayA->setDisabledImage(knob_disabled);
  reverbPredelayA->setFrameCount(61);
  reverbPredelayA->setEnabled(true);
  reverbPredelayA->setGeometry(x0 + 495, y0 + 133, 48, 48);
  reverbPredelayA->setTag(CC_REV_PREDELAY_A);
  connect(reverbPredelayA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbPredelayA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbToneA = new QImageDial(this);
  reverbToneA->setImage(knob_movie);
  reverbToneA->setDisabledImage(knob_disabled);
  reverbToneA->setFrameCount(61);
  reverbToneA->setEnabled(true);
  reverbToneA->setGeometry(x0 + 559, y0 + 133, 48, 48);
  reverbToneA->setTag(CC_REV_TONE_A);
  connect(reverbToneA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbToneA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbMixA = new QImageDial(this);
  reverbMixA->setImage(knob_movie);
  reverbMixA->setDisabledImage(knob_disabled);
  reverbMixA->setFrameCount(61);
  reverbMixA->setEnabled(true);
  reverbMixA->setGeometry(x0 + 623, y0 + 133, 48, 48);
  reverbMixA->setTag(CC_REV_MIX_A);
  connect(reverbMixA, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbMixA, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  classA = new QImageToggle(this);
  classA->setGeometry(x0 + 720, y0 + 23, 64, 88);
  classA->setImage(classAB);
  classA->setDisabledImage(class_disabled);
  classA->setEnabled(true);
  classA->setTag(CC_CLASS_A);
  connect(classA, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  topolA = new QImageToggle4(this);
  topolA->setGeometry(x0 + 804, y0 + 38, 48, 48);
  topolA->setImage(I_II_III_IV);
  topolA->setDisabledImage(I_II_III_IV_disabled);
  topolA->setEnabled(true);
  topolA->setTag(CC_TOPOL_A);
  connect(topolA, SIGNAL(valueChanged()), this, SLOT(toggle4Changed()));

  xtodeA = new QImageToggle(this);
  xtodeA->setGeometry(x0 + 871, y0 + 23, 64, 88);
  xtodeA->setImage(xtode);
  xtodeA->setDisabledImage(xtode_disabled);
  xtodeA->setEnabled(true);
  xtodeA->setTag(CC_XTODE_A);
  connect(xtodeA, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  boostA = new QImageToggle(this);
  boostA->setGeometry(x0 + 720, y0 + 112, 64, 88);
  boostA->setImage(boost);
  boostA->setDisabledImage(boost_disabled);
  boostA->setEnabled(true);
  boostA->setTag(CC_BOOST_A);
  connect(boostA, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  pivoltA = new QImageToggle(this);
  pivoltA->setGeometry(x0 + 795, y0 + 112, 64, 88);
  pivoltA->setImage(piv);
  pivoltA->setDisabledImage(piv_disabled);
  pivoltA->setEnabled(true);
  pivoltA->setTag(CC_PI_VOLTAGE_A);
  connect(pivoltA, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  capA = new QImageToggle(this);
  capA->setGeometry(x0 + 871, y0 + 112, 64, 88);
  capA->setImage(cap);
  capA->setDisabledImage(cap_disabled);
  capA->setEnabled(true);
  capA->setTag(CC_CAP_TYPE_A);
  connect(capA, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  voiceB = new QImageToggle4(this);
  voiceB->setGeometry(x0 + 34, y0 + 336, 48, 48);
  voiceB->setImage(I_II_III_IV);
  voiceB->setDisabledImage(I_II_III_IV_disabled);
  voiceB->setEnabled(true);
  voiceB->setTag(CC_VOICE_B);
  connect(voiceB, SIGNAL(valueChanged()), this, SLOT(toggle4Changed()));

  ampB = new QComboBox(this);
  ampB->addItem("None");
  ampB->addItem("Blackface Double Normal");
  ampB->addItem("Blackface Double Vib");
  ampB->addItem("Hiway 100");
  ampB->addItem("Super O");
  ampB->addItem("Gibtone 185");
  ampB->addItem("Tweed B-Man Normal");
  ampB->addItem("Tweed B-Man Bright");
  ampB->addItem("Blackface 'Lux Normal");
  ampB->addItem("Blackface 'Lux Vib");
  ampB->addItem("Divide 9/15");
  ampB->addItem("Phd Motorway");
  ampB->addItem("Class A-15");
  ampB->addItem("Class A-30");
  ampB->addItem("Brit J-45 Normal");
  ampB->addItem("Brit J-45 Bright");
  ampB->addItem("Brit Plexi 100 Normal");
  ampB->addItem("Brit Plexi 100 Bright");
  ampB->addItem("Brit P-75 Normal");
  ampB->addItem("Brit P-75 Bright");
  ampB->addItem("Brit J-800");
  ampB->addItem("Bomber Uber");
  ampB->addItem("Treadplate");
  ampB->addItem("Angel F-Ball");
  ampB->addItem("Line 6 Elektrik");
  ampB->addItem("Flip Top (Bass)");
  ampB->addItem("Solo 100 Clean");
  ampB->addItem("Solo 100 Crunch");
  ampB->addItem("Solo 100 Overdrive");
  ampB->addItem("Line 6 Doom");
  ampB->addItem("Line 6 Epic");
  ampB->setGeometry(x0 + 143, y0 + 340, 134, 22);
  ampB->setStyleSheet(comboStyle);
  connect(ampB, SIGNAL(currentIndexChanged(int)), this, SLOT(ampBChanged(int)));

  cabB = new QComboBox(this);
  cabB->addItem("None");
  cabB->addItem("2x12 Blackface Double");
  cabB->addItem("4x12 Hiway");
  cabB->addItem("1x(6x9) Super O");
  cabB->addItem("1x12 Gibtone F-Coil");
  cabB->addItem("4x10 Tweed B-Man");
  cabB->addItem("1x12 Blackface â€˜Lux");
  cabB->addItem("1x12 Brit 12-H");
  cabB->addItem("2x12 PhD Ported");
  cabB->addItem("1x12 Blue Bell");
  cabB->addItem("2x12 Silver Bell");
  cabB->addItem("4x12 Greenback 25");
  cabB->addItem("4x12 Blackback 30");
  cabB->addItem("4x12 Brit T-75");
  cabB->addItem("4x12 Uber");
  cabB->addItem("4x12 Tread V-30");
  cabB->addItem("4x12 XXL V-30");
  cabB->addItem("1x15 Flip Top (Bass)");
  cabB->setGeometry(x0 + 143, y0 + 370, 134, 22);
  cabB->setStyleSheet(comboStyle);
  connect(cabB, SIGNAL(currentIndexChanged(int)), this, SLOT(cabBChanged(int)));

  gainB = new QImageDial(this);
  gainB->setImage(knob_movie);
  gainB->setDisabledImage(knob_disabled);
  gainB->setFrameCount(61);
  gainB->setEnabled(true);
  gainB->setGeometry(x0 + 303, y0 + 336, 48, 48);
  gainB->setTag(CC_GAIN_B);
  connect(gainB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(gainB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  bassB = new QImageDial(this);
  bassB->setImage(knob_movie);
  bassB->setDisabledImage(knob_disabled);
  bassB->setFrameCount(61);
  bassB->setEnabled(true);
  bassB->setGeometry(x0 + 367, y0 + 336, 48, 48);
  bassB->setTag(CC_BASS_B);
  connect(bassB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(bassB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  middleB = new QImageDial(this);
  middleB->setImage(knob_movie);
  middleB->setDisabledImage(knob_disabled);
  middleB->setFrameCount(61);
  middleB->setEnabled(true);
  middleB->setGeometry(x0 + 431, y0 + 336, 48, 48);
  middleB->setTag(CC_MIDDLE_B);
  connect(middleB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(middleB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  trebleB = new QImageDial(this);
  trebleB->setImage(knob_movie);
  trebleB->setDisabledImage(knob_disabled);
  trebleB->setFrameCount(61);
  trebleB->setEnabled(true);
  trebleB->setGeometry(x0 + 495, y0 + 336, 48, 48);
  trebleB->setTag(CC_TREBLE_B);
  connect(trebleB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(trebleB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  presenceB = new QImageDial(this);
  presenceB->setImage(knob_movie);
  presenceB->setDisabledImage(knob_disabled);
  presenceB->setFrameCount(61);
  presenceB->setEnabled(true);
  presenceB->setGeometry(x0 + 559, y0 + 336, 48, 48);
  presenceB->setTag(CC_PRESENCE_B);
  connect(presenceB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(presenceB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  volumeB = new QImageDial(this);
  volumeB->setImage(knob_movie);
  volumeB->setDisabledImage(knob_disabled);
  volumeB->setFrameCount(61);
  volumeB->setEnabled(true);
  volumeB->setGeometry(x0 + 623, y0 + 336, 48, 48);
  volumeB->setTag(CC_VOLUME_B);
  connect(volumeB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(volumeB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbB = new QComboBox(this);
  reverbB->addItem("None");
  reverbB->addItem("Spring");
  reverbB->addItem("'63 Spring");
  reverbB->addItem("Plate");
  reverbB->addItem("Room");
  reverbB->addItem("Chamber");
  reverbB->addItem("Hall");
  reverbB->addItem("Cave");
  reverbB->addItem("Ducking");
  reverbB->addItem("Octo");
  reverbB->addItem("Tile");
  reverbB->addItem("Echo");
  reverbB->addItem("Particle Verb");
  reverbB->setGeometry(x0 + 280, y0 + 270, 134, 22);
  reverbB->setStyleSheet(comboStyle);
  connect(reverbB, SIGNAL(currentIndexChanged(int)), this, SLOT(reverbBChanged(int)));

  reverbLedB = new QImageLED(this);
  reverbLedB->setImage(led_yellow);
  reverbLedB->setDisabledImage(led_yellow_disabled);
  reverbLedB->setGeometry(x0 + 244, y0 + 232, 31, 31);
  reverbLedB->setEnabled(true);
  reverbLedB->setValue(true);

  reverbBypassB = new QImageToggle(this);
  reverbBypassB->setGeometry(x0 + 275, y0 + 232, 44, 31);
  reverbBypassB->setImage(onoff);
  reverbBypassB->setDisabledImage(onoff_disabled);
  reverbBypassB->setEnabled(true);
  reverbBypassB->setTag(CC_REV_BYPASS_B);
  reverbBypassB->setLeftRight(true);
  reverbBypassB->setValue(true);
  connect(reverbBypassB, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  reverbDecayB = new QImageDial(this);
  reverbDecayB->setImage(knob_movie);
  reverbDecayB->setDisabledImage(knob_disabled);
  reverbDecayB->setFrameCount(61);
  reverbDecayB->setEnabled(true);
  reverbDecayB->setGeometry(x0 + 431, y0 + 235, 48, 48);
  reverbDecayB->setTag(CC_REV_DECAY_B);
  connect(reverbDecayB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbDecayB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbPredelayB = new QImageDial(this);
  reverbPredelayB->setImage(knob_movie);
  reverbPredelayB->setDisabledImage(knob_disabled);
  reverbPredelayB->setFrameCount(61);
  reverbPredelayB->setEnabled(true);
  reverbPredelayB->setGeometry(x0 + 495, y0 + 235, 48, 48);
  reverbPredelayB->setTag(CC_REV_PREDELAY_B);
  connect(reverbPredelayB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbPredelayB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbToneB = new QImageDial(this);
  reverbToneB->setImage(knob_movie);
  reverbToneB->setDisabledImage(knob_disabled);
  reverbToneB->setFrameCount(61);
  reverbToneB->setEnabled(true);
  reverbToneB->setGeometry(x0 + 559, y0 + 235, 48, 48);
  reverbToneB->setTag(CC_REV_TONE_B);
  connect(reverbToneB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbToneB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  reverbMixB = new QImageDial(this);
  reverbMixB->setImage(knob_movie);
  reverbMixB->setDisabledImage(knob_disabled);
  reverbMixB->setFrameCount(61);
  reverbMixB->setEnabled(true);
  reverbMixB->setGeometry(x0 + 623, y0 + 235, 48, 48);
  reverbMixB->setTag(CC_REV_MIX_B);
  connect(reverbMixB, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(reverbMixB, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  classB = new QImageToggle(this);
  classB->setGeometry(x0 + 720, y0 + 227, 64, 88);
  classB->setImage(classAB);
  classB->setDisabledImage(class_disabled);
  classB->setEnabled(true);
  classB->setTag(CC_CLASS_B);
  connect(classB, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  topolB = new QImageToggle4(this);
  topolB->setGeometry(x0 + 804, y0 + 242, 48, 48);
  topolB->setImage(I_II_III_IV);
  topolB->setDisabledImage(I_II_III_IV_disabled);
  topolB->setEnabled(true);
  topolB->setTag(CC_TOPOL_B);
  connect(topolB, SIGNAL(valueChanged()), this, SLOT(toggle4Changed()));

  xtodeB = new QImageToggle(this);
  xtodeB->setGeometry(x0 + 871, y0 + 227, 64, 88);
  xtodeB->setImage(xtode);
  xtodeB->setDisabledImage(xtode_disabled);
  xtodeB->setEnabled(true);
  xtodeB->setTag(CC_XTODE_B);
  connect(xtodeB, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  boostB = new QImageToggle(this);
  boostB->setGeometry(x0 + 720, y0 + 316, 64, 88);
  boostB->setImage(boost);
  boostB->setDisabledImage(boost_disabled);
  boostB->setEnabled(true);
  boostB->setTag(CC_BOOST_B);
  connect(boostB, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  pivoltB = new QImageToggle(this);
  pivoltB->setGeometry(x0 + 795, y0 + 316, 64, 88);
  pivoltB->setImage(piv);
  pivoltB->setDisabledImage(piv_disabled);
  pivoltB->setEnabled(true);
  pivoltB->setTag(CC_PI_VOLTAGE_B);
  connect(pivoltB, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  capB = new QImageToggle(this);
  capB->setGeometry(x0 + 871, y0 + 316, 64, 88);
  capB->setImage(cap);
  capB->setDisabledImage(cap_disabled);
  capB->setEnabled(true);
  capB->setTag(CC_CAP_TYPE_B);
  connect(capB, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  channel = new QImageToggle(this);
  channel->setGeometry(x0 + 39, y0 + 137, 64, 88);
  channel->setImage(channelImg);
  channel->setDisabledImage(channel_disabled);
  channel->setEnabled(true);
  channel->setTag(CC_CHANNEL);
  connect(channel, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  master = new QImageDial(this);
  master->setImage(knob_movie);
  master->setDisabledImage(knob_disabled);
  master->setFrameCount(61);
  master->setEnabled(true);
  master->setGeometry(x0 + 140, y0 + 152, 48, 48);
  master->setTag(CC_MASTER_VOL);
  connect(master, SIGNAL(valueChanged()), this, SLOT(rotaryChanged()));
  connect(master, SIGNAL(mouseReleased()), this, SLOT(rotaryReleased()));

  lowVolLed = new QImageLED(this);
  lowVolLed->setImage(led_red);
  lowVolLed->setDisabledImage(led_red_disabled);
  lowVolLed->setGeometry(x0 + 171, y0 + 232, 31, 31);
  lowVolLed->setEnabled(true);

  lowVol = new QImageToggle(this);
  lowVol->setGeometry(x0 + 37, y0 + 232, 44, 31);
  lowVol->setImage(onoff);
  lowVol->setDisabledImage(onoff_disabled);
  lowVol->setEnabled(true);
  lowVol->setTag(CC_LOWVOLUME);
  lowVol->setLeftRight(true);
  lowVol->setValue(true);
  connect(lowVol, SIGNAL(valueChanged()), this, SLOT(toggleChanged()));

  mic = new QComboBox(this);
  mic->addItem("None");
  mic->addItem("57 Dynamic");
  mic->addItem("57 Dynamic, Off Axis");
  mic->addItem("409 Dynamic");
  mic->addItem("421 Dynamic");
  mic->addItem("4038 Ribbon");
  mic->addItem("121 Ribbon");
  mic->addItem("67 Condenser");
  mic->addItem("87 Condenser");
  mic->setGeometry(x0 + 62, y0 + 270, 134, 22);
  mic->setStyleSheet(comboStyle);
  connect(mic, SIGNAL(currentIndexChanged(int)), this, SLOT(micChanged(int)));

  QImageButton* midiButton = new QImageButton(this);
  midiButton->setGeometry(x0 + 11, y0 - 18, 65, 15);
  midiButton->setImage(midi);
  midiButton->setDisabledImage(midi_disabled);
  midiButton->setEnabled(true);
  connect(midiButton, SIGNAL(clicked()), this, SLOT(setupMIDI()));

  QImageButton* aboutButton = new QImageButton(this);
  aboutButton->setGeometry(x0 + 80, y0 - 18, 65, 15);
  aboutButton->setImage(about);
  aboutButton->setDisabledImage(about_disabled);
  aboutButton->setEnabled(true);
  connect(aboutButton, SIGNAL(clicked()), this, SLOT(about()));
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::getValuesFromDT()
////////////////////////////////////////////////////////////////////////////////
///\brief   Sync UI with the values from the actual DT.
///\param   [in] async: Is this called asyncally?
///\remarks This functions sends value request CCs to the DT. The UI is then
///         updated by the CC receive function.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::getValuesFromDT(bool async)
{
  // Avoid recursion:
  if (!receiveMutex.tryLock())
    return;

  // Lock UI:
  if (!async)
  {
    this->setEnabled(false);
    this->setCursor(Qt::WaitCursor);
  }

  sendControlChange(DT_MIDI_CHANNEL, 126, 127);

  // Send parameter requests:
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 0);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 17);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 18);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 19);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 29);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 30);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 31);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 32);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 33);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 34);
  Sleep(50);
  sendControlChange(DT_MIDI_CHANNEL, 83, 35);
  Sleep(50);

  // Force user interface release:
  sendBlockMessage(false);

  // Release UI:
  if (!async)
  {
    this->setCursor(Qt::ArrowCursor);
    this->setEnabled(true);
  }

  sendControlChange(DT_MIDI_CHANNEL, 126, 0);

  // Release mutex:
  receiveMutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::sendBlockMessage()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send ui block CCs to the DT.
///\param   [in] block: Block the UI?
///\remarks The DT just reflects everything that it receives at the input to
///         the output. So every clean to the DT is guarded by an unsed CC
///         with a boolean flag to either block or unblock the UI to avoid
///         MIDI feedback loops.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendBlockMessage(bool block)
{
  // Send the block message:
  sendControlChange(DT_MIDI_CHANNEL, 127, block ? 127 : 0);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::about()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the about buton signal.
///\remarks Shows an about box with informations about this application.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::about()
{
  // Create about box:
  AboutDialog dlg(this);
  dlg.exec();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::setupMIDI()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the File->Setup signal.
///\remarks Shows the MIDI setup dialog.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::setupMIDI()
{
  while (true)
  {
    // Show the setup window:
    if (!showSetupWindow())
      break;

    // Reopen ports:
    if (openMIDIPorts())
      break;

    // There was an error, ask user what to do:
    if (QMessageBox::question(this, tr("MIDI error"), tr("There was an error while establishing the MIDI connection to the device.\n\nWould you like to check the configuration?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
      break;
  }

  // Force unblock:
  blocked = false;
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::rotaryChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the dial changed event.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::rotaryChanged()
{
  // Is the UI locked?
  if (blocked)
    return;

  // Get sending control:
  QImageDial* dial = qobject_cast<QImageDial*>(sender());
  if (dial == 0)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, dial->tag(), (int)(dial->value() * 127.0) & 0xFF);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::rotaryReleased()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the dial released event.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::rotaryReleased()
{
  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::toggleChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the toggle changed event.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::toggleChanged()
{
  // Is the UI locked?
  if (blocked)
    return;

  // Get sending control:
  QImageToggle* toggle = qobject_cast<QImageToggle*>(sender());
  if (toggle == 0)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  if (toggle->tag() == CC_CHANNEL || toggle->tag() == CC_LOWVOLUME)
    sendControlChange(DT_MIDI_CHANNEL, toggle->tag(), toggle->value() ? : 127);
  else
    sendControlChange(DT_MIDI_CHANNEL, toggle->tag(), toggle->value() ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);

  // Update LEDs if needed:
  if (toggle->tag() == CC_REV_BYPASS_A)
    reverbLedA->setValue(toggle->value());
  else if (toggle->tag() == CC_REV_BYPASS_B)
    reverbLedB->setValue(toggle->value());
  if (toggle->tag() == CC_LOWVOLUME)
    lowVolLed->setValue(!toggle->value());
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::toggle4Changed()
////////////////////////////////////////////////////////////////////////////////
///\brief Handler for the toggle changed event.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::toggle4Changed()
{
  // Is the UI locked?
  if (blocked)
    return;

  // Get sending control:
  QImageToggle4* toggle = qobject_cast<QImageToggle4*>(sender());
  if (toggle == 0)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, toggle->tag(), toggle->value());

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  if (toggle->tag() == CC_VOICE_A || toggle->tag() == CC_VOICE_B)
    getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::ampAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the amp A combo box selection changed event.
///\param   [in] value: Index of the selected item.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::ampAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Load the amp's defaults too?
  if (!(QApplication::keyboardModifiers() & Qt::ShiftModifier))
  {
    // Block user interface:
    sendBlockMessage(true);

    // Send the value:
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_DEF_A, value);

    // Release the user interface:
    sendBlockMessage(false);

    // Sync state:
    getValuesFromDT();
  }
  else
  {
    sendBlockMessage(true);

    // Send the value:
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_A, value);

    // Release the user interface:
    sendBlockMessage(false);
  }
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::cabAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the cab A combo box selection changed event.
///\param   [in] value: Index of the selected item.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::cabAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_CAB_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb A combo box selection changed event.
///\param   [in] value: Index of the selected item.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TYPE_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::ampBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the amp B combo box selection changed event.
///\param   [in] value: Index of the selected item.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::ampBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Load the amp's defaults too?
  if (!(QApplication::keyboardModifiers() & Qt::ShiftModifier))
  {
    // Block user interface:
    sendBlockMessage(true);

    // Send the value:
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_DEF_B, value);

    // Release the user interface:
    sendBlockMessage(false);

    // Sync state:
    getValuesFromDT();
  }
  else
  {
    sendBlockMessage(true);

    // Send the value:
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_B, value);

    // Release the user interface:
    sendBlockMessage(false);
  }
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::cabBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the cab B combo box selection changed event.
///\param   [in] value: Index of the selected item.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::cabBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_CAB_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb B combo box selection changed event.
///\param   [in] value: Index of the selected item.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TYPE_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::micChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the mic combo box selection changed event.
///\param   [in] value: Index of the selected item.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::micChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_XLR_MIC, value);

  // Release the user interface:
  sendBlockMessage(false);
}

///////////////////////////////// End of File //////////////////////////////////
