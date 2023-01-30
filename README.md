# MicroCoder
A small GUI to simplify programming and teaching of microcode, mostly for educational purposes. 

Able to work with and export in binary, decimal and hexadecimal, to MIF and TDF file formats, with an intuitive table layout and many ease-of-use features

Built with QT 5.15.2 64 bit MinGW

![image](https://user-images.githubusercontent.com/25671117/214133540-88c51d15-20a3-4eb1-9104-918eb18e19d2.png)

# Installation Insturctions

## Run exectuable directly

Navigate to the MicroCoder_Release folder, and click on the executable Microcoder.exe

## Compile and run yourself

If you have QT installed yourself, you can open QT Creator, go to Open File or Project, and select the Microcoder.pro file (located in Microcoder_Source)

You can then run the program by building and running.

# Use Manual

## Starting off

When starting off to build a microprogram, you can create a program from scratch, import an existing configuration or import an existing program

### Creating a new program from scratch

Start by clicking new configuration

![image](https://user-images.githubusercontent.com/25671117/214374129-0b643e3a-96ea-48ab-b92b-398838182e90.png)

You will then be brought to an editor screen, this screen can be accessed at any time by clicking Configuration then Edit Configuration.

![image](https://user-images.githubusercontent.com/25671117/214374574-5196c7a7-562f-4b02-b175-8718651ea8c3.png)

You can edit the Conditions and Signals by right clicking on the respective boxes to create or delete items, or by left clicking on existing items
to edit name, width etc... (Note that the above is simply an example and not a default that comes up every time.

Once you are finished, navigate to File to click New Microprogram, and the following will be brung up

![image](https://user-images.githubusercontent.com/25671117/214375881-e9b441a9-7c06-47fd-a45f-c4c114c74d6f.png)

### Importing a configuration

Configurations have extension .upc when saved. These files can be saved and imported at a later date for use. It is also possible to 
import .upm (Microprogram files) just for their configuration. 

To import a configuration, click "File" and then "Load Configuration". Locate your desired file (if it is not appearing make sure you are filtering for 
the correct file type in the bottom right corner)

### Importing an existing program

You can import and expisting microprogram by using the "Load Microcode" button.

## Editing

There are many helpful features in the main editor. At it's core you can edit each value by double clicking on the respective cell. Signal rows can be edited with either decimal or hexadecimal values (if you can fit it in the bits), but cannot be edited using binary values.

### Adding/Deleting Rows

When you start with a new microprogram, you can add instructions by clicking on the "Microprogram" tab and clicking "New Instruction"

After you have rows in the table, you can add instructions after them / delete them by right clicking on respective row header (Small box with the row number)

### Moving rows

You can move rows by dragging the row header to a spot in-between the headers of the two rows you want to move it into (or all the way to the end or start if you want it that way.

### Signal Representation

If you want to view the value of a signal output as a different radix, you can right click on any cell of that signal. A menu will pop up that will allow you to change the viewing radix of that signal's values.

### Target Valdity Checking

Sometimes you will see a red highlight on one of the target boxes. It indicates that you have an unresolved reference and will change to normal once a label is implemented with that target's name or the target is changed to a valid reference.

## Exporting and Saving

Once you are satisifed with your program, you can export to either the TDF or MIF file format. You can access this by going to the "Microprogram" tab and clicking on "Generate". A menu will come up allowing you to choose the radixes you export in.

Also be sure to save perodically and before you exit the program. The editor will not notify you that your changes are unsaved, except for when the microcode file has not been saved at all (e.g. You made a new file and tried to exit the program).


# Credits
Framework created by Dr. Ken McIssac, with majority of key features added by Herry Jia.
