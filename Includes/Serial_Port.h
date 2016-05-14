/** @file Serial_Port.h
 * Easy-to-use serial port (UART) communication layer.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/02/2013
 * @version 1.1 : 25/02/2013, added Windows support (which does not save and restore previous parameters as UNIX version does).
 * @version 1.2 : 21/03/2015, splitted the supported operating system drivers in several files.
 * @version 1.3 : 14/05/2016, allowed multiple serial ports to be handled by the library.
 */
#ifndef H_SERIAL_PORT_H
#define H_SERIAL_PORT_H

#ifdef WIN32
	#include <windows.h> // Needed for INVALID_HANDLE_VALUE
#endif

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The invalid port ID value. */
#ifdef WIN32
	#define SERIAL_PORT_INVALID_ID INVALID_HANDLE_VALUE
#else
	#define SERIAL_PORT_INVALID_ID -1
#endif

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** A platform-independent serial port identifier. */
#ifdef WIN32
	typedef HANDLE TSerialPortID;
#else
	typedef int TSerialPortID;
#endif

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the specified serial port at the specified baud rate, 8 data bits, no parity, 1 stop bit.
 * @param String_Device_File_Name The name of the serial device, like "/dev/ttyS0" or "COM1".
 * @param Baud_Rate The desired baud rate. The function may fail if this is not a standard value.
 * @param Pointer_Serial_Port_ID On output, contain the opened serial port identifier.
 * @return 0 if the serial port was correctly initialized,
 * @return -1 if an error happened. Use errno to find the error.
 */
int SerialPortOpen(char *String_Device_File_Name, unsigned int Baud_Rate, TSerialPortID *Pointer_Serial_Port_ID);

/** Read a byte from the specified serial port.
 * @param Serial_Port_ID The serial port to read from.
 * @return The read byte.
 * @note This is a blocking function.
 */
unsigned char SerialPortReadByte(TSerialPortID Serial_Port_ID);

/** Read several bytes from the specified serial port.
 * @param Serial_Port_ID The serial port to read from.
 * @param Pointer_Buffer On output, contain the read bytes.
 * @param Bytes_Count How many bytes to read.
 * @note The function will block until all requested bytes are read.
 */
void SerialPortReadBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count);

/** Write a byte to the specified serial port.
 * @param Serial_Port_ID The serial port to write to.
 * @param Byte The byte to send.
 */
void SerialPortWriteByte(TSerialPortID Serial_Port_ID, unsigned char Byte);

/** Write several bytes to the specified serial port.
 * @param Serial_Port_ID The serial port to write to.
 * @param Pointer_Buffer The data to write.
 * @param Bytes_Count How many bytes to write.
 */
void SerialPortWriteBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count);

/** Check whether a byte was received by the serial port.
 * @param Serial_Port_ID The serial port to read from.
 * @param Pointer_Available_Byte Store On output, contain the received byte if there was one available.
 * @return 0 if no byte was received (and Pointer_Available_Byte has unknown value),
 * @return 1 if a byte is available (in this case the byte is stored into Pointer_Available_Byte).
 */
int SerialPortIsByteAvailable(TSerialPortID Serial_Port_ID, unsigned char *Pointer_Available_Byte);

/** Release the specified serial port.
 * @param Serial_Port_ID The serial port to close.
 * @warning The function does not restore the serial port previous parameters.
 */
void SerialPortClose(TSerialPortID Serial_Port_ID);

#endif