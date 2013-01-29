// Default empty project template
import bb.cascades 1.0
import nutty.ui 1.0

// creates one page with a label
Page {
    Container {
        layout: DockLayout {}
//        Label {
//            text: qsTr("Hello World")
//            textStyle.base: SystemDefaults.TextStyles.BigText
//            verticalAlignment: VerticalAlignment.Center
//            horizontalAlignment: HorizontalAlignment.Center
//        }
        NuttySlider {
            id: nutty
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            value: 0.7
//            preferredWidth: 600
        }
        Label {
//            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            text: "value : " + nutty.value
        }
        Label {
//            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            text: "immediateValue : " + nutty.immediateValue
        }
//        ImageView {
//            property real size: 1
//            imageSource: "asset:///images/handle_pressed.png"
//            preferredWidth: size
//            preferredHeight: size
//            onTouch: {
//                if(event.touchType == TouchType.Down) {
//                    size = 100;
//                } else if(event.touchType == TouchType.Up) {
//                    size = 1;
//                }
//
//            }
//            verticalAlignment: VerticalAlignment.Center
//            horizontalAlignment: HorizontalAlignment.Center
//            touchBehaviors: [
//                TouchBehavior {
//                    TouchReaction {
//                        eventType: TouchType.Down
//                        phase: PropagationPhase.AtTarget
//                        response: TouchResponse.StartTracking
//                    }
//                }
//            ]
//        }
    }
}

