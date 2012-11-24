////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff.
////////////////////////////////////////////////////////////////////////////////
///\file    dtdial.h
///\ingroup dtedit
///\brief   Specialized rotary widget definition.
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
#ifndef __DTDIAL_H_INCLUDED__
#define __DTDIAL_H_INCLUDED__

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////
///\class DTDial dtdial.h
///\brief A specialized QDial derivative.
/// This widget extends the commen QDial control with MIDI send options and
/// better dial change behaviour.
////////////////////////////////////////////////////////////////////////////////
class DTDial :
  public QDial
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::Dial()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent:    Parent window for this widget.
  ///\param   [in] controlID: Control change number to use.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  DTDial(QWidget* parent, int controlID) :
    QDial(parent),
    defaultValue(0),
    absoluteMode(false),
    circularMode(false),
    startY(0),
    startVal(0),
    linearSize(1.0),
    controlID(controlID)
  {
    // Set usual parameters:
    setMinimum(0);
    setMaximum(127);
    setTracking(true);
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::~DTDial()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~DTDial()
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::getDefaultValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DefaultValue property.
  ///\return  The default value of this dial.
  ///\remarks The value is always in the range [0,127]. To load the default
  ///         value just double click the widget.
  //////////////////////////////////////////////////////////////////////////////
  double getDefaultValue() const
  {
    // Return current value:
    return defaultValue;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::setDefaultValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the DefaultValue property.
  ///\param   [in] newVal: The new default value of this dial.
  ///\remarks The value is clipped to the range [0,127]. To load the default
  ///         value just double click the widget.
  //////////////////////////////////////////////////////////////////////////////
  void setDefaultValue(const double newVal)
  {
    // Set new value:
    defaultValue = newVal;

    // Clip value in the range [0,127]:
    if (defaultValue < 0)
      defaultValue = 0;
    else if (defaultValue > 127)
      defaultValue = 127;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::getAbsoluteMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the AbsoluteMode property.
  ///\return  Whether absolute mode is enabled or not.
  ///\remarks In absolute mode the dial value will instantly change to the
  ///         value indicated by the mouse click position. In relative mode it
  ///         will be changed relative to the click position.
  //////////////////////////////////////////////////////////////////////////////
  bool getAbsoluteMode() const
  {
    // Return current state:
    return absoluteMode;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::setAbsoluteMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the AbsoluteMode property.
  ///\param   [in] newState: Whether to enable absolute mode or not.
  ///\remarks In absolute mode the dial value will instantly change to the
  ///         value indicated by the mouse click position. In relative mode it
  ///         will be changed relative to the click position.
  //////////////////////////////////////////////////////////////////////////////
  void setAbsoluteMode(const bool newState)
  {
    // Set new state:
    absoluteMode = newState;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::getCircularMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the CircularMode property.
  ///\return  Whether circular motion is enabled or linear.
  ///\remarks In circular mode value can be changed by a circular move around
  ///         the center of the widget. In linear mode it is just an up down
  ///         movement. The linear movement range is defined by the LinearSize
  ///         property.
  //////////////////////////////////////////////////////////////////////////////
  bool getCircularMode() const
  {
    // Return current state:
    return circularMode;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::setCircularMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the CircularMode property.
  ///\param   [in]WnewState: Whether to enable circular motion or linear.
  ///\remarks In circular mode value can be changed by a circular move around
  ///         the center of the widget. In linear mode it is just an up down
  ///         movement. The linear movement range is defined by the LinearSize
  ///         property.
  //////////////////////////////////////////////////////////////////////////////
  void setCircularMode(const bool newState)
  {
    // Set new state:
    circularMode = newState;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::getLinearSize()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the LinearSize property.
  ///\return  The current extends of the linear move mode.
  ///\remarks In linear mode this property describes the virtual fader size or,
  ///         in different words, the number of pixels that the value range of
  ///         [0,1] will be mapped to.
  //////////////////////////////////////////////////////////////////////////////
  int getLinearSize() const
  {
    // Return current size:
    return linearSize;
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::setLinearSize()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the LinearSize property.
  ///\param   [in] newSize: The new extends of the linear move mode.
  ///\remarks In linear mode this property describes the virtual fader size or,
  ///         in different words, the number of pixels that the value range of
  ///         [0,127] will be mapped to.
  //////////////////////////////////////////////////////////////////////////////
  void setLinearSize(const int newSize)
  {
    // Set new size:
    linearSize = newSize;

    // Clip size:
    if (linearSize <= 0.0)
      linearSize = 1.0;
  }

signals:

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::valueChanged2()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when the value of this dial changes.
  ///\param   [in] controlID: Control change ID.
  ///\param   [in] newVal:    The new value.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged2(int controlID, int newVal);

protected:

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::mousePressEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse button pressed signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mousePressEvent(QMouseEvent* event)
  {
    // Enabled?
    if (!isEnabled())
        return;

    // Left click?
    if (event->buttons() == Qt::LeftButton)
    {
      // Use circular mode?
      if (circularMode)
      {
        // Get value from the mouse position point:
        startVal = valueFromMousePos(event->x(), event->y());

        // Make value current if needed:
        if (absoluteMode)
          setValue(startVal);
      }

      // No, lear is the way to go:
      else
      {
        // Save start values:
        startVal = value();
        startY   = event->y();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse button released signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent* /*event*/)
  {
    // just Swallow the event.
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::mouseDoubleClickEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse double click signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseDoubleClickEvent(QMouseEvent* event)
  {
    // Enabled?
    if (!isEnabled())
        return;

    // Left click?
    if (event->buttons() == Qt::LeftButton)
    {
      // Load default:
      setValue(defaultValue);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::mouseMoveEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse moved signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseMoveEvent(QMouseEvent* event)
  {
    // Enabled?
    if (!isEnabled())
        return;

    // Left drag:
    if (event->buttons() == Qt::LeftButton)
    {
      // Running in circles?
      if (circularMode)
      {
        // Get value from the mouse position point:
        int val = valueFromMousePos(event->x(), event->y());

        // Set value:
        if (absoluteMode)
          setValue(val);
        else
        {
          // Set new value relative to the last value:
          setValue(value() + (val - startVal));

          // Save current value for the next round:
          startVal = val;
        }
      }

      // No, we're imitating a fader:
      else
      {
        // Calc movement in pixels:
        double dy = startY - event->y();

        // Scale into a more usable range:
        double diff = dy / linearSize;

        // Set new value relative to the start value:
        setValue(startVal + diff);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::sliderChange()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Tracks slider changes.
  ///\param   [in] change: Details about has changed.
  //////////////////////////////////////////////////////////////////////////////
  void sliderChange(SliderChange change)
  {
    // Base handling:
    QDial::sliderChange(change);

    // We only want value changes:
    if (change != SliderValueChange)
      return;

    // Send our message:
    if (!signalsBlocked())
      emit valueChanged2(controlID, value());
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // DTDial::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Internal helper to calc a dial value for mouse coordinates.
  ///\param   [in] mx: Input position, X component.
  ///\param   [in] my: Input position, Y component.
  ///\return  The matching value for the input coordinates.
  //////////////////////////////////////////////////////////////////////////////
  int valueFromMousePos(const int mx, const int my) const
  {
    // Get coordinates with respect to the control center:
    double x = (size().width() / 2.0) - mx;
    double y = (size().height() / 2.0) - my;

    // Normalize the values to get a direction vector:
    double len = sqrt(x * x + y * y);
    if (len > 0.0)
    {
      x /= len;
      y /= len;

      // Calc value:
      double val = acos(y) * (x < 0.0 ? 1.0 : -1.0);

      // Move into range [0,1]:
      val += 3.14;
      val /= 6.28;

      // Return the value:
      return static_cast<int>(val * 127.0);
    }

    // We hit the center, return current value:
    return value();
  }

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  int defaultValue;  ///\> The default value of this value.
  bool absoluteMode; ///\> Use absolute or relative movement?
  bool circularMode; ///\> Use linear or circular movement?
  int startY;        ///\> Mouse down position for linear movement.
  double startVal;   ///\> Value when the movement starts.
  double linearSize; ///\> Scaling for linear movement.
  int controlID;     ///\> Control change number to use.
};

#endif // __DTDIAL_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
