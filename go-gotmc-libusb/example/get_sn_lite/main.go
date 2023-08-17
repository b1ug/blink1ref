// Copyright (c) 2016 The libusb developers. All rights reserved.
// Project site: https://github.com/gotmc/libusb
// Use of this source code is governed by a MIT-style license that
// can be found in the LICENSE.txt file for the project.

package main

import (
	"log"

	"github.com/gotmc/libusb"
)

func main() {
	ctx, _ := libusb.NewContext()
	defer ctx.Close()
	devices, _ := ctx.DeviceList()
	for _, device := range devices {
		usbDeviceDescriptor, _ := device.DeviceDescriptor()
		handle, _ := device.Open()
		defer handle.Close()
		serialNumber, _ := handle.StringDescriptorASCII(usbDeviceDescriptor.SerialNumberIndex)
		log.Printf("Found S/N: %s", serialNumber)
	}

}
