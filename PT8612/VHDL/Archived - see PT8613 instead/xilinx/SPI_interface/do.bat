echo off
cls
echo RUNNING XST
xst -intstyle ise -ifn __projnav/spi_test.xst -ofn spi_test.syr >xx
if errorlevel 1 goto failure
echo XST OK!


echo RUNNING BUILD
ngdbuild -intstyle ise -dd "./_ngo" -nt timestamp -uc spi_test.ucf  -p xc2vp20-ff896-5 spi_test.ngc spi_test.ngd >>xx
if errorlevel 1 goto failure
echo BUILD OK!

echo RUNNING MAP
map -intstyle ise -p xc2vp20-ff896-5 -cm area -pr b -k 4 -c 100 -tx off -o spi_test_map.ncd spi_test.ngd spi_test.pcf >>xx
if errorlevel 1 goto failure
echo MAP OK!

echo RUNNING PAR
par -w -intstyle ise -ol high -t 1 spi_test_map.ncd spi_test.ncd spi_test.pcf >>xx
if errorlevel 1 goto failure
echo PAR OK!

echo RUNNING TRACE
trce -intstyle ise -e 3 -l 3 -s 5 -xml spi_test spi_test.ncd -o spi_test.twr spi_test.pcf >>xx
if errorlevel 1 goto failure
echo TRACE OK!

echo RUNNING BITGEN
bitgen -intstyle ise -f spi_test.ut spi_test.ncd >>xx
echo BITGEN OK!
goto end

:failure
echo RUN FAILED

:end
echo VIEW FILE XX
