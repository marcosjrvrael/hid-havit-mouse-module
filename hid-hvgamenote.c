// SPDX-License-Identifier: GPL-3.0-or-later
/*
 *  HID driver for havit MS963WB
 *
 *  Copyright (c) 2024 Moj 
 */

/*
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/hid.h>

#include "hid-ids.h"

/* Fixed Mi Silent Mouse report descriptor */
/* Button's Usage Maximum changed from 3 to 5 to make side buttons work */
#define HAVIT_GAMENOTE_MOUSE_ORIG_RDESC_LENGTH 87
#define HAVIT_GAMENOTE_FAULT_OFFSET	17

static const __u8 havit_faulty_rdesc[] = {
0x05, 0x01,                    // Usage Page (Generic Desktop)        0
0x09, 0x02,                    // Usage (Mouse)                       2
0xa1, 0x01,                    // Collection (Application)            4
0x85, 0x01,                    //  Report ID (1)                      6
0x09, 0x01,                    //  Usage (Pointer)                    8
0xa1, 0x00,                    //  Collection (Physical)              10
0x05, 0x09,                    //   Usage Page (Button)               12
0x19, 0x01,                    //   Usage Minimum (1)                 14
0x29, 0x03,                    //   Usage Maximum (3)                 16
0x15, 0x00,                    //   Logical Minimum (0)               18
0x25, 0x01,                    //   Logical Maximum (1)               20
0x75, 0x01,                    //   Report Size (1)                   22
0x95, 0x05,                    //   Report Count (5)                  24
0x81, 0x02,                    //   Input (Data,Var,Abs)              26
0x75, 0x03,                    //   Report Size (3)                   28
0x95, 0x01,                    //   Report Count (1)                  30
0x81, 0x01,                    //   Input (Cnst,Arr,Abs)              32
0x05, 0x01,                    //   Usage Page (Generic Desktop)      34
0x09, 0x30,                    //   Usage (X)                         36
0x09, 0x31,                    //   Usage (Y)                         38
0x16, 0x01, 0x80,              //   Logical Minimum (-32767)          40
0x26, 0xff, 0x7f,              //   Logical Maximum (32767)           43
0x75, 0x10,                    //   Report Size (16)                  46
0x95, 0x02,                    //   Report Count (2)                  48
0x81, 0x06,                    //   Input (Data,Var,Rel)              50
0x09, 0x38,                    //   Usage (Wheel)                     52
0x15, 0x81,                    //   Logical Minimum (-127)            54
0x25, 0x7f,                    //   Logical Maximum (127)             56
0x75, 0x08,                    //   Report Size (8)                   58
0x95, 0x01,                    //   Report Count (1)                  60
0x81, 0x06,                    //   Input (Data,Var,Rel)              62
0xc0,                          //  End Collection                     64
0xc0,                          // End Collection                      65
0x05, 0x0c,                    // Usage Page (Consumer Devices)       66
0x09, 0x01,                    // Usage (Consumer Control)            68
0xa1, 0x01,                    // Collection (Application)            70
0x85, 0x02,                    //  Report ID (2)                      72
0x75, 0x10,                    //  Report Size (16)                   74
0x95, 0x01,                    //  Report Count (1)                   76
0x15, 0x01,                    //  Logical Minimum (1)                78
0x26, 0x8c, 0x02,              //  Logical Maximum (652)              80
0x19, 0x01,                    //  Usage Minimum (1)                  83
0x2a, 0x8c, 0x02,              //  Usage Maximum (652)                85
0x81, 0x00,                    //  Input (Data,Arr,Abs)               88
0xc0,                          // End Collection                      90
};

/*
 * we fix the report descriptor by setting byte 17 to 5
 */
static __u8 *havit_report_fixup(struct hid_device *hdev, __u8 *rdesc,
				 unsigned int *rsize)
{
	switch (hdev->product) {
	case USB_DEVICE_ID_HAVIT_GAMENOTE_MOUSE_C:
		if (*rsize == HAVIT_GAMENOTE_MOUSE_ORIG_RDESC_LENGTH) {
			hid_info(hdev, "fixing up Havit Mouse report descriptor\n");
			rdesc = havit_faulty_rdesc;
			*rsize = sizeof(havit_faulty_rdesc);
		}
		break;
	case USB_DEVICE_ID_HAVIT_GAMENOTE_MOUSE_W:
		if (*rsize == HAVIT_GAMENOTE_MOUSE_ORIG_RDESC_LENGTH) {
			hid_info(hdev, "fixing up Havit Mouse report descriptor\n");
			rdesc = havit_faulty_rdesc;
			*rsize = sizeof(havit_faulty_rdesc);
		}
		break;
	}
	if(memcmp(rdesc, havit_faulty_rdesc, HAVIT_GAMENOTE_FAULT_OFFSET) != 0) return rdesc;
    	rdesc[HAVIT_GAMENOTE_FAULT_OFFSET] = 0x05;
	return rdesc;
}

static const struct hid_device_id havit_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_HAVIT,
            USB_DEVICE_ID_HAVIT_GAMENOTE_MOUSE_C) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_HAVIT,
            USB_DEVICE_ID_HAVIT_GAMENOTE_MOUSE_W) },
	{ }
};

MODULE_DEVICE_TABLE(hid, havit_devices);

static struct hid_driver havit_driver = {
	.name = "havit",
	.id_table = havit_devices,
	.report_fixup = havit_report_fixup,
};
module_hid_driver(havit_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcos Oliveira <moliveira.dev@gmail.com>");
MODULE_DESCRIPTION("Fixing side buttons of Havit Gamenote BT mouse");