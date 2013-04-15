////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    qimageled.h
///\brief   Image based LED widget definition.
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
#ifndef __QIMAGELED_H_INCLUDED__
#define __QIMAGELED_H_INCLUDED__

#include "qimagewidget.h"

////////////////////////////////////////////////////////////////////////////////
///\class QImageLED qimageled.h
///\brief An image based LED control.
/// This widget is just a simple LED with interaction.
///
/// Simple usage example:
///
/// QImageLED* led = new QImageLED(this);
/// led->image().load(":/images/led.png");
/// led->disabledImage().load(":/images/led_disabled.png");
/// led->setGeometry(10, 10, 40, 40);
/// led->setEnabled(true);
///
////////////////////////////////////////////////////////////////////////////////
class QImageLED :
  public QImageWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::QImageLED()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  QImageLED(QWidget* parent = 0) :
    QImageWidget(parent),
    m_value(false)
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::~QImageLED()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~QImageLED()
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::value()
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
  // QImageLED::setValue()
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
  }

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief Internal helper that paints the actual widget.
  ///\param [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  virtual void drawWidget(QPainter& qp)
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
  // Member:
  bool m_value; ///\> The current value of this toggle.
};

#endif // __QIMAGELED_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
