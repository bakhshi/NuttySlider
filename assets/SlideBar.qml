import bb.cascades 1.0
import nutty.ui 1.0

Container {
    property int currentTime
    property int totalTime
    
    preferredWidth: my.width
    horizontalAlignment: HorizontalAlignment.Fill
    
    layout: AbsoluteLayout {}
    background: Color.create("#0088cc")

    TimeArea {
        id: currentTimeLabel
        timeInMsc: currentTime
        preferredWidth: my.timeAreaWidth
        preferredHeight: my.height
    }

    TimeArea {
        timeInMsc: totalTime
        preferredWidth: my.timeAreaWidth
        preferredHeight: my.height
        layoutProperties: AbsoluteLayoutProperties {
            positionX: my.width - my.timeAreaWidth
        }
    }

    CustomSlider {
        id: slider
        fromValue: 0
        toValue: totalTime
        value: currentTime
        preferredWidth: my.width - 2 * my.timeAreaWidth

        layoutProperties: AbsoluteLayoutProperties {
            positionX: my.timeAreaWidth
        }

        onHandleLongPressed: {
            smallStepSlider.visible = true;
            smallStepSlider.value = slider.value;
            if(slider.value - my.dt < slider.fromValue) {
                smallStepSlider.fromValue = slider.fromValue;
                smallStepSlider.toValue = slider.value + my.dt;
                smallStepSlider.preferredWidth = my.smallStepSliderWidth * (smallStepSlider.toValue - smallStepSlider.fromValue) / (2 * my.dt) + smallStepSlider.handleSize.width;
            }
            else if(slider.value + my.dt > slider.toValue) {
                smallStepSlider.fromValue = slider.value - my.dt;
                smallStepSlider.toValue = slider.toValue;
                smallStepSlider.preferredWidth = my.smallStepSliderWidth * (smallStepSlider.toValue - smallStepSlider.fromValue) / (2 * my.dt) + smallStepSlider.handleSize.width;
            }
            else {
                smallStepSlider.fromValue = slider.value - my.dt;
                smallStepSlider.toValue = slider.value + my.dt;
                smallStepSlider.preferredWidth = my.smallStepSliderWidth + smallStepSlider.handleSize.width;
            }
            smallStepSlider.layoutProperties.positionX = slider.handleLocalX() - smallStepSlider.handleLocalX() + slider.layoutProperties.positionX;
            my.longPressInitX = positionX;
        }

        onMove: {
            smallStepSlider.value = smallStepSlider.fromPosXToValue(windowX - smallStepSlider.layoutProperties.positionX - my.longPressInitX);
        }

        onHandleReleased: {
            smallStepSlider.visible = false;
        }
        
        onImmediateValueChanged: {
            currentTimeLabel.timeInMsc = immediateValue;
        }
    }

    CustomSlider {
        id: smallStepSlider
        preferredWidth: my.smallStepSliderWidth
        visible: false
        layoutProperties: AbsoluteLayoutProperties {
        }
        onValueChanged: {
            slider.value = value;
        }
        onPreferredWidthChanged: {
        }
    }

    attachedObjects: [
        // Dummy component for local variables
        ComponentDefinition {
            id: my
            property int width: 700
            property int height: 100
            property int timeAreaWidth: 200
            property int smallStepSliderWidth: 300
            property int dt: 10 * 1000 // delta time in seconds
            property real longPressInitX
        },
        LayoutUpdateHandler {
            id: layoutHandler
            onLayoutFrameChanged: {
                my.width = layoutFrame.width;
            }
        }
    ]
}