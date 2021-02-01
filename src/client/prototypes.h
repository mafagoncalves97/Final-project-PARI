/*File generated automatically. Do not edit*/
/*Generated on mafalda , dom jan 19 15:08:39 WET 2020 */
#ifdef __cplusplus
extern "C" {
#endif
/* src/client/main.c */
int main(int argc, char *argv[]);
/* src/client/myf.c */
int ChildMain(int argc, char *argv[]);
int ParentMain(int pid);
int GetSharedMem(void);
int EstablishConnection(void);
int GetInterface(int argc, char *argv[], Image *data);
Image *receive_image(int socketF);
int send_message(void);
IplImage *PerformImage(Image *data);
GdkPixbuf *pari_ConvertOpenCv2Gtk(IplImage *image, int dst_w, int dst_h);
void pari_RefreshDrawingArea(char *widgetName, IplImage *img);
void pari_destroy(GtkWidget *window, gpointer data);
gboolean UpdateDisplay(gpointer data);
void ParentHandler(int signum);
/* src/client/callbacks.c */
gboolean on_mydrawingareasingle_draw(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void on_button1_clicked(GtkWidget *widget, gpointer user_data);
void on_button2_clicked(GtkWidget *widget, gpointer user_data);
void on_button3_clicked(GtkWidget *widget, gpointer user_data);
void on_button4_clicked(GtkWidget *widget, gpointer user_data);
void on_button5_clicked(GtkWidget *widget, gpointer user_data);
void on_button6_clicked(GtkWidget *widget, gpointer user_data);
void on_button7_clicked(GtkWidget *widget, gpointer user_data);
void on_button8_clicked(GtkWidget *widget, gpointer user_data);
#ifdef __cplusplus
}
#endif
