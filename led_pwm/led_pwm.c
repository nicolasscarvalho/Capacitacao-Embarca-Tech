#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define array_length(array) (sizeof(array) / sizeof(array[0]))

// RGB  >>>  Red, Green, Blue  >>>  13, 11, 12
uint8_t led_pins[] = {11, 12, 13};

void gradual_pwm_state_change(uint8_t pwm_pin, uint16_t from, uint16_t to, bool growing, uint32_t sleep_time) {
    uint16_t level = from;
    int8_t step = growing ? 1 : -1;
    
    while (level != to) {
        pwm_set_gpio_level(pwm_pin, level);
        sleep_ms(sleep_time);
        level += step;
    }
}

int main() {
    stdio_init_all();

    // sets pwm function for each pin
    for (int index = 0; index < array_length(led_pins); index ++) {
        gpio_set_function(led_pins[index], GPIO_FUNC_PWM);
    }

    // maximum pwm levels -> 0 to max_fraction inclusive
    uint max_fraction = 99; 

    
    // gets pin slice, sets max_fraction and enables it
    for (int index = 0; index < array_length(led_pins); index ++) {
        uint slice_num =  pwm_gpio_to_slice_num(led_pins[index]);

        pwm_set_wrap(slice_num, max_fraction);
        // pwm_set_chan_level(slice_num, PWM_CHAN_A, 0); // fraction off
        // pwm_set_chan_level(slice_num, PWM_CHAN_B, max_fraction); // fraction on
        pwm_set_enabled(slice_num, true);
    }
    
    while (true) {

        for (int led_pin = 0; led_pin < array_length(led_pins); led_pin++) {
            gradual_pwm_state_change(
                led_pins[led_pin],      // pwm_pin
                0,                      // from
                max_fraction,           // to
                true,                   // growing
                10                      // sleep_time
            );

            gradual_pwm_state_change(
                led_pins[led_pin],      // pwm_pin
                max_fraction,           // from
                0,                      // to
                false,                  // growing
                10                      // sleep_time
            );
        }

    }
}
