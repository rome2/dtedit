////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    qimagetoggle.h
///\brief   Image based toggle button widget definition.
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
#ifndef __QIMAGETOGGLE_H_INCLUDED__
#define __QIMAGETOGGLE_H_INCLUDED__

#include "qimagewidget.h"

////////////////////////////////////////////////////////////////////////////////
///\class QImageToggle qimagetoggle.h
///\brief -
////////////////////////////////////////////////////////////////////////////////
class QImageToggle :
  public QImageWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::QImageToggle()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  QImageToggle(QWidget* parent = 0) :
    QImageWidget(parent),
    m_value(false),
    m_leftRight(false)
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::~QImageToggle()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~QImageToggle()
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::value()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this dial.
  ///\remarks The value is either true or false.
  //////////////////////////////////////////////////////////////////////////////
  bool value() const
  {
    // Return current value:
    return m_value;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::setValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Value property.
  ///\param   [in] newVal: The new value of this dial.
  ///\remarks The widget will be repainted and a valueChanged event will be
  ///         emitted.
  //////////////////////////////////////////////////////////////////////////////
  void setValue(const bool newVal)
  {
    // Anything to do?
    if (m_value == newVal)
      return;

    // Set new value:
    m_value = newVal;

    // Force redraw:
    update();

    // Notify listeners:
    if (!signalsBlocked())
      emit valueChanged();
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::leftRight()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the LeftRight property.
  ///\return  The current left to right mode.
  ///\remarks If this is true then we are working left to right instead of top
  ///         to bottom.
  //////////////////////////////////////////////////////////////////////////////
  bool leftRight() const
  {
    // Return current state:
    return m_leftRight;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::setLeftRight()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the LinearSize property.
  ///\param   [in] newState: The new left to right state.
  ///\remarks If this is true then we are working left to right instead of top
  ///         to bottom.
  //////////////////////////////////////////////////////////////////////////////
  void setLeftRight(const int newState)
  {
    // Set new state:
    m_leftRight = newState;
  }

signals:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::valueChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when the value of this button changes.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged();

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::mousePressEvent()
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
      // Save start position:
      m_startPos.setX(event->x());
      m_startPos.setY(event->y());
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse button pressed signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent* event)
  {
    // Enabled?
    if (!isEnabled())
        return;

    // Left click?
    if (!(event->buttons() & Qt::LeftButton))
    {
      // Check bounds:
      if (event->x() < 0 || event->x() >= width())
        return;
      if (event->y() < 0 || event->y() >= height())
        return;

      // Get current value:
      bool newVal = valueFromMousePos(event->x(), event->y());

      // Compare to start position:
      if (newVal != valueFromMousePos(m_startPos.x(), m_startPos.y()))
        return;

      // Update widget:
      setValue(newVal);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper that paints the actual widget.
  ///\param [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  void drawWidget(QPainter& qp)
  {
    if (isEnabled() || disabledImage().isNull())
    {
      // Get size of a single sub image:
      int w = image().width() / 2;
      int h = image().height();

      // Calc source position:
      int x = m_value ? w : 0;

      // Finally blit the image:
      qp.drawImage(0, 0, image(), x, 0, w, h);
    }
    else
    {
      // Just show the disabled image:
      qp.drawImage(0, 0, disabledImage());
    }
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Internal helper to calc a value for mouse coordinates.
  ///\param  [in] mx: Input position, X component.
  ///\param  [in] my: Input position, Y component.
  ///\return The matching value for the input coordinates.
  //////////////////////////////////////////////////////////////////////////////
  bool valueFromMousePos(const int mx, const int my) const
  {
    // Get center:
    int bx = width() / 2;
    int by = height() / 2;

    // Find matching half:
    if (m_leftRight)
      return mx < bx;
    return my < by;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QPoint m_startPos; ///\> Mouse down position.
  bool m_value;      ///\> The current value of this toggle.
  bool m_leftRight;  ///\> Work left to right?
};

#endif // __QIMAGETOGGLE_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
