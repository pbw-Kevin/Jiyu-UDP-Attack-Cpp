#include "ISocket.h"

#ifdef _WIN32
#include <winsock2.h>

#ifdef _MSC_VER
#pragma comment(lib,"ws2_32.lib")
#endif // _MSC_VER

#endif // _WIN32

#ifdef __linux__
#include <sys/socket.h>
#endif // __linux__
