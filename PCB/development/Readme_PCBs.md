# OrganDuino PCBs

Developed in Fritzing, these files represent the history of hardware OrganDuino development.

The original boards were larger than necessary, and attempted to use the TX/RX pins on the Arduino as polling sources.  While you can make a sketch do this, it won't be able to talk back via serial over USB.  Those were quickly tossed.

**The current working project uses the OrganDuino 2.0 PCB design, which corresponds to organduino9.fzz in this directory.**  Have this board fabbed for your purposes (at least one for each manual/pedalboard), or start there if you want to tweak things.

Possible improvements for a 3.0 version: Expose the 6 pin ICSP - not for the main Arduino chip, but for the USB driver.  If these were passed through the OrganDuino shields, one could safely blow that chip away into a USB MIDI device, while being able to recover and burn the bootloader back if desired via an AVR ISP.  Unfortunately, Fritzing does not currently have a built-in PCB layout which includes these pins for the 2560.
