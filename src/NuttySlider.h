/*
 * NuttySlider.h
 *
 *  Created on: Jan 27, 2013
 *      Author: bmnatsakanyan
 */

#ifndef NUTTYSLIDER_H_
#define NUTTYSLIDER_H_

#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>

namespace bb {
    namespace cascades {
        class Container;
        class ImageView;
        class AbsoluteLayoutProperties;
    }
}

class NuttySlider : public bb::cascades::CustomControl {
    Q_OBJECT

    Q_PROPERTY(float value READ value WRITE setValue NOTIFY valueChanged FINAL)
    Q_PROPERTY(float preferredHeight READ preferredHeight FINAL)
    Q_PROPERTY(float fromValue READ fromValue WRITE setFromValue NOTIFY fromValueChanged FINAL)
    Q_PROPERTY(float toValue READ toValue WRITE setToValue NOTIFY toValueChanged FINAL)
    Q_PROPERTY(float immediateValue READ immediateValue WRITE setImmediateValue NOTIFY immediateValueChanged FINAL)

public:
    NuttySlider(bb::cascades::Container* parent = 0);
    float value() const;
    void setValue(float value);

    float fromValue() const;
    void setFromValue(float value);

    float toValue() const;
    void setToValue(float value);

    float immediateValue() const;
    void setImmediateValue(float value);

    float preferredHeight() const;

Q_SIGNALS:
    void valueChanged(float value);
    void fromValueChanged(float value);
    void toValueChanged(float value);
    void immediateValueChanged(float value);

private Q_SLOTS:
    void handleLayoutFrameUpdated(QRectF frame);
    void sliderHandleTouched(bb::cascades::TouchEvent* event);
    void progressBarTouched(bb::cascades::TouchEvent* event);
    void handleProgressWidthChanged(float width);

private:
    void createConnections();
    void createProgressBar();
    void createHandle();
    void setHandlePosX(float x);
    void updateHandlePosX();
    float fromValueToPosX(float value) const;
    float fromPosXToValue(float posX) const;

private:
    // root container
    bb::cascades::Container* m_rootContainer;
    float m_rootContainerWidth;
    float m_rootContainerHeight;
    float m_rootContainerPositionX;


    // progress bar
    bb::cascades::Container* m_progressBarContainer;
    const float m_progressBarContainerHeight;
    bb::cascades::Image m_progressBarImage;
    bb::cascades::Image m_progressBarImagePressed;


    bb::cascades::ImageView* m_handle;
    bb::cascades::Image m_handleOnImg;
    bb::cascades::Image m_handleOffImg;
    bb::cascades::ImageView* m_progressImageView;
    float m_value;
    float m_fromValue;
    float m_toValue;
    float m_immediateValue;

    bb::cascades::AbsoluteLayoutProperties* m_handleLayoutProperties;

    float m_initX;
    float m_handleX;
    float m_dx;
};

#endif /* NUTTYSLIDER_H_ */
