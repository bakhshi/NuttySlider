// Default empty project template
#include "NuttySliderApp.hpp"
#include "CustomSlider.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

NuttySliderApp::NuttySliderApp(bb::cascades::Application *app)
: QObject(app)
{
    // Register our custom control
//    qmlRegisterType<NuttySlider>("nutty.ui", 1, 0, "NuttySlider");
    qmlRegisterType<CustomSlider>("nutty.ui", 1, 0, "CustomSlider");
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

