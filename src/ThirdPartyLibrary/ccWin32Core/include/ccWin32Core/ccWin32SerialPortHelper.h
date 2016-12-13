#pragma once

#include <string>
#include <mutex>
#include <list>
#include <thread>

#include "afxtempl.h"
#include "afxmt.h"

//enum HS_SERIAL_HELPER_ERROR {
//	HS_SH_ERROR_PORT   = 0xFFFFFFFF,
//	HS_SH_ERROR_WRITE  = 0xFFFFFFFE,  
//	HS_SH_ERROR_READ   = 0xFFFFFFFC
//};
//
//class ccWin32SerialPortHelper;
//
//class IHSWin32SerialCallback
//{
//public:
//	IHSWin32SerialCallback() {}
//	virtual ~IHSWin32SerialCallback() {}
//
//public:
//	virtual void    OnHSPSerialPortReceive(ccWin32SerialPortHelper* pSH, DWORD dwLength) = 0;
//	virtual void    OnHSPSerialPortOccurError(ccWin32SerialPortHelper* pSH, DWORD dwErrorCode) = 0;
//};

namespace Luna {

class ccWin32SerialPortHelper
{
public:
    ccWin32SerialPortHelper();
    virtual ~ccWin32SerialPortHelper();

public:
    //	�⺻ ���� �ڷ��
    // Flow control flags
    enum FLOW_CONTROL_FLAGS {
        FC_DTRDSR   = 0x01,
        FC_RTSCTS   = 0x02,
        FC_XONXOFF  = 0x04,
    };

    enum ASCII_CODES {
        ASCII_BEL   = 0x07,
        ASCII_BS    = 0x08,
        ASCII_LF    = 0x0A,
        ASCII_CR    = 0x0D,
        ASCII_XON   = 0x11,
        ASCII_XOFF  = 0x13,    
    };

    // Port
    enum	COM_PORT	{COM1 = 1, COM2, COM3, COM4, COM5, COM6, COM7};
    enum	PARITY_BIT	{PB_NONE, PB_OLD, PB_EVEN, PB_MARK, PB_SPACE};
    enum	BYTE_SIZE	{BS_SIZE4 = 4, BS_SIZE5, BS_SIZE6, BS_SIZE7, BS_SIZE8};
    enum	STOP_BITS	{SB_1_BITS, SB_1_5_BITS, SB_2_BITS};

    // Operations
public:
    BOOL 	isOpenPort()	{return _bOpenPort;};

    BOOL 	open(
        UINT		uComPort, 
        DWORD		dwRate, 
        PARITY_BIT	ParityBit = PB_NONE, 
        BYTE_SIZE	ByteSize = BS_SIZE8, 
        STOP_BITS	StopBits = SB_1_BITS);

    BOOL 	checkValidPort(
        UINT        uComPort, 
        DWORD       dwRate,
        PARITY_BIT	ParityBit = PB_NONE, 
        BYTE_SIZE	ByteSize = BS_SIZE8, 
        STOP_BITS	StopBits = SB_1_BITS);

    void	close();

    const std::string	&getPortName();

    BOOL 	putc(BYTE ch);
    BOOL 	puts(const char *pData);

    BOOL 	write(const void*, DWORD dwWriteSize);
    BOOL 	write(const void *lpBuf, DWORD dwCount, DWORD *pBytesWritten);
    BOOL 	writeString(std::string& str, DWORD *pBytesWritten = NULL);

    // �ڷ� �б�
    BOOL 	isEmpty();
    CHAR	getChar();
    DWORD	read(void*, DWORD dwReadSize);

    void	resetBuffer();

    void	setReceiveEvent(BOOL  bEvent = true);

    //	Frame������ ���� ��쿡 ó���Ѵ�.
    BOOL 	startPollThread();
    BOOL 	stopPollThread();

protected:
    virtual void    doSetupHardwareFlowControl(DCB& stDCB);

private:
    BOOL 	Open(
        UINT		uComPort, 
        DWORD		dwRate, 
        PARITY_BIT	ParityBit, 
        BYTE_SIZE	ByteSize, 
        STOP_BITS	StopBits);

    BOOL        settingPort();
    BOOL 		createRWEvent();

protected:
    BOOL                    _bReceiveEvent;
    std::list<char>	        _sReceiveData;

protected:
    virtual void            readFromDevice();

private:
    std::thread polling_thread_;
    bool        is_stop_thread_;

    BOOL 	    _bOpenPort;
    BOOL 	    _bActiveThread;

    HANDLE		_hFile;
    HANDLE		_hReadEvent;
    OVERLAPPED	_osWrite, _osRead;

    CWinThread* _pWatchThread;
    CEvent		_evWatchThreadKillEvent;

    // ��� ����
    std::string _strPort;
    DWORD		_dwBaudRate;
    PARITY_BIT	_ParityBit;
    BYTE_SIZE	_ByteSize;
    STOP_BITS	_StopBits;

    //	�б� ����
    UINT		_uAllocBufferSize;
    char*       _pReadingBuf;

    std::mutex  _mtx;
    std::mutex  _mtx_buffer;
};

}
