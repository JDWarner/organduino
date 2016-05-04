# organduino
------------
Tools and source code for an open source MIDI conversion of an old, analog organ.

# Contents
----------
* `sketches`: Arduino sketches and associated files.
* `sketches/libraries`: Arduino libraries needed for compilation of the sketches.

# Hardware
----------
* Arduino Mega 2560
* Raspberry Pi 2 model B
* Manuals and 32 AGO pedal board from a Rodgers Scarborough 110
* Lots of 10k resistors, diodes, jumpers, etc.
* Possibly PCB layouts

Note on the manuals and pedal board: Organ keys/pedals do not have touch sensitivity. They are *all* designed as open switches connecting to a single buss per manual or pedal board. The general approach here will be a key matrix 61 wide (the number of keys in a full organ manual) and several ranks deep.

As the project proceeds, additional manuals and/or handmade ranks of stops may be added, so while it is designed for this specific setup the goal was to be as extensible as possible.

# License
---------
Most of the best MIDI organ conversions are done closed source. We've decided to use the Apache 2.0 license here, which allows closed use, but hope that any such users consider contributing improvements back to the community.
