#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "can2040.h"
#include "RP2040.h"
#include "servo.h"

#define SERVO

#ifdef SERVO
static uint16_t input = 1500;
#endif

static struct can2040 cbus;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    switch (notify)
    {
    case CAN2040_NOTIFY_RX:
        printf("CAN message has been received.\n");
        printf("[%d] - (%d): %s\n", msg->id, msg->dlc, (char *)msg->data);
#ifdef SERVO
        input = (uint16_t)msg->data32[0];
#endif
        break;
    case CAN2040_NOTIFY_TX:
        printf("CAN message has been sent.\n");
        printf("[%d] - (%d): %s\n", msg->id, msg->dlc, (char *)msg->data);
        break;
    case CAN2040_NOTIFY_ERROR:
        printf("CAN Receive Buffer Overflow.\n");
        break;
    default:
        printf("Received unknown notify code.\n");
        break;
    }
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void)
{
    const uint32_t pio_num = 0;
    const uint32_t sys_clock = 125000000, bitrate = 500000;
    const uint32_t gpio_rx = 20, gpio_tx = 21;

    // Setup CAN bus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable IRQs
    irq_set_exclusive_handler(PIO0_IRQ_0_IRQn, PIOx_IRQHandler);
    NVIC_SetPriority(PIO0_IRQ_0_IRQn, 1);
    NVIC_EnableIRQ(PIO0_IRQ_0_IRQn);

    // Start CAN bus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

int main()
{
    stdio_init_all();
    printf("STDIO initialized.");
    canbus_setup();
    printf("CAN Bus initialized.\n");

#ifdef SERVO
    const uint servo_pin = 2;

    setServo(servo_pin, 1500);
    printf("Servo initialized.");
#else
    const uint joystick_pin = 28;

    adc_init();
    adc_gpio_init(joystick_pin);
    printf("ADC GPIO initialized.");
#endif

    while (1)
    {
#ifdef SERVO
        // map: (x - joystick_min) * (servo_max - servo_min) / (joystick__max - joystick_min) + out_min
        float millis = (input - 0) * (2500 - 1000) / (4096 - 0) + 1000;

        setMillis(servo_pin, millis);
        sleep_ms(50);
#else
        // 21 - 4095
        adc_select_input(2);

        uint16_t value = adc_read();

        printf(">joystick:%d\r\n", value);

        if (can2040_check_transmit(&cbus))
        {
            struct can2040_msg msg = {
                .id = 0xBEEF,
                .dlc = sizeof(value),
                .data32 = {value, 0}};

            if (can2040_transmit(&cbus, &msg) != 0)
            {
                printf("Transmit Error, TX Queue full.\n");
            }
        }
        else
        {
            printf("Error: Could not send message due to queue being full.\n");
        }

        sleep_ms(50);
#endif
    }
}
