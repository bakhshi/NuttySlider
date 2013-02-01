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

using namespace bb::cascades;

class NuttySlider : public CustomControl {
    Q_OBJECT

    Q_PROPERTY(float value READ value WRITE setValue NOTIFY valueChanged FINAL)
    Q_PROPERTY(float preferredHeight READ preferredHeight FINAL)
    Q_PROPERTY(float fromValue READ fromValue WRITE setFromValue NOTIFY fromValueChanged FINAL)
    Q_PROPERTY(float toValue READ toValue WRITE setToValue NOTIFY toValueChanged FINAL)
    Q_PROPERTY(float immediateValue READ immediateValue WRITE setImmediateValue NOTIFY immediateValueChanged FINAL)

public:
    NuttySlider(Container* parent = 0);
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
    void updateHandlePositionX();

private:
    void createConnections();
    void createProgressBar();
    void createHandle();


    float fromValueToPosX(float value) const;
    float fromPosXToValue(float positionX) const;

private:
    // root container
    Container* m_rootContainer;
    float m_rootContainerWidth;
    float m_rootContainerHeight;
    float m_rootContainerPositionX;


    // progress bar
    Container* m_progressBarContainer;
    const float m_progressBarContainerHeight;
    ImageView* m_progressBarImageView;
    Image m_progressBarImage;
    Image m_progressBarImagePressed;

    // handle
    ImageView* m_handle;
    Image m_handleOnImg;
    Image m_handleOffImg;

    // properties
    float m_value;
    float m_fromValue;
    float m_toValue;
    float m_immediateValue;

    AbsoluteLayoutProperties* m_handleLayoutProperties;

    float m_touchEventInitX;
    float m_handleInitX;

    bool m_handleTouched;
};

#endif /* NUTTYSLIDER_H_ */
