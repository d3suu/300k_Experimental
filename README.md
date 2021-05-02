# 300k_Experimental
Reverse engineering and experimental Silicon Motion 300k Pixel Camera Linux driver

## Why

For whatever reason, this camera does not want to work on my Void Linux installation. I want to learn USB programming by making standalone driver.

## Current progress

 - Device configuration dumped
 - Device configuration analyzed
 - Frame request and response dumped
 - Create nice wireshark dump for this repo (device plug-in sequence, video dump)
 - POC Device configuration in ~~PyUSB or~~ libusb

## TODO

 - POC Raw data dumper
 - POC Frame dumper in RGB
 - Full blown linux kernel driver (v4l2?)

## What I know so far

| Name               | Value                                                                                                                                                                                                                                                                         | Additional Info                                                                                                      |
|--------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------|
| idVendor           | 0x090c                                                                                                                                                                                                                                                                        | Silicon Motion, Inc. - Taiwan (formerly Feiya Technology Corp.)                                                      |
| idProduct          | 0x037c                                                                                                                                                                                                                                                                        | 300k Pixel Camera                                                                                                    |
| Image resolution   | 640x420                                                                                                                                                                                                                                                                       | GET DESCRIPTOR CONFIGURATION response returns 640x480, 160x120, 176x144, 320x240, 352x288, 640x480, with indexes 1-6 |
| Pixel format       | YVU                                                                                                                                                                                                                                                                           | Predefined YUY2 on rawpixels.net, GET DESCRIPTOR CONFIGURATION response returns YUY2 (16bits per pixel)              |
| Color format       | BT.709, sRGB                                                                                                                                                                                                                                                                  | bMatrixCoefficients: SMPTE 170M (BT.601) (4)                                                                         |
| bNumConfigurations | 1                                                                                                                                                                                                                                                                             |                                                                                                                      |
| Interfaces         | 4                                                                                                                                                                                                                                                                             | Only Interface 1 is used for video transfer                                                                          |
| USBVIDEO Controls  | Exposure Time [Absolute], Auto-Exposure Priority, IRIS [Absolute], Request Error Code, Roll [Absolute], Unit 2 Control 0x1-8, Brightness, Contrast, Hue, Saturation, Sharpness, Gamma, White Balance Temperature, Backlight Compensation, Power Line Frequency, Probe, Commit |                                                                                                                      |
