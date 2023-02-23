#include "Server.hh"
#include "Client.hh"

int main() {


//create our localhost client and server
Server server(true); //localhost
Client client(true); //localhost

//send req to server
client.send_request("hello");

//wait for the server to pop us the oldest request in teh q
std::string req = server.wait_for_request();
std::cout << " message from queue : " << req  << "\n";

//send the client the response from the server
server.send_response(req + " world");

//wait for the client to pop us the oldeest from q
std::string result = client.wait_for_response();
std::cout << "message from server : " << result << "\n";






}

