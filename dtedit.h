////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    dtedit.h
///\ingroup dtedit
///\brief   Main application include file.
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
#ifndef __DT25EDIT_H_INCLUDED__
#define __DT25EDIT_H_INCLUDED__

// MIDI tools:
#include "RtMidi/RtMidi.h"

#define DT_MIDI_CHANNEL 0

// Control IDs:
#define CC_AMP_A          11 // 83/35 // 83/30
#define CC_AMP_DEF_A      12
#define CC_GAIN_A         13 // 83/35 // 83/29
#define CC_BASS_A         14 // 83/35 // 83/29
#define CC_MIDDLE_A       15 // 83/35 // 83/29
#define CC_TREBLE_A       16 // 83/35 // 83/29 // 83/19 // 83/17 // 83/0
#define CC_VOLUME_A       17 // 83/35 // 83/29
#define CC_REV_MIX_A      18 // 83/35 // 83/29
#define CC_CHANNEL        19
#define CC_MASTER_VOL     20
#define CC_PRESENCE_A     21 // 83/35 // 83/29 // 83/19 // 83/17 // 83/0

#define CC_REV_BYPASS_A   36
#define CC_REV_TYPE_A     37 // 83/35 // 83/29

#define CC_REV_DECAY_A    52
#define CC_REV_PREDELAY_A 53
#define CC_REV_TONE_A     54
#define CC_REV_DECAY_B    56
#define CC_REV_PREDELAY_B 57
#define CC_REV_TONE_B     58

#define CC_AB_TOGGLE      66

#define CC_CAB_A          71 // 83/35 // 83/30

#define CC_CLASS_A        73 // 83/29 // 83/19 // 83/17 // 83/0
#define CC_BOOST_A        74 // 83/35 // 83/29
#define CC_XTODE_A        75 // 83/29 // 83/19 // 83/17 // 83/0
#define CC_TOPOL_A        77 // 83/35 // 83/29
#define CC_PI_VOLTAGE_A   78 // 83/35 // 83/30
#define CC_CAP_TYPE_A     79 // 83/35 // 83/29

#define CC_XLR_MIC        82 // 83/35 // 83/32 // 83/30
#define CC_UNKNOWN_1      83

#define CC_LOWVOLUME      85
#define CC_PI_VOLTAGE_B   86 // 83/34 // 83/31
#define CC_CAP_TYPE_B     87 // 83/34 // 83/31
#define CC_AMP_DEF_B      89

#define CC_AMP_B          91 // 83/35 // 83/32
#define CC_GAIN_B         92 // 83/33 // 83/30
#define CC_BASS_B         93 // 83/33 // 83/30 // 83/19 // 83/18 // 83/0
#define CC_MIDDLE_B       94 // 83/33 // 83/30
#define CC_TREBLE_B       95 // 83/33 // 83/30 // 83/19 // 83/18 // 83/0

#define CC_PRESENCE_B     102 // 83/33 // 83/30 // 83/19 // 83/18 // 83/0
#define CC_VOLUME_B       103 // 83/32 // 83/30 // 83/19 // 83/18 // 83/0

#define CC_REV_BYPASS_B   105
#define CC_REV_MIX_B      106 // 83/33 // 83/30
#define CC_REV_TYPE_B     107 // 83/33 // 83/30
#define CC_UNKNOWN_2      108 // 83/35 // 83/30
#define CC_UNKNOWN_3      109 // 83/34 // 83/32
#define CC_CAB_B          110 // 83/35 // 83/32

#define CC_TOPOL_B        114 // 83/33 // 83/30
#define CC_CLASS_B        115 // 83/34 // 83/31 // 83/19 // 83/18 // 83/0
#define CC_XTODE_B        116 // 83/34 // 83/31 // 83/19 // 83/18 // 83/0
#define CC_BOOST_B        117 // 83/34 // 83/31

#define CC_VOICE_A        120 // 83/19 // 83/17 // 83/0
#define CC_VOICE_B        121 // 83/19 // 83/18 // 83/0
#define CC_VOICING        122

// Amp models:
#define AMP_NONE                    0
#define AMP_BLACKFACE_DOUBLE_NORMAL 1
#define AMP_BLACKFACE_DOUBLE_VIB    2
#define AMP_HIWAY_100               3
#define AMP_SUPER_O                 4
#define AMP_GIBTONE_185             5
#define AMP_TWEED_B_MAN_NORMAL      6
#define AMP_TWEED_B_MAN_BRIGHT      7
#define AMP_BLACKFACE_LUX_NORMAL    8
#define AMP_BLACKFACE_LUX_VIB       9
#define AMP_DIVIDE_9_15             10
#define AMP_PHD_MOTORWAY            11
#define AMP_CLASS_A_15              12
#define AMP_CLASS_A_30              13
#define AMP_BRIT_J45_NORMAL         14
#define AMP_BRIT_J45_BRIGHT         15
#define AMP_BRIT_PLEXI_100_NORMAL   16
#define AMP_BRIT_PLEXI_100_BRIGHT   17
#define AMP_BRIT_P75_NORMAL         18
#define AMP_BRIT_P75_BRIGHT         19
#define AMP_BRIT_J800               20
#define AMP_BOMBER_UBER             21
#define AMP_TREADPLATE              22
#define AMP_ANGEL_F_BALL            23
#define AMP_LINE6_ELEKTRIK          24
#define AMP_FLIP_FLOP               25
#define AMP_SOLO_100_CLEAN          26
#define AMP_SOLO_100_CRUNCH         27
#define AMP_SOLO_100_OVERDRIVE      28
#define AMP_LINE6_DOOM              29
#define AMP_LINE6_EPIC              30

// Cab models:
#define CAB_NONE                  0
#define CAB_2X12_BLACKFACE_DOUBLE 1
#define CAB_4X12_HIWAY            2
#define CAB_1X6_SUPER_O           3
#define CAB_1X12_GIBTONE_F_COIL   4
#define CAB_4x10_TWEED_B_MAN      5
#define CAB_1X12_BLACKFACE_LUX    6
#define CAB_1X12_BRIT_12_H        7
#define CAB_2X12_PHD_PORTED       8
#define CAB_1X12_BLUE_BELL        9
#define CAB_2X12_SILVER_BELL      10
#define CAB_4X12_GREENBACK_25     11
#define CAB_4X12_BLACKBACK_30     12
#define CAB_4X12_BRIT_T_75        13
#define CAB_4X12_UBER             14
#define CAB_4X12_TREAD_V_30       15
#define CAB_4X12_XXL_V_30         16
#define CAB_1X15_FLIP_TOP         17

// XLR microphone mode:
#define MIC_NONE          0
#define MIC_DYNAMIC_57    1
#define MIC_DYNAMIC_58_OA 2
#define MIC_DYNAMIC_409   3
#define MIC_DYNAMIC_421   4
#define MIC_RIBBON_4038   5
#define MIC_RIBBON_121    6
#define MIC_CONDENSER_67  7
#define MIC_CONDENSER_87  8

// Direct voicing select:
#define VOICING_A_I   0
#define VOICING_A_II  1
#define VOICING_A_III 2
#define VOICING_A_IV  3
#define VOICING_B_I   4
#define VOICING_B_II  5
#define VOICING_B_III 6
#define VOICING_B_IV  7

// Topology / Negative Feedback Loop type:
#define TOPOL_TIGHT    0
#define TOPOL_LOOSE    1
#define TOPOL_ZERO     2
#define TOPOL_RESONANT 3

// Reverb types:
#define REV_NONE          0
#define REV_SPRING        1
#define REV_63_SPRING     2
#define REV_PLATE         3
#define REV_ROOM          4
#define REV_CHAMBER       5
#define REV_HALL          6
#define REV_CAVE          7
#define REV_DUCKING       8
#define REV_OCTO          9
#define REV_TILE          10
#define REV_ECHO          11
#define REV_PARTICLE_VERB 12

#endif // #ifndef __DT25EDIT_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
