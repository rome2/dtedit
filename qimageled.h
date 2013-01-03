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

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////
///\class QImageLED qimageled.h
///\brief An image based LED control.
/// This widget is just a simple LED with interaction.
///
/// Simple usage example:
///
/// QImageLED* led = new QImageLED(this);
/// led->getImage().load(":/images/led.png");
/// led->getDisabledImage().load(":/images/led_disabled.png");
/// led->setGeometry(10, 10, 40, 40);
/// led->setEnabled(true);
///
////////////////////////////////////////////////////////////////////////////////
class QImageLED :
  public QWidget
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
    QWidget(parent),
    value(false),
    tag(0)
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
  // QImageLED::getValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this dial.
  ///\remarks The value is either true or false.
  //////////////////////////////////////////////////////////////////////////////
  bool getValue() const
  {
    // Return current value:
    return value;
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
    if (value == newVal)
      return;

    // Set new value:
    value = newVal;

    // Force redraw:
    update();
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::getImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Image property.
  ///\return  The current image.
  ///\remarks This image is the source for the toggle. It must contain two sub
  ///         pictures ordered from left to right.
  //////////////////////////////////////////////////////////////////////////////
  QImage& getImage()
  {
    // Return our image:
    return image;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::getImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Image property, const version.
  ///\return  The current image.
  ///\remarks This image is the source for the toggle. It must contain two sub
  ///         pictures ordered from left to right.
  //////////////////////////////////////////////////////////////////////////////
  const QImage& getImage() const
  {
    // Return our image:
    return image;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::getDisabledImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DisabledImage property.
  ///\return  The current disabled state image.
  ///\remarks This image is shown when the widget is disabled. It should have
  ///         the same size as one frame of the toggle image.
  //////////////////////////////////////////////////////////////////////////////
  QImage& getDisabledImage()
  {
    // Return our image:
    return disabledImage;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::getDisabledImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DisabledImage property, const version.
  ///\return  The current disabled state image.
  ///\remarks This image is shown when the widget is disabled. It should have
  ///         the same size as one frame of the toggle image.
  //////////////////////////////////////////////////////////////////////////////
  const QImage& getDisabledImage() const
  {
    // Return our image:
    return disabledImage;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::getTag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Tag property.
  ///\return  The currently stored user value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  int getTag() const
  {
    // Return current tag:
    return tag;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::setTag()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Set accessor for the Tag property.
  ///\param   [in] newTag: The new user defined value.
  ///\remarks This tag is an arbitrary user defined value.
  //////////////////////////////////////////////////////////////////////////////
  void setTag(const int newTag)
  {
    // Set new tag:
    tag = newTag;
  }

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::paintEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the paint signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void paintEvent(QPaintEvent* event)
  {
    // Do we have a movie?
    if (image.isNull())
    {
      // Let the base class do the painting:
      QWidget::paintEvent(event);
      return;
    }

    // Draw the movie:
    QPainter qp(this);
    drawWidget(qp);
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::changeEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for general state change signals.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void changeEvent(QEvent* event)
  {
    // Base handling:
    QWidget::changeEvent(event);

    // Redraw if the enabled state changed:
    if (event->type() == QEvent::EnabledChange)
      update();
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // QImageLED::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Internal helperthat paints the actual widget.
  ///\param   [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  void drawWidget(QPainter& qp)
  {
    if (isEnabled() || disabledImage.isNull())
    {
      // Get size of a single sub image:
      int w = image.width() / 2;
      int h = image.height();

      // Calc source position:
      int x = value ? w : 0;

      // Finally blit the image:
      qp.drawImage(0, 0, image, x, 0, w, h);
    }

    else
    {
      // Just show the disabled image:
      qp.drawImage(0, 0, disabledImage);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  QImage image;         ///\> The knob movie image strip.
  QImage disabledImage; ///\> The knob movie image strip.
  bool value;           ///\> The current value of this toggle.
  int tag;              ///\> User defined value.
};

#endif // __QIMAGELED_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
