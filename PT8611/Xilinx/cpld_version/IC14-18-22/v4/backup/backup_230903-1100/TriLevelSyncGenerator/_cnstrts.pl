my $_IsMSWin32 = ( $^O eq 'MSWin32' );
$ucf_file='tri_level_sync_generator.ucf'; 
$ucf_file=~s/\\/\//g;
$ucf_file=~s/\\/\\\\/g if ( $_IsMSWin32 );
my $_PrgPath = 'C:/Programmer/xilinx_webpack/data/projnav';
$_PrgPath =~ s/\//\\/g if ( $_IsMSWin32 );
unshift @INC, $_PrgPath;
require projnav;
$ENV{"XILINX"}=$ARGV[1];
$ENV{"PATH"}="$ARGV[1]/bin/nt;$ARGV[0]";
if ( -z "$ucf_file" )
{
open  OUTPUT, ">$ucf_file";
print OUTPUT "//UCF file created by Project Navigator";
close OUTPUT;
}
&projnav::CopyFile( $ucf_file, '_test.ucf' );
my $_CeCmd  = "$ARGV[0]/constraints_editor";
$_CeCmd .= '.exe' if ( $_IsMSWin32 );
$_CeCmd .= " tri_level_sync_generator.ngd -uc $ucf_file";
$_CeCmd =~ s/\//\\/g if ( $_IsMSWin32 );
system( "$_CeCmd" );
if ( -z "$ucf_file" )
{
open  OUTPUT, ">$ucf_file";
print OUTPUT "//UCF file created by Project Navigator";
close OUTPUT;
}
&projnav::CopyFile( $ucf_file, '_test2.ucf' );
my $_ChkDate  = "$ARGV[0]/chkdate";
$_ChkDate .= '.exe' if ( $_IsMSWin32 );
$_ChkDate =~ s/\//\\/g if ( $_IsMSWin32 );
system( "$_ChkDate" );
if ( -e "_test.dat" )
{
open  OUTPUT, ">chkdate.err";
print OUTPUT "The Implement Design process will now be reset so that your constraint changes will be read.";
close OUTPUT;
unlink "last_used.ngd";
rename "tri_level_sync_generator.ngd","last_used.ngd";
}
else
{
open  OUTPUT, ">chkdate.err";
print OUTPUT "Existing implementation results (if any) will be retained.";
close OUTPUT;
}
unlink "_test.ucf";
unlink "_test2.ucf";
unlink "_test.dat";
