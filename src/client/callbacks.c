/**
 * @file callbacks.c
 * @author Mafalda Gonçalves (mafalda.goncalves@ua.pt)
 * @brief Ficheiro com as callbacks associadas às ações na interface gráfica.
 * @version 0.1
 * @date 2020-01-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "myf.h"
/**
 * @brief  Callback a executar com o evento draw da drawing area.
 *
 * @param  widget Identificador do widget.
 * @param  event Evento associado.
 * @param  user_data 
 * @return TRUE 
 */
gboolean on_mydrawingareasingle_draw(GtkWidget *widget, GdkEvent *event, gpointer user_data){

        int shm_id; 
        shm_id = GetSharedMem();
        //Em caso de falha.
        if(shm_id == -1) return -1;    

        //Ponteiro a apontar para a zona da memória
        Image *dataR = (Image*)shmat(shm_id, (void *) 0, 0);
        PerformImage(dataR);

        //A cada atualização da drawing area, envia um sinal ao pai e atualiza-a 
        kill(getppid(),SIGUSR1);
        pari_RefreshDrawingArea((char*)"mydrawingareasingle", src_imageG);

        //Caso a variável global associada à pre-visualização esteja ativa, desenha o retângulo associado a esta. 
        if (rectG==1){
                        GtkEntry *entryx0 = GTK_ENTRY(gtk_builder_get_object (builderG, "entryx0"));
                GtkEntry *entryy0 = GTK_ENTRY(gtk_builder_get_object (builderG, "entryy0"));
                GtkEntry *entryw = GTK_ENTRY(gtk_builder_get_object (builderG, "entryw"));
                GtkEntry *entryh = GTK_ENTRY(gtk_builder_get_object (builderG, "entryh"));

                const char* tsx = gtk_entry_get_text(entryx0);
                const char* tsy = gtk_entry_get_text(entryy0); 
                const char* tsw = gtk_entry_get_text(entryw); 
                const char* tsh = gtk_entry_get_text(entryh);  

                int valores[5];
                valores[0]=atoi(tsx);valores[1]=atoi(tsy);valores[2]=atoi(tsw);valores[3]=atoi(tsh);
                cvRectangle(src_imageG,cvPoint(valores[0],valores[1]),cvPoint(valores[0]+valores[2],valores[1]+valores[3]),cvScalar(0,0,0),1,8,0);
        }

        return TRUE;
}


/**
 * @brief  Permite abrir a janela do sobre (window 2) quando se clica no botão 1 (Sobre).
 * @param  w Identificador do widget.
 * @param user_data 
 */
void on_button1_clicked(GtkWidget * widget, gpointer user_data){

        GtkWidget *w2 = GTK_WIDGET(gtk_builder_get_object(builderG,"window2"));
        gtk_widget_show(w2);
  
}

/**
 * @brief  Permite abrir a janela propriedades (window 3) quando se clica no botão 2 (Propriedades).
 *
 * @param  w  Identificador do widget.
 * @param user_data 
 * @return void
 */
void on_button2_clicked(GtkWidget * widget, gpointer user_data)
{	
        GtkWidget *w6 = GTK_WIDGET(gtk_builder_get_object(builderG,"window3"));
        gtk_widget_show (w6);
          
}

/**
 * @brief Permite sair da aplicação ao clicar no botão 3 (Sair da Janela principal).
 * @param  w  Identificador do widget.
 * @param user_data 
 * @return void
 */
void on_button3_clicked(GtkWidget * widget, gpointer user_data)
{	
        gtk_widget_destroy (widget);
        gtk_main_quit();
}

/**
 * @brief  Permite fechar a window 2 (Sobre) quando se clica no botão 4 (Sair).
 * @param  w  Identificador do widget.
 * @param user_data 
 * @return void
 */
void on_button4_clicked(GtkWidget * widget, gpointer user_data){	

        GtkWidget *w3 = GTK_WIDGET(gtk_builder_get_object(builderG,"window2"));
        gtk_widget_hide (w3);
}


/**
 * @brief  Permite fechar a window 3 (Propriedades) quando se clica no botão 5 (Sair)
 *
 * @param  w  Identificador do widget.
 * @param user_data 
 * @return void
 */
void on_button5_clicked(GtkWidget * widget, gpointer user_data)
{	
        GtkWidget *w7 = GTK_WIDGET(gtk_builder_get_object(builderG,"window3"));
        gtk_widget_hide (w7);
          
}

/**
 * @brief  Função responsável por aceitar os dados introduzidos pelo utilizador e introduzi-los na shared memory quando se clica no botão 6 (Aplicar).
 *
 * @param  w  Identificador do widget.
 * @param user_data 
 * @return void
 */
void on_button6_clicked(GtkWidget * widget, gpointer user_data)
{	
        rectG=0;
        GtkEntry *entryy0 = GTK_ENTRY(gtk_builder_get_object (builderG, "entryy0"));
        GtkEntry *entryw = GTK_ENTRY(gtk_builder_get_object (builderG, "entryw"));
        GtkEntry *entryh = GTK_ENTRY(gtk_builder_get_object (builderG, "entryh"));
        const char* tsx = gtk_entry_get_text(GTK_ENTRY(user_data));
        const char* tsy = gtk_entry_get_text(entryy0); 
        const char* tsw = gtk_entry_get_text(entryw); 
        const char* tsh = gtk_entry_get_text(entryh);  

        int valores[5];
        valores[0]=atoi(tsx);valores[1]=atoi(tsy);valores[2]=atoi(tsw);valores[3]=atoi(tsh);

        if ((valores[0]+valores[2]>640)||(valores[1]+valores[3]>480)){
                ultrapassaG=1;
                GtkWidget *w8 = GTK_WIDGET(gtk_builder_get_object(builderG,"window4"));

                GtkWindow* w1= GTK_WINDOW(gtk_builder_get_object(builderG,"window1")); 
                GtkWindow* w4= GTK_WINDOW(gtk_builder_get_object(builderG,"window4")); 
                gtk_window_set_transient_for (w4,w1);
                gtk_widget_show (w8);
                
        }else{

                int shm_id; 
                shm_id = GetSharedMem();

                //Em caso de falha.
                if(shm_id == -1) return;    

                //Ponteiro a apontar para a zona da memória
                Image *dataR = (Image*)shmat(shm_id, (void *) 0, 0);

                //Introduz na shared memory os dados introduzidos pelo utilziador. 
                dataR->x0=valores[0];
                dataR->y0=valores[1];
                dataR->w=valores[2];
                dataR->h=valores[3];
                dataR->manda=1;
                
        }
}

/**
 * @brief  Função responsável pela alteração da imagem para a imagem original quando se clica no botão 7 (Imagem Original).
 *
 * @param  w  Identificador do widget.
 * @param user_data 
 * @return void
 */
void on_button7_clicked(GtkWidget * widget, gpointer user_data)
{	

        int shm_id; 
        shm_id = GetSharedMem();

        //Em caso de falha.
        if(shm_id == -1) return;    

        //Ponteiro a apontar para a zona da memória
        Image *dataR = (Image*)shmat(shm_id, (void *) 0, 0);

        //Guarda na shared memory os valores default
        dataR->x0=0;
        dataR->y0=0;
        dataR->w=0;
        dataR->h=0;
        dataR->manda=1;
        rectG=0;

}

/**
 * @brief  Função responsável pela pré-visualização dos dados introduzidos para o recorte da imagem.
 *
 * @param  w  Identificador do widget.
 * @param user_data 
 * @return void
 */
void on_button8_clicked(GtkWidget * widget, gpointer user_data)
{	

        int shm_id; 
        shm_id = GetSharedMem();

        //Em caso de falha.
        if(shm_id == -1) return;    

        //Ponteiro a apontar para a zona da memória
        Image *dataR = (Image*)shmat(shm_id, (void *) 0, 0);

        //Caso os dados introduzidos sejam válidos, altera uma variável global para a visualização da área a rcortar. 
        if ((dataR->x0+dataR->width<=640)&&(dataR->y0+dataR->height<=480)){
                rectG=1;
        }


}
