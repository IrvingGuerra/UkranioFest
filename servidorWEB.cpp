#define MG_ENABLE_HTTP_STREAMING_MULTIPART 1
#include "mongoose.h"
#include "Request.h"
#include "Reply.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <thread>
#include <time.h>
#include <sys/time.h>

#include <bits/stdc++.h>

#define BUFFERT 512

#define PORT 7778

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

static void sendPercent(struct mg_connection *nc, struct http_message *hm, double percent, int tiempo) {

	char res[20];
	sprintf(res, "%0.2f-%d", percent, tiempo);

	mg_send_head(nc,200,strlen(res), "Content-Type: text/plain");
	mg_printf(nc, "%s", res);

}

static string cleanText(string token){
	string response;
	transform(token.begin(),token.end(),token.begin(),::tolower);

	for (size_t i = 0; i < token.size(); ++i)
	{
		unsigned char c = token[i];
		//cout << int(c) << " ";
		if (('a' <= c && c<= 'z')){
			response+=c;
		}else if (c == 195){
			//Es caracter con acento o dieresis o el de la ñ
			c = token[++i];
			switch(c){
				case 161:
					response+='a';
				break;
				case 169:
					response+='e';
				break;
				case 173:
					response+='i';
				break;
				case 179:
					response+='o';
				break;
				case 186:
					response+='u';
				break;
				case 177:
					response+='n';
				break;
				case 188:
					response+='u';
				break;
			}
			
		}
	}



	return response;
}

size_t len_reply;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
 	struct http_message *hm = (struct http_message *) p;
	if (ev == MG_EV_HTTP_REQUEST) {
		if (mg_vcmp(&hm->uri, "/sendFile") == 0) { 
				
			/*	
			char fileName[100];
			char ips[n_servers][20];

			mg_get_http_var(&hm->body, "fileName", fileName,sizeof(fileName));
			mg_get_http_var(&hm->body, "ip1", ips[0],sizeof(ips[0]));
			mg_get_http_var(&hm->body, "ip2", ips[1],sizeof(ips[1]));
			mg_get_http_var(&hm->body, "ip3", ips[2],sizeof(ips[2]));
			mg_get_http_var(&hm->body, "ip4", ips[3],sizeof(ips[3]));

			string nameFile = string("www/textos/")+fileName;

			//cout << "Se abrira: " << nameFile << endl;
*/
			stringstream ss(hm->body.p);
			string boundary, line, trash, texto, ips_str, ip;
			getline(ss, boundary);
			getline(ss, trash);
			getline(ss, trash);
			getline(ss, trash);
			while(getline(ss, line)){
				if(line == boundary) break;
				texto += line + "\n";
			}
			getline(ss, trash);
			getline(ss, trash);
			getline(ss, ips_str);
			ss = stringstream(ips_str);
			vector<string> ips;
			//cout << texto << "\n";
			//cout << ips_str << "\n";
			while(ss >> ip){
				ips.push_back(ip);
				//cout << ip << "\n";
			}
			const int n_servers = ips.size();
			
		    Request r;

		    chrono::steady_clock::time_point begin = chrono::steady_clock::now();


		    stringstream input(texto);
		    string token;
		    vector<string> libro;
		    while(input >> token){
		    	token = cleanText(token);
		    	if(token.size()) libro.push_back(token);
		    	//cout << token << endl;
		    }
		    vector<string> partes;
		    string parte_actual;
		    for(const string & word : libro){
		    	if(parte_actual.size() + word.size() > BUFFERT){
		    		partes.push_back(parte_actual);
		    		parte_actual = "";
		    	}
		    	parte_actual += word + " ";
		    }
		    if(parte_actual.size()){
		    	partes.push_back(parte_actual);
		    }


		    set<string> disponibles;

		    vector<thread> hilos1, hilos2;

		    for (const string & who : ips)
		    {
		    	hilos1.emplace_back([&](){
			    	cout << "Enviando a: " << who << endl;
			    	try{
						char response =	*r.doOperation(who,PORT,Message::allowedOperations::newbook,NULL,0,len_reply);
						disponibles.insert(who);
			    	}catch(const char *msg){
			    		//
			    	}
			    });
		    }

		    for(int i = 0; i < n_servers; ++i){
		    	hilos1[i].join();
		    }

		
		    cout << "Hay " << disponibles.size() << " disponibles" << endl;


		    

		    if (disponibles.size() != 0)
		    {
		    	vector<string> bad_hosts;
		    	int cnt = 0;
		    	for(const string & who : disponibles)
			    {
			    	hilos2.emplace_back([&](){
				    	for(const string & parte : partes){
				    		try{
				    			char response = *r.doOperation(who,PORT,Message::allowedOperations::book,(char*)parte.c_str(),parte.size()+1,len_reply);
				    		}catch(const char *msg){
					    		bad_hosts.push_back(who);
					    		break;
					    	}		
				    	}
				    });
			    }
			    for(size_t i = 0; i < disponibles.size(); ++i){
			    	hilos2[i].join();
			    }
			    for(const string & who : bad_hosts){
			    	disponibles.erase(who);
			    }
			    again:
			    if(disponibles.size() != 0){

			    	int contador = 0;

				    int tam = libro.size() / disponibles.size();
				    size_t i = 0;
				    auto it = disponibles.begin();
				    int prev_l = -1;
				    for(; it != disponibles.end(); ++it, ++i){
				    	string who = *it;
				    	int left = tam * i;
				    	int right = left + tam - 1;
				    	if(prev_l != -1){
				    		left = prev_l;
				    	}
				    	if(i == disponibles.size()-1){
				    		right += libro.size() % disponibles.size();
				    	}
				    	int indices[2] = {left, right};
				    	cout << "left: " << indices[0] << " right: " << indices[1] << endl;
				    	try{
				    		int response = *(int*)r.doOperation(who,PORT,Message::allowedOperations::count,(char*)indices,sizeof(indices),len_reply);
			    			contador += response;
			    			prev_l = -1;
			    		}catch(const char *msg){
				    		prev_l = left;
				    		bad_hosts.push_back(who);
				    	}
				    }
				    if(prev_l != -1){
			    		for(const string & who : bad_hosts){
			    			disponibles.erase(who);
			    		}
			    		goto again;
			    	}
				    double percent = 100 * ((double)contador / libro.size());

				    chrono::steady_clock::time_point end = chrono::steady_clock::now();

				    int tiempo = chrono::duration_cast<chrono::milliseconds>(end-begin).count();

				    sendPercent(nc, hm, percent, tiempo); 
				}else{
					response(nc, hm, false);
				}
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
