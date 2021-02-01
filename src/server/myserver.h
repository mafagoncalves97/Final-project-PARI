/**
 * @file myserver.h
 * @author Mafalda Gonçalves (mafalda.goncalves@ua.pt)
 * @brief Header file com as definições relativas ao servidor TCP/IP.
 * @version 0.1
 * @date 2020-01-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <opencv2/core/core_c.h>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/highgui/highgui_c.h>


#ifdef _MAIN_C_
	int infinite_loopG;
#else
	extern int infinite_loopG;
#endif

#include "prototypes.h"

