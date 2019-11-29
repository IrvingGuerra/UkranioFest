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

static void sendPercent(struct mg_connection *nc, struct http_message *hm, double percent) {

	char response[10];

	stringstream ss;
	ss << fixed << setprecision(2) << percent;

	strcat(response,ss.str().c_str());

	mg_send_head(nc,200,strlen(response), "Content-Type: text/plain");
	mg_printf(nc, "%s", response);

}

static string cleanText(string token){
	string response;
	transform(token.begin(),token.end(),token.begin(),::tolower);
	for (unsigned char c: token)
	{
		if (('a' <= c && c<= 'z')){
			response+=c;
		}
	}
	return response;
}

const int n_servers = 3;
size_t len_reply;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
 	struct http_message *hm = (struct http_message *) p;
	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/sendFile") == 0) { 
				
			char fileName[100];
			char ips[n_servers][20];

			mg_get_http_var(&hm->body, "fileName", fileName,sizeof(fileName));
			mg_get_http_var(&hm->body, "ip1", ips[0],sizeof(ips[0]));
			mg_get_http_var(&hm->body, "ip2", ips[1],sizeof(ips[1]));
			mg_get_http_var(&hm->body, "ip3", ips[2],sizeof(ips[2]));

			string nameFile = string("www/textos/")+fileName;

			cout << "Se abrira: " << nameFile << endl;

		    Request r;

		    ifstream input(nameFile);
		    string token;
		    vector<string> libro;
		    int pos = 0;
		    while(input >> token){
		    	token = cleanText(token);
		    	if(token.size()) libro.push_back(token);
		    }
		    vector<string> partes;
		    string parte_actual;
		    for(const string & word : libro){
		    	if(parte_actual.size() + word.size() > BUFFERT){
		    		partes.push_back(parte_actual);
		    		parte_actual = "";
		    	}
		    	parte_actual += word;
		    }
		    if(parte_actual.size()){
		    	partes.push_back(parte_actual);
		    }


		    vector<int> disponibles;

		    for (int i = 0; i < n_servers; ++i)
		    {
		    	try{
					char response =	*r.doOperation(ips[i],7777,Message::allowedOperations::newbook,NULL,0,len_reply);
					disponibles.push_back(i);
		    	}catch(const char *msg){
		    		//
		    	}	

		    }

		    cout << "Hay " << disponibles.size() << " disponibles" << endl;

		    int tam = libro.size() / disponibles.size();

		    int contador = 0;

		    if (disponibles.size() != 0)
		    {
		    	for(int i:disponibles)
			    {
			    	for(const string & parte : partes){
			    		try{
			    			char response = *r.doOperation(ips[i],7777,Message::allowedOperations::book,(char*)parte.c_str(),parte.size()+1,len_reply);
			    		}catch(const char *msg){
				    		//
				    	}		
			    	}
			    }
			    for(int i = 0; i < disponibles.size(); ++i){
			    	int who = disponibles[i];
			    	int left = tam * i;
			    	int right = left + tam - 1;
			    	if(i == (int)disponibles.size()-1){
			    		right += libro.size() % disponibles.size();
			    	}
			    	int indices[2] = {left, right};

			    	try{
			    		int response = *(int*)r.doOperation(ips[who],7777,Message::allowedOperations::count,(char*)indices,sizeof(indices),len_reply);
		    			contador += response;
		    		}catch(const char *msg){
			    		//
			    	}
			    }
			    double percent = 100 * (1 - (double)contador / libro.size());
			    sendPercent(nc, hm, percent); 
		    }else{
		    	response(nc, hm, false); 
		    }

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
