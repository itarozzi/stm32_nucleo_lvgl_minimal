#include "main.h"
#include "ui_logic.h"
#include "ui/actions.h"
#include "ui/ui.h"
#include "ui/screens.h"
#include "ui/vars.h"

#include <stdint.h>
#include <sys/time.h>

uint32_t click_cnt = 0;
int32_t blue_button_press_cnt = 0;

int _gettimeofday(struct timeval *tv, void *tz) {
    if (tv) {
        tv->tv_sec  = 0;  // oppure usa il tuo RTC/tick
        tv->tv_usec = 0;
    }
    return 0;
}


// Callback from UI used to assign UI label directly
void action_btn_pressed(lv_event_t *e) {
    // TODO: Implement action btn_pressed here

        click_cnt++;

        lv_label_set_text_fmt(objects.label_test, "You clicked me\n%d times", click_cnt);
}


//  Callback from UI used to command Nucleo builtin led
void action_switch_led(lv_event_t *e) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}





/*** Getter and Setter for Native Variables  */


int32_t get_var_blue_button_press_cnt() {
    return blue_button_press_cnt;
}

void set_var_blue_button_press_cnt(int32_t value) {
    blue_button_press_cnt = value;
}
