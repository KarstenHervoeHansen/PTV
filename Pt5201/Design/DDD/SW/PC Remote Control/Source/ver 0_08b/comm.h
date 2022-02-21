//---------------------------------------------------------------------------
#ifndef commH
#define commH

// These classes are a modification of the copyrighted work of Harold Howe,
//  bcbdev.com 1996-1999. In his modules he do state a notice about his work.
//  This notice is shown below:
//
//  Notice: This file provides an object that encapsulates the win32 serial port
//           routines. This file may be distributed and used freely for program
//           development, but it may not be resold as part of a software
//           development library. In other words, don't take the source and
//           attempt to sell it to other developers.

class ECommError
{
public:
	enum ErrorType
  {
  	BAD_SERIAL_PORT    ,
    BAD_BAUD_RATE      ,
    BAD_PORT_NUMBER    ,
    BAD_STOP_BITS      ,
    BAD_PARITY         ,
    BAD_BYTESIZE       ,
    PORT_ALREADY_OPEN  ,
    PORT_NOT_OPEN      ,
    OPEN_ERROR         ,
    WRITE_ERROR        ,
    READ_ERROR         ,
    CLOSE_ERROR        ,
    PURGECOMM          ,
    FLUSHFILEBUFFERS   ,
    GETCOMMSTATE       ,
    SETCOMMSTATE       ,
    SETUPCOMM          ,
    SETCOMMTIMEOUTS    ,
    CLEARCOMMERROR
  };

	ECommError( ErrorType error);

  ErrorType Error;
  DWORD     Errno;   // Errno == return value from GetLastError. Can be used with FormatMessage
};

class TCommPort
{
	public:
  	TCommPort();
  	~TCommPort();
    void OpenCommPort( void);
    void CloseCommPort( void);

    void SetCommPort( char *port);
    char *GetCommPort( void);

    void SetBaudRate( unsigned int newBaud);
    unsigned int GetBaudRate( void);

    void SetParity( BYTE newParity); // see source for possible values
    BYTE GetParity( void);

    void SetByteSize( BYTE newByteSize);
    BYTE GetByteSize( void);

    void SetStopBits( BYTE newStopBits);
    BYTE GetStopBits( void);

    void WriteString( const char *outString);
    void WriteBuffer( BYTE  *buffer, unsigned int ByteCount);
    void WriteBufferSlowly( BYTE *buffer, unsigned int ByteCount);

    int ReadString( char *string, unsigned int MaxBytes);
    int ReadBytes( BYTE *bytes, unsigned int byteCount);

    void DiscardBytes( unsigned int MaxBytes);

    void PurgeCommPort( void);
    void FlushCommPort( void);

    void  PutByte( BYTE value);
    BYTE  GetByte();

    unsigned int BytesAvailable( void);

    bool Connected()
    {
        return m_CommOpen;
    }

  private:
    // Note: the destructor of the commport class automatically closes the
    //       port. This makes copy construction and assignment impossible.
    //       That is why I privatize them, and don't define them. In order
    //       to make copy construction and assignment feasible, we would need
    //       to employ a reference counting scheme.
    TCommPort( const TCommPort &);            	// privatize copy construction
    TCommPort & operator = ( const TCommPort&); // and assignment.

    void VerifyOpen()
    {
			if( !m_CommOpen)
      	throw ECommError( ECommError::PORT_NOT_OPEN) ;
    }

    void VerifyClosed()
    {
    	if( m_CommOpen)
				throw ECommError( ECommError::PORT_ALREADY_OPEN) ;
    }

	  // this stuff is private because we want to hide these details from clients
    bool          m_CommOpen;
    COMMTIMEOUTS  m_TimeOuts;
    char*					m_CommPort;
    DCB           m_dcb;        // a DCB is a windows structure used for
    														//  configuring the port
    HANDLE        m_hCom;       // handle to the comm port.
};

//---------------------------------------------------------------------------
#endif
