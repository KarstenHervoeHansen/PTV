       Calibration software PM 8550 V4.0 for CRT color sensors.


       1. GENERAL INFORMATION ABOUT PM 8550.

          1.1  Hardware requirements:
                PC:  Serial port COM1.
                     Monitor: VGA.
                    

                Display unit PM 5639/80
		Power supply box for PC and Industrial version of PM5639

                Cable to connect the COM1 port with the color sensor and
                the display unit PM 5639/80.

          1.2  The floppy diskette contains 2 files:

               PM8550.EXE           calibration program.
               README               this file.


       2.  INSTALLATION
           If you want to run the software from a harddisk, please copy the
           files PM8550.EXE and README from the floppy disk to the harddisk:

              COPY  A:PM8550.EXE  C: <ENTER>
              COPY  A:README      C: <ENTER>


       3.  START

           Connect the color sensor, the COM1-port and the display unit by
           using the cable. Switch the display unit ON. Please observe, that
           the display unit will shut off in 10 minutes if no keys on the
           display unit has been activated.
           Please observe, that the LCD display in the display unit will
           show "No Sensor". That is OK, because the display unit is only used
           to supply the color sensor with power.

	   Altenatively connect the power supply box for the PC and Industrial
	   Version of the PM5639


           The calibration software is started from the DOS prompt:

             C:\PM8550 [/B:48 | 96]  [/?]   <ENTER>
                   /B:48   select baud rate = 4800 (default)
                   /B:96   select baud rate = 9600
                   /?      Help display

             Examples: PM8550            4800 baud rate selected
                       PM8550 /B:96      9600 baud rate selected

           The PTV start-up screen will then be shown.
           Press ENTER.

           The software then performs the following:

                 a) Checks for a valid color sensor.
                    If it is not a valid color sensor, the program is halted.

                 b) Reads the serial number and displays it on the screen.

                 c) Reads the cal. status of the color sensor and
                    displays it on the screen (USER or FACTORY).

                 d) Reads the user text in the color sensor and displays it on
                    the screen.

                 e) Checks the write protect status of the color sensor.

                    If the color sensor is write protected, calibration and
                    restoring the factory calibration is not possible,
                    and you must follow the instructions in the menu
                    "Write Protect".

                    To set Write Protect ON/OFF in series 1 of the PM5639/90,
		    you may need the following tools:

                        2.5 mm  Allen key
                        soldering iron
                        scalpel


       4.  ENTER CALIBRATION DATA

           NOTE:  By pressing the F1 key, a help window is shown.

           a)  Select the CIE and luminance unit:
                       xy :  CIE 1931 chromaticity coordinates
                      u'v':  CIE 1976 chromaticity coordinates
                       uv :  CIE 1960 chromaticity coordinates

                    cd/m?, NIT or ftL can be used as the luminance unit.

           b)  Enter the chromaticity coordinates for the white color.

               Please observe, that it is possible to calibrate the color
               sensor without entering the chromaticity coordinates.

               Se help window for the limits.

           c)  Enter the luminance value. Se help window for the limits.

           d)  Enter the user text (optional).

               Please observe, that it is possible to change the user text
               without entering the chromaticity coordinates and the
               luminance value.


       5.  CALIBRATION

           Place the color sensor on the monitor and select the main menu
           "Start Cal".

           NOTE  If the user calibration data is not valid, it is not
                 possible to perform the calibration.

           After a successfull calibration, you should write protect the
           color sensor. If you do not set Write Protect ON, it is
           possible later on by using the submenu "Write Protect ON"
           in the mainmenu "Write Protect".

           Please observe, that it is possible to use the color sensor
           when the write protect is OFF.

           IMPORTANT:
	   After a calibration the color sensor must be powered OFF and
           the ON to make the calibration active


       6.  RESTORING THE FACTORY CALIBRATION

           It is possible to restore the original factory calibration with the
           function "Restore", if the Cal. Status field shows
           "USER" and the Write Protect is OFF.

           After restoring, the Cal. Status field is set to FACTORY.


           NOTE: If you restore the factory calibration, your own calibration
                 is overwritten.

