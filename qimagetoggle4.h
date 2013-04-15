////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    qimagetoggle4.h
///\brief   Image based 4 state toggle button widget definition.
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
#ifndef __QIMAGETOGGLE4_H_INCLUDED__
#define __QIMAGETOGGLE4_H_INCLUDED__

#include "qimagewidget.h"

////////////////////////////////////////////////////////////////////////////////
///\class QImageToggle qimagetoggle4.h
///\brief -
////////////////////////////////////////////////////////////////////////////////
class QImageToggle4 :
  public QImageWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::QImageToggle4()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  QImageToggle4(QWidget* parent = 0) :
    QImageWidget(parent),
    m_value(0)
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::~QImageToggle4()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~QImageToggle4()
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::value()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this toggle.
  ///\remarks The value is either 0, 1, 2 or 3.
  //////////////////////////////////////////////////////////////////////////////
  int value() const
  {
    // Return current value:
    return m_value;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::setValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Value property.
  ///\param   [in] newVal: The new value of this toggle.
  ///\remarks The widget will be repainted and a valueChanged event will be
  ///         emitted.
  //////////////////////////////////////////////////////////////////////////////
  void setValue(const int newVal)
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

signals:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::valueChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief This signal is emitted when the value of this toggle changes.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged();

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::mousePressEvent()
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
      // Save start position:
      m_startPos.setX(event->x());
      m_startPos.setY(event->y());
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Handler for the mouse button released signal.
  ///\param [in] event: Provides further details about the event.
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
      int newVal = valueFromMousePos(event->x(), event->y());

      // Compare to start position:
      if (newVal != valueFromMousePos(m_startPos.x(), m_startPos.y()))
        return;

      // Update widget:
      setValue(newVal);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle4::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper that paints the actual widget.
  ///\param [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  virtual void drawWidget(QPainter& qp)
  {
    if (isEnabled() || disabledImage().isNull())
    {
      // Get size of a single sub image:
      int w = image().width() / 4;
      int h = image().height();

      // Calc source position:
      int x = m_value * w;

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
  // QImageToggle4::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief  Internal helper to calc a value for mouse coordinates.
  ///\param  [in] mx: Input position, X component.
  ///\param  [in] my: Input position, Y component.
  ///\return The matching value for the input coordinates.
  //////////////////////////////////////////////////////////////////////////////
  int valueFromMousePos(const int mx, const int my) const
  {
    // Get center:
    int bx = width() / 2;
    int by = height() / 2;

    // Find matching quadrant:
    if (my < by && mx < bx)
      return 0;
    else if (my < by && mx >= bx)
      return 1;
    else if (my >= by && mx < bx)
      return 2;
    else
      return 3;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QPoint m_startPos; ///\> Mouse down position.
  int    m_value;    ///\> The current value of this toggle.
};

#endif // __QIMAGETOGGLE4_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
