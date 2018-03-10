#ifndef SOCKETCANINTERFACE_H
#define SOCKETCANINTERFACE_H

#include <thread>
#include "caninterface.h"
#include "../os/sockethandler.h"

/*
 * SocketCan interface for linux hosts
 */
class SocketCanInterface : public CanInterface, public Socket
{
public:
    SocketCanInterface(CanInterface::Callbacks &callbacks);
    ~SocketCanInterface();
    
    bool send(const CanMessage &message) override;
    
    bool recv(CanMessage &message) override;
    
    /* Closes the socket */
    void close();
    
    /* Starts the read loop thread for asynchronous operations */
    void runAsync();
    
    /* Binds the socket to a SocketCAN interface. Returns false
     * if an error occured. */
    bool bind(const char *ifname);
    
    /* Returns true if the socket exists */
    bool valid()
    {
        return socket_ > 0;
    }
    
    /* Sets the socket as nonblocking for async operations */
    void setNonblocking();
    
    /* Socket functions */
    int fd() override;
    void onRead() override;
    
private:
    int socket_ = 0;
};

#endif // SOCKETCANINTERFACE_H
