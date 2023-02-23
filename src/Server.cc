#include "Server.hh"

#define kSLEEP_MS 1 

Server::Server(bool f_localhost) {
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::pair);
  if (f_localhost) {
    m_sock.bind("tcp://127.0.0.1:8000");
    std::cout << "created a localhost server\n";
  }
  else {
    m_sock.bind("tcp://*:8000");
    std::cout << "created a server\n";
  }

  start_polling_thread();
  m_killed = false;

};


std::string Server::wait_for_request() {
  std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
  while (true) {
    if (not has_message()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
      continue;
    }
    return pop_queue(); 
  }
}


void Server::start_polling_thread() {
  m_threadPool.push_back(std::thread([&](){ poll_request(); }));
}

void Server::poll_request() {
  while (true) {

    if (m_killed) {
      std::cout << "server polling thread has been killed\n";
      return;
    }
  
    zmq::message_t message;

    auto ret = m_sock.recv(message, zmq::recv_flags::dontwait);

    if (not ret) {
      std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); //dont shread the cpu polling :)
      continue;
    }

    std::string msg_str = message_to_string(message);

    push_to_queue(msg_str);
  }
}


void Server::send_response(std::string f_response) {
  zmq_send(m_sock, strdup(f_response.c_str()), strlen(f_response.c_str()), 0);
}


std::string Server::pop_queue() {
    m_mutex.lock();
    std::string j = m_messageQueue.front();
    m_messageQueue.pop();
    m_mutex.unlock();
    return j;
}


void Server::push_to_queue(std::string& f_m) { 
  m_mutex.lock(); 
  //std::cout << "adding message to Server's queue : " << f_m << "\n";
  m_messageQueue.push((f_m));
  m_mutex.unlock();
};


std::string Server::message_to_string(zmq::message_t& f_msg) {
  size_t msg_size = f_msg.size();
  void* buffer = f_msg.data();
  char* sp = static_cast<char*>(buffer);
  std::string res(sp, msg_size);
  return res;
}


bool Server::has_message() {
  return not m_messageQueue.empty();
}


void Server::kill() {
  m_killed = true;
}

Server::~Server() {
  kill();
  for (auto& thread : m_threadPool) {
    thread.join();
  }
}



