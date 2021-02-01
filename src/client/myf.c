/**
 * @file myf.c
 * @author Mafalda Gonçalves (mafalda.goncalves@ua.pt)
 * @brief Ficheiro com as funções necessárias para o projeto.
 * @version 0.1
 * @date 2020-01-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#define _MY_F_C
#include "myf.h"

/**
 * @brief Função que executa as funções do filho. Tem acesso à shared memory e está responsável pela apresantação da interface gráfica.
 * 
 * @return int 
 */
int ChildMain(int argc,char *argv[])
{
        int n, shm_id;
        Image *dataR;
        int res;
        
        //Identificador da shared memory
        shm_id = GetSharedMem();

        //Em caso de falha
        if(shm_id == -1) return -1; 
        
        //Ponteiro a apontar para o segmento de memória
        dataR = (Image *)shmat(shm_id, (void *) 0, 0);
        
        if(dataR == (Image *) (-1))
        {
                perror("shmat");
                exit(1);
        }
        
        res=GetInterface(argc,argv,dataR);

        //Libertar o segmento de memória
        if( shmdt((void*)dataR) == -1)
        {
                perror("shmdt");
                exit(1);
        }

        if (res==0){
                //kill(getppid(),SIGTERM);
        }

        
        return 0;
}

/**
 * @brief Função que executa as tarefas do pai. Tem acesso à shared memory e responsável pelo estabelecimento da ligação TCP/IP.
 * @return int Retorna o identificador da shared memory de modo a poder ser destruída após a execução do programa.
 */
int ParentMain(int pid)
{
        int n, shm_id;
        int res;
        Image *Imagem=(Image*)malloc(sizeof(Image));

        //Identificador da shared memory.S
        shm_id = GetSharedMem();

        //Em caso de falha.
        if(shm_id == -1) return -1;    
        
        //Ponteiro a apontar para a zona da memória
        Image *dataR = (Image*)shmat(shm_id, (void *) 0, 0);
        
        if(dataR == (Image *) (-1))
        {
                perror("shmat");
                exit(1);
        }
        
        EstablishConnection();
       
        //Estrutura com os dados da imagem recebidos por TCP/IP
        while (1){
               
                Imagem = receive_image(socketG);
                dataR->depth=Imagem->depth;
                dataR->width=Imagem->width;
                dataR->height=Imagem->height;
                dataR->nChannels=Imagem->nChannels;
                dataR->widthStep=Imagem->widthStep;
                dataR->manda=Imagem->manda;   
                memcpy(dataR->imageData,Imagem->imageData,strlen(Imagem->imageData));
                PodeMandar=1;
                
        }
               
        //Liberta o segmento de memória
        if(  shmdt(dataR) == -1 )
        {
                perror("shmdt");
                exit(1);
        }
        
        return shm_id;

}

/**
 * @brief  Conecta-se a uma shared memory.Desenvolvida nas aulas de PARI.
 * @return int Retorna o identificador da shared memory ou, em caso de falha, -1
 */
int GetSharedMem()
{       
        key_t key;                       
        int shm_id;

        //Gera uma chave aleatória com base num nome de um ficheiro e numa letra 
        key = ftok("/", 'X');       
        if(key == -1) { perror("ftok"); return -1; }

        //Conecta-se à shared memory e obtém o identificador da mesma
        shm_id = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);

        if(shm_id == -1) { perror("shmget"); return -1; }

        return shm_id;
}

/**
 * @brief Função que estabelece a ligação TCP/IP com o sistema remoto (servidor).
 * 
 */
int EstablishConnection()
{       
        int ret;
        struct sockaddr_in server;

        char ip[16]="192.168.43.9";     
        //char ip[16]="169.254.202.242";
        //char ip[16]="192.168.1.4";

        int port=60000;                 
        struct hostent *serverByname;

        //Cria um socket para a comunicação
        socketG = socket(AF_INET , SOCK_STREAM , 0);
        if (socketG == -1) printf("Could not create socket");

        server.sin_addr.s_addr = inet_addr(ip);
        server.sin_family      = AF_INET;
        server.sin_port        = htons(port);

        ret=connect(socketG, (struct sockaddr *)&server, sizeof(server));
        if (ret < 0) { puts("connect error"); return 0; }

}

/**
 * @brief Função responsável pela construção e apresentação da interface gráfica
 * 
 * @param argc argumento de entrada do main
 * @param argv argumento de entrada do main
 * @return int 
 */
int GetInterface(int argc,char *argv[],Image*data ){
        
        GtkWidget *t;
        gtk_init(&argc, &argv);

        //Abre a interface do ficheiro escolhido
        builderG = gtk_builder_new();
        gtk_builder_add_from_file(builderG, "proj.glade", NULL);

        //Conecta os sinais à interface
        gtk_builder_connect_signals(builderG, NULL);

        //Define valores default para as entries
        GtkEntry *entryx0 = GTK_ENTRY(gtk_builder_get_object (builderG, "entryx0"));
        GtkEntry *entryy0 = GTK_ENTRY(gtk_builder_get_object (builderG, "entryy0"));
        GtkEntry *entryw = GTK_ENTRY(gtk_builder_get_object (builderG, "entryw"));
        GtkEntry *entryh = GTK_ENTRY(gtk_builder_get_object (builderG, "entryh"));
        gtk_entry_set_text(entryx0,"100");
        gtk_entry_set_text(entryy0,"100"); 
        gtk_entry_set_text(entryw,"300"); 
        gtk_entry_set_text(entryh,"300"); 

        //ID da janela principal e alguns sinais principais
        t= GTK_WIDGET(gtk_builder_get_object(builderG, "window1"));
        if(t)
        {
                g_signal_connect(G_OBJECT(t), "destroy", G_CALLBACK(pari_destroy), NULL);
        }
        
        //Aciona um timeout de modo a chamar a função UpdateDisplay com um intervalo de 30 ms
        g_timeout_add(50, (GSourceFunc) UpdateDisplay, (gpointer) NULL);

        //Gtk Loop
        gtk_main();

        return 0;

}

/**
 * @brief Permite a receção dos dados relativos à imagem enviada pelo servidor. 
 * 
 * @param socketF Socket da comunicação associado à ligação.
 * @return Retorna um ponteiro para uma estrutura de dados com a informação da imagem.
 */
Image* receive_image(int socketF){

        int retR,retRf;
        int dataIm[5];
        memset(dataIm, 0, sizeof(dataIm));
        memset(ImageG.imageData, 0, sizeof(ImageG.imageData));
        
                //Recebe em primeiro lugar as dimensões da imagem 
                retR=recv(socketF,dataIm,sizeof(dataIm), 0);
                
                //Guarda numa variável global as dimensões recebidas
                ImageG.width=dataIm[0];
                ImageG.height=dataIm[1];
                ImageG.nChannels=dataIm[2];
                ImageG.depth=dataIm[3];
                ImageG.widthStep=dataIm[4];

                long int readBytes=0;
        
                if (retR>0){

                        char Im[ImageG.width*ImageG.height*ImageG.nChannels+50];
                        memset(Im, 0, ImageG.width*ImageG.height*ImageG.nChannels);

                        
                        do{     
                                //Após receber as dimensões, recebe os dados da imagem
                                retRf=recv(socketF,Im,ImageG.width*ImageG.height*ImageG.nChannels, 0);

                                for (int i=0;i<retRf;i++){
                                        ImageG.imageData[i+readBytes]=Im[i];
                                }

                                readBytes=readBytes+retRf;
                                memset(Im, 0, ImageG.width*ImageG.height*ImageG.nChannels);
                        }while(readBytes<ImageG.width*ImageG.height*ImageG.nChannels);
                       //printf("Imagem recebida %lu/%d bytes e bytes recebidos %ld \n",strlen(ImageG.imageData),ImageG.width*ImageG.height*ImageG.nChannels,readBytes);
                }
        
        return &ImageG;
}


/**
 * @brief Função responsável para o envio de dados relativos à próxima imagem a receber. 
 * 
 * @return int 
 */
int send_message(){

        int shm_id; 
        shm_id = GetSharedMem();

        //Em caso de falha.
        if(shm_id == -1) return 0;    

        //Ponteiro a apontar para a zona da memória
        Image *dataR = (Image*)shmat(shm_id, (void *) 0, 0);

        int timetoReceive[5];

        //Array com os dados a enviar
        timetoReceive[0]=dataR->manda;
        timetoReceive[1]=dataR->x0;
        timetoReceive[2]=dataR->y0;
        timetoReceive[3]=dataR->w;
        timetoReceive[4]=dataR->h;
  
        send(socketG,timetoReceive,sizeof(timetoReceive), 0);
        printf("Mensagem enviada \n");
        return 0;        
}


/**
 * @brief Copia a IplImage recebida para uma variável global.
 * 
 * @return IplImage *
 */
IplImage* PerformImage(Image* data)
{          
        src_imageG = cvCreateImageHeader(cvSize(data->width,data->height),data->depth,data->nChannels);
        cvSetData(src_imageG, data->imageData, data->widthStep);
        return src_imageG;
}

/**
 * @brief  Função que converte uma IplImage em pixbuf por questões de visualização. Desenvolvida nas aulas de PARI.
 *
 * @param  image IplImage a converter
 * @param  dst_w largura do novo pixbuf
 * @param  dst_h Altura do novo pixbuf
 * @return The GdkPixbuf *
 */
GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage * image, int dst_w, int dst_h){
        IplImage *gtkMask=image;
        GdkPixbuf *pix, *rpix;
        pix = gdk_pixbuf_new_from_data((guchar *) gtkMask->imageData,
                                       GDK_COLORSPACE_RGB,
                                       FALSE,
                                       gtkMask->depth,
                                       gtkMask->width,
                                       gtkMask->height,
                                       gtkMask->widthStep,
                                       NULL, 
                                       NULL  
                                       );

        rpix = gdk_pixbuf_scale_simple(pix, dst_w, dst_h, GDK_INTERP_BILINEAR);
        g_object_unref(pix); 
        return rpix;
}


/**
 * @brief Copia a IplImage to pixbuf. Desenvolvida nas aulas de PARI.
 *
 * @param  widgetName String com o nome do widget no glade.
 * @param  img IPlImage para converter.
 * @return void
 */
void pari_RefreshDrawingArea(char *widgetName, IplImage *img){

        GtkWidget *da=GTK_WIDGET(gtk_builder_get_object (builderG, widgetName));
        if( ! da ) { printf("failed for %s\n", widgetName); return;}

        int wd=gtk_widget_get_allocated_width(da);  
        int ht=gtk_widget_get_allocated_height(da); 

        GdkPixbuf *pix=pari_ConvertOpenCv2Gtk(img, wd, ht);
        cairo_t  *cr = gdk_cairo_create (gtk_widget_get_window(da)); 
        gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
        cairo_paint(cr);
        cairo_fill(cr);
        cairo_destroy(cr);
        g_object_unref(pix);  
}

/**
 * @brief  Callback para destruir a interface. Desenvolvida nas aulas de PARI.
 * @param  window Janela principal.
 * @param  data 
 * @return void
 */
void pari_destroy(GtkWidget * window, gpointer data){
        gtk_main_quit(); 
}

/**
 * @brief  Handler para o timeout. Devido a este, assegura um refresh periódico da drawing area. 
 *
 * @param  data
 * @return boolean Para ativar o timeout.
 */
gboolean UpdateDisplay(gpointer data){
        GtkWidget *da = GTK_WIDGET(gtk_builder_get_object(builderG, "mydrawingareasingle"));
        gtk_widget_queue_draw(da);
        return TRUE; 
}

/**
 * @brief Função responsável pela receção de sinais no processo pai. 
 * 
 * @param signum 
 */
void ParentHandler(int signum){

        Image *Imagem=(Image*)malloc(sizeof(Image));
        int shm_id; 
        shm_id = GetSharedMem();

        //Em caso de falha.
        if(shm_id == -1) return;    

        //Ponteiro a apontar para a zona da memória
        Image *dataR = (Image*)shmat(shm_id, (void *) 0, 0);
        switch(signum){

                //Caso seja este enviado pelo filho, envia uma mensagem ao servidor.
                case SIGUSR1:
                        if (PodeMandar==1)
                                send_message();
                        PodeMandar=0;
                        break;
        }
}
