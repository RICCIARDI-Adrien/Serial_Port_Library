# Serial Port Library
A Linux/Windows C library providing easy access to PC serial ports.

## Description
This library implementation is really simple to be easily portable between various operating systems.  
Thus, blocking functions use busy loops to avoid using threads.  
Be careful, some functions are tailored for usage simplicity and don't signal eventual error.