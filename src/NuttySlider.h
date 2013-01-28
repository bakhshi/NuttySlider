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

public:
    NuttySlider(bb::cascades::Container *parent = 0);
    float value() const;
    void setValue(float value);
    float preferredHeight() const;

 Q_SIGNALS:
    void valueChanged(float value);

private Q_SLOTS:
    void handleLayoutFrameUpdated(QRectF frame);
    void sliderHandleTouched(bb::cascades::TouchEvent* event);

private:
    void createConnections();
    void createProgressBar();
    void createHandle();
    void setHandlePosX(float x);

private:
    bb::cascades::Container* m_rootContainer;
    bb::cascades::Container* m_progressBar;
    bb::cascades::ImageView* m_handle;
    bb::cascades::Image m_handleOnImg;
    bb::cascades::Image m_handleOffImg;
    float m_value;
    const float m_preferredHeight;
    bb::cascades::AbsoluteLayoutProperties* m_handleLayoutProperties;
    float m_width;
    float m_positionX;
    float m_dx;
};

#endif /* NUTTYSLIDER_H_ */
