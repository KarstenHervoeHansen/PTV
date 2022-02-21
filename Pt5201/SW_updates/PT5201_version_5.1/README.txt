**********************************************************
To upload program version 5.1 into PT5201/LT428 FLASH program memory:

This upload program is applicable for the following s/w versions:

PT5201: sw 4.1 to 4.9
LT428:  sw 1.0

How to determine s/w version:
- Run the supplied Windows interface program.
- Check the number in the field in the lower part: SW Revision


For upgradable instruments the process consists in following steps:

- Store s/w to PC
- Preparing PT5201/LT428 for upload
- Uploading of s/w
- Check of uploaded s/w.
- Change of SW revision number.


STORE S/W TO PC
- Copy the following files to one directory:
  loadall.exe - loader program
  load51.ini - .ini file for loader.exe
  520151.hex - Software ver. 5.1 in hex format


PREPARE PT5201/LT428 FOR DOWNLOAD  
- Power down the PT5201/LT428.
- Remove the top cover from generator.
- Install the PP2 jumper.
	PP2 is a single jumper positioned between the Altera Flex and Max IC's.
- Connect serial cable between the generator and your computer's COM1 serial port.
- Apply power to the generator.
  Leds D28 and D31 should blink together.

  
UPLOADING OF S/W
- Open a DOS-box under your Windows 95/98 operating system.
- Run the program loadall.exe with the following command line
  parameters:
  loadall load51.ini.
- After approx. 2 minutes the program will terminate, writing
  "...uploaded." "Done!!!" on the screen.
- The new software version is now downloaded and ready to use.
- Switch the PT5201 generator off.
- Remove the PP2 jumper.
- Switch the generator on.
  The D28 LED should blink, which indicates that the new 
  software executes corrcectly.

CHECK OF UPLOADED S/W
To verify if the program upload was successful the program's checksums
can be verified from a serial terminal program like Windows Hyperterminal.      
 
The verification is done as follow:
 
- set the terminal to 115200, 8, n, 2, no flow control
- if using Hyperterminal, set Properties, Settings, 
  ASCII-settings with checked "Send new ...."

- Power down the PT5201/LT428.
- Verify that the PP2 jumper is installed.
- Apply power to the generator.
  Leds D28 and D31 should blink together.
- send the command IDN? to check communication. 
  Instrument should return with identification and other data.
- Issue the commands at the end of this document and check 
  that the PT5201/LT428 generator responds as required.  Each 
  command should be ended with 'RETURN' key.
- If the checksums are incorrect, repeat the download procedure.     
- Switch the PT5201 generator off.
- Remove the PP2 jumper.
- Switch the generator on.
  The D28 LED should blink, which indicates that the new 
  software executes correcectly.


CHANGE OF S/W REVISION NUMBER
Now software revision number, which is stored in PT5201/LT428
non-volatile memory, will have to be updated.  
This involves issuing commands via SCPI interface, as described 
under the "Remote Interface" in the Operating Manual.  

Proceed as follows:
- Set your terminal to 9600,8,n,2, no flow control.
  In case using Hyperterminal, set Properties, Settings, 
  ASCII-settings with checked "Send new ...." 
- Write:
    *IDN?
    The PT5201/LT428 should respond with its id string.
- Write:
    :FACT:PASS 'PASS5201'    (Note 'PASS5201' is with capital letters)
    :FACT:PASS?              (Instrument should now respond with OFF)
    :FACT:MAIN:IDN:SWR 051   (New ID for PT5201)    
  - The revision number should now be updated inside the PT5201.
  - Write:
    *IDN?
    and verify that the PT5201 responds with new revision number.
    
For LT428:
    :FACT:MAIN:IDN:SWR 011


WARNING:
Do NOT attempt to send other commands after :FACT:PASS 'PASS5201' has been issued,
as this command opens for commands only to be used during production !!

After ID has been changed power off the instrument. 
At next power-on the special access to change s/w ID has been disabled.


  
Checksum commands and correct values for PT5201 software
ver, 5.1:

Command:    Response:
fch? 0       30672
fch? 1       32230
fch? 4       35026
fch? 5       27678
fch? 6       10758
fch? 7       60182
fch? 8       1030
fch? 9       4907
fch? 10      63842


* 24/04-2002*
*JK/Gbg*

