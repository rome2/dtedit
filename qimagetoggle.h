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

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////
///\class QImageToggle qimagetoggle.h
///\brief -
////////////////////////////////////////////////////////////////////////////////
class QImageToggle :
  public QWidget
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
    QWidget(parent),
    value(false),
    leftRight(false)
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
  // QImageToggle::getValue()
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
    if (value == newVal)
      return;

    // Set new value:
    value = newVal;

    // Force redraw:
    update();

    // Notify listeners:
    if (!signalsBlocked())
      emit valueChanged(this, value);
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::getImage()
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
  // QImageToggle::getImage()
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
  // QImageToggle::getDisabledImage()
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
  // QImageToggle::getDisabledImage()
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
  // QImageToggle::getLeftRight()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the LeftRight property.
  ///\return  The current left to right mode.
  ///\remarks If this is true then we are working left to right instead of top
  ///         to bottom.
  //////////////////////////////////////////////////////////////////////////////
  bool getLeftRight() const
  {
    // Return current state:
    return leftRight;
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
    leftRight = newState;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::getTag()
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
  // QImageToggle::setTag()
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

signals:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::valueChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when the value of this dial changes.
  ///\param   [in] sender: The sending control.
  ///\param   [in] newVal: The new value.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged(QImageToggle* sender, bool newVal);

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::paintEvent()
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
  // QImageToggle::changeEvent()
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
      startPos.setX(event->x());
      startPos.setY(event->y());
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
      if (newVal != valueFromMousePos(startPos.x(), startPos.y()))
        return;

      // Update widget:
      setValue(newVal);
    }
  }

private:

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Internal helper to calc a value for mouse coordinates.
  ///\param   [in] mx: Input position, X component.
  ///\param   [in] my: Input position, Y component.
  ///\return  The matching value for the input coordinates.
  //////////////////////////////////////////////////////////////////////////////
  bool valueFromMousePos(const int mx, const int my) const
  {
    // Get center:
    int bx = width() / 2;
    int by = height() / 2;

    // Find matching half:
    if (leftRight)
      return mx < bx;
    return my < by;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageToggle::drawWidget()
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
  QPoint startPos;      ///\> Mouse down position.
  QImage image;         ///\> The knob movie image strip.
  QImage disabledImage; ///\> The knob movie image strip.
  bool value;           ///\> The current value of this toggle.
  bool leftRight;       ///\> Work left to right?
  int tag;              ///\> User defined value.
};

#endif // __QIMAGETOGGLE_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
