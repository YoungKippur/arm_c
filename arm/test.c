// cmake .. -DPICO_BOARD=pico_w
// make

#include <stdio.h>
#include <stdlib.h> 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/binary_info.h"
#include "servo.h"
#include <string.h>

#define SERVO_PIN 2
#define SERVO_PIN2 0

#define LED_PIN 3
#define PUL_PIN 4

#define POTE_PIN 26
#define POTE_PIN2 27

int estado = 2, caso = 2, p;
char str[50];
int maxValue = 2200-600;

int main(){
    stdio_init_all();
    adc_init();

    setServo(SERVO_PIN, 600);
    setServo(SERVO_PIN2, 600);

    adc_gpio_init(POTE_PIN);
    adc_gpio_init(POTE_PIN2);

    gpio_init(PUL_PIN);
    gpio_set_dir(PUL_PIN, GPIO_IN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    while (1) {
        switch(caso){
            case 1:
                scanf("%s", str);
                // printf("%s\n", str);
                setMillis(SERVO_PIN2, (atoi(str) * maxValue/180)+600);
                scanf("%s", str);
                // printf("%s\n", str);
                setMillis(SERVO_PIN, (atoi(str) * maxValue/180)+600);
                sleep_ms(20);
                break;
            case 2:
                adc_select_input(0);
                int adc_x_raw = adc_read();
                adc_select_input(1);
                int adc_y_raw = adc_read();
                printf("%dz%dz%dz\n", adc_x_raw, adc_y_raw, estado);
                if (estado == 1){
                    gpio_put(LED_PIN, 1);
                    scanf("%s", str);
                    setMillis(SERVO_PIN2, (atoi(str) * maxValue/180)+600);
                    scanf("%s", str);
                    setMillis(SERVO_PIN, (atoi(str) * maxValue/180)+600);
                    estado = 2;
                }
                gpio_put(LED_PIN, 0);
                sleep_ms(100);
                break;
        }
        p = gpio_get(PUL_PIN);
        if (p){
            while (p==1){
                p = gpio_get(PUL_PIN);
            }
            estado = 1;
        }
    }
    return 0;
}