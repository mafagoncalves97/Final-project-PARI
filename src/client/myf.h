/**
 * @file myf.h
 * @author Mafalda Gonçalves (mafalda.goncalves@ua.pt)
 * @brief Header file do programa com as definições transversais ao cliente TCP/IP.
 * @version 0.1
 * @date 2020-01-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   
#include <sys/time.h>  
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <opencv2/core/core_c.h>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/highgui/highgui_c.h>

//Tamanho da shared memory
#define SHM_SIZE 2000000 

/**
 * @brief Estrutura de dados para armazenar as dimensões das imagens recebidas, assim como os dados delas.
 * 
 */
typedef struct { int manda=1; int x0=0; int y0=0; int w=0; int h=0; int width; int height; int depth; int nChannels; int widthStep; char imageData[1500000];} Image;


#ifdef _MAIN_C_
		GtkBuilder * builderG;
		IplImage* src_imageG;
		Image ImageG;
		int socketG;
		int PodeMandar=1;
		int rectG=0;
		int ultrapassaG=0;
#else
		extern GtkBuilder * builderG;
		extern IplImage* src_imageG;
		extern Image ImageG;
		extern int socketG;
		extern int PodeMandar;
		extern int rectG;
		extern int ultrapassaG;
#endif


#include "prototypes.h"
