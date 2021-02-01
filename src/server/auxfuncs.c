/**
 * @file auxfuncs.c
 * @author Mafalda Gonçalves (mafalda.goncalves@ua.pt)
 * @brief Ficheiro com as funções auxiliares para a execução do servidor TCP/IP.
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "myserver.h"

/**
 * @brief Imprime a mensage de erro associada ao erro em execução e cancela a execução.
 * 
 * @param msg Mensagem a imprimir.
 */
void myerror(const char *msg)
{
	perror(msg);
	exit(1);
}

/**
 * @brief Permite a captura de uma imagem e o envio dos seus dados para o cliente, pelo socket associado.
 * 
 * @param socket Socket associado à comunicação.
 * @param clientID Identificador do cliente para o qual vai enviar a imagem.
 * @return int Retorna 1 em caso de sucesso.
 */
int send_image(int socket,char *clientID,int timetoSend[])
{
	
	int numBytesSent;
	IplImage *frame;
	int x0,y0,w,h;
	CvRect retangulo;
	int dataIm[5];
	memset(dataIm,0,sizeof(dataIm));
	IplImage* cropped;
	CvCapture * image =cvCaptureFromCAM(0);
	frame=cvQueryFrame(image);
	x0=timetoSend[1];
	y0=timetoSend[2];
	w=timetoSend[3];
	h=timetoSend[4];
	retangulo=cvRect(x0,y0,w,h);
	
	//Caso seja requisitado pelo cliente, recorta a imagem capturada com as medidas recebidas. 
	if (timetoSend[1]!=0){
		cropped=cvCreateImage(cvSize(retangulo.width,retangulo.height),frame->depth,frame->nChannels);
		cvSetImageROI(frame,retangulo);
		cvCopy(frame,cropped);
		dataIm[0]=cropped->width;
		dataIm[1]=cropped->height;
		dataIm[2]=cropped->nChannels;
		dataIm[3]=cropped->depth;
		dataIm[4]=cropped->widthStep;
	}else{
		dataIm[0]=frame->width;
		dataIm[1]=frame->height;
		dataIm[2]=frame->nChannels;
		dataIm[3]=frame->depth;
		dataIm[4]=frame->widthStep;
		cropped=cvCreateImage(cvSize(frame->width,frame->height),frame->depth,frame->nChannels);
		cvCopy(frame,cropped);
	}
	
	//As dimensões da imagem são enviadas em primeiro lugar
	numBytesSent = send(socket,dataIm,sizeof(dataIm), 0);
	
	if (numBytesSent>0){
			
			//Após serem recebidas, será enviado um array com os dados da imagem.
			char Im[1000000];
			memset(Im,0,sizeof(Im));
			memcpy(Im,cropped->imageData,strlen(cropped->imageData));
			int retRf=send(socket,Im,dataIm[0]*dataIm[1]*dataIm[2], 0);
			printf("Image sent %u de %d bytes \n", strlen(cropped->imageData), retRf);
			
	}else{

		//Em caso de nada ser enviado
		myerror("Nothing sent.");
	}
	
	cvReleaseCapture(&image);
	cvReleaseImage(&frame);
	cvReleaseImage(&cropped);
	
	return 1;

}

/**
 * @brief Função responsável pela receção das mensagens enviadas pelo cliente. 
 * 
 * @param socket 
 * @param clntName 
 * @return int 
 */
int receive_message(int socket,char* clntName){

        int ret;
        int timetoSend[5];
			
		while(1){
			
			ret=recv(socket,timetoSend,sizeof(timetoSend), 0);

			if (ret>0){
				//Tendo em conta a mensagem enviada pelo cliente, o servidor envia ou não uma nova imagem. 
				if (timetoSend[0]==1){
					send_image(socket,clntName,timetoSend);
					return timetoSend[0];
				}
			}
		}

     
}
