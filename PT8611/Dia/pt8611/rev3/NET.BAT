@rem diagram path :	F:\PT-Trilevel\dia\ver1\tlg_rev1.c
@rem pcb path :		F:\PT-Trilevel\pcb\ver1\tlg_rev1.pcb

@rem %1 : Drive
@rem %2 : common path
@rem %3 : input file name
@rem %4 : output file name (.net)

call e:\dk-utili\orc_net f: \PT8611 \dia\pt8611\rev3\tsg_rev3.c  \pcb\pt8611\rev3\tsg
f:\dka_util\net_cnv\net_cnv  f:\PT8611\pcb\pt8611\rev3\tsg
del f:\PT8611\pcb\pt8611\rev3\tsg.net
copy f:\PT8611\pcb\pt8611\rev3\tsg.cnv f:\PT-Tri~1\pcb\pt8611\rev3\tsg.net
del f:\PT8611\pcb\pt8611\rev3\tsg.cnv