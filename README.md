# organduino
------------
Tools and source code for an open source MIDI conversion of an old, analog organ.

# Contents
----------
* `sketches`: Arduino sketches and associated files.
* `sketches/libraries`: Arduino libraries needed for compilation of the sketches.

# Hardware
----------
* Arduino Mega 2560 (required, nothing else has enough GPIO pins)
* Manuals and 32 AGO pedal board from any buss-wired analog organ.  Digital matrix wired keyboards are more complex and differ in approaches per manufacturer; they cannot be used with this approach.  Each key should have its own dedicated wire.
* OrganDuino PCBs, one each for every manual plus one for the pedalboard/expression pedals.  Additional boards and customized Arduino sketches will also be required for any physical stop hardware; we recommend using a separate Arduino Mega 2560 for stops if you want physical stops.
* A fine-tipped soldering iron, patience, and a lot of fast SMD diodes which match the corresponding PCB footprint.

Note on the manuals and pedal board: Organ keys/pedals do not have touch sensitivity. They are *all* designed as open switches connecting to a single buss per manual or pedal board. The general approach here will be a key matrix 61 wide (the number of keys in a full organ manual) and several ranks deep.

As the project proceeds, additional manuals and/or handmade ranks of stops may be added, so while it is designed for this specific setup the goal was to be as extensible as possible.

# License
---------
Most of the best MIDI organ conversions are done closed source. We've decided to use the Apache 2.0 license here, which allows closed use, but hope that any such users consider contributing improvements back to the community.
