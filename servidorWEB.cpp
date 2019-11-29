#define MG_ENABLE_HTTP_STREAMING_MULTIPART 1
#include "mongoose.h"
#include "Request.h"
#include "Reply.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

#include <bits/stdc++.h>

#define BUFFERT 512

using namespace std;

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;


static void response(struct mg_connection *nc, struct http_message *hm, bool status) {

	char response[10];

	if (status == true)
	{
		strcpy (response, "SUCCESS");
	}else{
		strcpy (response, "FAILED");
	}

	mg_send_head(nc,200,strlen(response), "Content-Type: text/plain");
	mg_printf(nc, "%s", response);

}

static wstring cleanText(wstring token){
	wstring response;
	transform(token.begin(),token.end(),token.begin(),::tolower);
	for (wchar_t c: token)
	{
		if (('a' <= c && c<= 'z')||('é' <= c && c<= 'ñ'))
		{
			response+=c;
		}
	}
	return response;
}


static void ev_handler(struct mg_connection *nc, int ev, void *p) {
 	struct http_message *hm = (struct http_message *) p;
	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/sendFile") == 0) { 
				
			char fileName[20];
			char ip1[20];
			char ip2[20];
			char ip3[20];

			mg_get_http_var(&hm->body, "fileName", fileName,sizeof(fileName));
			mg_get_http_var(&hm->body, "ip1", ip1,sizeof(ip1));
			mg_get_http_var(&hm->body, "ip2", ip2,sizeof(ip2));
			mg_get_http_var(&hm->body, "ip3", ip3,sizeof(ip3));

			printf("Se enviara file: %s\n", fileName);
			
			/*
			int fd;
		    char buf[BUFFERT];
		    off_t count = 0, m; //long
		    bzero(&buf,BUFFERT);
		    long int n;

*/
		    Request r;

		    wifstream input(string("www/textos")+fileName);
		    wstring token;

		    while(input >> token){
		    	token = cleanText(token);
		    	wcout << token << endl;
		    }


/*

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
				        r.doOperation(ip, "8888",
                                   Message::allowedOperations::registerVote,
                                   (char *)reg, sizeof(registro), len_reply);

				    }
				    //Desbloqueamos el server enviando un 0
		    	}
		    }
		    //response(nc, hm, true);  

		}else{
			mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
		}

		*/

		    response(nc, hm, true); 
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
