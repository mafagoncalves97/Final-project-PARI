/**
 * @file main.c
 * @author Mafalda Gonçalves (mafalda.goncalves@ua.pt)
 * @brief Programa principal que realiza a divisão em dois processos. Um responsável pelo establecimento da ligação e outro pela interface gráfica.
 * @date 2020-01-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#define _MAIN_C_

#include "myf.h"


int main(int argc, char *argv[])
{	
	//Cria um processo filho
	int pid = fork();
	int s_id;

	if(pid == -1) { printf("Could not fork(). Exiting\n"); return -1; }

	//O filho
	if(pid == 0)            
	{
		ChildMain(argc,argv);
		
	}
	//O pai
	else 
	{
		signal(SIGUSR1,ParentHandler);
		//Retorna o identificador da shared memory para a poder destruir.
		s_id = ParentMain(pid); 
		
		//Fecha o socket antes de terminar
		close(socketG);
		cvReleaseImage(&src_imageG);
		
		//Permite a eliminação da shared memory.
		if(s_id > 0) shmctl(s_id, IPC_RMID, NULL);  
	}
	
	//return 0;
}
