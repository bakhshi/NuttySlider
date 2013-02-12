import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout {}
        SlideBar {
            verticalAlignment: VerticalAlignment.Center
            currentTime: 3653 * 1000
            totalTime: 4567 * 1000
        }
    }
}