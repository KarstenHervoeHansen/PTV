@ECHO OFF
@REM ###########################################
@REM # Script file to run the flow 
@REM # 
@REM ###########################################
@REM #
@REM # Command line for ngdbuild
@REM #
ngdbuild -p xc2vp20ff896-6 -nt timestamp -bm flash_test14.bmm C:/MyProject/flash_test_edk14/implementation/flash_test14.ngc -uc flash_test14.ucf flash_test14.ngd 

@REM #
@REM # Command line for map
@REM #
map -o flash_test14_map.ncd -pr b flash_test14.ngd flash_test14.pcf 

@REM #
@REM # Command line for par
@REM #
par -w -ol high flash_test14_map.ncd flash_test14.ncd flash_test14.pcf 

@REM #
@REM # Command line for post_par_trce
@REM #
trce -e 3 -xml flash_test14.twx flash_test14.ncd flash_test14.pcf 

