// screen_menu_settings.cpp

#include "screen_menus.hpp"
#include "gui.hpp"
#include "config.h"
#include "app.h"
#include "marlin_client.h"
#include "screen_menu.hpp"
#include "screen_menus.hpp"
#include "ScreenHandler.hpp"
#include "cmsis_os.h"
#include "sys.h"
#include "eeprom.h"
#include "eeprom_loadsave.h"
#include "dump.h"
#include "sound.hpp"
#include "WindowMenuItems.hpp"
#include "MItem_menus.hpp"
#include "MItem_tools.hpp"
#include "i18n.h"
#include "Marlin/src/core/serial.h"
#include "DialogMoveZ.hpp"
#include "netdev.h"

/*****************************************************************************/

class MI_LOAD_SETTINGS : public WI_LABEL_t {
    constexpr static const char *const label = N_("Load Settings from file");

public:
    MI_LOAD_SETTINGS()
        : WI_LABEL_t(_(label), 0, is_enabled_t::yes, is_hidden_t::no) {}
    virtual void click(IWindowMenu & /*window_menu*/) override {
        // FIXME: Some error reporting
        // FIXME: How do we _activate_ the new settings?
        // TODO: Loading other things than just network
        netdev_load_ini_to_eeprom();
    }
};

#ifdef _DEBUG
using Screen = ScreenMenu<EFooter::On, MI_RETURN, MI_TEMPERATURE, MI_CURRENT_PROFILE, MI_MOVE_AXIS, MI_DISABLE_STEP,
    MI_FOOTER_SETTINGS, MI_FACTORY_DEFAULTS, MI_SERVICE, MI_HW_SETUP, MI_TEST, MI_FW_UPDATE, MI_ESP_UPDATE, MI_FILAMENT_SENSOR, MI_FS_AUTOLOAD, MI_TIMEOUT, MI_FAN_CHECK,
    #ifdef BUDDY_ENABLE_ETHERNET
    MI_LAN_SETTINGS,
    MI_TIMEZONE,
    MI_LOAD_SETTINGS,
    #endif // BUDDY_ENABLE_ETHERNET
    MI_SAVE_DUMP, MI_SOUND_MODE, MI_SOUND_VOLUME, MI_PRUSALINK,
    MI_DEVHASH_IN_QR, MI_LANGUAGE, MI_LANGUAGUE_USB, MI_LANGUAGUE_XFLASH, MI_LOAD_LANG, MI_SORT_FILES,
    MI_SOUND_TYPE, MI_XFLASH_RESET, MI_XFLASH_DELETE, MI_HF_TEST_0, MI_HF_TEST_1,
    MI_EEPROM, MI_EXPERIMENTAL_SETTINGS>;
#else
using Screen = ScreenMenu<EFooter::On, MI_RETURN, MI_TEMPERATURE, MI_CURRENT_PROFILE, MI_MOVE_AXIS, MI_DISABLE_STEP,
    MI_FOOTER_SETTINGS, MI_FACTORY_DEFAULTS, MI_HW_SETUP, MI_FW_UPDATE, MI_ESP_UPDATE, MI_FILAMENT_SENSOR, MI_FS_AUTOLOAD, MI_TIMEOUT, MI_FAN_CHECK,
    #ifdef BUDDY_ENABLE_ETHERNET
    MI_LAN_SETTINGS,
    MI_TIMEZONE,
    MI_LOAD_SETTINGS,
    #endif //BUDDY_ENABLE_ETHERNET
    MI_SAVE_DUMP, MI_SOUND_MODE, MI_SOUND_VOLUME, MI_PRUSALINK, MI_DEVHASH_IN_QR, MI_LANGUAGE>;
#endif

class ScreenMenuSettings : public Screen {
public:
    constexpr static const char *label = N_("SETTINGS");
    ScreenMenuSettings();

protected:
    virtual void windowEvent(EventLock /*has private ctor*/, window_t *sender, GUI_event_t event, void *param) override;
};

ScreenFactory::UniquePtr GetScreenMenuSettings() {
    return ScreenFactory::Screen<ScreenMenuSettings>();
}

ScreenMenuSettings::ScreenMenuSettings()
    : Screen(_(label)) {
    if (sheet_number_of_calibrated() > 1) {
        Item<MI_CURRENT_PROFILE>().UpdateLabel();
        Item<MI_CURRENT_PROFILE>().Show();
    }
}

void ScreenMenuSettings::windowEvent(EventLock /*has private ctor*/, window_t *sender, GUI_event_t event, void *param) {
    if (event == GUI_event_t::HELD_RELEASED) {
        DialogMoveZ::Show();
        return;
    }

    if (event == GUI_event_t::LOOP) { // don't return LOOP must be sent to parrent too
        Item<MI_FILAMENT_SENSOR>().CheckDisconnected();
    }

    SuperWindowEvent(sender, event, param);
}
