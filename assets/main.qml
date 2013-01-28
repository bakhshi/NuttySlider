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
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredWidth: 600
        }
        ImageView {
            property real size: 1
            imageSource: "asset:///images/handle_pressed.png"
            preferredWidth: size
            preferredHeight: size
            onTouch: {
                if(event.touchType == TouchType.Down) {
                    size = 100;
                } else if(event.touchType == TouchType.Up) {
                    size = 1;
                }

            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            touchBehaviors: [
                TouchBehavior {
                    TouchReaction {
                        eventType: TouchType.Down
                        phase: PropagationPhase.AtTarget
                        response: TouchResponse.StartTracking
                    }
                }
            ]
        }
    }
}

