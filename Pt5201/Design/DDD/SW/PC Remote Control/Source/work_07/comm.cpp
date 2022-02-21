//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "comm.h"

//---------------------------------------------------------------------------
// comm.cpp
// purpose :  member function code for TCommPort, serial communictaions API
//            encapsulation
// copyright: Harold Howe, bcbdev.com 1996-1999.
// notice   : This file provides an object that encapsulates the win32 serial port routines.
//            This file may be distributed and used freely for program development,
//            but it may not be resold as part of a software development library.
//            In other words, don't take the source and attempt to sell it to other developers.

ECommError::ECommError( ErrorType error)
	: Error( error),
  	Errno( GetLastError())
{
}

//---------------------------------------------------------------------------
// TCommPort::TCommPort() (constructor)
//
// scope:     TCommPort public constructor function.
// purpose :  Initialize serial port settings (baud rate et al) to
//            reasonable values
// args :     none
// returns :  none, its a constructor.
// written :  10/31/96 by H Howe
// remarks :  TCommPort is a stand alone class (not derived from anybody).
//            All the constructor needs to do is initialize the private
//            data members that are being hidden from the user.
// methods :  use auto assignment (that : stuff) to initialize bool open
//            flag and the comm port handle.  Then fill in the DCB with
//            decent initial values in case the comm port is opened before
//            any of the values are set by the user.
//---------------------------------------------------------------------------
TCommPort::TCommPort()
  : m_CommOpen( false),
    m_CommPort( "COM1"),
    m_hCom( 0)
{
	// initialize the comm port to  N82 9600 baud communications.  These values
  // will be used to initialize the port if OpenCommPort is called before any
  // of the SetXXXX functions are called.
  m_dcb.DCBlength = sizeof( DCB);
  m_dcb.BaudRate  = 9600;
  m_dcb.ByteSize  = 8;
  m_dcb.Parity    = NOPARITY;			///NOPARITY and friends are #defined in windows.h
  m_dcb.StopBits  = TWOSTOPBITS;	//TWOSTOPBIT is also from windows.h
}
// end of TCommPort::TCommPort (constructor)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::~TCommPort() (destructor)
//---------------------------------------------------------------------------
TCommPort::~TCommPort()
{
	if( m_CommOpen)
  	CloseCommPort();
}
// end of TCommPort::~TCommPort (destructor)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::OpenCommPort()
//
// scope:     TCommPort public function.
// purpose :  Open the comm port and configure it with the current
//            settings.
// args :     void
// returns :  bool indicating success (true) or failure.
// written :  10/31/96 by H Howe
// remarks :  Opening a comm port in windows involves getting a handle
//            to a port (comm 1 or 2), getting the current port properties,
//            and changing the properties to suit your needs. Each of the
//            steps can fail. This function bundles all of these tasks
//            into one place.
// methods :  use CreateFile as described in the win32 help file. Check
//            for errors.  Then get the DCB properties.  Our private DCB
//            properties are then copied into the 4 major settings.
//---------------------------------------------------------------------------
void TCommPort::OpenCommPort( void)
{
	if( m_CommOpen)   // if already open, don't bother
  	return;

	// we need to get the default settings while preserving the settings
  // that we override. The DCB struct has 20 or so members. We override 4.
  // Make of copy of the settings we care about.
  DCB tempDCB;

  tempDCB.BaudRate = m_dcb.BaudRate;
  tempDCB.ByteSize = m_dcb.ByteSize;
  tempDCB.Parity   = m_dcb.Parity;
  tempDCB.StopBits = m_dcb.StopBits;

  m_hCom = CreateFile(
  			 m_CommPort,
         GENERIC_READ | GENERIC_WRITE,
         0,   					 	// comm devices must be opened w/exclusive-access
         NULL, 					 	//	no security attrs
         OPEN_EXISTING,  	// comm devices must use OPEN_EXISTING
         0,    						// not overlapped I/O
          NULL  					// hTemplate must be NULL for comm devices
        );

	// If CreateFile fails, throw an exception. CreateFile will fail if the
  // port is already open, or if the com port does not exist.
  if( m_hCom == INVALID_HANDLE_VALUE)
  	throw ECommError( ECommError::OPEN_ERROR);

	// Now get the DCB properties of the port we just opened
	if( !GetCommState( m_hCom, &m_dcb))
	{
		// Something is hay wire, close the port and return
	  CloseHandle( m_hCom);
  	throw ECommError( ECommError::GETCOMMSTATE);
  }

  // dcb contains the actual port properties.  Now copy our settings into this dcb
  m_dcb.BaudRate = tempDCB.BaudRate;
  m_dcb.ByteSize = tempDCB.ByteSize;
  m_dcb.Parity   = tempDCB.Parity;
  m_dcb.StopBits = tempDCB.StopBits;

	// Now we can set the properties of the port with our settings.
  if( !SetCommState( m_hCom, &m_dcb))
  {
	  // Something is hay wire, close the port and return
    CloseHandle( m_hCom);
    throw ECommError( ECommError::SETCOMMSTATE);
  }

  // Set the intial size of the transmit and receive queues. These are
  // not exposed to outside clients of the class either. Perhaps they should be?
  // I set the receive buffer to 9k, and the transmit buffer to 32k.
  if( !SetupComm( m_hCom, 1024*9, 1024*32))
  {
		// Something is hay wire, close the port and return
    CloseHandle( m_hCom);
    throw ECommError( ECommError::SETUPCOMM);
  }

  // These values are just default values that I determined empirically.
  // Adjust as necessary. I don't expose these to the outside because
  // most people aren't sure how they work (uhhh, like me included).
  m_TimeOuts.ReadIntervalTimeout         = 15;
  m_TimeOuts.ReadTotalTimeoutMultiplier  = 1;
  m_TimeOuts.ReadTotalTimeoutConstant    = 250;
  m_TimeOuts.WriteTotalTimeoutMultiplier = 1;
  m_TimeOuts.WriteTotalTimeoutConstant   = 250;

  if( !SetCommTimeouts( m_hCom, &m_TimeOuts))
 {
  	// Something is hay wire, close the port and return
    CloseHandle( m_hCom);
    throw ECommError( ECommError::SETCOMMTIMEOUTS);
  }

	// if we made it to here then success
	 m_CommOpen = true;
}
// end of TCommPort::OpenCommPort
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::CloseCommPort()
//
// scope:     TCommPort public function.
// purpose :  Closes the comm port.
// args :     void
// returns :
// written :
// remarks :
// methods :  use API function to close the comm handle.
//---------------------------------------------------------------------------
void TCommPort::CloseCommPort( void)
{
	if( !m_CommOpen)       					// if already closed, return
  	return;

	if( CloseHandle( m_hCom) != 0) 	// CloseHandle is non-zero on success
  {
  	m_CommOpen = false;
	}
  else
  	throw ECommError( ECommError::CLOSE_ERROR);
}
// end of TCommPort::CloseCommPort
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::SetBaudRate()
//
// scope:     TCommPort public function.
// purpose :  set the comm port baud rate
// args :     int of new baud rate
// returns :  bool to indicate success (true) or failure
// written :  10/31/96 by H Howe
// remarks :  If the port is open, SetCommState will be used to try and
//            set the baud rate immediately.  If the comm port is closed
//            the best we can do is save the new baud rate in our private
//            data member.
// methods :  first, make a copy of the old baud rate in case we fail at
//            some point. Then change the baud rate member of dcb.  If the
//            port is open, try to set the new baud rate.
//---------------------------------------------------------------------------
void TCommPort::SetBaudRate( unsigned int newBaud)
{
	unsigned int oldBaudRate = m_dcb.BaudRate;// make a backup of the old baud rate

  m_dcb.BaudRate = newBaud;                	// assign new rate

  if( m_CommOpen)                    				// check for open comm port
  {
  	if( !SetCommState( m_hCom, &m_dcb))   	// try to set the new comm settings
    {                                      	// if failure
    	m_dcb.BaudRate = oldBaudRate;        	// restore old baud rate
      throw ECommError( ECommError::BAD_BAUD_RATE);     // bomb out
    }
	}
}
// end of TCommPort::SetBaudRate()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::SetByteSize()
//
// scope:     TCommPort public function.
// purpose :  set the comm port byte size
// args :     int of new byte size
// returns :  bool to indicate success (true) or failure
// written :  10/31/96 by H Howe
// remarks :  If the port is open, SetCommState will be used to try and
//            set the byte size immediately.  If the comm port is closed
//            the best we can do is save the new byte size in our private
//            data member.
// methods :  first, make a copy of the old byte size in case we fail at
//            some point. Then change the byte size member of dcb.  If the
//            port is open, try to set the new byte size.
//---------------------------------------------------------------------------
void TCommPort::SetByteSize( BYTE newByteSize)
{
	BYTE oldByteSize = m_dcb.ByteSize; 				// make a backup of the old byte size

  m_dcb.ByteSize = newByteSize;             // assign new size

	if( m_CommOpen)                     			// check for open comm port
 	{
  	if( !SetCommState( m_hCom,&m_dcb))     	// try to set the new comm settings
    {                                      	// if failure
    	m_dcb.ByteSize = oldByteSize;         // restore old byte size
      throw ECommError( ECommError::BAD_BYTESIZE);  // bomb out
    }
  }
}
// end of TCommPort::SetByteSize()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::SetParity ()
//
// scope:     TCommPort public function.
// purpose :  set the comm port parity
// args :     int of new parity
// returns :  bool to indicate success (true) or failure
// written :  10/31/96 by H Howe
// remarks :  If the port is open, SetCommState will be used to try and
//            set the parity immediately.  If the comm port is closed the
//            best we can do is save the new parity in our private data
//            member.  Note, the arg to this function should be one of the
//            windows #defines for parity (NOPARITY,ODDPARITY,EVENPARITY)
// methods :  first, make a copy of the old parity in case we fail at some
//            point. Then change the parity member of dcb.  If the port is
//            open, try to set the new parity.
//---------------------------------------------------------------------------
void TCommPort::SetParity( BYTE newParity)
{
	BYTE oldParity = m_dcb.Parity;     				// make a backup of the old parity
  m_dcb.Parity = newParity;                 // assign new parity

 	if( m_CommOpen)                     			// check for open comm port
  {
  	if(!SetCommState( m_hCom, &m_dcb))     	// try to set the new comm settings
    {                                    		// if failure
    	m_dcb.Parity = oldParity;             // restore old parity
      throw ECommError( ECommError::BAD_PARITY);  // bomb out
    }
  }
}
// end of TCommPort::SetParity()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::SetStopBits()
//
// scope:     TCommPort public function.
// purpose :  set the number of stop bits.
// args :     int arg for number of stopbits.
// returns :  bool to indicate success (true) or failure
// written :  10/31/96 by H Howe
// remarks :  If the port is open, SetCommState will be used to try and
//            set the # of stop bits immediately.  Otherwise the best we
//            can do is save the new parity in our private data member.
//            Note, the arg to this function should be one of the windows
//            #defines for stop bits(ONESTOPBIT ONE5STOPBITS TWOSTOPBITS)
// methods :  first, make a copy of the old stopbits # in case we fail.
//            Then change the stop bit member of dcb.  If the port is open
//            open, try to set the new stop bit number.
//---------------------------------------------------------------------------
void TCommPort::SetStopBits(BYTE newStopBits)
{
	BYTE oldStopBits = m_dcb.StopBits; 				// make a backup of old #of stop bits
  m_dcb.StopBits = newStopBits;             // assign new # of stop bits

  if( m_CommOpen)                     			// check for open comm port
  {
	  if(!SetCommState( m_hCom, &m_dcb))     	// try to set the new comm settings
    {                                    		// if failure
  	  m_dcb.StopBits = oldStopBits;         // restore old # of stop bits
      throw ECommError( ECommError::BAD_STOP_BITS);  // bomb out
    }
  }
}
// end of TCommPort::SetParity()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::GetBaudrate()
//
//---------------------------------------------------------------------------
unsigned int TCommPort::GetBaudRate(void)
{
	return( m_dcb.BaudRate);
}
// end of TCommPort::GetBaudRate()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::GetByteSize()
//
//---------------------------------------------------------------------------
BYTE TCommPort::GetByteSize(void)
{
	return( m_dcb.ByteSize);
}
// end of TCommPort::GetByteSize()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::GetParity()
//
// scope:     TCommPort public function.
//---------------------------------------------------------------------------
BYTE TCommPort::GetParity(void)
{
  return( m_dcb.Parity);
}
// end of TCommPort::GetParity()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::GetStopBits()
//
// scope:     TCommPort public function.
//---------------------------------------------------------------------------
BYTE TCommPort::GetStopBits(void)
{
  return( m_dcb.StopBits);
}
// end of TCommPort::GetStopBits()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::WriteBuffer()
//
// scope:     TCommPort public function.
//---------------------------------------------------------------------------
void TCommPort::WriteBuffer( BYTE *buffer, unsigned int ByteCount)
{
	VerifyOpen();

  DWORD dummy;

  if(( ByteCount == 0) || ( buffer == NULL))
	  return;

  if( !WriteFile( m_hCom,buffer, ByteCount, &dummy, NULL))
  	throw ECommError( ECommError::WRITE_ERROR);
}
// end of TCommPort::WriteBuffer()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::WriteBufferSlowly()
//
// scope:     TCommPort public function.
//---------------------------------------------------------------------------
void TCommPort::WriteBufferSlowly( BYTE *buffer, unsigned int ByteCount)
{
	VerifyOpen();

  DWORD dummy;
  BYTE *ptr = buffer;

	for ( unsigned int j=0; j<ByteCount; j++)
  {
  	if( !WriteFile( m_hCom, ptr,1, &dummy, NULL))
    	throw ECommError( ECommError::WRITE_ERROR);

    // Use FlushCommPort to wait until the character has been sent.
    FlushCommPort();
    ++ptr;
	}
}
// end of TCommPort::WriteBufferSlowly()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::WriteString()
//
// scope:     TCommPort public function.
//---------------------------------------------------------------------------
void TCommPort::WriteString( const char *outString)
{
	VerifyOpen();

  DWORD dummy;

  if( !WriteFile( m_hCom, outString, strlen( outString), &dummy, NULL))
	  throw ECommError( ECommError::WRITE_ERROR);
}
// end of TCommPort::WriteString()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::ReadBytes()
//
//---------------------------------------------------------------------------
int TCommPort::ReadBytes( BYTE *buffer, unsigned int MaxBytes)
{
	VerifyOpen();
  DWORD bytes_read;

	if( !ReadFile( m_hCom, buffer, MaxBytes, &bytes_read, NULL))
  	throw ECommError( ECommError::READ_ERROR);

	// add a null terminate if bytes_read < byteCount
  // if the two are equal, there is no space to put a null terminator
  if( bytes_read < MaxBytes)
  	buffer[bytes_read] = '\0';

	return bytes_read;
}
// end of TCommPort::ReadBytes()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::ReadString()
//
//---------------------------------------------------------------------------
int TCommPort::ReadString( char *str, unsigned int MaxBytes)
{
	VerifyOpen();

  if( MaxBytes == 0u)
  	return( 0);

  str[0]='\0';
	if( BytesAvailable() == 0)
	  return( 0);

  BYTE NewChar;
  unsigned int Index=0;

  while(( Index < MaxBytes) && ( NewChar != 0x0A))
  {
		NewChar = GetByte();

    // if the byte is a \r or \n, don't add it to the string
    if( (NewChar != '\r') && (NewChar != '\n'))
    {
    	str[Index] = (char) NewChar;
      Index++;
    }

    // when /r is received, we are done reading the string, so return
    // don't forget to terminate the string with a \0.
    if( NewChar == '\r')
    {
	    str[Index] = '\0';
      return( Index + 1);
    }
  }

  // if the while loop false through, then MaxBytes were received without
  // receiveing a \n. Add null terminator to the string and return the number
  str[MaxBytes-1]='\0';

  return( MaxBytes);
}
// end of TCommPort::ReadString()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::DiscardBytes()
//
//---------------------------------------------------------------------------
void TCommPort::DiscardBytes(unsigned int MaxBytes)
{
	VerifyOpen();

	if ( MaxBytes == 0)
	  return;

  BYTE *dummy = new BYTE[MaxBytes];
  ReadBytes( dummy, MaxBytes);

  delete []dummy;
}
// end of TCommPort::DiscardBytes()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::PurgeCommPort()
//
//---------------------------------------------------------------------------
void TCommPort::PurgeCommPort(void)
{
	VerifyOpen();

//  if( !PurgeComm( m_hCom, PURGE_RXCLEAR))
  if( !PurgeComm( m_hCom, PURGE_TXCLEAR))
	  throw ECommError( ECommError::PURGECOMM);
}
// end of TCommPort::PurgeCommPort()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::FlushCommPort()
//
//---------------------------------------------------------------------------
void TCommPort::FlushCommPort(void)
{
	VerifyOpen();

  if( !FlushFileBuffers( m_hCom))
	  throw ECommError( ECommError::FLUSHFILEBUFFERS);
}
// end of TCommPort::FlushCommPort()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::PutByte()
//
//---------------------------------------------------------------------------
void TCommPort::PutByte(BYTE value)
{
	VerifyOpen();

  DWORD dummy;

  if( !WriteFile( m_hCom, &value, 1, &dummy,NULL))
	  throw ECommError( ECommError::WRITE_ERROR);
}
// end of TCommPort::PutByte()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::GetByte()
//
//---------------------------------------------------------------------------
BYTE TCommPort::GetByte()
{
	VerifyOpen();

  DWORD dummy;
  BYTE  value;

  if( !ReadFile( m_hCom, &value, 1, &dummy, NULL))
	  throw ECommError( ECommError::READ_ERROR);

	return value;
}
// end of TCommPort::GetByte()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::BytesAvailable()
//
//---------------------------------------------------------------------------
unsigned int TCommPort::BytesAvailable(void)
{
	VerifyOpen();

  COMSTAT comstat;
  DWORD dummy;

  if( !ClearCommError( m_hCom, &dummy, &comstat))
	  throw ECommError(ECommError::CLEARCOMMERROR);

  return( comstat.cbInQue);
}
// end of TCommPort::BytesAvailable()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::SetCommPort()
//
//---------------------------------------------------------------------------
void TCommPort::SetCommPort( char *port)
{
	VerifyClosed();   	// can't change comm port once comm is open
                      // could close and reopen, but don't want to

  m_CommPort = port;
}
// end of TCommPort::SetCommPort()
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TCommPort::GetCommPort()
//
//---------------------------------------------------------------------------
char *TCommPort::GetCommPort( void)
{
	return( m_CommPort);
}
// end of TCommPort::GetCommPort()
//---------------------------------------------------------------------------


