#include "Client.hh"

#define kSLEEP_MS 1

Client::Client(bool f_localhost) {
  m_sock = zmq::socket_t(m_ctx, zmq::socket_type::pair);
  if (f_localhost) {
    m_sock.connect("tcp://127.0.0.1:8000");
    std::cout << "created a localhost client \n";
  }
  else {
    //this should be an addr we choose, cannot be *
    std::cout << "this is invalid for now..\n";
    return;
    #if 0
    m_sock.connect("tcp://*:8000");
    std::cout << "created a server!\n";
    #endif
  }
  start_polling_thread();
  m_killed = false;
};


void Client::send_request(std::string f_response) {
  zmq_send(m_sock, strdup(f_response.c_str()), strlen(f_response.c_str()), 0);
}


void Client::start_polling_thread() {
  m_threadPool.push_back(std::thread([&](){ poll_response(); })); 
}


std::string Client::wait_for_response() {
  std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
  while (true) {
    if (not has_message()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(kSLEEP_MS)); 
      continue;
    }   
    return pop_queue(); 
  }
}


void Client::poll_response() {

  while (true) {

    if (m_killed) {
      std::cout << "client polling thread has been killed\n";
      return;
    }

    zmq::message_t message;

    auto ret = m_sock.recv(message, zmq::recv_flags::dontwait);

    if (not ret) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500)); //dont shread the cpu polling :)
      continue;
    }

    std::string msg_str = message_to_string(message);

    push_to_queue(msg_str);
  }
}


std::string Client::pop_queue() {
    m_mutex.lock();
    std::string j = m_messageQueue.front();
    m_messageQueue.pop();
    m_mutex.unlock();
    return j;
}


void Client::push_to_queue(std::string& f_m) { 
  m_mutex.lock();
  m_messageQueue.push((f_m));
  m_mutex.unlock();
};


std::string Client::message_to_string(zmq::message_t& f_msg) {
  size_t msg_size = f_msg.size();
  void* buffer = f_msg.data();
  char* sp = static_cast<char*>(buffer);
  std::string res(sp, msg_size);
  return res;
}


bool Client::has_message() {
  return not m_messageQueue.empty();
}


void Client::kill() {
  m_killed = true;
}


Client::~Client() {
  kill();
  for (auto& thread : m_threadPool) {
    thread.join();
  }
}
