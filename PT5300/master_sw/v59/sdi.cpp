

int SDICal(int TVLines, int SDINo, int GetXX1)
  {
  // GetXX1 = NO         XX1_Stik opdateres ikke af brugeren
  // GetXX1 = YES        XX1_Stik opdateres af brugeren
  char 	TxtStr[100],
        resp[20],
        *SDIGenlKU,
        *strread,
        *HBread,
        HWVer[20],
        SWVer[20];
  		
  int 	handle_returned, 
  	    controlID_returned,
  	 	  bytes_read,
  	    k,
  		  NegDelay;
  
  unsigned int  SDIphase,   // variable for holding offset which will be 
                            // written by Set G-system and offset command.
                phase_read, // holds value read from novram by
                            // Read phase G command.
                MaxPhase;
  
  float  sdidelay;  // signal delay measured by VCA
  double dd;



// SDINo:     SDI TPG = 1,2 eller 5       
//            SDI Black = 34, 56 eller 78
//            SDI TSG = 22,23 eller 24
// TVLines:   L625 eller L525

  Fmt(TxtStr,"%s< SDI Test Pattern Generator #%i (SDI%i) ",SDINo,TVLines);
  SetPanelAttribute (sdiPNL, ATTR_TITLE, TxtStr);
  
  // Check at SDI genlock er monteret
  SDIGenlKU = ReadSDIGenlKUNumber();
  
  
  
  VCAOk = CheckReadyVCA();
  
  vca_signaldelay();
  
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind PT5210 D1 5-6 til DUT SDI GENLOCK IN\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind DUT SDI GENLOCK OUT til R&S VCA A-in\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n F4: OK\n F9: STOP\n");

// indstil PT5210 i test rack og genlock i DUT

  WriteCOMPort(PT5210COM,":OUTP:SB56:SYST SDI625; PATT BLACK; "); 
  Delay(0.1);
  WriteCOMPort(PT5210COM,":OUTP:SB34:SYST SDI625; PATT BLACK; "); 
  WriteCOMPort(DUTCOM,":INP:GENL:INP SDI;"); Delay(xd);
  WriteCOMPort(DUTCOM,"SYST SDI625;"); Delay(xd);
  WriteCOMPort(DUTCOM,"DEL 0,0,0;");  Delay(xd);
  vca_selectsystem(L625);

  Delay(0.2);
 
  switch (SDINo)
    {  		    
    case SDITPG2:
      WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
      Delay(0.05);
      WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;");	// SDI TPG2 = 8633
      break;   
    }
  Delay(0.1);

 // Reset SDI TPG  8602/03/32
  WriteCOMPort(DUTCOM, "COMM 'SG',0;");  // 8602/03/32/33: Set G-system and offset.
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'SX$';");   // Store phase G
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'SO',0;");  // Set Genlock Offset
  Delay(0.05);
  MaxPhase = 276480000;		// 625*1728*256
  
 SDIphase = 0;

 Fmt(DUTStr,"%s<COMM 'SG',%i;",SDIphase); // 8602/03/32/33: Set G-system and offset.
 WriteCOMPort(DUTCOM, DUTStr);

 Delay(3);
 bytes_read = vca_signaldelay_readdelay(&sdidelay);
 Delay(0.5);

 done        = FALSE;
 SDITimingOk = FALSE;
 SDIphase    = 0;
 
 do
   {
   bytes_read = vca_signaldelay_readdelay(&sdidelay); // get measurement from VCA
  
   if ((sdidelay > 72e-9) && (sdidelay < 76e-9) && (bytes_read > 0))
     SDITimingOk = TRUE;
   else
     SDITimingOk = FALSE;

   dd = (sdidelay * 27.0e6) - 2;   // -2 for at ramme et delay = 74 ns (2 samples)
  
   if (dd < 0)
     {		// korrekt afrunding
     dd       = dd - 0.5;
     NegDelay = TRUE;
     }
    else
     {
     dd       = dd + 0.5;
     NegDelay = FALSE;
     }
  
   k = (int) dd;
   k = k  * 256;

   if (abs(k) > MaxPhase)
     k = 0;
   if (k > 0)
     SDIphase = MaxPhase - k;
   if (k < 0)
     SDIphase = abs(k);

   Fmt(DUTStr,"%s<COMM 'SG',%i;", SDIphase); // 8602/03/32/33: Set G-system and offset.
   WriteCOMPort(DUTCOM, DUTStr);

   Fmt(TxtStr,"%s< %f[b4p3] µS   (%s)", sdidelay * 1e6, DUTStr);
   SetCtrlVal (sdiPNL, SDI_PNL_DELAY, TxtStr);

   Delay(3);
 
   if (SDITimingOk == FALSE)
     SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX, " *");
  
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   } while ((SDITimingOk == FALSE) && (done == FALSE));

 Delay(3);

// ---------- step delay -2 samples ------------------------------------------ 
 do 
   {
   bytes_read = vca_signaldelay_readdelay(&sdidelay);
  
   if ((sdidelay == 0) && (bytes_read > 0))
   	 SDITimingOk = TRUE;
   	else
   	 SDITimingOk = FALSE;

   Fmt(TxtStr,"%s< %f[b4p3] µS    (%s)", sdidelay * 1e6, DUTStr);
   SetCtrlVal (sdiPNL, SDI_PNL_DELAY, TxtStr);


   if ((sdidelay > 36e-9) && (sdidelay < 38e-9))
     SDIphase = SDIphase - 256;

   if ((sdidelay > 72e-9) && (sdidelay < 76e-9))
     SDIphase = SDIphase - 512;

   if (sdidelay < 0)
     SDIphase = SDIphase + 512;
   
   
   Fmt(DUTStr,"%s<COMM 'SG',%i;",SDIphase); // 8602/03/32/33: Set G-system and offset.
   WriteCOMPort(DUTCOM, DUTStr);
   SetCtrlVal (sdiPNL, SDI_PNL_DELAY, DUTStr);

   Delay(3);

   if (SDITimingOk == FALSE)
     SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
  
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   } while ((SDITimingOk == FALSE) && (done == FALSE));
 
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n");
 
 // gem data i EEPROM
 // At this point phse G is correct inside the TPG. We store it
 // in TPG's novram.
 Delay(0.1);
  if (SDITimingOk == TRUE)
    {
    WriteCOMPort(DUTCOM,"COMM 'SX$';"); // Store phase G
    
    Delay(0.2);
    
    // check at data er gemt korrekt i EEPROM
    FlushInQ(DUTCOM);
    WriteCOMPort(DUTCOM,"COMM 'SX?';"); // Read phase G
    strread = ReadlnCOMPort(DUTCOM, 0.5);

    Scan(strread,"%s>%i[u]", &phase_read);
    if (SDIphase != phase_read)
      SDITimingOk = FALSE;
    } //if
  
 
  if (SDITimingOk == TRUE)
    {
    SDICalphase[0] = phase_read;   // cal data from L625
 
    WriteSDICalDate(SDINo,XX1_Stik);
    }

  // Indstil genlock til intern, således at 'config error' undgås når PT8606 fjernes efter test
  WriteCOMPort(DUTCOM,":INP:GENL:INP INT;");
  Delay(xd);

  return SDITimingOk;
  }

int vca_signaldelay_readdelay(float *delaytotal)
  {
  char *dataread;
  float aa[4];
  float	SD_total;		 // delay in seconds

  FlushInQ(VCACOM);
  WriteCOMPort(VCACOM, "READ:SCAL:SIGD?\r\n");
  dataread = ReadlnCOMPort(VCACOM,2.0);
  Scan(dataread,"%s>%4f[b4x]", aa);
  SD_total    = aa[0];
  *delaytotal = SD_total;
  Fmt(&SD_frames, "%i<%f", aa[1]);
  Fmt(&SD_lines,  "%i<%f", aa[2]);
  Fmt(&SD_samples,"%i<%f", aa[3]);
  
  return(strlen(dataread));   
  }

int CheckReadyVCA(void)
  {
  // Kontrollerer at VCA'en er tændt og kan kommunikere med PC'en.
  // return:  TRUE hvis der er kontakt til VCA'en, ellers FALSE.
  char IDStr[40];
  int  ConfirmReply,
       IDOk;
  
  vca_reset();
  vca_readid();
  CopyString (IDStr, 0, VCA_ID, 0, 17);
  IDOk = TRUE;
  if (CompareStrings(IDStr, 0, "ROHDE&SCHWARZ,VCA", 0, 0) != 0)
    IDOk = FALSE;
  
  if (IDOk == FALSE)
    {
    do
      {
      ConfirmReply = ConfirmPopup (" R&S VCA ", " RS-232  (COM5)\n Ingen forbindelse mellem PC og VCA");
      if (ConfirmReply == NO)
        IDOk = FALSE;
      
      vca_reset();
      vca_readid();
      CopyString (IDStr, 0, VCA_ID, 0, 17);
      } while ((ConfirmReply == YES) & (CompareStrings (IDStr, 0, "ROHDE&SCHWARZ,VCA", 0, 0) != 0));
    } //if
  return IDOk;
  }


void vca_reset(void)
  {
	WriteCOMPort(VCACOM, "*RST\r\n");		 // see page 2.9 - 2.16
	Delay(0.5);
  }


void vca_readid(void)
  {
  char *vcadata;

	FlushInQ(VCACOM);
	WriteCOMPort(VCACOM, "*IDN?\r\n");
	VCA_ID = ReadlnCOMPort(VCACOM,1.0);
  }		


void vca_selectinput(int input)
  {
  switch (input)
    {
    case SERIAL_A:
      WriteCOMPort(VCACOM, "ROUT:OPEN (@SER)\r\n");
      break;
    case SERIAL_B:
      WriteCOMPort(VCACOM, "ROUT:OPEN (@BSER)\r\n");
      break;
    default:
      WriteCOMPort(VCACOM, "ROUT:OPEN (@BSER)\r\n");
      break;
    }
  Delay(0.1);
  }

void vca_selectsystem(int system)
  {
  switch (system)
    {
    case L625:
      WriteCOMPort(VCACOM, "SYST:STAN L625\r\n");
      break;
    case L525:
      WriteCOMPort(VCACOM, "SYST:STAN LOB5\r\n");
      break;
    }
   Delay(0.1); 
  }

void vca_signaldelay(void)
  {
  WriteCOMPort(VCACOM, "SENS:FUNC \"SIGD\"\r\n");
  Delay(0.5);
  }
