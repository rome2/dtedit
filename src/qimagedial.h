////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    qimagedial.h
///\brief   Image based rotary widget definition.
///\author  Rolf Meyerhoff (rm@matrix44.de)
///\version 1.0
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
#ifndef __QIMAGEDIAL_H_INCLUDED__
#define __QIMAGEDIAL_H_INCLUDED__

#include "qimagewidget.h"

////////////////////////////////////////////////////////////////////////////////
///\class QImageDial qimagedial.h
///\brief An image based dial control.
/// This widget serves basically the same purpose as the QDial control but it is
/// based on an image strip instead of custom drawing. Additionally it works in
/// linear mode as well and it supports relative movements. A default value can
/// be set that will be recalled when a user double clicks the widget.
///
/// Simple usage example:
///
/// QImageDial* dial = new QImageDial(this);
/// dial->image().load(":/images/knob.png");
/// dial->disabledImage().load(":/images/knob_disabled.png");
/// dial->setFrameCount(31);
/// dial->setGeometry(10, 10, 40, 40);
/// dial->setEnabled(true);
/// connect(dial, SIGNAL(valueChanged()), this, SLOT(dialValueChanged()));
///
////////////////////////////////////////////////////////////////////////////////
class QImageDial :
  public QImageWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::QImageDial()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  QImageDial(QWidget* parent = 0) :
    QImageWidget(parent),
    m_value(0.5),
    m_defaultValue(0.5),
    m_frameCount(0),
    m_absoluteMode(false),
    m_circularMode(false),
    m_startY(0),
    m_startVal(0.5),
    m_linearSize(128)
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::~QImageDial()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~QImageDial()
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::value()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this dial.
  ///\remarks The value is always in the range [0,1].
  //////////////////////////////////////////////////////////////////////////////
  double value() const
  {
    // Return current value:
    return m_value;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::setValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Value property.
  ///\param   [in] newVal: The new value of this dial.
  ///\remarks The value is clipped to the range [0,1]. If this value is set then
  ///         the widget will be repainted and a valueChanged event will be
  ///         emitted.
  //////////////////////////////////////////////////////////////////////////////
  void setValue(const double newVal)
  {
    // Set new value:
    m_value = newVal;

    // Clip value in the range [0,1]:
    if (m_value < 0.0)
      m_value = 0.0;
    else if (m_value > 1.0)
      m_value = 1.0;

    // Schedule redraw:
    update();

    // Notify listeners:
    if (!signalsBlocked())
      emit valueChanged();
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::defaultValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DefaultValue property.
  ///\return  The default value of this dial.
  ///\remarks The value is always in the range [0,1]. To load the default value
  ///\        just double click the widget.
  //////////////////////////////////////////////////////////////////////////////
  double defaultValue() const
  {
    // Return current value:
    return m_defaultValue;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::setDefaultValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the DefaultValue property.
  ///\param   [in] newVal: The new default value of this dial.
  ///\remarks The value is clipped to the range [0,1]. To load the default value
  ///\        just double click the widget.
  //////////////////////////////////////////////////////////////////////////////
  void setDefaultValue(const double newVal)
  {
    // Set new value:
    m_defaultValue = newVal;

    // Clip value in the range [0,1]:
    if (m_defaultValue < 0.0)
      m_defaultValue = 0.0;
    else if (m_defaultValue > 1.0)
      m_defaultValue = 1.0;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::frameCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the FrameCount property.
  ///\return  The current number of frames for the knob movie.
  ///\remarks This property tells this widget how man frames there are in the
  ///         knob movie and thus how to determine the subpicture to show.
  //////////////////////////////////////////////////////////////////////////////
  int frameCount() const
  {
    // Return current count:
    return m_frameCount;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::setFrameCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the FrameCount property.
  ///\param   [in] newCount: The new number of frames for the knob movie.
  ///\remarks This property tells this widget how man frames there are in the
  ///         knob movie and thus how to determine the subpicture to show.
  //////////////////////////////////////////////////////////////////////////////
  void setFrameCount(const int newCount)
  {
    // Set new value:
    m_frameCount = newCount;

    // Clip value:
    if (m_frameCount <= 0)
      m_frameCount = 1;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::absoluteMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the AbsoluteMode property.
  ///\return  Whether absolute mode is enabled or not.
  ///\remarks In absolute mode the dial value will instantly change to the
  ///         value indicated by the mouse click position. In relative mode it
  ///         will be changed relative to the click position.
  //////////////////////////////////////////////////////////////////////////////
  bool absoluteMode() const
  {
    // Return current state:
    return m_absoluteMode;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::setAbsoluteMode()
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
    m_absoluteMode = newState;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::circularMode()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the CircularMode property.
  ///\return  Whether circular motion is enabled or linear.
  ///\remarks In circular mode value can be changed by a circular move around
  ///         the center of the widget. In linear mode it is just an up down
  ///         movement. The linear movement range is defined by the LinearSize
  ///         property.
  //////////////////////////////////////////////////////////////////////////////
  bool circularMode() const
  {
    // Return current state:
    return m_circularMode;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::setCircularMode()
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
    m_circularMode = newState;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::linearSize()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the LinearSize property.
  ///\return  The current extends of the linear move mode.
  ///\remarks In linear mode this property describes the virtual fader size or,
  ///         in different words, the number of pixels that the value range of
  ///         [0,1] will be mapped to.
  //////////////////////////////////////////////////////////////////////////////
  int linearSize() const
  {
    // Return current size:
    return m_linearSize;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::setLinearSize()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the LinearSize property.
  ///\param   [in] newSize: The new extends of the linear move mode.
  ///\remarks In linear mode this property describes the virtual fader size or,
  ///         in different words, the number of pixels that the value range of
  ///         [0,1] will be mapped to.
  //////////////////////////////////////////////////////////////////////////////
  void setLinearSize(const int newSize)
  {
    // Set new size:
    m_linearSize = newSize;

    // Clip size:
    if (m_linearSize <= 0)
      m_linearSize = 1;
  }

signals:

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::valueChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief This signal is emitted when the value of this dial changes.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged();

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::mouseReleased()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief This signal is emitted when the mouse was released.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleased();

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::wheelEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse wheel signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void wheelEvent(QWheelEvent* event)
  {
    //Clac value delta:
    double delta = event->delta() * 0.00025;

    // Update value:
    setValue(m_value + delta);
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::mousePressEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse button pressed signal.
  ///\param [in] event: Provides further details about the event.
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
      if (m_circularMode)
      {
        // Get value from the mouse position point:
        m_startVal = valueFromMousePos(event->x(), event->y());

        // Make value current if needed:
        if (m_absoluteMode)
          setValue(m_startVal);
      }

      // No, linear is the way to go:
      else
      {
        // Save start values:
        m_startVal = m_value;
        m_startY   = event->y();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse button released signal.
  ///\param [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent* /* event */)
  {
    // Enabled?
    if (!isEnabled())
        return;

    // Notify listeners:
    if (!signalsBlocked())
      emit mouseReleased();
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::mouseDoubleClickEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse double click signal.
  ///\param [in] event: Provides further details about the event.
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
      setValue(m_defaultValue);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::mouseMoveEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse moved signal.
  ///\param [in] event: Provides further details about the event.
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
      if (m_circularMode)
      {
        // Get value from the mouse position point:
        double val = valueFromMousePos(event->x(), event->y());

        // Set value:
        if (m_absoluteMode)
          setValue(val);
        else
        {
          // Set new value relative to the last value:
          setValue(m_value + (val - m_startVal));

          // Save current value for the next round:
          m_startVal = val;
        }
      }

      // No, we're imitating a fader:
      else
      {
        // Calc movement in pixels:
        double dy = m_startY - event->y();

        // Scale into a more usable range:
        double diff = dy / m_linearSize;

        // Set new value relative to the start value:
        setValue(m_startVal + diff);
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper that paints the actual widget.
  ///\param [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  void drawWidget(QPainter& qp)
  {
    if (isEnabled() || disabledImage().isNull())
    {
      // Get size of a single sub image:
      int w = image().width() / m_frameCount;
      int h = image().height();

      // Calc active frame:
      int l = (int)(m_value * (m_frameCount - 1));
      if (l < 0)
        l = 0;
      if (l > (m_frameCount - 1))
        l = m_frameCount - 1;

      // Calc source position:
      int x = w * l;
      int y = 0;

      // Finally blit the image:
      qp.drawPixmap(0, 0, image(), x, y, w, h);
    }

    else
    {
      // Just show the disabled image:
      qp.drawPixmap(0, 0, disabledImage());
    }
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Internal helper to calc a dial value for mouse coordinates.
  ///\param  [in] mx: Input position, X component.
  ///\param  [in] my: Input position, Y component.
  ///\return The matching value for the input coordinates.
  //////////////////////////////////////////////////////////////////////////////
  double valueFromMousePos(const int mx, const int my) const
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
      return val;
    }

    // We hit the center, return current value:
    return m_value;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  double m_value;        ///\> The current value of this dial.
  double m_defaultValue; ///\> The default value of this value.
  int    m_frameCount;   ///\> The number of frames in the knob movie image.
  bool   m_absoluteMode; ///\> Use absolute or relative movement?
  bool   m_circularMode; ///\> Use linear or circular movement?
  int    m_startY;       ///\> Mouse down position for linear movement.
  double m_startVal;     ///\> Value when the movement starts.
  int    m_linearSize;   ///\> Scaling for linear movement.
};

#endif // __QIMAGEDIAL_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
