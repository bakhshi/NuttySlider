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
#include <bb/cascades/TouchBehavior>
#include <bb/cascades/TouchResponse>
#include <QDebug>

NuttySlider::NuttySlider(bb::cascades::Container* parent)
    :CustomControl(parent),
     m_fromValue(0.0),
     m_toValue(1.0),
     m_preferredHeight(100)
{
    setValue(m_fromValue);
    setImmediateValue(m_fromValue);
    setPreferredHeight(m_preferredHeight);
    m_rootContainer = bb::cascades::Container::create()
                  .layout(new bb::cascades::AbsoluteLayout())
                  .background(bb::cascades::Color::fromARGB(0xfff2eded));

    createProgressBar();
    createHandle();

    m_rootContainer->add(m_progressBar);
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

float NuttySlider::preferredHeight() const
{
    return m_preferredHeight;
}

void NuttySlider::handleLayoutFrameUpdated(QRectF frame)
{
    m_progressBar->setPreferredWidth(frame.width()
                                     - m_preferredHeight);
    m_width = frame.width();
    m_positionX = frame.x();
    setHandlePosX();
    qDebug() << "frame width : " << frame.width();
    qDebug() << "frame x : " << frame.x();
    qDebug() << "frame y : " << frame.y();
}

void NuttySlider::createConnections()
{
    bb::cascades::LayoutUpdateHandler::create(m_rootContainer)
        .onLayoutFrameChanged(this, SLOT(handleLayoutFrameUpdated(QRectF)));
    connect(m_handle, SIGNAL(touch(bb::cascades::TouchEvent*)),
            this, SLOT(sliderHandleTouched(bb::cascades::TouchEvent*)));
    connect(this, SIGNAL(valueChanged(float)), this, SLOT(setHandlePosX()));
    connect(this, SIGNAL(fromValueChanged(float)), this, SLOT(setHandlePosX()));
    connect(this, SIGNAL(toValueChanged(float)), this, SLOT(setHandlePosX()));
    connect(m_progressImageView, SIGNAL(preferredWidthChanged(float)),
             this, SLOT(handleProgressWidthChanged(float)));
}

void NuttySlider::createProgressBar()
{
    bb::cascades::AbsoluteLayoutProperties* layoutProperties
                            = bb::cascades::AbsoluteLayoutProperties::create();
    float progressBarHeight = 0.3 * m_preferredHeight;
    layoutProperties->setPositionX(m_preferredHeight / 2);
    layoutProperties->setPositionY((m_preferredHeight
                                    - progressBarHeight) / 2);

    m_progressBar = bb::cascades::Container::create()
                    .layout(new bb::cascades::DockLayout())
                    .layoutProperties(layoutProperties)
                    .preferredHeight(progressBarHeight)
                    .background(bb::cascades::Color::fromARGB(0xfffddded));

    bb::cascades::ImageView* barImageView =
            bb::cascades::ImageView::create("asset:///images/bar.amd")
                                    .preferredHeight(m_preferredHeight)
                                    .preferredWidth(m_width - m_preferredHeight)
                                    .horizontal(bb::cascades::HorizontalAlignment::Fill)
                                    .vertical(bb::cascades::VerticalAlignment::Center);

    m_progressImageView = bb::cascades::ImageView::create("asset:///images/progress.amd")
                                    .preferredHeight(m_preferredHeight)
                                    .preferredWidth(m_width - m_preferredHeight)
                                    .horizontal(bb::cascades::HorizontalAlignment::Left)
                                    .vertical(bb::cascades::VerticalAlignment::Center);
    m_progressBar->add(barImageView);
    m_progressBar->add(m_progressImageView);
}

void NuttySlider::createHandle()
{
    m_handleOnImg = bb::cascades
                 ::Image(QUrl("asset:///images/handle_pressed.png"));
    m_handleOffImg = bb::cascades
                  ::Image(QUrl("asset:///images/handle_inactive.png"));

    m_handle = bb::cascades::ImageView::create()
               .preferredWidth(m_preferredHeight)
               .preferredHeight(m_preferredHeight);

    m_handle->setImage(m_handleOffImg);

    m_handle->addTouchBehavior(
        bb::cascades::TouchBehavior::create()
            .addTouchReaction(bb::cascades::TouchType::Down,
                              bb::cascades::PropagationPhase::AtTarget,
                              bb::cascades::TouchResponse::StartTracking));
}

void NuttySlider::sliderHandleTouched(bb::cascades::TouchEvent* event)
{
    bb::cascades::TouchType::Type type = event->touchType();

    if(bb::cascades::TouchType::Down == type) {
        m_initX = event->windowX();
        bb::cascades::AbsoluteLayoutProperties* layoutProperties
        = dynamic_cast<bb::cascades::AbsoluteLayoutProperties*>(m_handle->layoutProperties());
        if(!layoutProperties)
            return;
        float handlePosX = layoutProperties->positionX() + m_positionX;
        m_handle->setImage(m_handleOnImg);
        m_dx = event->localX() - handlePosX;
        m_handleX = handlePosX;

        delete layoutProperties;

        qDebug() << "handlePosX : " << handlePosX;
    }

    if(bb::cascades::TouchType::Move == type) {
        qDebug() << "Initx : " << m_initX;
        qDebug() << "handlex : " << m_handleX;
        qDebug() << "dx : " <<  - m_initX + event->windowX();
        setHandlePosX(m_handleX - m_initX + event->windowX());
        bb::cascades::AbsoluteLayoutProperties* layoutProperties
        = dynamic_cast<bb::cascades::AbsoluteLayoutProperties*>(m_handle->layoutProperties());
        if(!layoutProperties)
            return;
        float handlePosX = layoutProperties->positionX();
        setImmediateValue(fromPosXToValue(handlePosX));
        m_progressImageView->setPreferredWidth(handlePosX);
        qDebug() << "event localX : " << event->localX() - m_dx;
    }

    if(bb::cascades::TouchType::Up == type) {

        m_handle->setImage(m_handleOffImg);
        bb::cascades::AbsoluteLayoutProperties* layoutProperties
        = dynamic_cast<bb::cascades::AbsoluteLayoutProperties*>(m_handle->layoutProperties());
        if(!layoutProperties)
            return;
        float handlePosX = layoutProperties->positionX();
        setValue(fromPosXToValue(handlePosX));
    }

    if(bb::cascades::TouchType::Cancel == type) {
//        m_handle->setImage(m_handleOffImg);
    }
}

void NuttySlider::setHandlePosX(float x)
{
    float startX = 0;
    float endX = m_width - m_preferredHeight;
    if(x < startX)
        x = startX;
    if(x > endX)
        x = endX;
    bb::cascades::AbsoluteLayoutProperties* layoutProperties
                            = bb::cascades::AbsoluteLayoutProperties::create();
    layoutProperties->setPositionX(x);
    m_handle->setLayoutProperties(layoutProperties);
}

void NuttySlider::setHandlePosX()
{
    float startX = 0;
    float endX = m_width - m_preferredHeight;
    float x = fromValueToPosX(m_value);
    if(x < startX)
        x = startX;
    if(x > endX)
        x = endX;

    bb::cascades::AbsoluteLayoutProperties* layoutProperties
                            = bb::cascades::AbsoluteLayoutProperties::create();
    layoutProperties->setPositionX(x);
    m_handle->setLayoutProperties(layoutProperties);
    m_progressImageView->setPreferredWidth(x);
}

float NuttySlider::fromValueToPosX(float value) const
{
    float factor = (value - m_fromValue) / (m_toValue - m_fromValue);

    return (m_width - m_preferredHeight) * factor;
}

float NuttySlider::fromPosXToValue(float posX) const
{
    float factor = posX / (m_width - m_preferredHeight);

    return factor * (m_toValue - m_fromValue) + m_fromValue;
}

void NuttySlider::handleProgressWidthChanged(float width)
{
    if(!width)
        m_progressImageView->setVisible(false);
    else {
        m_progressImageView->setVisible(true);
    }
}
