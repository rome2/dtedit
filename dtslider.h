////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    dtslider.h
///\ingroup dtedit
///\brief   Specialized slider widget definition.
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
#ifndef __DTSLIDER_H_INCLUDED__
#define __DTSLIDER_H_INCLUDED__

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////
///\class DTSlider dtslider.h
///\brief A specialized QSlider derivative.
/// This widget extends the commen QDial control with MIDI send options.
////////////////////////////////////////////////////////////////////////////////
class DTSlider :
  public QSlider
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // DTSlider::DTSlider()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent:    Parent window for this widget.
  ///\param   [in] controlID: Control change number to use.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  DTSlider(QWidget* parent, int controlID) :
    QSlider(parent),
    reversed(false),
    controlID(controlID)
  {
    // Set usual parameters:
    setMinimum(0);
    setMaximum(1);
    setTickPosition(QSlider::NoTicks);
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTSlider::~DTSlider()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~DTSlider()
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::getReversed()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Reversed property.
  ///\return  The current reversed state of this dial.
  ///\remarks If this property is true then the dial works the other way around.
  //////////////////////////////////////////////////////////////////////////////
  bool getReversed() const
  {
    // Return current state:
    return reversed;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::setReversed()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Reversed property.
  ///\param   [in] newState: The new reversed state of this dial.
  ///\remarks If this property is true then the dial works the other way around.
  //////////////////////////////////////////////////////////////////////////////
  void setReversed(const bool newState = true)
  {
    // Set new state:
    reversed = newState;
  }

signals:

  //////////////////////////////////////////////////////////////////////////////
  // DTSlider::valueChanged2()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when the value of this dial changes.
  ///\param   [in] controlID: Control change ID.
  ///\param   [in] newVal:    The new value.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged2(int controlID, int newVal);

protected:

  //////////////////////////////////////////////////////////////////////////////
  // DTSlider::sliderChange()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Tracks slider changes.
  ///\param   [in] change: Details about has changed.
  //////////////////////////////////////////////////////////////////////////////
  void sliderChange(SliderChange change)
  {
    // Base handling:
    QSlider::sliderChange(change);

    // We only want value changes:
    if (change != SliderValueChange)
      return;

    // Send our message:
    if (reversed)
      emit valueChanged2(controlID, value() ? 0 : 127);
    else
      emit valueChanged2(controlID, value() ? 127 : 0);
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  bool reversed; ///\>Reverse operation?
  int controlID; ///\> Control change number to use.
};

#endif // __DTDIAL_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
