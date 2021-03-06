/*
 * NuttySlider.cpp
 *
 *  Created on: Jan 27, 2013
 *      Author: bmnatsakanyan
 */

#include "CustomSlider.h"
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/Color>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/TouchBehavior>
#include <bb/cascades/TouchResponse>
#include <bb/cascades/ImplicitAnimationController>
#include <bb/cascades/ImageViewLoadEffect>
#include <bb/cascades/LongPressHandler>
#include <bb/cascades/LongPressEvent>
#include <bb/cascades/ImageTracker>
#include <QTimer>



CustomSlider::CustomSlider(Container* parent)
    :CustomControl(parent),
     m_rootContainerHeight(100),
     m_progressBarContainerHeight(25),
     m_fromValue(0.0),
     m_toValue(1.0)
{
    setUpdateInterval(1);
    setValue(m_fromValue);
    setPreferredHeight(m_rootContainerHeight);
    m_rootContainer = Container::create()
                  .layout(new AbsoluteLayout())
                  .preferredHeight(m_rootContainerHeight)
                  .background(Color::fromARGB(0x10f2eded));

    m_rootContainer->setMaxHeight(m_rootContainerHeight);

    createProgressBar();
    createHandle();
    m_dummyContainer = Container::create()
                  .layout(new AbsoluteLayout())
                  .background(Color::fromARGB(0x000088cc));
    m_dummyContainer->addTouchBehavior(
        TouchBehavior::create()
            .addTouchReaction(TouchType::Down,
                              PropagationPhase::AtTarget,
                              TouchResponse::StartTracking));
//    m_rootContainer->addTouchBehavior(
//        TouchBehavior::create()
//            .addTouchReaction(TouchType::Down,
//                              PropagationPhase::Bubbling,
//                              TouchResponse::StartTracking));


    m_rootContainer->add(m_progressBarContainer);
    m_rootContainer->add(m_dummyContainer);
    m_rootContainer->add(m_handle);
    setRoot(m_rootContainer);

    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    createConnections();
}

float CustomSlider::value() const
{
    return m_value;
}

void CustomSlider::setValue(float value)
{
    if(m_value == value)
        return;

    m_value = value;

    if(m_immediateValue != m_value)
        setImmediateValue(m_value);

    emit valueChanged(m_value);
}

float CustomSlider::fromValue() const
{
    return m_fromValue;
}

void CustomSlider::setFromValue(float value)
{
    if(m_fromValue == value)
        return;

    m_fromValue = value;
    emit fromValueChanged(m_fromValue);
//    setValue(m_fromValue);
}

float CustomSlider::toValue() const
{
    return m_toValue;
}

void CustomSlider::setToValue(float value)
{
    if(m_toValue == value)
        return;

    m_toValue = value;
//    setValue(m_fromValue);
    emit toValueChanged(m_toValue);
}

float CustomSlider::immediateValue() const
{
    return m_immediateValue;
}

void CustomSlider::setImmediateValue(float value, bool fireEvent)
{
    if(m_immediateValue == value)
        return;

    m_immediateValue = value;

    updateHandlePositionX();

    if(fireEvent)
        emit immediateValueChanged(m_immediateValue);
}

bool CustomSlider::dragging() const
{
    return m_dragging;
}

void CustomSlider::setDragging(bool draggingState)
{
    if(m_dragging == draggingState)
        return;

    m_dragging = draggingState;
    emit draggingChanged(m_dragging);
}
void CustomSlider::resetValue()
{
    setValue(m_fromValue);
}

void CustomSlider::handleLayoutFrameUpdated(QRectF frame)
{
    m_progressBarContainer->setPreferredWidth(frame.width() - m_rootContainerHeight);
    m_rootContainerWidth = frame.width();
    m_rootContainerHeight = frame.height();
    m_rootContainerPositionX = frame.x();
    m_dummyContainer->setPreferredWidth(m_rootContainerWidth);
    m_dummyContainer->setPreferredHeight(m_rootContainerHeight);
    updateHandlePositionX();
}

void CustomSlider::createConnections()
{
    LayoutUpdateHandler::create(m_rootContainer)
        .onLayoutFrameChanged(this, SLOT(handleLayoutFrameUpdated(QRectF)));
    connect(m_handle, SIGNAL(touch(bb::cascades::TouchEvent*)),
            this, SLOT(sliderHandleTouched(bb::cascades::TouchEvent*)));
    //connect(this, SIGNAL(immediateValueChanged(float)), this, SLOT(updateHandlePositionX()));
    connect(this, SIGNAL(fromValueChanged(float)), this, SLOT(updateHandlePositionX()));
    connect(this, SIGNAL(toValueChanged(float)), this, SLOT(updateHandlePositionX()));
    connect(m_dummyContainer, SIGNAL(touch(bb::cascades::TouchEvent*)),
            this, SLOT(progressBarTouched(bb::cascades::TouchEvent*)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onDragUpdateTimerTimeout()));
    connect(this, SIGNAL(preferredWidthChanged(float)), this, SLOT(updateRootContainerPreferredWidth(float)));


    bool result = connect(m_handleImageTracker, SIGNAL(sizeChanged(int, int)),
                    this, SLOT(onHandleImageSizeChanged(int, int)));
    Q_ASSERT(result);
    Q_UNUSED(result);
}

void CustomSlider::createProgressBar()
{
    AbsoluteLayoutProperties* layoutProperties
                            = AbsoluteLayoutProperties::create();
    layoutProperties->setPositionX(m_rootContainerHeight / 2);
    layoutProperties->setPositionY((m_rootContainerHeight
                                    - m_progressBarContainerHeight) / 2);

    m_progressBarContainer = Container::create()
                    .layout(new DockLayout())
                    .layoutProperties(layoutProperties)
                    .preferredHeight(m_progressBarContainerHeight)
                    .background(Color::fromARGB(0xfffddded));

    ImageView* barImageView =
            ImageView::create("asset:///images/bar.amd")
                                    .preferredHeight(m_progressBarContainerHeight)
                                    .horizontal(HorizontalAlignment::Fill)
                                    .vertical(VerticalAlignment::Center);

    m_progressBarImage = bb::cascades
                 ::Image(QUrl("asset:///images/progress.amd"));
    m_progressBarImagePressed = bb::cascades
                  ::Image(QUrl("asset:///images/progress_pressed.amd"));
    m_progressBarImageView = ImageView::create()
                                    .preferredHeight(m_progressBarContainerHeight)
                                    .horizontal(HorizontalAlignment::Left)
                                    .vertical(VerticalAlignment::Center);
    m_progressBarImageView->setImage(m_progressBarImage);
    m_progressBarImageView->setImplicitLayoutAnimationsEnabled(false);

    m_progressBarContainer->add(barImageView);
    m_progressBarContainer->add(m_progressBarImageView);
}

void CustomSlider::createHandle()
{
    m_handleOnImg = bb::cascades
                 ::Image(QUrl("asset:///images/handle_pressed.png"));
    m_handleOffImg = bb::cascades
                  ::Image(QUrl("asset:///images/handle_inactive.png"));

    m_handle = ImageView::create()
               .preferredWidth(m_rootContainerHeight)
               .preferredHeight(m_rootContainerHeight);

    m_handleImageTracker = new ImageTracker(m_handleOnImg.source());


    AbsoluteLayoutProperties* lp = AbsoluteLayoutProperties::create();
    m_handle->setLayoutProperties(lp);

    m_handle->setImage(m_handleOffImg);
    m_handle->setImplicitLayoutAnimationsEnabled(false);

    m_handle->addTouchBehavior(
        TouchBehavior::create()
            .addTouchReaction(TouchType::Down,
                              PropagationPhase::AtTarget,
                              TouchResponse::StartTracking));
    LongPressHandler* longPressHandler = LongPressHandler::create()
        .onLongPressed(this, SLOT(onHandleLongPressed(bb::cascades::LongPressEvent*)));
    m_handle->addGestureHandler(longPressHandler);
}

void CustomSlider::sliderHandleTouched(TouchEvent* event)
{

    if(event->propagationPhase() == PropagationPhase::AtTarget && isEnabled()) {
        m_handleTouched = true;
        TouchType::Type type  = event->touchType();
        AbsoluteLayoutProperties* layoutProperties
            = dynamic_cast<AbsoluteLayoutProperties*>(m_handle->layoutProperties());

        if(!layoutProperties) {
            return;
        }

        if(TouchType::Down == type) {
            m_progressBarImageView->setImage(m_progressBarImagePressed);
            m_handle->setImage(m_handleOnImg);
            m_touchEventInitX = event->windowX();
            m_handleInitX = layoutProperties->positionX() + m_rootContainerPositionX;
            return;
        }

        float handlePosX = m_handleInitX - m_touchEventInitX + event->windowX();

        if(TouchType::Move == type) {
            if(!m_handleLongPressed) {
                setDragging(true);
                if(!m_timer->isActive()) {
                    setImmediateValue(fromPosXToValue(handlePosX), true);
                    m_timer->start(m_updateInterval);
                }
                else
                    setImmediateValue(fromPosXToValue(handlePosX), false);

                return;
            }
            else {
                emit move(event->windowX());
            }
        }

        if(TouchType::Up == type) {
            m_handle->setImage(m_handleOffImg);
            m_progressBarImageView->setImage(m_progressBarImage);
            float handlePosX = layoutProperties->positionX();
            if(!m_handleLongPressed)
                setValue(fromPosXToValue(handlePosX));
            m_handleTouched = false;
            setDragging(false);
            if(m_timer->isActive())
                m_timer->stop();
            m_handleLongPressed = false;
            emit handleReleased();

            return;
        }
    }

}

void CustomSlider::progressBarTouched(TouchEvent* event)
{
    if(event->propagationPhase() == PropagationPhase::AtTarget && isEnabled()) {
        TouchType::Type type = event->touchType();
        float handlePosX = event->localX() - m_rootContainerHeight / 2;

        if(TouchType::Down == type) {
            m_handle->setImage(m_handleOnImg);
            m_progressBarImageView->setImage(m_progressBarImagePressed);
            setImmediateValue(fromPosXToValue(handlePosX));
            return;
        }

        if(TouchType::Move == type) {
            setDragging(true);
            if(!m_timer->isActive()) {
                setImmediateValue(fromPosXToValue(handlePosX));
                m_timer->start(m_updateInterval);
            }
            return;
        }
        else
            setImmediateValue(fromPosXToValue(handlePosX), false);

        if(TouchType::Up == type) {
            m_handle->setImage(m_handleOffImg);
            m_progressBarImageView->setImage(m_progressBarImage);
            setValue(fromPosXToValue(handlePosX));
            setDragging(false);
            if(m_timer->isActive())
                m_timer->stop();
            return;
        }
    }
}

void CustomSlider::updateHandlePositionX()
{
    float x = fromValueToPosX(m_immediateValue);

    Q_ASSERT(dynamic_cast<AbsoluteLayoutProperties*>(m_handle->layoutProperties()) != 0);
    ((AbsoluteLayoutProperties*)m_handle->layoutProperties())->setPositionX(x);
    if(x == 0) {
        m_progressBarImageView->setVisible(false);
    }
    else {
        m_progressBarImageView->setVisible(true);
        m_progressBarImageView->setPreferredWidth(x);
    }
}

float CustomSlider::fromValueToPosX(float value) const
{
    float factor = (value - m_fromValue) / (m_toValue - m_fromValue);

    return (m_rootContainerWidth - m_rootContainerHeight) * factor;
}

float CustomSlider::fromPosXToValue(float positionX) const
{
    float endX = m_rootContainerWidth - m_rootContainerHeight;

    if(positionX < 0)
        positionX = 0;
    if(positionX > endX)
        positionX = endX;

    float factor = positionX / endX;

    return factor * (m_toValue - m_fromValue) + m_fromValue;
}

void CustomSlider::onDragUpdateTimerTimeout()
{
    emit immediateValueChanged(m_immediateValue);
}

void CustomSlider::setUpdateInterval(int interval)
{
    if(m_updateInterval == interval)
        return;

    m_updateInterval = interval;
}

void CustomSlider::onHandleLongPressed(bb::cascades::LongPressEvent* event)
{
    m_handleLongPressed = true;
    emit handleLongPressed(event->x());
}

float CustomSlider::handleLocalX() const
{
    return fromValueToPosX(m_immediateValue);
}

void CustomSlider::onHandleImageSizeChanged(int width, int height)
{
    m_handleSize.setWidth(width);
    m_handleSize.setHeight(height);
    emit handleSizeChanged(m_handleSize);
}

QSize CustomSlider::handleSize() const
{
    return m_handleSize;
}

void CustomSlider::updateRootContainerPreferredWidth(float width)
{
    m_rootContainerWidth = width;
}
