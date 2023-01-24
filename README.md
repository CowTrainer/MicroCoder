# MicroCoder
A small GUI to simplify programming and teaching of microcode, mostly for educational purposes. 

Able to work with and export in binary, decimal and hexadecimal, to MIF and TDF file formats, with an intuitive table layout and many ease-of-use features

Built with QT 5.15.2 64 bit MinGW

![image](https://user-images.githubusercontent.com/25671117/214133540-88c51d15-20a3-4eb1-9104-918eb18e19d2.png)

# Installation Insturctions

## Run exectuable directly

Navigate to the MicroCoder_Release folder, and click on the executable Microcoder.exe

## Compile and run yourself

If you have QT installed yourself, you can open QT Creator, go to Open File or Project, and select the Microcoder.pro file.

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
the correct file type in the bottom right corner










Configuration files have a .upc extension, Microprograms have a .upm extension and contain the configuration they were built using, so you can start by importing one of them aswell


# Credits
Framework created by Dr. Ken McIssac, with majority of key features added by Herry Jia.
