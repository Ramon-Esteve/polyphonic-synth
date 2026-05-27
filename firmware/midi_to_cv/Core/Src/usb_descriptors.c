/*
 * usb_descriptors.c
 *
 * TinyUSB USB descriptor definitions for the SYNTH project.
 * This file tells the host (PC/DAW) everything about our USB MIDI device:
 * what it is, what it can do, and how to talk to it.
 */

#include <stdio.h>       // for snprintf (serial number formatting)
#include <string.h>      // for memcpy, strlen
#include "tusb.h"        // TinyUSB core — provides all descriptor types and macros

//--------------------------------------------------------------------+
// USB VID / PID
//--------------------------------------------------------------------+

/*
 * VID (Vendor ID): 0xCafe is TinyUSB's placeholder VID for development.
 * For a real product you need to buy/license a VID from USB-IF.
 * Fine for now during development.
 *
 * PID (Product ID): auto-generated from which USB classes are enabled.
 * Since only CFG_TUD_MIDI=1, the PID will be 0x4000 | (1<<3) = 0x4008.
 * This matters because Windows caches drivers per VID/PID — if you later
 * enable CDC too, change the PID manually or Windows will use the old driver.
 */
#define PID_MAP(itf, n)  ((CFG_TUD_##itf) ? (1 << (n)) : 0)
#define USB_PID          (0x4000 | PID_MAP(CDC, 0) | PID_MAP(MSC, 1) | \
                          PID_MAP(HID, 2) | PID_MAP(MIDI, 3) | PID_MAP(VENDOR, 4))

//--------------------------------------------------------------------+
// Device Descriptor
//--------------------------------------------------------------------+

/*
 * The device descriptor is the first thing the host reads after plug-in.
 * It identifies the device at the top level — class, USB version, VID/PID, etc.
 */
static tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,           // USB 2.0
    .bDeviceClass       = 0x00,             // class defined at interface level
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0xCafe,           // development VID
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,           // firmware version 1.0

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

/*
 * USB MIDI needs TWO interfaces:
 *   - Audio Control interface (required by spec, does nothing)
 *   - MIDI Streaming interface (actual MIDI data)
 */
enum {
    ITF_NUM_MIDI = 0,
    ITF_NUM_MIDI_STREAMING,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_MIDI_DESC_LEN)

/*
 * EP1 OUT (0x01) = host → device (MIDI from DAW to synth)
 * EP1 IN  (0x81) = device → host (MIDI from synth to DAW)
 * 0x80 bit = direction IN (device → host)
 */
#define EPNUM_MIDI_OUT  0x01
#define EPNUM_MIDI_IN   0x81

static uint8_t const desc_fs_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
    TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, 0, EPNUM_MIDI_OUT, EPNUM_MIDI_IN, 64)
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_fs_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

enum {
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
    STRID_SERIAL,
};

static char const *string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 },  // 0: language = English (US)
    "Ramón Esteve",                  // 1: manufacturer
    "MIDI to CV",                    // 2: product name
    NULL,                            // 3: serial — built at runtime
};

static uint16_t _desc_str[32 + 1];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    size_t chr_count;

    switch (index) {

        case STRID_LANGID:
            memcpy(&_desc_str[1], string_desc_arr[0], 2);
            chr_count = 1;
            break;

        case STRID_SERIAL: {
            /*
             * STM32F446RE unique device ID — 96 bits at fixed address 0x1FFF7A10
             * Three 32-bit words = 12 bytes. Formatted as 24-char hex string.
             * This is what shows up as the serial number in Device Manager.
             */
            uint32_t uid[3];
            uid[0] = *(volatile uint32_t *)0x1FFF7A10;
            uid[1] = *(volatile uint32_t *)0x1FFF7A14;
            uid[2] = *(volatile uint32_t *)0x1FFF7A18;

            static char serial_str[25];
            snprintf(serial_str, sizeof(serial_str), "%08lX%08lX%08lX",
                     uid[0], uid[1], uid[2]);

            chr_count = strlen(serial_str);
            for (size_t i = 0; i < chr_count; i++) {
                _desc_str[1 + i] = serial_str[i];
            }
            break;
        }

        default: {
            if (index >= sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) {
                return NULL;
            }

            const char *str = string_desc_arr[index];
            chr_count = strlen(str);
            const size_t max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1;
            if (chr_count > max_count) chr_count = max_count;

            // ASCII → UTF-16
            for (size_t i = 0; i < chr_count; i++) {
                _desc_str[1 + i] = str[i];
            }
            break;
        }
    }

    // Header: type in high byte, total length in low byte
    _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

    return _desc_str;
}