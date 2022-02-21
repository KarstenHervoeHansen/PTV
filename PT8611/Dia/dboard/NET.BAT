@rem diagram path :	F:\PT-Trilevel\dia\ver1\tlg_rev1.c
@rem pcb path :		F:\PT-Trilevel\pcb\ver1\tlg_rev1.pcb

@rem %1 : Drive
@rem %2 : common path
@rem %3 : input file name
@rem %4 : output file name (.net)

call e:\dk-utili\orc_net f: \PT-Tri~1 \dia\dboard\dboard.c  \pcb\dboard\dboard
f:\dka_util\net_cnv\net_cnv  f:\PT-Tri~1\pcb\dboard\dboard
del f:\PT-Tri~1\pcb\dboard\dboard.net
copy f:\PT-Tri~1\pcb\dboard\dboard.cnv f:\PT-Tri~1\pcb\dboard\dboard.net
del f:\PT-Tri~1\pcb\dboard\dboard.cnv