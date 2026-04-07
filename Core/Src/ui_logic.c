#include "ui/actions.h"
#include "ui/ui.h"
#include "ui/screens.h"

#include <sys/types.h>
#include <sys/time.h>
u_int64_t click_cnt = 0;


int _gettimeofday(struct timeval *tv, void *tz) {
    if (tv) {
        tv->tv_sec  = 0;  // oppure usa il tuo RTC/tick
        tv->tv_usec = 0;
    }
    return 0;
}

void action_btn_pressed(lv_event_t *e) {
    // TODO: Implement action btn_pressed here

        click_cnt++;

        lv_label_set_text_fmt(objects.label_test, "You clicked me\n%d times", click_cnt);
}
