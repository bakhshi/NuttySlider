// Default empty project template
#ifndef NuttySliderApp_HPP_
#define NuttySliderApp_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class NuttySliderApp : public QObject
{
    Q_OBJECT
public:
    NuttySliderApp(bb::cascades::Application *app);
    virtual ~NuttySliderApp() {}
};


#endif /* NuttySliderApp_HPP_ */
