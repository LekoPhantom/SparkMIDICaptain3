#ifndef MIDI_h
  #define MIDI_h

  #define SER_RX 16
  HardwareSerial *ser1;


  #ifdef USB_S3
    #include <usb/usb_host.h>
    #include "show_desc.hpp"
    #include "usbhhelp.hpp"

    RingBuffer USBHostBuf;
  #endif


  #ifdef USB_HOST
    #include <usbh_midi.h>
    #include <SPI.h>

    USB Usb;
    USBH_MIDI Midi(&Usb);
    bool usb_connected;
    uint16_t rcvd;
    uint8_t chan;
    uint8_t midi_buf[50*4];
  #endif

#include "RingBuffer.h"

#ifdef BLE_MIDI
class MIDIState 
{
  public:
    MIDIState() {};
    void initialise(RingBuffer *rb);
    bool process(byte mi_data[3]);

    RingBuffer *midi_stream;
    int midi_status;
    int midi_cmd_count;
    int midi_data_count;
    int midi_data[2];
};
#endif

  void setup_midi();
  bool update_midi(byte *mid);

#endif
