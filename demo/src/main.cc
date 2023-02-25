#include <simplezmqserver.hh>

int main() {

  //create our localhost client and server
  const std::string ip {"127.0.0.1"};
  const std::string port {"8000"};
  const std::string topic {"B"};
  
  Server server(ip, port, topic); 
  Client client(ip, port, topic); 

  //give the server time to start polling before we send everything
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::vector<std::string> messages {"hello", "there", "server", "how", "are", "you?"};

  //send the server messages
  for (const auto& message : messages) {
    client.send_request(message);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  //let the messaages fill the queue, then read from there

  while (server.has_message()) {
    std::cout << "message from queue : " << server.pop_queue() << "\n";
  } 
  
  std::cout << " resending messages to server.\n";

  //send the server messages again, this time we poll directly for each message 
  for (const auto& message : messages) {
    client.send_request(message);
  }
  

  //wait for the server to receive each message
  for (int i = 0; i < messages.size(); ++i) {

    std::string req = server.wait_for_request();

    if (req != messages[i]) {
      std::cout << "read in the wrong message, error!\n";
      continue;
    }

    std::cout << "server received message : " << req  << "\n";
  }



}

