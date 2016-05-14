/** @file Serial_Port_Linux.c
 * @see Serial_Port.h for description.
 * @author Adrien RICCIARDI
 */
#if defined(__linux__) || defined(__CYGWIN__) // This file will compile on Linux only
#include <errno.h>
#include <fcntl.h>
#include <Serial_Port.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SerialPortOpen(char *String_Device_File_Name, unsigned int Baud_Rate, TSerialPortID *Pointer_Serial_Port_ID)
{
	struct termios Serial_Port_Parameters;
	speed_t Speed;
	int Serial_Port_File_Descriptor = -1;
	
	// Choose the right speed constant
	switch (Baud_Rate)
	{
		case 0:
			Speed = B0;
			break;
			
		case 50:
			Speed = B50;
			break;
			
		case 75:
			Speed = B75;
			break;
			
		case 110:
			Speed = B110;
			break;
			
		case 134:
			Speed = B134;
			break;
			
		case 150:
			Speed = B150;
			break;
			
		case 200:
			Speed = B200;
			break;
			
		case 300:
			Speed = B300;
			break;
		
		case 600:
			Speed = B600;
			break;
		
		case 1200:
			Speed = B1200;
			break;
			
		case 1800:
			Speed = B1800;
			break;
			
		case 2400:
			Speed = B2400;
			break;
			
		case 4800:
			Speed = B4800;
			break;
			
		case 9600:
			Speed = B9600;
			break;
			
		case 19200:
			Speed = B19200;
			break;
			
		case 38400:
			Speed = B38400;
			break;
			
		case 57600:
			Speed = B57600;
			break;
			
		case 115200:
			Speed = B115200;
			break;
			
		case 230400:
			Speed = B230400;
			break;
			
		default:
			errno = EINVAL;
			goto Exit_Error;
	}
	
	// Open the serial port device file
	Serial_Port_File_Descriptor = open(String_Device_File_Name, O_RDWR | O_NONBLOCK);
	if (Serial_Port_File_Descriptor == -1) goto Exit_Error;
	
	// Configure new parameters
	memset(&Serial_Port_Parameters, 0, sizeof(Serial_Port_Parameters));
	Serial_Port_Parameters.c_iflag = IGNBRK | IGNPAR; // Ignore break, no parity
	Serial_Port_Parameters.c_oflag = 0;
	Serial_Port_Parameters.c_cflag = CS8 | CREAD | CLOCAL; // 8 data bits, receiver enabled, ignore modem control lines
	Serial_Port_Parameters.c_lflag = 0; // Use raw mode
	
	// Set speeds
	if (cfsetispeed(&Serial_Port_Parameters, Speed) == -1) goto Exit_Error;
	if (cfsetospeed(&Serial_Port_Parameters, Speed) == -1) goto Exit_Error;
	
	// Set parameters
	if (tcsetattr(Serial_Port_File_Descriptor, TCSANOW, &Serial_Port_Parameters) == -1) goto Exit_Error;
	
	*Pointer_Serial_Port_ID = Serial_Port_File_Descriptor;
	return 0;
	
Exit_Error:
	if (Serial_Port_File_Descriptor != -1) close(Serial_Port_File_Descriptor); // Close the serial port if it was previously opened
	return -1;
}

unsigned char SerialPortReadByte(TSerialPortID Serial_Port_ID)
{
	unsigned char Byte;
	
	while (read(Serial_Port_ID, &Byte, 1) <= 0);
	return Byte;
}

void SerialPortReadBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	unsigned char Byte, *Pointer_Buffer_Byte = Pointer_Buffer;
	
	while (Bytes_Count > 0)
	{
		// Try to get a byte
		if (read(Serial_Port_ID, &Byte, 1) == 1)
		{
			*Pointer_Buffer_Byte = Byte;
			Pointer_Buffer_Byte++;
			Bytes_Count--;
		}
	}
}

void SerialPortWriteByte(TSerialPortID Serial_Port_ID, unsigned char Byte)
{
	write(Serial_Port_ID, &Byte, 1);
}

void SerialPortWriteBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	write(Serial_Port_ID, Pointer_Buffer, Bytes_Count);
}

int SerialPortIsByteAvailable(TSerialPortID Serial_Port_ID, unsigned char *Pointer_Available_Byte)
{
	if (read(Serial_Port_ID, Pointer_Available_Byte, 1) == 1) return 1;
	return 0;
}

void SerialPortClose(TSerialPortID Serial_Port_ID)
{
	close(Serial_Port_ID);
}

#endif
	