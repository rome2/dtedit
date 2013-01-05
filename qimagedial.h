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

#include <QtGui>

////////////////////////////////////////////////////////////////////////////////
///\class QImageDial qimagedial.h
///\brief An image based QDial alternative.
/// This widget serves basically the same purpose as the QDial control but it is
/// based on an image strip instead of custom drawing. Additionally it works in
/// linear mode as well and it supports relative movements. A default value can
/// be set that will be recalled when a user double clicks the widget.
///
/// Simple usage example:
///
/// QImageDial* dial = new QImageDial(this);
/// dial->getImage().load(":/images/knob.png");
/// dial->getDisabledImage().load(":/images/knob_disabled.png");
/// dial->setFrameCount(31);
/// dial->setGeometry(10, 10, 40, 40);
/// dial->setEnabled(true);
/// connect(dial, SIGNAL(valueChanged(QImageDial*, double)), this, SLOT(dialValueChanged(QImageDial*, double)));
///
////////////////////////////////////////////////////////////////////////////////
class QImageDial :
  public QWidget
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
    QWidget(parent),
    value(0.5),
    defaultValue(0.5),
    frameCount(0),
    absoluteMode(false),
    circularMode(false),
    startY(0),
    startVal(0.5),
    linearSize(128),
    tag(0)
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
  // QImageDial::getValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Value property.
  ///\return  The current value of this dial.
  ///\remarks The value is always in the range [0,1].
  //////////////////////////////////////////////////////////////////////////////
  double getValue() const
  {
    // Return current value:
    return value;
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
    value = newVal;

    // Clip value in the range [0,1]:
    if (value < 0.0)
      value = 0.0;
    else if (value > 1.0)
      value = 1.0;

    // Schedule redraw:
    update();

    // Notify listeners:
    if (!signalsBlocked())
      emit valueChanged(this, value);
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getDefaultValue()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DefaultValue property.
  ///\return  The default value of this dial.
  ///\remarks The value is always in the range [0,1]. To load the default value
  ///\        just double click the widget.
  //////////////////////////////////////////////////////////////////////////////
  double getDefaultValue() const
  {
    // Return current value:
    return defaultValue;
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
    defaultValue = newVal;

    // Clip value in the range [0,1]:
    if (defaultValue < 0.0)
      defaultValue = 0.0;
    else if (defaultValue > 1.0)
      defaultValue = 1.0;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Image property.
  ///\return  The current image.
  ///\remarks This image is the source for the knob movie. It must contain
  ///         FrameCount sub pictures ordered from left to right.
  //////////////////////////////////////////////////////////////////////////////
  QImage& getImage()
  {
    // Return our image:
    return image;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the Image property, const version.
  ///\return  The current image.
  ///\remarks This image is the source for the knob movie. It must contain
  ///         FrameCount sub pictures ordered from left to right.
  //////////////////////////////////////////////////////////////////////////////
  const QImage& getImage() const
  {
    // Return our image:
    return image;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getDisabledImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DisabledImage property.
  ///\return  The current disabled state image.
  ///\remarks This image is shown when the widget is disabled. It should have
  ///         the same size as one frame of the knob movie.
  //////////////////////////////////////////////////////////////////////////////
  QImage& getDisabledImage()
  {
    // Return our image:
    return disabledImage;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getDisabledImage()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the DisabledImage property, const version.
  ///\return  The current disabled state image.
  ///\remarks This image is shown when the widget is disabled. It should have
  ///         the same size as one frame of the knob movie.
  //////////////////////////////////////////////////////////////////////////////
  const QImage& getDisabledImage() const
  {
    // Return our image:
    return disabledImage;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getFrameCount()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Get accessor for the FrameCount property.
  ///\return  The current number of frames for the knob movie.
  ///\remarks This property tells this widget how man frames there are in the
  ///         knob movie and thus how to determine the subpicture to show.
  //////////////////////////////////////////////////////////////////////////////
  int getFrameCount() const
  {
    // Return current count:
    return frameCount;
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
    frameCount = newCount;

    // Clip value:
    if (frameCount <= 0)
      frameCount = 1;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getAbsoluteMode()
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
    absoluteMode = newState;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getCircularMode()
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
    circularMode = newState;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getLinearSize()
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
    linearSize = newSize;

    // Clip size:
    if (linearSize <= 0)
      linearSize = 1;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::getTag()
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
  // QImageDial::setTag()
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
  // QImageDial::valueChanged()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when the value of this dial changes.
  ///\param   [in] sender: The sending control.
  ///\param   [in] newVal: The new value.
  //////////////////////////////////////////////////////////////////////////////
  void valueChanged(QImageDial* sender, double newVal);

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::mouseReleased()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   This signal is emitted when the mouse was released.
  ///\param   [in] sender: The sending control.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleased(QImageDial* sender);

protected:

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::paintEvent()
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
  // QImageDial::changeEvent()
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
  // QImageDial::wheelEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse wheel signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void wheelEvent(QWheelEvent* event)
  {
    //Clac value delta:
    double delta = event->delta() * 0.00025;

    // Update value:
    setValue(value + delta);
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::mousePressEvent()
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

      // No, linear is the way to go:
      else
      {
        // Save start values:
        startVal = value;
        startY   = event->y();
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageButton::mouseReleaseEvent()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Handler for the mouse button released signal.
  ///\param   [in] event: Provides further details about the event.
  //////////////////////////////////////////////////////////////////////////////
  void mouseReleaseEvent(QMouseEvent* /* event */)
  {
    // Enabled?
    if (!isEnabled())
        return;

    // Notify listeners:
    if (!signalsBlocked())
      emit mouseReleased(this);
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::mouseDoubleClickEvent()
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
  // QImageDial::mouseMoveEvent()
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
        double val = valueFromMousePos(event->x(), event->y());

        // Set value:
        if (absoluteMode)
          setValue(val);
        else
        {
          // Set new value relative to the last value:
          setValue(value + (val - startVal));

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

private:

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::valueFromMousePos()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Internal helper to calc a dial value for mouse coordinates.
  ///\param   [in] mx: Input position, X component.
  ///\param   [in] my: Input position, Y component.
  ///\return  The matching value for the input coordinates.
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
    return value;
  }

  //////////////////////////////////////////////////////////////////////////////
  // QImageDial::drawWidget()
  //////////////////////////////////////////////////////////////////////////////
  ///\brief   Internal helperthat paints the actual widget.
  ///\param   [in] qp: Device context to use.
  //////////////////////////////////////////////////////////////////////////////
  void drawWidget(QPainter& qp)
  {
    if (isEnabled() || disabledImage.isNull())
    {
      // Get size of a single sub image:
      int w = image.width() / frameCount;
      int h = image.height();

      // Calc active frame:
      int l = (int)(value * (frameCount - 1));
      if (l < 0)
        l = 0;
      if (l > (frameCount - 1))
        l = frameCount - 1;

      // Calc source position:
      int x = w * l;
      int y = 0;

      // Finally blit the image:
      qp.drawImage(0, 0, image, x, y, w, h);
    }

    else
    {
      // Just show the disabled image:
      qp.drawImage(0, 0, disabledImage);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Member:
  double value;         ///\> The current value of this dial.
  double defaultValue;  ///\> The default value of this value.
  int frameCount;       ///\> The number of frames in the knob movie image.
  bool absoluteMode;    ///\> Use absolute or relative movement?
  bool circularMode;    ///\> Use linear or circular movement?
  int startY;           ///\> Mouse down position for linear movement.
  double startVal;      ///\> Value when the movement starts.
  QImage image;         ///\> The knob movie image strip.
  QImage disabledImage; ///\> The knob movie image strip.
  int linearSize;       ///\> Scaling for linear movement.
  int tag;              ///\> User defined value.
};

#endif // __QIMAGEDIAL_H_INCLUDED__
///////////////////////////////// End of File //////////////////////////////////
