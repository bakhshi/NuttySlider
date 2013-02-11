import bb.cascades 1.0
import nutty.ui 1.0


Page {
    Container {
        id: sliderComponent
        property real smallStepsSliderWidth: 300
        property real deltaValue: 0.05

        layout: AbsoluteLayout {}

        CustomSlider {
            id: customSlider
            property real longPressX

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            onHandleLongPressed: {
                smallStepsSlider.visible = true;

                var currentValue = customSlider.value;
                smallStepsSlider.value = currentValue;
                if(customSlider.value - sliderComponent.deltaValue < customSlider.fromValue) {
                    smallStepsSlider.fromValue = customSlider.fromValue;
                    smallStepsSlider.toValue = customSlider.value + sliderComponent.deltaValue;
                    smallStepsSlider.preferredWidth = sliderComponent.smallStepsSliderWidth * (smallStepsSlider.toValue - smallStepsSlider.fromValue) / (2 * sliderComponent.deltaValue) + smallStepsSlider.handleSize.width;
                }
                else if(customSlider.value + sliderComponent.deltaValue > customSlider.toValue) {
                    smallStepsSlider.fromValue = customSlider.value - sliderComponent.deltaValue;
                    smallStepsSlider.toValue = customSlider.toValue;
                    smallStepsSlider.preferredWidth = sliderComponent.smallStepsSliderWidth * (smallStepsSlider.toValue - smallStepsSlider.fromValue) / (2 * sliderComponent.deltaValue) + smallStepsSlider.handleSize.width;
                }
                else {
                    smallStepsSlider.fromValue = customSlider.value - sliderComponent.deltaValue;
                    smallStepsSlider.toValue = customSlider.value + sliderComponent.deltaValue;
                    smallStepsSlider.preferredWidth = sliderComponent.smallStepsSliderWidth + smallStepsSlider.handleSize.width;
                }
                smallStepsSlider.layoutProperties.positionX = customSlider.handleLocalX() - smallStepsSlider.handleLocalX() + customSlider.layoutProperties.positionX;
                customSlider.longPressX = positionX;
            }

            onHandleReleased: {
                smallStepsSlider.visible = false;
            }

            layoutProperties: AbsoluteLayoutProperties {
                positionX: 100;
                positionY: 300
            }

            onMove: {
                var centerX = smallStepsSlider.fromValueToPosX(smallStepsSlider.immediateValue);
                smallStepsSlider.value = smallStepsSlider.fromPosXToValue(windowX - smallStepsSlider.layoutProperties.positionX - customSlider.longPressX);
            }
        }

        CustomSlider {
            id: smallStepsSlider
            preferredWidth: sliderComponent.smallStepsSliderWidth
            visible: false
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 200
            }
            onValueChanged: {
                customSlider.value = value;
            }
            onPreferredWidthChanged: {
            }
        }

        attachedObjects: [
            LayoutUpdateHandler {
                id: handler
                onLayoutFrameChanged: {
                    customSlider.preferredWidth = layoutFrame.width - 200;
                    customSlider.layoutProperties.positionX = layoutFrame.x + 100;
                }
            }
        ]

    }
}