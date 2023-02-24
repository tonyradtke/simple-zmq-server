<h2> Introduction </h2>

  - This is intended to be a simple string based request/response server using a c++ binding for zmq.

<h2> Dependencies </h2>

  - [cppzmq](https://github.com/zeromq/cppzmq)


<h2> Building </h2>

<ol>
  <li> Download and build Dependencies <
    <ul>
      <li> download [cppzmq](https://github.com/zeromq/cppzmq) </li>
      <li> follow the build instructions </li>
    </ul>
  </li>
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
  <li>  Build and Run the demo to make sure things are working 
    <ul> 
      <li> cd demo </li>
      <li> mkdir build </li>
      <li> cd build </li>
      <li> cmake . </li>
      <li> make </li>
      <li> ./bin/\_\_demo\_\_ </li>
    </ul>
  </li>
  <li> Include and Link against simple-zmq-server and cppzmq in your project </li>
    <ul>
      <li> find\_library("simple-zmq-server" STATIC lib) </li>
      <li> find\_package(cppzmq) </li>
      <li> target\_link\_libraries(${YOUR\_BINARY} PUBLIC "simple-zmq-server") </li>
      <li> target\_link\_libraries(${YOUR\_BINARY} PUBLIC cppzmq) </li>
    </ul> 
  </li>
</ol>




