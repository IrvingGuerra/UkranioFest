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

static string cleanText(string token){
	string response;
	transform(token.begin(),token.end(),token.begin(),::tolower);
	for (unsigned char c: token)
	{
		cout << int(c) << " ";
		if (('a' <= c && c<= 'z')){
			response+=c;
		}else if (c == 225){
			response+='a';
		}else if (c == 233){
			response+='e';
		}else if (c == 237){
			response+='i';
		}else if (c == 243){
			response+='o';
		}else if (c == 250){
			response+='u';
		}else if (c == 241){
			response+='n';
		}
	}
	return response;
}


static void ev_handler(struct mg_connection *nc, int ev, void *p) {
 	struct http_message *hm = (struct http_message *) p;
	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/sendFile") == 0) { 
				
			char fileName[100];
			char ip1[20];
			char ip2[20];
			char ip3[20];

			mg_get_http_var(&hm->body, "fileName", fileName,sizeof(fileName));
			mg_get_http_var(&hm->body, "ip1", ip1,sizeof(ip1));
			mg_get_http_var(&hm->body, "ip2", ip2,sizeof(ip2));
			mg_get_http_var(&hm->body, "ip3", ip3,sizeof(ip3));

			string nameFile = string("www/textos/")+fileName;

			cout << "Se abrira: " << nameFile << endl;

		
		    Request r;

	
		    ifstream input(nameFile);
		    string token;
		    while(input >> token){
		    	token = cleanText(token);
		    	cout << token << endl;
		    }
		    

		response(nc, hm, true); 

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
