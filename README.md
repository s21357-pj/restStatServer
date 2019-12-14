# GNU RestAPI Statistic Server v. 0.1.0
PRG1 project
Use pistache C++ library.

#Installation from source code:
./configure
make
sudo make install

#Installation from rpm build:
sudo rpm -Uvh restStatServer-0.1.0-1.x86_64.rpm

#Usage: 
restStatServer [port] [threads]
Default: 80 port and 2 threads.

For execute server application on tcp port 80
you should have superuser permissions.
Otherwise you can execute application on underprivileged ports > 1024

Examples:
      sudo restStatServer
      restStatServer 8080 4
