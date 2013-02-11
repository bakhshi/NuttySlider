import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout {}
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            background: Color.Cyan;
            preferredWidth: 300
            preferredHeight: 300
            animations: [
                ScaleTransition {
                    id: scaleAnimation
                    toX: 2.0
                    duration: 1000
                }
            ]
//            onCreationCompleted: {
//                scaleAnimation.play();
//            }
        }
        
        Button {
            text: "Animate"
            onClicked: {
                if(scaleAnimation.toX == 2.0)
                    scaleAnimation.toX == 0.5;
                scaleAnimation.play();
            }
        }
    }
}