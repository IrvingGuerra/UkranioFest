#define MG_ENABLE_HTTP_STREAMING_MULTIPART 1
#include "mongoose.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

#define BUFFERT 512

using namespace std;

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;


static void response(struct mg_connection *nc, struct http_message *hm, bool status) {

	char response[5];

	if (status == true)
	{
		strcpy (response, "SUCCESS");
	}else{
		strcpy (response, "FAILED");
	}

	mg_send_head(nc,200,strlen(response), "Content-Type: text/plain");
	mg_printf(nc, "%s", response);

}


static void ev_handler(struct mg_connection *nc, int ev, void *p) {
 	struct http_message *hm = (struct http_message *) p;
	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/sendFile") == 0) { 
				
			char fileName[20];

			mg_get_http_var(&hm->body, "fileName", fileName,sizeof(fileName));

			printf("Se enviara file: %s\n", fileName);
			
			int fd;
		    char buf[BUFFERT];
		    off_t count = 0, m; //long
		    bzero(&buf,BUFFERT);
		    long int n;

		    if ((fd = open(fileName,O_RDONLY))==-1){
		        perror("Error al abrir la imagen");
		        return EXIT_FAILURE;
		    }else{
		    	//El archivo existe
		    	for (int i = 0; i < 3; ++i){
		    		//Se envia a 3 PC
		    		n = read(fd,buf,BUFFERT);
			    	char temp_buf [1];
			    	while(n){
				        if(n==-1){
				            perror("Error al leer la transmicion");
				            return EXIT_FAILURE;
				        }


				        /*
				        m = sendto(s,buf,n,0,(struct sockaddr*)&direccionForanea,longitudForanea);

				        if( m == -1 ){
				            perror("Error al enviar el archivo");
				            return EXIT_FAILURE;
				        }

				        //Verificamos que el paquete le llego

				        recvfrom(s,&temp_buf,1,0,(struct sockaddr *)&direccionForanea,&longitudForanea);

				        if (temp_buf[0] == 't'){
				            //Si se envio el paquete correctamente, se envia el sig pedazo
				            count += m;
				            bzero(buf,BUFFERT);
				            n = read(fd,buf,BUFFERT);
				        }else{
				            std::cout << "[ FAIL ] " << std::tab  << "Reenviando pedazo: " << count << std::endl;
				        }
				        */
				    }
				    //Desbloqueamos el server enviando un 0
		    	}
		    }
		    //response(nc, hm, true);  

		}else{
			mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
		}
	}

}

int main(void) {
	struct mg_mgr mgr;
	struct mg_connection *nc;
	mg_mgr_init(&mgr, NULL);

	printf("Starting web server on port %s\n", s_http_port);
	nc = mg_bind(&mgr, s_http_port, ev_handler);
	if (nc == NULL) {
		printf("Failed to create listener\n");
		return 1;
	}
	// Set up HTTP server parameters
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = "www"; // Serve current directory
	s_http_server_opts.enable_directory_listing = "yes";
	for (;;) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);
	return 0;
}
