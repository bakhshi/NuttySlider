/*
 * NuttySlider.cpp
 *
 *  Created on: Jan 27, 2013
 *      Author: bmnatsakanyan
 */

#include "NuttySlider.h"
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



NuttySlider::NuttySlider(Container* parent)
    :CustomControl(parent),
     m_rootContainerHeight(100),
     m_progressBarContainerHeight(25),
     m_fromValue(0.0),
     m_toValue(1.0)
{
    setValue(m_fromValue);
    setPreferredHeight(m_rootContainerHeight);
    m_rootContainer = Container::create()
                  .layout(new AbsoluteLayout())
                  .background(Color::fromARGB(0xfff2eded));

    createProgressBar();
    createHandle();
    m_rootContainer->addTouchBehavior(
        TouchBehavior::create()
            .addTouchReaction(TouchType::Down,
                              PropagationPhase::AtTarget,
                              TouchResponse::StartTracking));
    m_rootContainer->addTouchBehavior(
        TouchBehavior::create()
            .addTouchReaction(TouchType::Down,
                              PropagationPhase::Bubbling,
                              TouchResponse::StartTracking));

    m_rootContainer->add(m_progressBarContainer);
    m_rootContainer->add(m_handle);
    setRoot(m_rootContainer);
    createConnections();
}

float NuttySlider::value() const
{
    return m_value;
}

void NuttySlider::setValue(float value)
{
    if(m_value == value)
        return;

    m_value = value;

    if(m_immediateValue != m_value)
        setImmediateValue(m_value);

    emit valueChanged(m_value);
}

float NuttySlider::fromValue() const
{
    return m_fromValue;
}

void NuttySlider::setFromValue(float value)
{
    if(m_fromValue == value)
        return;

    m_fromValue = value;
    emit fromValueChanged(m_fromValue);
}

float NuttySlider::toValue() const
{
    return m_toValue;
}

void NuttySlider::setToValue(float value)
{
    if(m_toValue == value)
        return;

    m_toValue = value;
    emit toValueChanged(m_toValue);
}

float NuttySlider::immediateValue() const
{
    return m_immediateValue;
}

void NuttySlider::setImmediateValue(float value)
{
    if(m_immediateValue == value)
        return;

    m_immediateValue = value;
    emit immediateValueChanged(m_immediateValue);
}

void NuttySlider::resetValue()
{
    setValue(m_fromValue);
}

void NuttySlider::handleLayoutFrameUpdated(QRectF frame)
{
    m_progressBarContainer->setPreferredWidth(frame.width() - m_rootContainerHeight);
    m_rootContainerWidth = frame.width();
    m_rootContainerHeight = frame.height();
    m_rootContainerPositionX = frame.x();
    updateHandlePositionX();
}

void NuttySlider::createConnections()
{
    LayoutUpdateHandler::create(m_rootContainer)
        .onLayoutFrameChanged(this, SLOT(handleLayoutFrameUpdated(QRectF)));
    connect(m_handle, SIGNAL(touch(bb::cascades::TouchEvent*)),
            this, SLOT(sliderHandleTouched(bb::cascades::TouchEvent*)));
    connect(this, SIGNAL(immediateValueChanged(float)), this, SLOT(updateHandlePositionX()));
    connect(this, SIGNAL(fromValueChanged(float)), this, SLOT(updateHandlePositionX()));
    connect(this, SIGNAL(toValueChanged(float)), this, SLOT(updateHandlePositionX()));
    connect(m_rootContainer, SIGNAL(touch(bb::cascades::TouchEvent*)),
            this, SLOT(progressBarTouched(bb::cascades::TouchEvent*)));
}

void NuttySlider::createProgressBar()
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

void NuttySlider::createHandle()
{
    m_handleOnImg = bb::cascades
                 ::Image(QUrl("asset:///images/handle_pressed.png"));
    m_handleOffImg = bb::cascades
                  ::Image(QUrl("asset:///images/handle_inactive.png"));

    m_handle = ImageView::create()
               .preferredWidth(m_rootContainerHeight)
               .preferredHeight(m_rootContainerHeight);


    AbsoluteLayoutProperties* lp = AbsoluteLayoutProperties::create();
    m_handle->setLayoutProperties(lp);

    m_handle->setImage(m_handleOffImg);
    m_handle->setImplicitLayoutAnimationsEnabled(false);

    m_handle->addTouchBehavior(
        TouchBehavior::create()
            .addTouchReaction(TouchType::Down,
                              PropagationPhase::AtTarget,
                              TouchResponse::StartTracking));
}

void NuttySlider::sliderHandleTouched(TouchEvent* event)
{

    if(event->propagationPhase() == PropagationPhase::AtTarget) {
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
            setImmediateValue(fromPosXToValue(handlePosX));
            return;
        }

        if(TouchType::Up == type || TouchType::Cancel == type) {
            m_handle->setImage(m_handleOffImg);
            m_progressBarImageView->setImage(m_progressBarImage);
            float handlePosX = layoutProperties->positionX();
            setValue(fromPosXToValue(handlePosX));
            m_handleTouched = false;
            return;
        }
    }

}

void NuttySlider::progressBarTouched(TouchEvent* event)
{
    if(!m_handleTouched) {
        TouchType::Type type = event->touchType();
        float handlePosX = event->localX() - m_rootContainerHeight / 2;

        if(TouchType::Down == type) {
            m_handle->setImage(m_handleOnImg);
            m_progressBarImageView->setImage(m_progressBarImagePressed);
            setImmediateValue(fromPosXToValue(handlePosX));
            return;
        }

        if(TouchType::Move == type) {
            setImmediateValue(fromPosXToValue(handlePosX));
            return;
        }

        if(TouchType::Up == type) {
            m_handle->setImage(m_handleOffImg);
            m_progressBarImageView->setImage(m_progressBarImage);
            setValue(fromPosXToValue(handlePosX));
            return;
        }
    }
}

void NuttySlider::updateHandlePositionX()
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

float NuttySlider::fromValueToPosX(float value) const
{
    float factor = (value - m_fromValue) / (m_toValue - m_fromValue);

    return (m_rootContainerWidth - m_rootContainerHeight) * factor;
}

float NuttySlider::fromPosXToValue(float positionX) const
{
    float endX = m_rootContainerWidth - m_rootContainerHeight;

    if(positionX < 0)
        positionX = 0;
    if(positionX > endX)
        positionX = endX;

    float factor = positionX / endX;

    return factor * (m_toValue - m_fromValue) + m_fromValue;
}
