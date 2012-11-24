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
    gainA->setValue(value);
    gainA->blockSignals(oldState);
    break;
  case CC_BASS_A:
    oldState = bassA->blockSignals(true);
    bassA->setValue(value);
    bassA->blockSignals(oldState);
    break;
  case CC_MIDDLE_A:
    oldState = middleA->blockSignals(true);
    middleA->setValue(value);
    middleA->blockSignals(oldState);
    break;
  case CC_TREBLE_A:
    oldState = trebleA->blockSignals(true);
    trebleA->setValue(value);
    trebleA->blockSignals(oldState);
    break;
  case CC_PRESENCE_A:
    oldState = presenceA->blockSignals(true);
    presenceA->setValue(value);
    presenceA->blockSignals(oldState);
    break;
  case CC_VOLUME_A:
    oldState = volumeA->blockSignals(true);
    volumeA->setValue(value);
    volumeA->blockSignals(oldState);
    break;
  case CC_VOICE_A:
    oldState = voiceA1->blockSignals(true);
    voiceA1->setChecked(value == 0);
    voiceA1->blockSignals(oldState);
    oldState = voiceA2->blockSignals(true);
    voiceA2->setChecked(value == 1);
    voiceA2->blockSignals(oldState);
    oldState = voiceA3->blockSignals(true);
    voiceA3->setChecked(value == 2);
    voiceA3->blockSignals(oldState);
    oldState = voiceA4->blockSignals(true);
    voiceA4->setChecked(value == 3);
    voiceA4->blockSignals(oldState);
    break;
  case CC_REV_BYPASS_A:
    oldState = reverbBypassA->blockSignals(true);
    reverbBypassA->setChecked(value >= 64);
    reverbBypassA->blockSignals(oldState);
    break;
  case CC_REV_TYPE_A:
    oldState = reverbA->blockSignals(true);
    reverbA->setCurrentIndex(value);
    reverbA->blockSignals(oldState);
    break;
  case CC_REV_DECAY_A:
    oldState = reverbDecayA->blockSignals(true);
    reverbDecayA->setValue(value);
    reverbDecayA->blockSignals(oldState);
    break;
  case CC_REV_PREDELAY_A:
    oldState = reverbPredelayA->blockSignals(true);
    reverbPredelayA->setValue(value);
    reverbPredelayA->blockSignals(oldState);
    break;
  case CC_REV_TONE_A:
    oldState = reverbToneA->blockSignals(true);
    reverbToneA->setValue(value);
    reverbToneA->blockSignals(oldState);
    break;
  case CC_REV_MIX_A:
    oldState = reverbMixA->blockSignals(true);
    reverbMixA->setValue(value);
    reverbMixA->blockSignals(oldState);
    break;
  case CC_CLASS_A:
    oldState = classA->blockSignals(true);
    classA->setValue(value >= 64 ? 1 : 0);
    classA->blockSignals(oldState);
    break;
  case CC_TOPOL_A:
    oldState = topolA1->blockSignals(true);
    topolA1->setChecked(value == 0);
    topolA1->blockSignals(oldState);
    oldState = topolA2->blockSignals(true);
    topolA2->setChecked(value == 1);
    topolA2->blockSignals(oldState);
    oldState = topolA3->blockSignals(true);
    topolA3->setChecked(value == 2);
    topolA3->blockSignals(oldState);
    oldState = topolA4->blockSignals(true);
    topolA4->setChecked(value == 3);
    topolA4->blockSignals(oldState);
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
    gainB->setValue(value);
    gainB->blockSignals(oldState);
    break;
  case CC_BASS_B:
    oldState = bassB->blockSignals(true);
    bassB->setValue(value);
    bassB->blockSignals(oldState);
    break;
  case CC_MIDDLE_B:
    oldState = middleB->blockSignals(true);
    middleB->setValue(value);
    middleB->blockSignals(oldState);
    break;
  case CC_TREBLE_B:
    oldState = trebleB->blockSignals(true);
    trebleB->setValue(value);
    trebleB->blockSignals(oldState);
    break;
  case CC_PRESENCE_B:
    oldState = presenceB->blockSignals(true);
    presenceB->setValue(value);
    presenceB->blockSignals(oldState);
    break;
  case CC_VOLUME_B:
    oldState = volumeB->blockSignals(true);
    volumeB->setValue(value);
    volumeB->blockSignals(oldState);
    break;
  case CC_VOICE_B:
    oldState = voiceB1->blockSignals(true);
    voiceB1->setChecked(value == 0);
    voiceB1->blockSignals(oldState);
    oldState = voiceB2->blockSignals(true);
    voiceB2->setChecked(value == 1);
    voiceB2->blockSignals(oldState);
    oldState = voiceB3->blockSignals(true);
    voiceB3->setChecked(value == 2);
    voiceB3->blockSignals(oldState);
    oldState = voiceB4->blockSignals(true);
    voiceB4->setChecked(value == 3);
    voiceB4->blockSignals(oldState);
    break;
  case CC_REV_BYPASS_B:
    oldState = reverbBypassB->blockSignals(true);
    reverbBypassB->setChecked(value >= 64);
    reverbBypassB->blockSignals(oldState);
    break;
  case CC_REV_TYPE_B:
    oldState = reverbB->blockSignals(true);
    reverbB->setCurrentIndex(value);
    reverbB->blockSignals(oldState);
    break;
  case CC_REV_DECAY_B:
    oldState = reverbDecayB->blockSignals(true);
    reverbDecayB->setValue(value);
    reverbDecayB->blockSignals(oldState);
    break;
  case CC_REV_PREDELAY_B:
    oldState = reverbPredelayB->blockSignals(true);
    reverbPredelayB->setValue(value);
    reverbPredelayB->blockSignals(oldState);
    break;
  case CC_REV_TONE_B:
    oldState = reverbToneB->blockSignals(true);
    reverbToneB->setValue(value);
    reverbToneB->blockSignals(oldState);
    break;
  case CC_REV_MIX_B:
    oldState = reverbMixB->blockSignals(true);
    reverbMixB->setValue(value);
    reverbMixB->blockSignals(oldState);
    break;
  case CC_CLASS_B:
    oldState = classB->blockSignals(true);
    classB->setValue(value >= 64 ? 1 : 0);
    classB->blockSignals(oldState);
    break;
  case CC_TOPOL_B:
    oldState = topolB1->blockSignals(true);
    topolB1->setChecked(value == 0);
    topolB1->blockSignals(oldState);
    oldState = topolB2->blockSignals(true);
    topolB2->setChecked(value == 1);
    topolB2->blockSignals(oldState);
    oldState = topolB3->blockSignals(true);
    topolB3->setChecked(value == 2);
    topolB3->blockSignals(oldState);
    oldState = topolB4->blockSignals(true);
    topolB4->setChecked(value == 3);
    topolB4->blockSignals(oldState);
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
    lowVol->setChecked(value >= 64);
    lowVol->blockSignals(oldState);
    break;
  case CC_CHANNEL:
    oldState = this->channel->blockSignals(true);
    this->channel->setValue(value >= 64 ? 0 : 1);
    this->channel->blockSignals(oldState);
    break;
  case CC_MASTER_VOL:
    oldState = master->blockSignals(true);
    master->setValue(value);
    master->blockSignals(oldState);
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
  gainA = new DTDial(group, CC_GAIN_A);
  gainA->setGeometry(200, 26, 32, 32);
  connect(gainA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Drive");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Channel A bass dial:
  bassA = new DTDial(group, CC_BASS_A);
  bassA->setGeometry(264, 26, 32, 32);
  connect(bassA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Bass");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Channel A middle dial:
  middleA = new DTDial(group, CC_MIDDLE_A);
  middleA->setGeometry(328, 26, 32, 32);
  connect(middleA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Middle");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Channel A treble dial:
  trebleA = new DTDial(group, CC_TREBLE_A);
  trebleA->setGeometry(394, 26, 32, 32);
  connect(trebleA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Treble");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(378, 58, 64, 20);

  // Channel A presence dial:
  presenceA = new DTDial(group, CC_PRESENCE_A);
  presenceA->setGeometry(460, 26, 32, 32);
  connect(presenceA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Presence");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(444, 58, 64, 20);

  // Channel A volume dial:
  volumeA = new DTDial(group, CC_VOLUME_A);
  volumeA->setGeometry(524, 26, 32, 32);
  connect(volumeA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
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
  reverbDecayA = new DTDial(group, CC_REV_DECAY_A);
  reverbDecayA->setGeometry(200, 26, 32, 32);
  connect(reverbDecayA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Decay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Reverb A predelay dial:
  reverbPredelayA = new DTDial(group, CC_REV_PREDELAY_A);
  reverbPredelayA->setGeometry(264, 26, 32, 32);
  connect(reverbPredelayA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Pre Delay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Reverb A tone dial:
  reverbToneA = new DTDial(group, CC_REV_TONE_A);
  reverbToneA->setGeometry(328, 26, 32, 32);
  connect(reverbToneA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Tone");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Reverb A mix dial:
  reverbMixA = new DTDial(group, CC_REV_MIX_A);
  reverbMixA->setGeometry(394, 26, 32, 32);
  connect(reverbMixA, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
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
  classA = new DTSlider(group, CC_CLASS_A);
  classA->setGeometry(26, 40, 32, 28);
  connect(classA, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
  label = new QLabel(group);
  label->setText("Class A/B");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 20, 64, 20);
  label = new QLabel(group);
  label->setText("Class A");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 68, 64, 20);

  // Channel A Pentode/Triode switch:
  xtodeA = new DTSlider(group, CC_XTODE_A);
  xtodeA->setGeometry(196, 40, 32, 28);
  connect(xtodeA, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
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
  boostA = new DTSlider(group, CC_BOOST_A);
  boostA->setGeometry(26, 132, 32, 28);
  connect(boostA, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
  label = new QLabel(group);
  label->setText("On");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 112, 64, 20);
  label = new QLabel(group);
  label->setText("Boost");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 160, 64, 20);

  // Channel A PI voltage switch:
  pivoltA = new DTSlider(group, CC_PI_VOLTAGE_A);
  pivoltA->setGeometry(110, 132, 32, 28);
  connect(pivoltA, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int,int)));
  label = new QLabel(group);
  label->setText("PIV High");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 112, 64, 20);
  label = new QLabel(group);
  label->setText("PIV Low");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 160, 64, 20);

  // Channel A Cap X/Y switch:
  capA = new DTSlider(group, CC_CAP_TYPE_A);
  capA->setGeometry(196, 132, 32, 28);
  connect(capA, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
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
  gainB = new DTDial(group, CC_GAIN_B);
  gainB->setGeometry(200, 26, 32, 32);
  connect(gainB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Drive");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Channel B bass dial:
  bassB = new DTDial(group, CC_BASS_B);
  bassB->setGeometry(264, 26, 32, 32);
  connect(bassB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Bass");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Channel B middle dial:
  middleB = new DTDial(group, CC_MIDDLE_B);
  middleB->setGeometry(328, 26, 32, 32);
  connect(middleB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Middle");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Channel B treble dial:
  trebleB = new DTDial(group, CC_TREBLE_B);
  trebleB->setGeometry(394, 26, 32, 32);
  connect(trebleB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Treble");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(378, 58, 64, 20);

  // Channel B presence dial:
  presenceB = new DTDial(group, CC_PRESENCE_B);
  presenceB->setGeometry(460, 26, 32, 32);
  connect(presenceB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Presence");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(444, 58, 64, 20);

  // Channel B volume dial:
  volumeB = new DTDial(group, CC_VOLUME_B);
  volumeB->setGeometry(524, 26, 32, 32);
  connect(volumeB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
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
  reverbDecayB = new DTDial(group, CC_REV_DECAY_B);
  reverbDecayB->setGeometry(200, 26, 32, 32);
  connect(reverbDecayB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Decay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(184, 58, 64, 20);

  // Reverb B predelay dial:
  reverbPredelayB = new DTDial(group, CC_REV_PREDELAY_B);
  reverbPredelayB->setGeometry(264, 26, 32, 32);
  connect(reverbPredelayB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Pre Delay");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(248, 58, 64, 20);

  // Reverb B tone dial:
  reverbToneB = new DTDial(group, CC_REV_TONE_B);
  reverbToneB->setGeometry(328, 26, 32, 32);
  connect(reverbToneB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
  label = new QLabel(group);
  label->setText("Tone");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(312, 58, 64, 20);

  // Reverb B mix dial:
  reverbMixB = new DTDial(group, CC_REV_MIX_B);
  reverbMixB->setGeometry(394, 26, 32, 32);
  connect(reverbMixB, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
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
  classB = new DTSlider(group, CC_CLASS_B);
  classB->setGeometry(26, 40, 32, 28);
  connect(classB, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
  label = new QLabel(group);
  label->setText("Class A/B");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 20, 64, 20);
  label = new QLabel(group);
  label->setText("Class A");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 68, 64, 20);

  // Channel B Pentode/Triode switch:
  xtodeB = new DTSlider(group, CC_XTODE_B);
  xtodeB->setGeometry(196, 40, 32, 28);
  connect(xtodeB, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
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
  boostB = new DTSlider(group, CC_BOOST_B);
  boostB->setGeometry(26, 132, 32, 28);
  connect(boostB, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
  label = new QLabel(group);
  label->setText("On");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 112, 64, 20);
  label = new QLabel(group);
  label->setText("Boost");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(10, 160, 64, 20);

  // Channel B PI voltage switch:
  pivoltB = new DTSlider(group, CC_PI_VOLTAGE_B);
  pivoltB->setGeometry(110, 132, 32, 28);
  connect(pivoltB, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
  label = new QLabel(group);
  label->setText("PIV High");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 112, 64, 20);
  label = new QLabel(group);
  label->setText("PIV Low");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(94, 160, 64, 20);

  // Channel B Cap X/Y switch:
  capB = new DTSlider(group, CC_CAP_TYPE_B);
  capB->setGeometry(196, 132, 32, 28);
  connect(capB, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int,int)));
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
  channel = new DTSlider(group, CC_CHANNEL);
  channel->setGeometry(46, 55, 32, 28);
  channel->setReversed();
  connect(channel, SIGNAL(valueChanged2(int, int)), this, SLOT(sliderChanged(int, int)));
  label = new QLabel(group);
  label->setText("Channel A");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(30, 35, 64, 20);
  label = new QLabel(group);
  label->setText("Channel B");
  label->setAlignment(Qt::AlignHCenter);
  label->setGeometry(30, 83, 64, 20);

  // Master volume dial:
  master = new DTDial(group, CC_MASTER_VOL);
  master->setGeometry(130, 51, 32, 32);
  connect(master, SIGNAL(valueChanged2(int, int)), this, SLOT(dialChanged(int, int)));
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

  // Force unblock:
  blocked = false;
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
// MainWindow::dialChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the dial changed event.
///\param   [in] controlID: Control channel of the dial.
///\param   [in] value:     New value of the dial.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::dialChanged(int controlID, int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, controlID, value);

  // Release the user interface:
  sendBlockMessage(false);
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::sliderChanged()
////////////////////////////////////////////////////////////////////////////////
///\brief   Handler for the slider changed event.
///\param   [in] controlID: Control channel of the slider.
///\param   [in] value:     New value of the slider.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sliderChanged(int controlID, int value)
{
  // Is the UI locked?
  if (blocked)
    return;

  // Block user interface:
  sendBlockMessage(true);

  // Send the value:
  sendControlChange(DT_MIDI_CHANNEL, controlID, value);

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
// MainWindow::topolA2toggled()
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

///////////////////////////////// End of File //////////////////////////////////
