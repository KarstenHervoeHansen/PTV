@rem %1 : Drive
@rem %2 : common path
@rem %3 : input file name
@rem %4 : output file name (-net)
@rem call e:\dk-utili\orc_net f: \newdsp2185 \dia\fpga.c  \pcb\fpga
call e:\dk-utili\orc_net P: \HDTV \dia\HDTV_N.C  \pcb\HDTV_N
f:\dka_util\net_cnv\net_cnv P:\HDTV\pcb\HDTV_N
del P:\HDTV\PCB\HDTV_N.NET
copy P:\HDTV\pcb\HDTV_N.cnv P:\HDTV\pcb\HDTV_N.net
del P:\HDTV\PCb\HDTV_N.cnv
