<h2> Introduction </h2>

  - This is intended to be a simple string based server/client using a c++ binding for zmq.
  - Server operates similar to the consumer in the producer consumer patter, polling and adding 
    requests to its queue.
  - Client just sends requests to a server.

<h2> Example </h2>

  - Here we will create a localhost server and client, and have the client send some messages to the server. 

  ```cpp

    //create our localhost client and server
    Server server; 
    Client client; 

    //give the server time to start polling before we send everything
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::vector<std::string> messages {"hello", "there", "server", "how", "are", "you?"};

    //send the server messages
    for (const auto& message : messages) {
      client.send_request(message);
    }

  ```

  - Now we will read all the messages from the server's message queue :

  ```cpp

    while (server.has_message()) {
      std::cout << "message from queue : " << server.pop_queue() << "\n";
    }  

  ```

  - Alternatively, we can directly wait for the server to receive each message : 

  ```cpp
  
    std::cout << " resending messages to server.\n";
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

  ```
 
  

<h2> Dependencies </h2>

  - [cppzmq](https://github.com/zeromq/cppzmq)


<h2> Building </h2>

<ol>
  <li> Download and build Dependencies <
    <ul>
      <li> download cppzmq : https://github.com/zeromq/cppzmq </li>
      <li> follow the build instructions </li>
    </ul>
  </li>
  <br>
  <li> Download, Build and Install simple-zmq-server with CMake 
    <ul>
      <li> git clone https://github.com/tonyradtke/simple-zmq-server.git </li>
      <li> cd simple-zmq-server </li>
      <li> mkdir build </li>
      <li> cd build </li>
      <li> cmake .. </li>
      <li> make install </li>
    </ul>
  </li>
  <br>
  <li>  Build and Run the demo to make sure things are working 
    <ul> 
      <li> cd demo </li>
      <li> mkdir build </li>
      <li> cd build </li>
      <li> cmake . </li>
      <li> make </li>
      <li> ./bin/__demo__ </li>
    </ul>
  </li>
  <br>
  <li> Include and Link against simple-zmq-server and cppzmq in your project </li>
    <ul>
      <li> find_library("simple-zmq-server" STATIC lib) </li>
      <li> find_package(cppzmq) </li>
      <li> target_link_libraries(${YOUR_BINARY} PUBLIC "simple-zmq-server") </li>
      <li> target_link_libraries(${YOUR_BINARY} PUBLIC cppzmq) </li>
      <li> include simplezmqserver.hh in your project </li>
    </ul> 
  </li>
  <br>
</ol>




