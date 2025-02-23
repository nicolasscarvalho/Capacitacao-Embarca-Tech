#include <stdio.h>
#include "pico/stdlib.h"

uint32_t calculate_frequency_period(float frequency) {
    float full_period = 1.0/frequency * 1000.0;
    return (uint32_t)(full_period/2);
}

int main() {

    // RGB  >>>  Red, Green, Blue  >>>  13, 11, 12
    uint8_t led_pins[] = {11, 12, 13};

    stdio_init_all();
    
    // initializes LEDs
    for (int index = 0; index < 3; index++) {
        gpio_init(led_pins[index]);
    }

    // configure LED pin as output
    for (int index = 0; index < 3; index++) {
        gpio_set_dir(led_pins[index], GPIO_OUT);
    }

    while (true) {
        // turns on a LED, waits 500 ms and turns it off before the next
        gpio_put(13, true);
        sleep_ms(500);
        
        gpio_put(13, false);
        gpio_put(11, true);
        sleep_ms(500);

        gpio_put(11, false);
        gpio_put(12, true);
        sleep_ms(500);
        gpio_put(12, false);

        // pulses a LED at a specific frequency
        gpio_put(13, true);
        sleep_ms(calculate_frequency_period(2));
        gpio_put(13, false);
        
        // pulses a LED at a specific frequency for "X" seconds
        uint8_t seconds = 5;
        uint32_t frequency_period = calculate_frequency_period(3);
        uint32_t times = (uint32_t)(seconds * 1000 / (2 * frequency_period));
        
        for (int step = 0; step < times; step++){
            gpio_put(11, true);
            sleep_ms(frequency_period);
            gpio_put(11, false);
            sleep_ms(frequency_period);
        }
    }
}
