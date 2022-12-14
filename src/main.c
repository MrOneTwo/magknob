/*
 * Documentation for USB HID https://www.usb.org/sites/default/files/documents/hid1_11.pdf
 */

#include <libopencm3/cm3/memorymap.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scs.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/tpiu.h>
#include <libopencm3/stm32/dbgmcu.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/usb/dwc/otg_fs.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>

#include <libopencm3/stm32/st_usbfs.h>

#include "printf.h"
#include "board.h"
#include "version.h"
#include "keycodes.h"
#include "as5601.h"
#include "profile_trace.h"

#include <string.h>

void _putchar(char c)
{
  (void)c;
}

uint32_t str_len(const char *str)
{
  return (*str) ? str_len(++str) + 1 : 0;
}

// This has to be aligned for better ITM performance - CPU can push up to 4 bytes at once into the
// ITM's buffer but that requires the buffer to be aligned.
#define PRINT_BUF_SIZE (128)
char print_buf[PRINT_BUF_SIZE] __attribute__((aligned(4)));


////////////////////////
//
//  USB
//

// Endpoint address should start with 8 (0b10000000). The rest of the bits
// map to a EP register (0x4000_0000 (APB1) + 0x5c00 + EP). Not the same as
// packet buffer memory location which live in the 0x4000_6000 space.
// The is 8 bidirectional endpoints which means addresses 0x80 - 0x87 should
// be valid.
#define ENDPOINT_ADDRESS (0x81)
#define ENDPOINT_MAX_SIZE  (16)

typedef enum Usb_state_e {
  USB_UNINITILIZED = 0,
  USB_INITILIZED = 1,
} Usb_state_e;

Usb_state_e usb_state;

static usbd_device *usbd_dev;

// Explanation of the decriptors below.
// http://www.usbmadesimple.co.uk/ums_5.htm
const struct usb_device_descriptor dev_descr = {
  .bLength = USB_DT_DEVICE_SIZE,     //
  .bDescriptorType = USB_DT_DEVICE,  //
  .bcdUSB = 0x0200,                  // spec version
  .bDeviceClass = 0,                 // class information in interface descriptor
  .bDeviceSubClass = 0,              // class information in interface descriptor
  .bDeviceProtocol = 0,              // class information in interface descriptor
  .bMaxPacketSize0 = 64,             // either 8, 16, 32, 64
  .idVendor = 0xfeed,                // Planck VID
  .idProduct = 0x6060,               // Planck PID
  .bcdDevice = 0x0200,               // device release number
  .iManufacturer = 1,                // index to manufacturer string
  .iProduct = 2,                     // index to product string
  .iSerialNumber = 3,                // index to serial number
  .bNumConfigurations = 1,           // number of possible configurations
};

static const uint8_t hid_report_descriptor[] = {
  0x05, 0x0C, /* USAGE_PAGE (Consumer Device)       */
  0x09, 0x01, /* USAGE (Consumer Control)           */
  0xA1, 0x01, /* COLLECTION (Application)           */
  0x85, 0x01, /*   REPORT_ID (1)                    */
  0x05, 0x0C, /*   Usage Page (Consumer Devices)    */
  0x15, 0x00, /*   Logical Minimum (0)              */
  0x25, 0x01, /*   Logical Maximum (1)              */
  0x75, 0x01, /*   Report Size (1)                  */
  0x95, 0x07, /*   Report Count (7)                 */
  0x09, 0xB5, /*   Usage (Scan Next Track)          */
  0x09, 0xB6, /*   Usage (Scan Previous Track)      */
  0x09, 0xB7, /*   Usage (Stop)                     */
  0x09, 0xCD, /*   Usage (Play / Pause)             */
  0x09, 0xE2, /*   Usage (Mute)                     */
  0x09, 0xE9, /*   Usage (Volume Up)                */
  0x09, 0xEA, /*   Usage (Volume Down)              */
  0x81, 0x02, /*   Input (Data, Variable, Absolute  */
  0x75, 0x01, /*   Report Size (1)                  */
  0x95, 0x01, /*   Report Count (1)                 */
  0x81, 0x01, /*   INPUT (Constant)                 */
  0xc0,       /* END_COLLECTION                     */

  0x05, 0x01, /* USAGE_PAGE (Generic Desktop)       */
  0x09, 0x06, /* USAGE (Keyboard)                   */
  0xa1, 0x01, /* COLLECTION (Application)           */
  0x85, 0x02, /*   REPORT_ID (2)                    */
              /*   Modifier keys                    */
  0x05, 0x07, /*   USAGE_PAGE (Keyboard)            */
  0x19, 0xe0, /*   USAGE_MINIMUM (LCtrl)            */
  0x29, 0xe7, /*   USAGE_MAXIMUM (RGui)             */
  0x15, 0x00, /*   LOGICAL_MINIMUM (0)              */
  0x25, 0x01, /*   LOGICAL_MAXIMUM (1)              */
  0x95, 0x08, /*   REPORT_COUNT (8)                 */
  0x75, 0x01, /*   REPORT_SIZE (1)                  */
  0x81, 0x02, /*   INPUT (Data,Variable,Absolute)   */
              /*   Reserved byte                    */
  0x95, 0x08, /*   REPORT_COUNT (8)                 */
  0x75, 0x01, /*   REPORT_SIZE (1)                  */
  0x81, 0x01, /*   INPUT (Constant)                 */
              /*   Keyboard buttons                 */
  0x05, 0x07, /*   USAGE_PAGE (Keyboard)            */
  0x19, 0x00, /*   USAGE_MINIMUM (Reserved - no event indicated) */
  0x29, 0x65, /*   USAGE_MAXIMUM (Keyboard Application) */
  0x15, 0x00, /*   LOGICAL_MINIMUM (0)              */
  0x25, 0x7f, /*   LOGICAL_MAXIMUM (101)            */
  0x95, 0x06, /*   REPORT_COUNT (6)                 */
  0x75, 0x08, /*   REPORT_SIZE (8)                  */
  0x81, 0x00, /*   INPUT (Data,Array,Absolute)      */
  0xc0        /* END_COLLECTION                     */
};


// Create an anonymous struct and create a hid_function variable
// out of that and initilize it with a default values.
static const struct {
  struct usb_hid_descriptor hid_descriptor;
  struct {
    uint8_t bReportDescriptorType;
    uint16_t wDescriptorLength;
  } __attribute__((packed)) hid_report;
} __attribute__((packed)) hid_function =
{
  .hid_descriptor = {
    .bLength = sizeof(hid_function),
    .bDescriptorType = USB_DT_HID,
    .bcdHID = 0x0100,
    .bCountryCode = 0,
    .bNumDescriptors = 1,
  },

  .hid_report = {
    .bReportDescriptorType = USB_DT_REPORT,
    .wDescriptorLength = sizeof(hid_report_descriptor),
  }
};

const struct usb_endpoint_descriptor hid_endpoint = {
  .bLength = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType = USB_DT_ENDPOINT,
  .bEndpointAddress = ENDPOINT_ADDRESS,   // bit 7 is direction 0 = OUT, 1 = IN
  .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
  .wMaxPacketSize = ENDPOINT_MAX_SIZE,    //
  .bInterval = 0x0a,                      // polling interval in miliseconds (0x0a = 10 ms)
  .extra = (void*)0,
  .extralen = 0,
};

const struct usb_interface_descriptor hid_iface = {
  .bLength = USB_DT_INTERFACE_SIZE,
  .bDescriptorType = USB_DT_INTERFACE,
  .bInterfaceNumber = 0,
  .bAlternateSetting = 0,
  .bNumEndpoints = 1,
  .bInterfaceClass = USB_CLASS_HID,
  .bInterfaceSubClass = 0,
  .bInterfaceProtocol = 0,
  .iInterface = 0,

  .endpoint = &hid_endpoint,

  .extra = &hid_function,
  .extralen = sizeof(hid_function),
};

const struct usb_interface ifaces[] = {
  {
    .num_altsetting = 1,
    .altsetting = &hid_iface,
  }
};

const struct usb_config_descriptor config = {
  .bLength = USB_DT_CONFIGURATION_SIZE,
  .bDescriptorType = USB_DT_CONFIGURATION,
  .wTotalLength = 0,
  .bNumInterfaces = sizeof(ifaces)/sizeof(ifaces[0]),
  .bConfigurationValue = 1,
  .iConfiguration = 0,
  .bmAttributes = 0xC0,
  .bMaxPower = 0x32,

  .interface = ifaces,
};

static const char *usb_strings[] = {
  "Curious Technologies",
  "MagKnob",
  "USB version",
};

/* Buffer used for control requests. */
static uint8_t usbd_control_buffer[128];

static enum usbd_request_return_codes
hid_control_request(usbd_device *dev,
                    struct usb_setup_data *req,
                    uint8_t **buf, uint16_t *len,
                    void (**complete)(usbd_device *dev, struct usb_setup_data *req))
{
  TRACE_PRINT(0, "hid_control_request\r\n");
  (void)complete;
  (void)dev;

  // The 0-4 bits of bmRequestType indicate whether the requested descriptor is
  // associated with the device, interface, endpoint or other.
  // The wValue field specifies the Descriptor Type in the high byte and the
  // Descriptor Index in the low byte.
  if((req->bmRequestType != 0x81) ||
     (req->bRequest != USB_REQ_GET_DESCRIPTOR) ||
     (req->wValue != 0x2200))
    return USBD_REQ_NOTSUPP;

  // Point to the HID report descriptor.
  *buf = (uint8_t *)hid_report_descriptor;
  *len = sizeof(hid_report_descriptor);

  systick_counter_enable();
  usb_state = USB_INITILIZED;
  return USBD_REQ_HANDLED;
}

static void usb_rx_cb(usbd_device* usbd_dev, uint8_t ep)
{
  (void)usbd_dev;
  (void)ep;

  // I'm not getting anything here. That might be because it's an interrupt
  // endpoint and there is no data to receive...?
  // {
  //   uint8_t pm[ENDPOINT_MAX_SIZE] = {};
  //   usbd_ep_read_packet(usbd_dev, ENDPOINT_ADDRESS, (void*)pm, ENDPOINT_MAX_SIZE);
  //   snprintf(print_buf, PRINT_BUF_SIZE, "usb buf %x %x %x %x\r\n"
  //                                       "        %x %x %x %x\r\n"
  //                                       "        %x %x %x %x\r\n"
  //                                       "        %x %x %x %x\r\n",
  //     pm[0], pm[1], pm[2], pm[3],
  //     pm[4], pm[5], pm[6], pm[7],
  //     pm[8], pm[9], pm[10], pm[11],
  //     pm[12], pm[13], pm[14], pm[15]
  //   );
  //   TRACE_PRINT(0, print_buf);
  // }
}

static void hid_set_config(usbd_device *dev, uint16_t wValue)
{
  (void)wValue;
  (void)dev;

  usbd_ep_setup(dev, ENDPOINT_ADDRESS, USB_ENDPOINT_ATTR_INTERRUPT, ENDPOINT_MAX_SIZE, usb_rx_cb);

  usbd_register_control_callback(
        dev,
        USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
        USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
        hid_control_request);
}

static uint16_t usb_get_frame_number(void)
{
  // This increments on every frame sent by the host - updated on SOF interrupt.
  uint16_t fnr_fn = (*USB_FNR_REG) & USB_FNR_FN;
  return fnr_fn;
}

//
//  USB - end
//
////////////////////////

static uint32_t systick_counter = 0;

static void setup_clock(void)
{
  // usbd.h says 'It is required that the 48MHz USB clock is already available.'
  // The clock configurations are set in the rcc.c.
  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
  // SysTick interrupt every N clock pulses: set reload to N-1
  // 72MHz / 8 = 9MHz
  // 9000000Hz / 90000 = 100Hz
  systick_set_reload(89999);
  systick_interrupt_enable();
}


int main(void)
{
  // itm_init();
  // If you want to trace before setting up the clock you need to change
  // OpenOCD's core frequency to 8000000.
  setup_clock();
  board_init();
  snprintf(print_buf, PRINT_BUF_SIZE, "\r\n--- magknob %s ---\r\n", FIRMWARE_VERSION);
  TRACE_PRINT(0, print_buf);

  as5601_set_impulses_per_rotation(AS5601_REG_VAL_ABN_64);
  as5601_set_watchdog(true);

  ////////////////////////
  //
  //  USB (PA12 - D+, PA11 - D-)
  //

  /*
   * This is a somewhat common cheap hack to trigger device re-enumeration
   * on startup.  Assuming a fixed external pullup on D+, (For USB-FS)
   * setting the pin to output, and driving it explicitly low effectively
   * "removes" the pullup.  The subsequent USB init will "take over" the
   * pin, and it will appear as a proper pullup to the host.
   * The magic delay is somewhat arbitrary, no guarantees on USBIF
   * compliance here, but "it works" in most places.
   */
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
  gpio_clear(GPIOA, GPIO12);
  // Delay the enumeration - this works.
  for (uint32_t i = 0; i < 0x800000; i++)
  {
    __asm__("nop");
  }

  usbd_dev = usbd_init(&st_usbfs_v1_usb_driver,
                       &dev_descr,
                       &config,
                       usb_strings, sizeof(usb_strings)/sizeof(char*),
                       usbd_control_buffer, sizeof(usbd_control_buffer));

  // TODO(michalc): look into other usbd_register_ functions.
  usbd_register_set_config_callback(usbd_dev, hid_set_config);

  //
  //  USB - end
  //
  ////////////////////////

  while (1)
  {
    // This function first reads the USB_ISTR_REG to determine what's there to handle.
    // It might be a reset, ctr (correct transfer), suspend, wakeup, sof (start of frame).
    usbd_poll(usbd_dev);
  }
}


// Reflect the USB HID report in form of a struct.
struct composite_report_t {
  uint8_t report_id;
  union {
    struct {
      uint8_t mask;
    } __attribute__((packed)) media;

    struct {
      uint8_t modifiers;
      uint8_t reserved;
      uint8_t keys_down[6];
      //uint8_t leds; // not part of the report atm
    } __attribute__((packed)) keyboard;
  };
} __attribute__((packed));
typedef struct composite_report_t composite_report_t;

static int encoder_pos_prev = 0;

#define MEDIA_MASK_VOL_DOWN (0x40)
#define MEDIA_MASK_VOL_UP   (0x20)

#define REPORT_MEDIA_ID    (1)
#define REPORT_KEYBOARD_ID (2)

static void controller_state_to_report(composite_report_t * const cr)
{
  int encoder_pos = board_encoder_get_counter();

  // TODO(michalc): move this out from here
  {
    // uint8_t reg = AS5601_REG_STATUS;
    // uint8_t data = 0;

    // i2c_transfer7(I2C1, AS5601_I2C_ADDR, &reg, 1, &data, 1);


    // snprintf(print_buf, PRINT_BUF_SIZE, "0x%X == 0x%X\r\n"
    //                                     "  MH: %s (mag too strong)\r\n"
    //                                     "  ML: %s (mag too weak)\r\n"
    //                                     "  MD: %s (mag detected)\r\n",
    //                                     reg, data,
    //                                     (data & AS5601_REG_STATUS_MH_MASK) ? "1" : "0",
    //                                     (data & AS5601_REG_STATUS_ML_MASK) ? "1" : "0",
    //                                     (data & AS5601_REG_STATUS_MD_MASK) ? "1" : "0");

    //snprintf(print_buf, PRINT_BUF_SIZE, "encoder_pos cur:prev %d:%d\r\n", encoder_pos, encoder_pos_prev);
    //TRACE_PRINT(0, print_buf);
  }

  int8_t rot_dir = 0;

  if (encoder_pos < encoder_pos_prev) {
    rot_dir = 1;
  }
  else if (encoder_pos > encoder_pos_prev) {
    rot_dir = -1;
  }

  // Reverse behavior for two edge cases.
  if (encoder_pos_prev == ENCODER_WRAP_VALUE && encoder_pos == 0) {
    rot_dir = -1;
  }
  else if (encoder_pos_prev == 0 && encoder_pos == ENCODER_WRAP_VALUE) {
    rot_dir = 1;
  }

  if (rot_dir == 1) cr->media.mask = MEDIA_MASK_VOL_UP;
  if (rot_dir == -1) cr->media.mask = MEDIA_MASK_VOL_DOWN;
  cr->report_id = REPORT_MEDIA_ID;

  encoder_pos_prev = encoder_pos;
}

// systick counter gets enabled in the hid_control_request().
// That's because polling the controller makes sense if USB works well.
void sys_tick_handler(void)
{
  if (systick_counter % 20 == 0)
    gpio_toggle(GPIOC, GPIO13);

  // Translate the hardware state into a HID report.
  composite_report_t report = {};
  controller_state_to_report(&report);

  // For Bluepill board this resolves to st_usbfs_ep_write_packet.
  usbd_ep_write_packet(usbd_dev, ENDPOINT_ADDRESS, (void*)&report, sizeof(struct composite_report_t));

  systick_counter += 1;
}
