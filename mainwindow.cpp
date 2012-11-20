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
#include <Qt/qdom.h>

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

  // Initialize status bar:
  statusBar()->setSizeGripEnabled(false);
  statusBar()->showMessage(tr("Ready."));

  // Crate all actions:
  createActions();

  // Create main menu:
  createMenu();

  // Create the main edit area:
  createEditArea();

  // Init size and position (screen center):
  int w = 975;
  int h = 480;
  int x = (QApplication::desktop()->width()  / 2) - (w / 2);
  int y = (QApplication::desktop()->height() / 2) - (h / 2);

  // Load settings:
  QSettings settings;
  x = settings.value("mainwindow/x",      QVariant(x)).toInt();
  y = settings.value("mainwindow/y",      QVariant(y)).toInt();
  midiInName  = settings.value("MIDI/inputName",  QVariant("")).toString();
  midiOutName = settings.value("MIDI/outputName", QVariant("")).toString();
  defaultsAction->setChecked(settings.value("misc/loadDefaults", QVariant(true)).toBool());

  // Place window:
  setGeometry(x, y, w, h);
  setFixedSize(w, h);
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
  //TODO: Check for unsaved documents etc.

  // Save window position:
  QSettings settings;
  QRect rc = geometry();
  settings.setValue("mainwindow/x",      rc.left());
  settings.setValue("mainwindow/y",      rc.top());
  settings.setValue("mainwindow/width",  rc.width());
  settings.setValue("mainwindow/height", rc.height());

  // Save MIDI state:
  settings.setValue("MIDI/inputName",  midiInName);
  settings.setValue("MIDI/outputName", midiOutName);

  // Save editor state:
  settings.setValue("misc/loadDefaults", defaultsAction->isChecked());

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

  if (blocked)
    return;

  switch (controlNumber)
  {
  case CC_AMP_A:
    ampA->setCurrentIndex(value);
    break;
  case CC_CAB_A:
    cabA->setCurrentIndex(value);
    break;
  case CC_GAIN_A:
    gainA->setValue(value);
    break;
  case CC_BASS_A:
    bassA->setValue(value);
    break;
  case CC_MIDDLE_A:
    middleA->setValue(value);
    break;
  case CC_TREBLE_A:
    trebleA->setValue(value);
    break;
  case CC_PRESENCE_A:
    presenceA->setValue(value);
    break;
  case CC_VOLUME_A:
    volumeA->setValue(value);
    break;
  case CC_VOICE_A:
    voiceA1->setChecked(value == 0);
    voiceA2->setChecked(value == 1);
    voiceA3->setChecked(value == 2);
    voiceA4->setChecked(value == 3);
    break;
  case CC_REV_BYPASS_A:
    reverbBypassA->setChecked(value >= 64);
    break;
  case CC_REV_TYPE_A:
    reverbA->setCurrentIndex(value);
    break;
  case CC_REV_DECAY_A:
    reverbDecayA->setValue(value);
    break;
  case CC_REV_PREDELAY_A:
    reverbPredelayA->setValue(value);
    break;
  case CC_REV_TONE_A:
    reverbToneA->setValue(value);
    break;
  case CC_REV_MIX_A:
    reverbMixA->setValue(value);
    break;
  case CC_CLASS_A:
    classA->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_TOPOL_A:
    topolA1->setChecked(value == 0);
    topolA2->setChecked(value == 1);
    topolA3->setChecked(value == 2);
    topolA4->setChecked(value == 3);
    break;
  case CC_XTODE_A:
    xtodeA->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_BOOST_A:
    boostA->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_PI_VOLTAGE_A:
    pivoltA->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_CAP_TYPE_A:
    capA->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_AMP_B:
    ampB->setCurrentIndex(value);
    break;
  case CC_CAB_B:
    cabB->setCurrentIndex(value);
    break;
  case CC_GAIN_B:
    gainB->setValue(value);
    break;
  case CC_BASS_B:
    bassB->setValue(value);
    break;
  case CC_MIDDLE_B:
    middleB->setValue(value);
    break;
  case CC_TREBLE_B:
    trebleB->setValue(value);
    break;
  case CC_PRESENCE_B:
    presenceB->setValue(value);
    break;
  case CC_VOLUME_B:
    volumeB->setValue(value);
    break;
  case CC_VOICE_B:
    voiceB1->setChecked(value == 0);
    voiceB2->setChecked(value == 1);
    voiceB3->setChecked(value == 2);
    voiceB4->setChecked(value == 3);
    break;
  case CC_REV_BYPASS_B:
    reverbBypassB->setChecked(value >= 64);
    break;
  case CC_REV_TYPE_B:
    reverbB->setCurrentIndex(value);
    break;
  case CC_REV_DECAY_B:
    reverbDecayB->setValue(value);
    break;
  case CC_REV_PREDELAY_B:
    reverbPredelayB->setValue(value);
    break;
  case CC_REV_TONE_B:
    reverbToneB->setValue(value);
    break;
  case CC_REV_MIX_B:
    reverbMixB->setValue(value);
    break;
  case CC_CLASS_B:
    classB->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_TOPOL_B:
    topolB1->setChecked(value == 0);
    topolB2->setChecked(value == 1);
    topolB3->setChecked(value == 2);
    topolB4->setChecked(value == 3);
    break;
  case CC_XTODE_B:
    xtodeB->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_BOOST_B:
    boostB->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_PI_VOLTAGE_B:
    pivoltB->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_CAP_TYPE_B:
    capB->setValue(value >= 64 ? 1 : 0);
    break;
  case CC_XLR_MIC:
    mic->setCurrentIndex(value);
    break;
  case CC_LOWVOLUME:
    lowVol->setChecked(value >= 64);
    break;
  case CC_CHANNEL:
    this->channel->setValue(value >= 64 ? 0 : 1);
    break;
  case CC_MASTER_VOL:
    master->setValue(value);
    break;
  }
}

void MainWindow::readFile(const QString& fileName)
{
  // Open the file:
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
  {
    // Warn user:
    QMessageBox::warning(this, tr("File error"), tr("Error while opening the file."), QMessageBox::Ok);
    return;
  }

  // Read the file:
  QDomDocument doc;
  if (!doc.setContent(&file))
  {
    // Warn user:
    QMessageBox::warning(this, tr("File error"), tr("Error while reading the file."), QMessageBox::Ok);
    file.close();
    return;
  }
  file.close();

  // Get root element:
  QDomElement root = doc.documentElement();
  if (root.tagName() != "dtsettings")
  {
    QMessageBox::warning(this, tr("File error"), tr("Error while reading the file: Root element not found."), QMessageBox::Ok);
    return;
  }

  // Block UI:
  sendBlockMessage(true);
  this->setEnabled(false);
  this->setCursor(Qt::WaitCursor);

  // Loop through children:
  QDomNode node = root.firstChild();
  while (!node.isNull())
  {
    // Get tag:
    QDomElement element = node.toElement();
    if (!element.isNull())
    {
      // Is this a channel node?
      if (element.tagName() == "channel")
      {
        // Get channel and voicing:
        int channel = element.attribute("value", "0").toInt();
        int voicing = element.attribute("selected_voicing", "0").toInt();

        // Loop through voicings:
        QDomNode voiceNode = node.firstChild();
        while (!voiceNode.isNull())
        {
          // Get tag
          QDomElement voiceElement = node.toElement();
          if (!voiceElement.isNull())
          {
            // Is this a channel node?
            if (voiceElement.tagName() == "voice")
            {
              // Get number of this voice:
              int voiceNumber = voiceElement.attribute("value", "0").toInt();

              // Switch to this voice:
              if (channel == 0)
                sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, voiceNumber);
              else
                sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, voiceNumber);
              Sleep(50);

              // Are we in channel A?
              if (channel == 0)
              {
                // Set values:
                sendControlChange(DT_MIDI_CHANNEL, CC_AMP_A,          voiceElement.attribute("amp", "1").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_A,     voiceElement.attribute("cab", "1").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_GAIN_A,         voiceElement.attribute("drive", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_BASS_A,         voiceElement.attribute("bass", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_MIDDLE_A,       voiceElement.attribute("middle", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_TREBLE_A,       voiceElement.attribute("treble", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_PRESENCE_A,     voiceElement.attribute("presence", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_VOLUME_A,       voiceElement.attribute("volume", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_CLASS_A,        voiceElement.attribute("class", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A,        voiceElement.attribute("topology", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_XTODE_A,        voiceElement.attribute("xtode", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_BOOST_A,        voiceElement.attribute("boost", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_PI_VOLTAGE_A,   voiceElement.attribute("pi_voltage", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_A,     voiceElement.attribute("cap", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_BYPASS_A,   voiceElement.attribute("reverb_enabled", "127").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_TYPE_A,     voiceElement.attribute("reverb_type", "1").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_DECAY_A,    voiceElement.attribute("reverb_decay", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_PREDELAY_A, voiceElement.attribute("reverb_predelay", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_TONE_A,     voiceElement.attribute("reverb_tone", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_MIX_A,      voiceElement.attribute("reverb_mix", "64").toInt());
              }
              else
              {
                sendControlChange(DT_MIDI_CHANNEL, CC_AMP_B,          voiceElement.attribute("amp", "1").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_B,     voiceElement.attribute("cab", "1").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_GAIN_B,         voiceElement.attribute("drive", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_BASS_B,         voiceElement.attribute("bass", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_MIDDLE_B,       voiceElement.attribute("middle", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_TREBLE_B,       voiceElement.attribute("treble", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_PRESENCE_B,     voiceElement.attribute("presence", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_VOLUME_B,       voiceElement.attribute("volume", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_CLASS_B,        voiceElement.attribute("class", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B,        voiceElement.attribute("topology", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_XTODE_B,        voiceElement.attribute("xtode", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_BOOST_B,        voiceElement.attribute("boost", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_PI_VOLTAGE_B,   voiceElement.attribute("pi_voltage", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_B,     voiceElement.attribute("cap", "0").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_BYPASS_B,   voiceElement.attribute("reverb_enabled", "127").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_TYPE_B,     voiceElement.attribute("reverb_type", "1").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_DECAY_B,    voiceElement.attribute("reverb_decay", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_PREDELAY_B, voiceElement.attribute("reverb_predelay", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_TONE_B,     voiceElement.attribute("reverb_tone", "64").toInt());
                sendControlChange(DT_MIDI_CHANNEL, CC_REV_MIX_B,      voiceElement.attribute("reverb_mix", "64").toInt());
              }
            }
          }

          // Next node:
          voiceNode = voiceNode.nextSibling();
        }

        // Set current voicing:
        if (channel == 0)
          sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, voicing);
        else
          sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, voicing);
      }

      // Is this the master node?
      else if (element.tagName() == "master")
      {
        // Set current channel:
        sendControlChange(DT_MIDI_CHANNEL, CC_CHANNEL, element.attribute("selected_channel", "0").toInt());
        Sleep(50);

        // Set volume and mic simualtion:
        sendControlChange(DT_MIDI_CHANNEL, CC_MASTER_VOL, element.attribute("master_volume", "0").toInt());
        sendControlChange(DT_MIDI_CHANNEL, CC_XLR_MIC,    element.attribute("xlr_mic", "1").toInt());
        sendControlChange(DT_MIDI_CHANNEL, CC_LOWVOLUME,  element.attribute("lowvolume_mode", "0").toInt());
      }
    }

    // Next node:
    node = node.nextSibling();
  }

  // Release UI:
  sendBlockMessage(false);
  this->setEnabled(true);
  this->setCursor(Qt::ArrowCursor);

  // Sync UI:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::createActions()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create all required actions.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::createActions()
{
  // File->Open:
  openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  openAction->setShortcuts(QKeySequence::Open);
  openAction->setStatusTip(tr("Load settings from file"));
  openAction->setIconVisibleInMenu(true);
  connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

  // File->Save:
  saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
  saveAction->setShortcuts(QKeySequence::Save);
  saveAction->setStatusTip(tr("Save current settings to file."));
  saveAction->setIconVisibleInMenu(true);
  connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

  // File->Save as:
  saveAsAction = new QAction(QIcon(":/images/save_as.png"), tr("Save &as..."), this);
  saveAsAction->setShortcuts(QKeySequence::SaveAs);
  saveAsAction->setStatusTip(tr("Save settings under a new name."));
  saveAsAction->setIconVisibleInMenu(true);
  connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

  // File->Quit:
  quitAction = new QAction(QIcon(":/images/close.png"), tr("&Quit"), this);
  quitAction->setShortcuts(QKeySequence::Quit);
  quitAction->setStatusTip(tr("Quit the application"));
  quitAction->setIconVisibleInMenu(true);
  connect(quitAction, SIGNAL(triggered()), this, SLOT(exitApplication()));

  // Options->Setup:
  setupAction = new QAction(QIcon(":/images/midi.png"), tr("&MIDI Setup..."), this);
  setupAction->setStatusTip(tr("Setup MIDI connections"));
  setupAction->setIconVisibleInMenu(true);
  connect(setupAction, SIGNAL(triggered()), this, SLOT(setupMIDI()));

  // Options->Set amps with defaults:
  defaultsAction = new QAction(tr("&Load amps with defaults"), this);
  defaultsAction->setStatusTip(tr("Load default power amp and cab setting when switching amp models"));
  defaultsAction->setCheckable(true);

  // Help->About:
  aboutAction = new QAction(QIcon(":/images/about.png"), tr("&About..."), this);
  aboutAction->setStatusTip(tr("Show the application's About box"));
  aboutAction->setIconVisibleInMenu(true);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  // Help->About Qt:
  aboutQtAction = new QAction(QIcon(":/images/qt.png"), tr("About &Qt..."), this);
  aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
  aboutQtAction->setIconVisibleInMenu(true);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::createMenu()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create the main menu.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::createMenu()
{
  // File menu:
  fileMenu = menuBar()->addMenu(tr("&File"));
  //fileMenu->addAction(openAction);
  //fileMenu->addAction(saveAction);
  //fileMenu->addAction(saveAsAction);
  //fileMenu->addSeparator();
  fileMenu->addAction(quitAction);

  // Options menu:
  optionsMenu = menuBar()->addMenu(tr("&Options"));
  optionsMenu->addAction(defaultsAction);
  optionsMenu->addSeparator();
  optionsMenu->addAction(setupAction);

  // Help menu:
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::createEditArea()
////////////////////////////////////////////////////////////////////////////////
///\brief   Create the main edit area.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::createEditArea()
{
  // Group box for the channel A preamp values:
  QGroupBox* group = new QGroupBox(this);
  group->setTitle("Preamp A");
  group->setFlat(false);
  group->setGeometry(15, menuBar()->height() + 8, 670, 92);

  // Channel A amp model selector:
  ampA = new QComboBox(group);
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
  ampA->setGeometry(50, 26, 134, 22);
  connect(ampA, SIGNAL(currentIndexChanged(int)), this, SLOT(ampAChanged(int)));
  QLabel* label = new QLabel(group);
  label->setText("Amp");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 30, 30, 20);

  // Channel A cabinet selector:
  cabA = new QComboBox(group);
  cabA->addItem("None");
  cabA->addItem("2x12 Blackface Double");
  cabA->addItem("4x12 Hiway");
  cabA->addItem("1x(6x9) Super O");
  cabA->addItem("1x12 Gibtone F-Coil");
  cabA->addItem("4x10 Tweed B-Man");
  cabA->addItem("1x12 Blackface ‘Lux");
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
  cabA->setGeometry(50, 53, 134, 22);
  connect(cabA, SIGNAL(currentIndexChanged(int)), this, SLOT(cabAChanged(int)));
  label = new QLabel(group);
  label->setText("Cab");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 58, 30, 20);

  // Channel A gain dial:
  gainA = new QDial(group);
  gainA->setMinimum(0);
  gainA->setMaximum(127);
  gainA->setGeometry(200, 26, 32, 32);
  gainA->setTracking(true);
  connect(gainA, SIGNAL(valueChanged(int)), this, SLOT(gainAChanged(int)));
  label = new QLabel(group);
  label->setText("Drive");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Channel A bass dial:
  bassA = new QDial(group);
  bassA->setMinimum(0);
  bassA->setMaximum(127);
  bassA->setGeometry(264, 26, 32, 32);
  bassA->setTracking(true);
  connect(bassA, SIGNAL(valueChanged(int)), this, SLOT(bassAChanged(int)));
  label = new QLabel(group);
  label->setText("Bass");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Channel A middle dial:
  middleA = new QDial(group);
  middleA->setMinimum(0);
  middleA->setMaximum(127);
  middleA->setGeometry(328, 26, 32, 32);
  middleA->setTracking(true);
  connect(middleA, SIGNAL(valueChanged(int)), this, SLOT(middleAChanged(int)));
  label = new QLabel(group);
  label->setText("Middle");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Channel A treble dial:
  trebleA = new QDial(group);
  trebleA->setMinimum(0);
  trebleA->setMaximum(127);
  trebleA->setGeometry(394, 26, 32, 32);
  trebleA->setTracking(true);
  connect(trebleA, SIGNAL(valueChanged(int)), this, SLOT(trebleAChanged(int)));
  label = new QLabel(group);
  label->setText("Treble");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(378, 58, 64, 20);

  // Channel A presence dial:
  presenceA = new QDial(group);
  presenceA->setMinimum(0);
  presenceA->setMaximum(127);
  presenceA->setGeometry(460, 26, 32, 32);
  presenceA->setTracking(true);
  connect(presenceA, SIGNAL(valueChanged(int)), this, SLOT(presenceAChanged(int)));
  label = new QLabel(group);
  label->setText("Presence");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(444, 58, 64, 20);

  // Channel A volume dial:
  volumeA = new QDial(group);
  volumeA->setMinimum(0);
  volumeA->setMaximum(127);
  volumeA->setGeometry(524, 26, 32, 32);
  volumeA->setTracking(group);
  connect(volumeA, SIGNAL(valueChanged(int)), this, SLOT(volumeAChanged(int)));
  label = new QLabel(group);
  label->setText("Volume");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(508, 58, 64, 20);

  // Channel A voice selector:
  voiceA1 = new QRadioButton(group);
  voiceA1->setText("I");
  voiceA1->setGeometry(580, 26, 40, 25);
  voiceA1->setChecked(true);
  connect(voiceA1, SIGNAL(toggled(bool)), this, SLOT(voiceA1toggled(bool)));
  voiceA2 = new QRadioButton(group);
  voiceA2->setText("II");
  voiceA2->setGeometry(620, 26, 40, 25);
  connect(voiceA2, SIGNAL(toggled(bool)), this, SLOT(voiceA2toggled(bool)));
  voiceA3 = new QRadioButton(group);
  voiceA3->setText("III");
  voiceA3->setGeometry(580, 51, 40, 25);
  connect(voiceA3, SIGNAL(toggled(bool)), this, SLOT(voiceA3toggled(bool)));
  voiceA4 = new QRadioButton(group);
  voiceA4->setText("IV");
  voiceA4->setGeometry(620, 51, 40, 25);
  connect(voiceA4, SIGNAL(toggled(bool)), this, SLOT(voiceA4toggled(bool)));

  // Group box for the channel A reverb values:
  group = new QGroupBox(this);
  group->setTitle("Reverb A");
  group->setFlat(false);
  group->setGeometry(235, menuBar()->height() + 110, 450, 92);

  // Reverb A bypass switch:
  reverbBypassA = new QCheckBox(group);
  reverbBypassA->setText("Enabled");
  connect(reverbBypassA, SIGNAL(stateChanged(int)), this, SLOT(reverbBypassAChanged(int)));
  reverbBypassA->setGeometry(10, 22, 134, 22);

  // Channel A reverb selector:
  reverbA = new QComboBox(group);
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
  reverbA->setGeometry(50, 53, 134, 22);
  connect(reverbA, SIGNAL(currentIndexChanged(int)), this, SLOT(reverbAChanged(int)));
  label = new QLabel(group);
  label->setText("Type");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 58, 30, 20);

  // Reverb A decay dial:
  reverbDecayA = new QDial(group);
  reverbDecayA->setMinimum(0);
  reverbDecayA->setMaximum(127);
  reverbDecayA->setGeometry(200, 26, 32, 32);
  reverbDecayA->setTracking(true);
  connect(reverbDecayA, SIGNAL(valueChanged(int)), this, SLOT(reverbDecayAChanged(int)));
  label = new QLabel(group);
  label->setText("Decay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Reverb A predelay dial:
  reverbPredelayA = new QDial(group);
  reverbPredelayA->setMinimum(0);
  reverbPredelayA->setMaximum(127);
  reverbPredelayA->setGeometry(264, 26, 32, 32);
  reverbPredelayA->setTracking(true);
  connect(reverbPredelayA, SIGNAL(valueChanged(int)), this, SLOT(reverbPredelayAChanged(int)));
  label = new QLabel(group);
  label->setText("Pre Delay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Reverb A tone dial:
  reverbToneA = new QDial(group);
  reverbToneA->setMinimum(0);
  reverbToneA->setMaximum(127);
  reverbToneA->setGeometry(328, 26, 32, 32);
  reverbToneA->setTracking(true);
  connect(reverbToneA, SIGNAL(valueChanged(int)), this, SLOT(reverbToneAChanged(int)));
  label = new QLabel(group);
  label->setText("Tone");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Reverb A mix dial:
  reverbMixA = new QDial(group);
  reverbMixA->setMinimum(0);
  reverbMixA->setMaximum(127);
  reverbMixA->setGeometry(394, 26, 32, 32);
  reverbMixA->setTracking(true);
  connect(reverbMixA, SIGNAL(valueChanged(int)), this, SLOT(reverbMixAChanged(int)));
  label = new QLabel(group);
  label->setText("Mix");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(378, 58, 64, 20);

  // Group box for the channel A poweramp values:
  group = new QGroupBox(this);
  group->setTitle("Poweramp A");
  group->setFlat(false);
  group->setGeometry(700, menuBar()->height() + 8, 255, 194);

  // Channel A Class A/AB switch:
  classA = new QSlider(group);
  classA->setGeometry(26, 40, 32, 28);
  classA->setMinimum(0);
  classA->setMaximum(1);
  classA->setTickPosition(QSlider::NoTicks);
  connect(classA, SIGNAL(valueChanged(int)), this, SLOT(classAChanged(int)));
  label = new QLabel(group);
  label->setText("Class A/B");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 20, 64, 20);
  label = new QLabel(group);
  label->setText("Class A");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 68, 64, 20);

  // Channel A Pentode/Triode switch:
  xtodeA = new QSlider(group);
  xtodeA->setGeometry(196, 40, 32, 28);
  xtodeA->setMinimum(0);
  xtodeA->setMaximum(1);
  xtodeA->setTickPosition(QSlider::NoTicks);
  connect(xtodeA, SIGNAL(valueChanged(int)), this, SLOT(xtodeAChanged(int)));
  label = new QLabel(group);
  label->setText("Pentode");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 20, 64, 20);
  label = new QLabel(group);
  label->setText("Triode");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 68, 64, 20);

  // Channel A topology selector:
  topolA1 = new QRadioButton(group);
  topolA1->setText("I");
  topolA1->setGeometry(90, 18, 40, 25);
  topolA1->setChecked(true);
  connect(topolA1, SIGNAL(toggled(bool)), this, SLOT(topolA1toggled(bool)));
  topolA2 = new QRadioButton(group);
  topolA2->setText("II");
  topolA2->setGeometry(130, 18, 40, 25);
  connect(topolA2, SIGNAL(toggled(bool)), this, SLOT(topolA2toggled(bool)));
  topolA3 = new QRadioButton(group);
  topolA3->setText("III");
  topolA3->setGeometry(90, 43, 40, 25);
  connect(topolA3, SIGNAL(toggled(bool)), this, SLOT(topolA3toggled(bool)));
  topolA4 = new QRadioButton(group);
  topolA4->setText("IV");
  topolA4->setGeometry(130, 43, 40, 25);
  connect(topolA4, SIGNAL(toggled(bool)), this, SLOT(topolA4toggled(bool)));
  label = new QLabel(group);
  label->setText("Topology");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(90, 68, 80, 20);

  // Channel A boost switch:
  boostA = new QSlider(group);
  boostA->setGeometry(26, 132, 32, 28);
  boostA->setMinimum(0);
  boostA->setMaximum(1);
  boostA->setTickPosition(QSlider::NoTicks);
  connect(boostA, SIGNAL(valueChanged(int)), this, SLOT(boostAChanged(int)));
  label = new QLabel(group);
  label->setText("On");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 112, 64, 20);
  label = new QLabel(group);
  label->setText("Boost");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 160, 64, 20);

  // Channel A PI voltage switch:
  pivoltA = new QSlider(group);
  pivoltA->setGeometry(110, 132, 32, 28);
  pivoltA->setMinimum(0);
  pivoltA->setMaximum(1);
  pivoltA->setTickPosition(QSlider::NoTicks);
  connect(pivoltA, SIGNAL(valueChanged(int)), this, SLOT(pivoltAChanged(int)));
  label = new QLabel(group);
  label->setText("PIV High");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 112, 64, 20);
  label = new QLabel(group);
  label->setText("PIV Low");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 160, 64, 20);

  // Channel A Cap X/Y switch:
  capA = new QSlider(group);
  capA->setGeometry(196, 132, 32, 28);
  capA->setMinimum(0);
  capA->setMaximum(1);
  capA->setTickPosition(QSlider::NoTicks);
  connect(capA, SIGNAL(valueChanged(int)), this, SLOT(capAChanged(int)));
  label = new QLabel(group);
  label->setText("Tight");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 112, 64, 20);
  label = new QLabel(group);
  label->setText("Smooth");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 160, 64, 20);

  // Group box for the channel B preamp values:
  group = new QGroupBox(this);
  group->setTitle("Preamp B");
  group->setFlat(false);
  group->setGeometry(15, menuBar()->height() + 314, 670, 92);

  // Channel B amp model selector:
  ampB = new QComboBox(group);
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
  ampB->setGeometry(50, 26, 134, 22);
  connect(ampB, SIGNAL(currentIndexChanged(int)), this, SLOT(ampBChanged(int)));
  label = new QLabel(group);
  label->setText("Amp");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 30, 30, 20);

  // Channel B cabinet selector:
  cabB = new QComboBox(group);
  cabB->addItem("None");
  cabB->addItem("2x12 Blackface Double");
  cabB->addItem("4x12 Hiway");
  cabB->addItem("1x(6x9) Super O");
  cabB->addItem("1x12 Gibtone F-Coil");
  cabB->addItem("4x10 Tweed B-Man");
  cabB->addItem("1x12 Blackface ‘Lux");
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
  cabB->setGeometry(50, 53, 134, 22);
  connect(cabB, SIGNAL(currentIndexChanged(int)), this, SLOT(cabBChanged(int)));
  label = new QLabel(group);
  label->setText("Cab");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 58, 30, 20);

  // Channel B gain dial:
  gainB = new QDial(group);
  gainB->setMinimum(0);
  gainB->setMaximum(127);
  gainB->setGeometry(200, 26, 32, 32);
  gainB->setTracking(true);
  connect(gainB, SIGNAL(valueChanged(int)), this, SLOT(gainBChanged(int)));
  label = new QLabel(group);
  label->setText("Drive");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Channel B bass dial:
  bassB = new QDial(group);
  bassB->setMinimum(0);
  bassB->setMaximum(127);
  bassB->setGeometry(264, 26, 32, 32);
  bassB->setTracking(true);
  connect(bassB, SIGNAL(valueChanged(int)), this, SLOT(bassBChanged(int)));
  label = new QLabel(group);
  label->setText("Bass");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Channel B middle dial:
  middleB = new QDial(group);
  middleB->setMinimum(0);
  middleB->setMaximum(127);
  middleB->setGeometry(328, 26, 32, 32);
  middleB->setTracking(true);
  connect(middleB, SIGNAL(valueChanged(int)), this, SLOT(middleBChanged(int)));
  label = new QLabel(group);
  label->setText("Middle");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Channel B treble dial:
  trebleB = new QDial(group);
  trebleB->setMinimum(0);
  trebleB->setMaximum(127);
  trebleB->setGeometry(394, 26, 32, 32);
  trebleB->setTracking(true);
  connect(trebleB, SIGNAL(valueChanged(int)), this, SLOT(trebleBChanged(int)));
  label = new QLabel(group);
  label->setText("Treble");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(378, 58, 64, 20);

  // Channel B presence dial:
  presenceB = new QDial(group);
  presenceB->setMinimum(0);
  presenceB->setMaximum(127);
  presenceB->setGeometry(460, 26, 32, 32);
  presenceB->setTracking(true);
  connect(presenceB, SIGNAL(valueChanged(int)), this, SLOT(presenceBChanged(int)));
  label = new QLabel(group);
  label->setText("Presence");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(444, 58, 64, 20);

  // Channel B volume dial:
  volumeB = new QDial(group);
  volumeB->setMinimum(0);
  volumeB->setMaximum(127);
  volumeB->setGeometry(524, 26, 32, 32);
  volumeB->setTracking(group);
  connect(volumeB, SIGNAL(valueChanged(int)), this, SLOT(volumeBChanged(int)));
  label = new QLabel(group);
  label->setText("Volume");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(508, 58, 64, 20);

  // Channel B voice selector:
  voiceB1 = new QRadioButton(group);
  voiceB1->setText("I");
  voiceB1->setGeometry(580, 26, 40, 25);
  voiceB1->setChecked(true);
  connect(voiceB1, SIGNAL(toggled(bool)), this, SLOT(voiceB1toggled(bool)));
  voiceB2 = new QRadioButton(group);
  voiceB2->setText("II");
  voiceB2->setGeometry(620, 26, 40, 25);
  connect(voiceB2, SIGNAL(toggled(bool)), this, SLOT(voiceB2toggled(bool)));
  voiceB3 = new QRadioButton(group);
  voiceB3->setText("III");
  voiceB3->setGeometry(580, 51, 40, 25);
  connect(voiceB3, SIGNAL(toggled(bool)), this, SLOT(voiceB3toggled(bool)));
  voiceB4 = new QRadioButton(group);
  voiceB4->setText("IV");
  voiceB4->setGeometry(620, 51, 40, 25);
  connect(voiceB4, SIGNAL(toggled(bool)), this, SLOT(voiceB4toggled(bool)));

  // Group box for the channel B reverb values:
  group = new QGroupBox(this);
  group->setTitle("Reverb B");
  group->setFlat(false);
  group->setGeometry(235, menuBar()->height() + 212, 450, 92);

  // Reverb B bypass switch:
  reverbBypassB = new QCheckBox(group);
  reverbBypassB->setText("Enabled");
  connect(reverbBypassB, SIGNAL(stateChanged(int)), this, SLOT(reverbBypassBChanged(int)));
  reverbBypassB->setGeometry(10, 22, 134, 22);

  // Channel B reverb selector:
  reverbB = new QComboBox(group);
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
  reverbB->setGeometry(50, 53, 134, 22);
  connect(reverbB, SIGNAL(currentIndexChanged(int)), this, SLOT(reverbBChanged(int)));
  label = new QLabel(group);
  label->setText("Type");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 58, 30, 20);

  // Reverb B decay dial:
  reverbDecayB = new QDial(group);
  reverbDecayB->setMinimum(0);
  reverbDecayB->setMaximum(127);
  reverbDecayB->setGeometry(200, 26, 32, 32);
  reverbDecayB->setTracking(true);
  connect(reverbDecayB, SIGNAL(valueChanged(int)), this, SLOT(reverbDecayBChanged(int)));
  label = new QLabel(group);
  label->setText("Decay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Reverb B predelay dial:
  reverbPredelayB = new QDial(group);
  reverbPredelayB->setMinimum(0);
  reverbPredelayB->setMaximum(127);
  reverbPredelayB->setGeometry(264, 26, 32, 32);
  reverbPredelayB->setTracking(true);
  connect(reverbPredelayB, SIGNAL(valueChanged(int)), this, SLOT(reverbPredelayBChanged(int)));
  label = new QLabel(group);
  label->setText("Pre Delay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Reverb B tone dial:
  reverbToneB = new QDial(group);
  reverbToneB->setMinimum(0);
  reverbToneB->setMaximum(127);
  reverbToneB->setGeometry(328, 26, 32, 32);
  reverbToneB->setTracking(true);
  connect(reverbToneB, SIGNAL(valueChanged(int)), this, SLOT(reverbToneBChanged(int)));
  label = new QLabel(group);
  label->setText("Tone");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Reverb B mix dial:
  reverbMixB = new QDial(group);
  reverbMixB->setMinimum(0);
  reverbMixB->setMaximum(127);
  reverbMixB->setGeometry(394, 26, 32, 32);
  reverbMixB->setTracking(true);
  connect(reverbMixB, SIGNAL(valueChanged(int)), this, SLOT(reverbMixBChanged(int)));
  label = new QLabel(group);
  label->setText("Mix");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(378, 58, 64, 20);

  // Group box for the channel B poweramp values:
  group = new QGroupBox(this);
  group->setTitle("Poweramp B");
  group->setFlat(false);
  group->setGeometry(700, menuBar()->height() + 212, 255, 194);

  // Channel B Class A/AB switch:
  classB = new QSlider(group);
  classB->setGeometry(26, 40, 32, 28);
  classB->setMinimum(0);
  classB->setMaximum(1);
  classB->setTickPosition(QSlider::NoTicks);
  connect(classB, SIGNAL(valueChanged(int)), this, SLOT(classBChanged(int)));
  label = new QLabel(group);
  label->setText("Class A/B");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 20, 64, 20);
  label = new QLabel(group);
  label->setText("Class A");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 68, 64, 20);

  // Channel B Pentode/Triode switch:
  xtodeB = new QSlider(group);
  xtodeB->setGeometry(196, 40, 32, 28);
  xtodeB->setMinimum(0);
  xtodeB->setMaximum(1);
  xtodeB->setTickPosition(QSlider::NoTicks);
  connect(xtodeB, SIGNAL(valueChanged(int)), this, SLOT(xtodeBChanged(int)));
  label = new QLabel(group);
  label->setText("Pentode");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 20, 64, 20);
  label = new QLabel(group);
  label->setText("Triode");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 68, 64, 20);

  // Channel B topology selector:
  topolB1 = new QRadioButton(group);
  topolB1->setText("I");
  topolB1->setGeometry(90, 18, 40, 25);
  topolB1->setChecked(true);
  connect(topolB1, SIGNAL(toggled(bool)), this, SLOT(topolB1toggled(bool)));
  topolB2 = new QRadioButton(group);
  topolB2->setText("II");
  topolB2->setGeometry(130, 18, 40, 25);
  connect(topolB2, SIGNAL(toggled(bool)), this, SLOT(topolB2toggled(bool)));
  topolB3 = new QRadioButton(group);
  topolB3->setText("III");
  topolB3->setGeometry(90, 43, 40, 25);
  connect(topolB3, SIGNAL(toggled(bool)), this, SLOT(topolB3toggled(bool)));
  topolB4 = new QRadioButton(group);
  topolB4->setText("IV");
  topolB4->setGeometry(130, 43, 40, 25);
  connect(topolB4, SIGNAL(toggled(bool)), this, SLOT(topolB4toggled(bool)));
  label = new QLabel(group);
  label->setText("Topology");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(90, 68, 80, 20);

  // Channel B boost switch:
  boostB = new QSlider(group);
  boostB->setGeometry(26, 132, 32, 28);
  boostB->setMinimum(0);
  boostB->setMaximum(1);
  boostB->setTickPosition(QSlider::NoTicks);
  connect(boostB, SIGNAL(valueChanged(int)), this, SLOT(boostBChanged(int)));
  label = new QLabel(group);
  label->setText("On");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 112, 64, 20);
  label = new QLabel(group);
  label->setText("Boost");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 160, 64, 20);

  // Channel B PI voltage switch:
  pivoltB = new QSlider(group);
  pivoltB->setGeometry(110, 132, 32, 28);
  pivoltB->setMinimum(0);
  pivoltB->setMaximum(1);
  pivoltB->setTickPosition(QSlider::NoTicks);
  connect(pivoltB, SIGNAL(valueChanged(int)), this, SLOT(pivoltBChanged(int)));
  label = new QLabel(group);
  label->setText("PIV High");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 112, 64, 20);
  label = new QLabel(group);
  label->setText("PIV Low");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 160, 64, 20);

  // Channel B Cap X/Y switch:
  capB = new QSlider(group);
  capB->setGeometry(196, 132, 32, 28);
  capB->setMinimum(0);
  capB->setMaximum(1);
  capB->setTickPosition(QSlider::NoTicks);
  connect(capB, SIGNAL(valueChanged(int)), this, SLOT(capBChanged(int)));
  label = new QLabel(group);
  label->setText("Tight");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 112, 64, 20);
  label = new QLabel(group);
  label->setText("Smooth");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(180, 160, 64, 20);

  // Group box for the global values:
  group = new QGroupBox(this);
  group->setTitle("Master");
  group->setFlat(false);
  group->setGeometry(15, menuBar()->height() + 110, 205, 194);

  // Channel switch:
  channel = new QSlider(group);
  channel->setGeometry(46, 55, 32, 28);
  channel->setMinimum(0);
  channel->setMaximum(1);
  channel->setTickPosition(QSlider::NoTicks);
  connect(channel, SIGNAL(valueChanged(int)), this, SLOT(channelChanged(int)));
  label = new QLabel(group);
  label->setText("Channel A");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(30, 35, 64, 20);
  label = new QLabel(group);
  label->setText("Channel B");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(30, 83, 64, 20);

  // Master volume dial:
  master = new QDial(group);
  master->setMinimum(0);
  master->setMaximum(127);
  master->setGeometry(130, 51, 32, 32);
  master->setTracking(true);
  connect(master, SIGNAL(valueChanged(int)), this, SLOT(masterChanged(int)));
  label = new QLabel(group);
  label->setText("Volume");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(114, 83, 64, 20);

  // Low volume mode switch:
  lowVol = new QCheckBox(group);
  lowVol->setText("Low Volume Mode");
  connect(lowVol, SIGNAL(stateChanged(int)), this, SLOT(lowVolChanged(int)));
  lowVol->setGeometry(10, 125, 134, 22);

  // Master mic selector:
  mic = new QComboBox(group);
  mic->addItem("None");
  mic->addItem("57 Dynamic");
  mic->addItem("57 Dynamic, Off Axis");
  mic->addItem("409 Dynamic");
  mic->addItem("421 Dynamic");
  mic->addItem("4038 Ribbon");
  mic->addItem("121 Ribbon");
  mic->addItem("67 Condenser");
  mic->addItem("87 Condenser");
  mic->setGeometry(50, 156, 134, 22);
  connect(mic, SIGNAL(currentIndexChanged(int)), this, SLOT(micChanged(int)));
  label = new QLabel(group);
  label->setText("Mic");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 161, 30, 20);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::getValuesFromDT()
////////////////////////////////////////////////////////////////////////////////
///\brief   Sync UI with the values from the actual DT.
///\remarks This functions sends value request CCs to the DT. The UI is then
///         updated by the CC receive function.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::getValuesFromDT()
{
  // Lock UI:
  this->setEnabled(false);
  this->setCursor(Qt::WaitCursor);

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

  // Release UI
  this->setCursor(Qt::ArrowCursor);
  this->setEnabled(true);
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
// MainWindow::exitApplication()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the Exit-Application signal.
///\remarks Closes the window and quits the application.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::exitApplication()
{
  // Close this window:
  close();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::about()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the Help->About signal.
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
  if (defaultsAction->isChecked())
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
// MainWindow::gainAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the gain A dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::gainAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_GAIN_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::bassAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the bass A dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::bassAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_BASS_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::middleAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the middle A dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::middleAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_MIDDLE_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::trebleAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the treble A dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::trebleAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TREBLE_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::presenceAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the presence A dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::presenceAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_PRESENCE_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

//////////////////////////////////////////////////////////////////////////////
// MainWindow::volumeAChanged()
//////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the volume A dial changed event.
///\param   [in] value: New value of the dial.
//////////////////////////////////////////////////////////////////////////////
void MainWindow::volumeAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOLUME_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceA1toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice A1 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceA1toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 0);

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceA2toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice A2 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceA2toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 1);

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceA3toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice A3 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceA3toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 2);

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceA4toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice A4 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceA4toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 3);

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbBypassAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb A bypass checkbox change event.
///\param   [in] value: New state of the box.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbBypassAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_BYPASS_A, value == Qt::Checked ? 127 : 0);

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
// MainWindow::reverbDecayAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb A decay dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbDecayAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_DECAY_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbPredelayAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb A pre delay dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbPredelayAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_PREDELAY_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbToneAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb A tone dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbToneAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TONE_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbMixAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb A mix dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbMixAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_MIX_A, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::classAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the class A/AB slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::classAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_CLASS_A, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::xtodeAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the pentode/triode slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::xtodeAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_XTODE_A, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::boostAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the boost on/off slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::boostAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_BOOST_A, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::pivoltAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the PI voltage slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::pivoltAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_PI_VOLTAGE_A, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::capAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the cap X/Y slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::capAChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_A, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::capAChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the cap X/Y slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolA1toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::topolA2toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the topology A2 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolA2toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 1);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::topolA3toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the topology A3 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolA3toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 2);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::topolA4toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the topology A4 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolA4toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 3);

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

  if (defaultsAction->isChecked())
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
// MainWindow::gainBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the gain B dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::gainBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_GAIN_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::bassBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the bass B dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::bassBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_BASS_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::middleBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the middle B dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::middleBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_MIDDLE_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::trebleBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the treble B dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::trebleBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TREBLE_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::presenceBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the presence B dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::presenceBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_PRESENCE_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::volumeBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the volume B dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::volumeBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOLUME_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceB1toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice B1 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceB1toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 0);

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceB2toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice B2 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceB2toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 1);

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceB3toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice B3 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceB3toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 2);

  // Release the user interface:
  sendBlockMessage(false);

  // Sync state:
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::voiceB4toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the voice B4 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::voiceB4toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 3);

  // Release the user interface:
  sendBlockMessage(false);
  getValuesFromDT();
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbBypassBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb B bypass checkbox change event.
///\param   [in] value: New state of the box.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbBypassBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_BYPASS_B, value == Qt::Checked ? 127 : 0);

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
// MainWindow::reverbDecayBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb B decay dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbDecayBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_DECAY_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbPredelayBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb B predelay dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbPredelayBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_PREDELAY_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbToneBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb B tone dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbToneBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TONE_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::reverbMixBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the reverb B mix dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::reverbMixBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_MIX_B, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::classBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the class A/AB slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::classBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_CLASS_B, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::xtodeBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the pentode/triode slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::xtodeBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_XTODE_B, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::boostBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the boost on/off slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::boostBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_BOOST_B, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::pivoltBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the PI voltage slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::pivoltBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_PI_VOLTAGE_B, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::capBChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the cap X/Y slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::capBChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_B, value ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::topolB1toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the topology B1 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolB1toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::topolB2toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the topology B2 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolB2toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 1);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::topolB3toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the topology B3 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolB3toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 2);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::topolB4toggled()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the topology B4 radio button toggle event.
///\param   [in] checked: New state of the button.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::topolB4toggled(bool checked)
{
  // Is the UI locked?
  if (blocked || !checked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 3);

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

////////////////////////////////////////////////////////////////////////////////
// MainWindow::lowVolChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the low volume checkbox change event.
///\param   [in] value: New state of the box.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::lowVolChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_LOWVOLUME, value == Qt::Checked ? 127 : 0);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::capchannelChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for channel slider changed event.
///\param   [in] value: New value of the slider.
///\remarks The slider value can only be 0 and 1 so it's working as a switch.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::channelChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_CHANNEL, value ? 0 : 127);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::masterChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the master dial changed event.
///\param   [in] value: New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::masterChanged(int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, CC_MASTER_VOL, value);

  // Release the user interface:
  sendBlockMessage(false);
}

///////////////////////////////// End of File //////////////////////////////////
