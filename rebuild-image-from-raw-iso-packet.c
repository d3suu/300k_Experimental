#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Usage
 * Save whole "URB_ISOCHRONOUS in" response packet from Wireshark as "packet.raw",
 * run this program, it should generate "image.raw". You can use that "image.raw" on
 * rawpixels.net with settings:
 * width: 640 height: 200 (or more, it does not send full frame in single packet)
 * offset: 0
 * Predefined format: YUY2
 * Pixel Format: YUV (Don't ignore Alpha, Alpha is last)
 * bpp1-4: 8, Big Endian
 * Pixel plane: PackedYUV
 * Alignment, subsampH, subsampV: 1
 *
 * Notes
 * Image can be shifted horizontally due to partial first line. To fix this for debug image,
 * remove first ISO packet data from "image.raw".
 */


struct iso_packet_header {
	uint32_t iso_data_offset;
	uint32_t iso_data_length;
	uint32_t iso_usbd_status;
};

int main(){
	FILE* packet_f = fopen("./packet.raw", "rb");
	FILE* image_f = fopen("./image.raw", "wb");
	size_t header_pos_start = 39; // After usb headers, start of first packet header
	size_t data_pos_start = header_pos_start + (12*128); // After usb headers, after packet headers
	int packet_n = 0;
	
	struct iso_packet_header* iso_packet_header_buffer = malloc(sizeof(struct iso_packet_header));
	
	// Jump to ISO packet headers
	fseek(packet_f, header_pos_start, SEEK_SET);

	// Loop
	while(packet_n<128){
		// Read header (offset + size)
		fread(iso_packet_header_buffer, sizeof(struct iso_packet_header), 1, packet_f);
		printf("Packet number: %d\nISO Data offset: %.8x\nISO Data length: %.8x (%u bytes)\nISO USBD status: %.8x\n", packet_n, iso_packet_header_buffer->iso_data_offset, iso_packet_header_buffer->iso_data_length, iso_packet_header_buffer->iso_data_length, iso_packet_header_buffer->iso_usbd_status);

		// Jump to data with offset and data header offset (12)
		fseek(packet_f, data_pos_start + iso_packet_header_buffer->iso_data_offset + 12, SEEK_SET);
		// Create buffer for image
		char* image_buffer = malloc(iso_packet_header_buffer->iso_data_length);
		// Read image from packet.raw
		fread(image_buffer, iso_packet_header_buffer->iso_data_length-12, 1, packet_f);
		// Write image to image.raw
		fwrite(image_buffer, iso_packet_header_buffer->iso_data_length-12, 1, image_f);
		// Free buffer allocation
		free(image_buffer);

		// Jump back to next header
		fseek(packet_f, header_pos_start + (12*(packet_n+1)), SEEK_SET);
		packet_n++;
	}

	fclose(packet_f);
	return 0;
}

