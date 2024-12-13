#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "can2040.h"
#include "RP2040.h"

#define SENDER

static struct can2040 cbus;

#ifndef SENDER
static bool sendMsg = false;
#endif

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    printf("Callback triggered.\n");

    switch (notify)
    {
    case CAN2040_NOTIFY_RX:
        printf("CAN message has been received.\n");
        printf("[%d] - (%d): %s\n", msg->id, msg->dlc, (char *)msg->data);

#ifndef SENDER
        sendMsg = true;
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
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 20, gpio_tx = 21;

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
    char input[8] = "RCVDMSG\0";

    stdio_init_all();
    canbus_setup();

    printf("STDIO and CAN Bus initialized.\n");

    while (true)
    {
#ifdef SENDER
        fgets(input, sizeof(input), stdin);
        printf("Received user input: '%s'\n", input);

        if (can2040_check_transmit(&cbus))
#else
        if (sendMsg && can2040_check_transmit(&cbus))
#endif
        {
            struct can2040_msg msg = {
                .id = 0xBEEF,
                .dlc = sizeof(input),
            };

            memcpy(msg.data, input, sizeof(msg.data));

            printf("Sending CAN message.\n");

            if (can2040_transmit(&cbus, &msg) == 0)
            {
                printf("Transmit Success.\n");
            }
            else
            {
                printf("Transmit Error, TX Queue full.\n");
            }
        }
#ifdef SENDER
        else
        {
            printf("Error: Could not send message due to queue being full.\n");
        }
#else
        sendMsg = false;
#endif

        sleep_ms(1000);
    }
}
