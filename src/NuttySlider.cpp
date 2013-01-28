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
     m_value(0),
     m_preferredHeight(100)
{
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
                    .layout(new bb::cascades::AbsoluteLayout())
                    .layoutProperties(layoutProperties)
                    .preferredHeight(progressBarHeight)
                    .background(bb::cascades::Color::fromARGB(0xfffddded));
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
        m_handle->setImage(m_handleOnImg);
        bb::cascades::AbsoluteLayoutProperties* layoutProperties
        = dynamic_cast<bb::cascades::AbsoluteLayoutProperties*>(m_handle->layoutProperties());
        if(!layoutProperties)
            return;
        float handlePosX = layoutProperties->positionX();
        m_dx = event->localX() - handlePosX;
        delete layoutProperties;
        qDebug("down");
    }

    if(bb::cascades::TouchType::Move == type) {
        setHandlePosX(event->localX() - m_dx);
    }

    if(bb::cascades::TouchType::Up == type) {
        m_handle->setImage(m_handleOffImg);
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
