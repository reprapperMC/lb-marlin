# LulzBot Marlin firmware

This is the development branch of Marlin for LulzBot printers.

The source on this branch can compile firmware for the TAZ and Mini series, as well as all the current toolheads. This firmware also supports some internal R&D prototypes and toolheads.

# Safety and warnings:

**This repository may contain untested software.** It has not been extensively tested and may damage your printer and present other hazards. Use at your own risk. Do not operate your printer while unattended and be sure to power it off when leaving the room. Please consult the documentation that came with your printer for additional safety and warning information.

# Compilation from the command line on Linux using "avr-gcc"

Run the "build-lulzbot-firmware.sh" from the top level directory. This will build the ".hex" files for every printer and toolhead combination. The ".hex" files will be saved in the "build" subdirectory.

# Compilation using Arduino IDE

To select what firmware to build, modify the lines starting with "//#define" towards the bottom of the "Marlin/Configuration_LulzBot.h" file. Remove the leading "//" and modify the text after "LULZBOT_" and "TOOLHEAD_" so that it specifies the desired printer model or the desired toolhead, as listed in the top of the file.

For example, to compile for the Mini 2, modify the lines such that they read:

  #define LULZBOT_Hibiscus_Mini2
  #define TOOLHEAD_Finch_AerostruderV2

To compile for a TAZ using a standard toolhead, modify the lines such that they read:

  #define LULZBOT_Oliveoil_TAZ6
  #define TOOLHEAD_Tilapia_SingleExtruder

Then, open the "Marlin.ino" file from the "Marlin" subdirectory in the Arduino IDE. Select the board "Arduino/Genuino Mega or Mega 2560" from the "Board" submenu menu of the "Tools" menu and the port to which your printer is connected from the "Port" submenu from the "Tools" menu.

To compile and upload the firmware to your printer, select "Upload" from the "Sketch" menu.

# Understanding and Modifying the Default Configuration

If you are trying to modify Marlin's configuration, you may find that our configurations files differ from standard Marlin. This is because we support over 40 different product combination from a common source, rather than just the printer and accessories you may own. Understanding how this process works is will help you undderstand how to make simple changes to your copy of Marlin.

The configuration files consist of the following files:

   - Configuration_LulzBot.h  <--- LulzBot specific
   - Conditionals_LulzBot.h   <--- LulzBot specific
   - Configuration.h          <--- Standard Marlin
   - Configuration_adv.h      <--- Standard Marlin

In standard Marlin, you modify "Configuration.h" and "Configuration_adv.h" with values for your particular printer. However, we do not maintain separate configurations for each of our printers and toolheads, as there are too many. Instead, we use two additional files to progmatically generate default settings for printers and toolheads.

The way this works is that "Configuration_LulzBot.h" defines a variable for the printer and a variable for the toolhead (i.e. LULZBOT_Oliveoil_TAZ6 and TOOLHEAD_Tilapia_SingleExtruder). The "Conditionals_LulzBot.h" then takes this and generates a set of default values based on those two selections. All of these default values are stored in variables prefixed by "LULZBOT_". There is one LULZBOT_ variable for each of the standard Marlin variables we modify from the Marlin default.

If you look at either "Configuration.h" or "Configuration_adv.h", you will see that many lines are configured using the "LULZBOT_" variables , i.e:

  #define X_MIN_POS     LULZBOT_X_MIN_POS

This defines the standard Marlin variable "X_MIN_POS" with the value of the variable "LULZBOT_X_MIN_POS", which is generated by "Conditionals_LulzBot.h" based on your selections in "Configuration_LulzBot.h"

If you did not like the default values we provide for your printer, you can substitute values in place of the LULZBOT_ variables, like this:

  #define X_MIN_POS      100

This is the easiest way of doing small changes as it does not involve modifying "Conditionals_LulzBot.h", which is a far more difficult task (and is only necessary if you want to maintain the ability conditionally compile for other printers or toolheads).