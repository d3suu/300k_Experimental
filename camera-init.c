#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "libusb.h"

// Compile with: gcc camera-init.c -I/usr/include/libusb-1.0 -lusb-1.0

// Testing code, not for production

static void uerr(char* s){
	printf("%s\n", s);
	exit(1);
}

static libusb_device_handle* camera_handle = NULL;

int main(){
	// init libusb
	int r = libusb_init(NULL);
	if (r < 0) uerr("Failed to init libusb");
	printf("Libusb init OK\n");

	// open device
	camera_handle = libusb_open_device_with_vid_pid(NULL, 0x090c, 0x037c);
	printf("Debug: camera_handle: %X\n", camera_handle);
	if ((camera_handle ? 0 : -ENODEV) < 0) uerr("Device not found - May need root");
	printf("Device found\n");

	// detach kernel driver
	r = libusb_set_auto_detach_kernel_driver(camera_handle, 1);
	if ( r != LIBUSB_SUCCESS ) uerr("Kernel driver detach failed");

	// claim interface 0
	r = libusb_claim_interface(camera_handle, 0); // Interface 0 for configuration
	printf("Debug: libusb_claim_interface(0): %d\n", r);
	if (r < 0) uerr("Interface error");
	printf("Interface 0 claimed\n");

	// get descriptor DEVICE
	struct libusb_device_descriptor* camera_device_descriptor = malloc(sizeof(struct libusb_device_descriptor));
	r = libusb_get_descriptor(camera_handle, LIBUSB_DT_DEVICE, 0x00, (unsigned char*)camera_device_descriptor, sizeof(struct libusb_device_descriptor));
	if (r != sizeof(struct libusb_device_descriptor)) uerr("GET DESCRIPTOR DEVICE error");
	printf("Device descriptor: %.4x:%.4x\n", camera_device_descriptor->idVendor, camera_device_descriptor->idProduct);

	// get descriptor CONFIGURATION
	// Actually get configuration two times, first for setup (wLength = 9), second for everything (wLength = 441)
	struct libusb_config_descriptor* camera_config_descriptor_init = malloc(sizeof(struct libusb_config_descriptor));
	r = libusb_get_descriptor(camera_handle, LIBUSB_DT_CONFIG, 0x00, (unsigned char*)camera_config_descriptor_init, 9);
	if (r != 9) uerr("GET DESCRIPTOR CONFIGURATION error");
	printf("Config init: wTotalLength: %d\n", camera_config_descriptor_init->wTotalLength);

	// I know static endpoint/interfaces, can I simply go for video frames?
	// Set configuration 1
	// Set interface 1
	// Claim interface 1
	// URB_ISOCHRO in and just go?

	return 0;
}
