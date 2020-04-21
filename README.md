# organduino

Tools and source code for open source MIDI conversion of old, analog organ hardware with common buss wiring for each manual/pedalboard.

# Contents

* `sketches`: Arduino sketches and associated files.
* `sketches/libraries`: Arduino libraries needed for compilation of the sketches.
* Instructions, in form of this readme.

# Hardware

* Arduino Mega 2560 (required, nothing else has enough GPIO pins)
* Multiple manuals and 32 AGO pedal board from any buss-wired analog organ.  Digital matrix wired keyboards are more complex and differ in approaches per manufacturer; they cannot be used with this approach/project.  Each key should have its own dedicated wire which closes a circut with the common buss for the manual.
* Multiple OrganDuino 2.0 PCBs, one each for every manual plus one for the pedalboard/expression pedals.  Additional boards and customized Arduino sketches will also be required for any physical stop hardware; we recommend using a separate Arduino Mega 2560 for stops if you want physical stops.

Note on the manuals and pedal board: Organ keys/pedals do not have touch sensitivity. They are *all* designed as open switches which activate when pressed.  This project is exclusively designed for older hardware where all keys/pedals for a given manual are connected to a single buss per manual or pedal board. The general approach here will be a key matrix 61 wide (the number of keys in a full organ manual) and several ranks deep.  The design allows for up to 5 manuals and a full AGO pedalboard to be driven from a single Arduino Mega 2560 plus multiple stacked shields (PCB design files for the stacked shield included in this repository).

You may start with one manual or pedalboard.  As your project/ambitions grow, additional manuals and/or handmade ranks of stops may be added.  No more than 6 ranks of 61 switches can be controlled from a single Arduino with this design, but multiple additional Arduinos can be added.

# Populating the OrganDuino boards

You will need:
* OrganDuino PCB(s)
* Stacking Arduino Mega2560 headers
* One fast SMD diode for each key, which fits the board footprint; Vishay part number SD101CW-E3-08 verified working
* A fine-tipped soldering iron
* Fine-tipped tweezers
* Patience.

## Instructions

The OrganDuino PCB enables each manual to be a "row" in a key matrix.  We'll be polling the position of key 1 on every manual via a single pin.  The only way to do this without getting spurious signal from adjacent manuals is a key matrix.  Every key requires its own diode.

Diodes seem like little parts that are easy to deal with until you have space constraints on a board and need to populate 61 of them.  OrganDuino PCBs thus use surface mount diodes, relatively large ones.

If you have never hand-soldered SMD components, these are a large footprint and very doable.  Solder the SMD components FIRST.  Heat only one pad for each diode with the fine-tipped soldering iron, and apply a dab of solder to the pad.  Do this for all diode mounts.  Then, using tweezers, get the component positioned next to the pad.  With your solder iron in one hand and SMD diode held by tweezers in the other, heat the pad with a small amount of solder on it and once the solder is molten, slide the diode pad into the molten solder.  Remove soldering iron, continue to hold in place with tweezers until the solder solidifies (a few seconds).  Place all diodes this way.  Now go back and solder the other lead of the diodes to the second, untouched pads.

Double check the orientation of all SMD diodes.  The recommended part has rather poor marking to deliniate which side is which, and all diodes must be positioned properly.

Once the board is completely populated with diodes, solder the stackable Mega2560 headers and finally bridge the appropriate solder jumper on the back side.  Each board must have a different solder jumper bridged.

Map/verify all keys are working for the manual/pedalboard in question with a digital multimeter with continuity testing or similar.  Connect the buss to any of the 24 through-holes marked "Drive Pin(s)".  Once the switches are verified working, solder each manual key to the appropriate through-hole on the periphery of the OrganDuino 2.0 PCB.  Keys are soldered in chromatic order and grouped by octave for reference; if you look down at the top of the OrganDuino 2.0 PCB and orient it so you can read "OrganDuino" in the upper left corner, the first(bottom/lowest) key will be soldered at the far lower left.

Stack this PCB with all others on the Arduino Mega 2560.

Power up the Arduino.  Upload a sketch (midi_organ.ino is a verified working 4 manual, 1 pedalboard sketch).  Watch the MIDI events roll in.  Connect your organ emulation software to the MIDI serial bridge, and enjoy!

# Software

Currently, output from this board is MIDI over serial.  It needs to be converted into true MIDI.  Hairless-MIDISerial works great for this, though the project currently has no official working release on OSX 10.15+.  Users have forked, compiled, and released working updates for OSX 10.15+.

The included Arduino sketches are designed to be used with OrganDuino 2.0 PCB, stacked (up to 6).  61 inputs are pulled high with pullup resistors while the 6 buss pins are also pulled high.  To poll a given manual, the buss pin is written low and all 61 potential inputs are polled in digital read mode; if the key is pressed, they will be pulled to ground and read low.  The state of all keys/pedals is stored in global variables and only changes in the state are sent via MIDI events, for increased efficiency.  The exact polling speed is incredibly fast but variable (depends on how many MIDI events must be sent over serial in a given loop), but the latency is not perceptible.  It is fast enough to catch keys bouncing.

A rudimentary debounce method is included as well, though for this project ultimately no debouncing was used.  Keys do bounce, but thanks to real-life organ sample sets having soft starts or millisecond delays in the samples this was imperceptible.  Responsiveness was felt more important.  If your uses and hardware need debouncing, this is one way; one could also use any of multiple libraries for the purpose (though take care, some will introduce unacceptable latency when applied to this many inputs).

Advanced Arduino users may note it is possible to blow away the Arduino bootloader chip and convert the USB output into a true MIDI over USB.  However, this is nearly irreversible without directly accessing the 6-pin ATMEL bootloader with a programmer and the OrganDuino 2.0 PCBs cover these pins without exposing them.  MIDI over serial works with imperceptible latency for 4 manuals and a pedalboard, so it is strongly recommended to keep the Arduino bootloader and use MIDI over serial with a bridge.

# Future work

An OrganDuino 3.0 board would expose the 6-pin header for the Atmel USB communication chip through all shields, allowing users to safely burn that chip into a true USB MIDI device.  The Arduino bootloader could then be recovered via the 6-pin header and an AVR ISP.  This would safely eliminate the need for serial over USB bridging, once a stable configuration is reached.

Expression pedal input is currently not supported.  It is suggested to use OrganDuino only for on/off devices like keys and stops.  PRs welcome for alternative approaches which would support expression pedals, both sketches and potentially PCB hardware.

# License

Most of the best MIDI organ conversions are done closed source. We've decided to use the Apache 2.0 license here, which allows closed use, but hope that any such users consider contributing improvements back to the community.
