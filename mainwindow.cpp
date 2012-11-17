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
#include "mainwindow.h"
#include "setupdialog.h"
#include "aboutdialog.h"

////////////////////////////////////////////////////////////////////////////////
// MainWindow::MainWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Initialization constructor of this window.
///\param   [in] parent: Parent window for this window.
///\remarks Basically initializes the entire gui.
////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  midiInName(""),
  midiOutName(""),
  midiOK(false),
  blocked(false)
{
  // Init title:
  setWindowTitle("DT Edit");
  setWindowIcon(QIcon(":/images/dtedit.png"));

  // Initialize status bar:
  statusBar()->setSizeGripEnabled(false);
  statusBar()->showMessage(tr("Ready."));

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

  // File menu:
  fileMenu = menuBar()->addMenu(tr("&File"));
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

  QGroupBox* group = new QGroupBox(this);
  group->setTitle("Preamp A");
  group->setFlat(false);
  group->setGeometry(15, menuBar()->height() + 8, 670, 92);

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

  group = new QGroupBox(this);
  group->setTitle("Reverb A");
  group->setFlat(false);
  group->setGeometry(235, menuBar()->height() + 110, 450, 92);

  reverbBypassA = new QCheckBox(group);
  reverbBypassA->setText("Enabled");
  connect(reverbBypassA, SIGNAL(stateChanged(int)), this, SLOT(reverbBypassAChanged(int)));
  reverbBypassA->setGeometry(10, 22, 134, 22);

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

  group = new QGroupBox(this);
  group->setTitle("Poweramp A");
  group->setFlat(false);
  group->setGeometry(700, menuBar()->height() + 8, 255, 194);

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

  group = new QGroupBox(this);
  group->setTitle("Preamp B");
  group->setFlat(false);
  group->setGeometry(15, menuBar()->height() + 314, 670, 92);

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

  group = new QGroupBox(this);
  group->setTitle("Reverb B");
  group->setFlat(false);
  group->setGeometry(235, menuBar()->height() + 212, 450, 92);

  reverbBypassB = new QCheckBox(group);
  reverbBypassB->setText("Enabled");
  connect(reverbBypassB, SIGNAL(stateChanged(int)), this, SLOT(reverbBypassBChanged(int)));
  reverbBypassB->setGeometry(10, 22, 134, 22);

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

  group = new QGroupBox(this);
  group->setTitle("Poweramp B");
  group->setFlat(false);
  group->setGeometry(700, menuBar()->height() + 212, 255, 194);

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

  group = new QGroupBox(this);
  group->setTitle("Master");
  group->setFlat(false);
  group->setGeometry(15, menuBar()->height() + 110, 205, 194);

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

  lowVol = new QCheckBox(group);
  lowVol->setText("Low Volume Mode");
  connect(lowVol, SIGNAL(stateChanged(int)), this, SLOT(lowVolChanged(int)));
  lowVol->setGeometry(10, 125, 134, 22);

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

void MainWindow::ampAChanged(int value)
{
  if (blocked)
    return;

  if (defaultsAction->isChecked())
  {
    sendBlockMessage(true);
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_DEF_A, value);
    sendBlockMessage(false);
    getValuesFromDT();
  }
  else
  {
    sendBlockMessage(true);
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_A, value);
    sendBlockMessage(false);
  }
}

void MainWindow::cabAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_CAB_A, value);
  sendBlockMessage(false);
}

void MainWindow::gainAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_GAIN_A, value);
  sendBlockMessage(false);
}

void MainWindow::bassAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_BASS_A, value);
  sendBlockMessage(false);
}

void MainWindow::middleAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_MIDDLE_A, value);
  sendBlockMessage(false);
}

void MainWindow::trebleAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TREBLE_A, value);
  sendBlockMessage(false);
}

void MainWindow::presenceAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_PRESENCE_A, value);
  sendBlockMessage(false);
}

void MainWindow::volumeAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOLUME_A, value);
  sendBlockMessage(false);
}

void MainWindow::voiceA1toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 0);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::voiceA2toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 1);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::voiceA3toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 2);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::voiceA4toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_A, 3);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::reverbAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TYPE_A, value);
  sendBlockMessage(false);
}

void MainWindow::reverbBypassAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_BYPASS_A, value == Qt::Checked ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::reverbDecayAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_DECAY_A, value);
  sendBlockMessage(false);
}

void MainWindow::reverbPredelayAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_PREDELAY_A, value);
  sendBlockMessage(false);
}

void MainWindow::reverbToneAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TONE_A, value);
  sendBlockMessage(false);
}

void MainWindow::reverbMixAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_MIX_A, value);
  sendBlockMessage(false);
}

void MainWindow::classAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_CLASS_A, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::xtodeAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_XTODE_A, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::boostAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_BOOST_A, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::pivoltAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_PI_VOLTAGE_A, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::capAChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_A, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::topolA1toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 0);
  sendBlockMessage(false);
}

void MainWindow::topolA2toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 1);
  sendBlockMessage(false);
}

void MainWindow::topolA3toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 2);
  sendBlockMessage(false);
}

void MainWindow::topolA4toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_A, 3);
  sendBlockMessage(false);
}

void MainWindow::ampBChanged(int value)
{
  if (blocked)
    return;

  if (defaultsAction->isChecked())
  {
    sendBlockMessage(true);
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_DEF_B, value);
    sendBlockMessage(false);
    getValuesFromDT();
  }
  else
  {
    sendBlockMessage(true);
    sendControlChange(DT_MIDI_CHANNEL, CC_AMP_B, value);
    sendBlockMessage(false);
  }
}

void MainWindow::cabBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_CAB_B, value);
  sendBlockMessage(false);
}

void MainWindow::gainBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_GAIN_B, value);
  sendBlockMessage(false);
}

void MainWindow::bassBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_BASS_B, value);
  sendBlockMessage(false);
}

void MainWindow::middleBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_MIDDLE_B, value);
  sendBlockMessage(false);
}

void MainWindow::trebleBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TREBLE_B, value);
  sendBlockMessage(false);
}

void MainWindow::presenceBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_PRESENCE_B, value);
  sendBlockMessage(false);
}

void MainWindow::volumeBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOLUME_B, value);
  sendBlockMessage(false);
}

void MainWindow::voiceB1toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 0);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::voiceB2toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 1);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::voiceB3toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 2);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::voiceB4toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_VOICE_B, 3);
  sendBlockMessage(false);
  getValuesFromDT();
}

void MainWindow::reverbBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TYPE_B, value);
  sendBlockMessage(false);
}

void MainWindow::reverbBypassBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_BYPASS_B, value == Qt::Checked ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::reverbDecayBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_DECAY_B, value);
  sendBlockMessage(false);
}

void MainWindow::reverbPredelayBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_PREDELAY_B, value);
  sendBlockMessage(false);
}

void MainWindow::reverbToneBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_TONE_B, value);
  sendBlockMessage(false);
}

void MainWindow::reverbMixBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_REV_MIX_B, value);
  sendBlockMessage(false);
}

void MainWindow::classBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_CLASS_B, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::xtodeBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_XTODE_B, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::boostBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_BOOST_B, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::pivoltBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_PI_VOLTAGE_B, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::capBChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_CAP_TYPE_B, value ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::topolB1toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 0);
  sendBlockMessage(false);
}

void MainWindow::topolB2toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 1);
  sendBlockMessage(false);
}

void MainWindow::topolB3toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 2);
  sendBlockMessage(false);
}

void MainWindow::topolB4toggled(bool checked)
{
  if (blocked || !checked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_TOPOL_B, 3);
  sendBlockMessage(false);
}

void MainWindow::micChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_XLR_MIC, value);
  sendBlockMessage(false);
}

void MainWindow::lowVolChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_LOWVOLUME, value == Qt::Checked ? 127 : 0);
  sendBlockMessage(false);
}

void MainWindow::channelChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_CHANNEL, value ? 0 : 127);
  sendBlockMessage(false);
}

void MainWindow::masterChanged(int value)
{
  if (blocked)
    return;

  sendBlockMessage(true);
  sendControlChange(DT_MIDI_CHANNEL, CC_MASTER_VOL, value);
  sendBlockMessage(false);
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
// MainWindow::getValuesFromDT()
////////////////////////////////////////////////////////////////////////////////
///\brief   Sync UI with the values from the actual DT.
///\remarks This functions sends value request CCs to the DT. The UI is then
///         updated by the CC receive function.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::getValuesFromDT()
{
  // Send parameter requests:
  sendControlChange(DT_MIDI_CHANNEL, 83, 0);
  sendControlChange(DT_MIDI_CHANNEL, 83, 17);
  sendControlChange(DT_MIDI_CHANNEL, 83, 18);
  sendControlChange(DT_MIDI_CHANNEL, 83, 19);
  sendControlChange(DT_MIDI_CHANNEL, 83, 29);
  sendControlChange(DT_MIDI_CHANNEL, 83, 30);
  sendControlChange(DT_MIDI_CHANNEL, 83, 31);
  sendControlChange(DT_MIDI_CHANNEL, 83, 32);
  sendControlChange(DT_MIDI_CHANNEL, 83, 33);
  sendControlChange(DT_MIDI_CHANNEL, 83, 34);
  sendControlChange(DT_MIDI_CHANNEL, 83, 35);
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
// MainWindow::openMIDIPorts()
////////////////////////////////////////////////////////////////////////////////
///\brief   Open the MIDI devices for input/output.
///\return  Returns true if successfull or false otherwise.
///\remarks Always closes the port priot trying to open them again.
////////////////////////////////////////////////////////////////////////////////
bool MainWindow::openMIDIPorts()
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
// MainWindow::showSetupWindow()
////////////////////////////////////////////////////////////////////////////////
///\brief   Show the configuration dialog.
///\return  Returns true if successfull or false otherwise.
///\remarks A false return value means that the user pressed cancel.
////////////////////////////////////////////////////////////////////////////////
bool MainWindow::showSetupWindow()
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
// MainWindow::setup()
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
// MainWindow::noteOnReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new note on message arrives.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::noteOnReceived(unsigned char channel, unsigned char noteNumber, unsigned char velocity)
{
  // Log message:
  QString s;
  s.sprintf("Note on received (note %d, channel %d, velocity %d)", noteNumber, channel + 1, velocity);
  qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::noteOffReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new note off message arrives.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::noteOffReceived(unsigned char channel, unsigned char noteNumber, unsigned char velocity)
{
  // Log message:
  QString s;
  s.sprintf("Note off received (note %d, channel %d, velocity %d)", noteNumber, channel + 1, velocity);
  qDebug() << s;
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
  // Log message:
  //QString s;
  //s.sprintf("Control change received (number %d, channel %d, value %d)", controlNumber, channel + 1, value);
  //qDebug() << s;

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

////////////////////////////////////////////////////////////////////////////////
// MainWindow::programChangeReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new program change message arrives.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Program number.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::programChangeReceived(unsigned char channel, unsigned char value)
{
  // Log message:
  QString s;
  s.sprintf("Program change received (channel %d, value %d)", channel + 1, value);
  qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::channelAftertouchReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new channel aftertouch message arrives.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::channelAftertouchReceived(unsigned char channel, unsigned char value)
{
  // Log message:
  QString s;
  s.sprintf("Channel aftertouch received (channel %d, value %d)", channel + 1, value);
  qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::pitchBendReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new pitch bend message arrives.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pitch bend value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::pitchBendReceived(unsigned char channel, unsigned short value)
{
  // Log message:
  QString s;
  s.sprintf("Pitch bend received (channel %d, value %d)", channel + 1, value);
  qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::polyAftertouchReceived()
////////////////////////////////////////////////////////////////////////////////
///\brief   This is called when a new poly pressure message arrives.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] value:      Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::polyAftertouchReceived(unsigned char channel, unsigned char noteNumber, unsigned char value)
{
  // Log message:
  QString s;
  s.sprintf("Polyphonic aftertouch received (note %d, channel %d, value %d)", noteNumber, channel + 1, value);
  qDebug() << s;
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
  // Log message:
  QString s;
  s.sprintf("SysEx received (size %d bytes)", static_cast<int>(buff.size()));
  qDebug() << s;
}

////////////////////////////////////////////////////////////////////////////////
// MainWindow::sendNoteOn()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a note on message.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendNoteOn(unsigned char channel, unsigned char noteNumber, unsigned char velocity)
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
// MainWindow::sendNoteOff()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a note off message.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] velocity:   Note velocity.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendNoteOff(unsigned char channel, unsigned char noteNumber, unsigned char velocity)
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
// MainWindow::sendControlChange()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a control change message.
///\param   [in] channel:       MIDI channel of this message.
///\param   [in] controlNumber: Control number.
///\param   [in] value:         Control value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendControlChange(unsigned char channel, unsigned char controlNumber, unsigned char value)
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
// MainWindow::sendProgramChange()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a program change message.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Program number.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendProgramChange(unsigned char channel, unsigned char value)
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
// MainWindow::sendChannelAftertouch()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a channel aftertouch message.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendChannelAftertouch(unsigned char channel, unsigned char value)
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
// MainWindow::sendPitchBend()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a pitch bend message.
///\param   [in] channel: MIDI channel of this message.
///\param   [in] value:   Pitch bend value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendPitchBend(unsigned char channel, unsigned short value)
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
// MainWindow::sendPolyAftertouch()
////////////////////////////////////////////////////////////////////////////////
///\brief   Send a poly aftertouch message.
///\param   [in] channel:    MIDI channel of this message.
///\param   [in] noteNumber: Note number.
///\param   [in] value:      Pressure value.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::sendPolyAftertouch(unsigned char channel, unsigned char noteNumber, unsigned char value)
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
// MainWindow::onMIDIMessage()
////////////////////////////////////////////////////////////////////////////////
///\brief   Callback for incoming MIDI messages.
///\param   [in] timeStamp: Time stamp of the message.
///\param   [in] message:   The raw MIDI message as byte buffer.
///\remarks Updates the surface depending on the MIDI data.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::onMIDIMessage(const double /* timeStamp */, const std::vector<unsigned char>& message)
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
// MainWindow::onMIDIMessageProxy()
////////////////////////////////////////////////////////////////////////////////
///\brief   Callback for incoming MIDI messages.
///\param   [in] timeStamp: Time stamp of the message.
///\param   [in] message:   The raw MIDI message as byte buffer.
///\param   [in] userData:  User data set when the port was created.
///\remarks The userData holds a pointer to this class so this function
///         delegates the call to the member function of the class.
////////////////////////////////////////////////////////////////////////////////
void MainWindow::onMIDIMessageProxy(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
  // Delegate to the class function:
  static_cast<MainWindow*>(userData)->onMIDIMessage(timeStamp, *message);
}

///////////////////////////////// End of File //////////////////////////////////
