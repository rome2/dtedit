////////////////////////////////////////////////////////////////////////////////
// (c) 2012 Rolf Meyerhoff. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
///\file    qimagebutton.h
///\brief   Image based button widget definition.
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
#ifndef __QIMAGEBUTTON_H_INCLUDED__
#define __QIMAGEBUTTON_H_INCLUDED__

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////
///\class QImageButton qimagebutton.h
///\brief -
////////////////////////////////////////////////////////////////////////////////
class QImageButton :
  public QWidget
{
  Q_OBJECT// Qt magic...

public:

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::QImageButton()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Default constructor of this widget.
  ///\param   [in] parent: Parent window for this widget.
  ///\remarks Just initializes the members.
  //////////////////////////////////////////////////////////////////////////////
  QImageButton(QWidget* parent = 0) :
    QWidget(parent),
    down(false)
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::~QImageButton()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Destructor of this window.
  ///\remarks Cleans up used resources.
  //////////////////////////////////////////////////////////////////////////////
  virtual ~QImageButton()
  {
    // Nothing to do here.
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::getImage()
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
  // QImageButton::getImage()
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
  // QImageButton::getDisabledImage()
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
  // QImageButton::getDisabledImage()
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
  // QImageButton::getTag()
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
  // QImageButton::setTag()
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
  // QImageButton::clicked()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when this button was pressed.
  ///\param   [in] sender: The sending control.
  //////////////////////////////////////////////////////////////////////////////
  void clicked(QImageButton* sender);

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::paintEvent()
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
  // QImageButton::changeEvent()
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
  // QImageButton::mousePressEvent()
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
      // Update display:
      down = true;
      update();
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse button released signal.
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
      // Force redraw:
      down = false;
      update();

      // Check bounds:
      if (event->x() < 0 || event->x() >= width())
        return;
      if (event->y() < 0 || event->y() >= height())
        return;

      // Notify listeners:
      if (!signalsBlocked())
        emit clicked(this);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::mouseMoveEvent()
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
      // Check bounds:
      bool inside = true;
      if (event->x() < 0 || event->x() >= width() || event->y() < 0 || event->y() >= height())
        inside = false;

      // State changed?
      if (inside != down)
      {
        // Repaint:
        down = inside;
        update();
      }
    }
  }
private:

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::drawWidget()
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
      int x = down ? w : 0;

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
  bool down;           ///\> The current value of this toggle.
  int tag;              ///\> User defined value.
};

#endif // __QIMAGEBUTTON_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
