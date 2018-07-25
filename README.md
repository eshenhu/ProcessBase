# ProcessBase

Process class is a base class for providing the following features:

i)  *nix cmdline options capability.

ii) implement the *nix signal handler, if you need customized signal handler, just override the member function.

iii) g3log intergrated.

iv)  provide bootup parameter for more detailed control. f.g.
   - single instance / multipule instance 
   - ...

v) you can use boost::asio::io_service to take your task in the main thread.

More usages please refer to the examples.
