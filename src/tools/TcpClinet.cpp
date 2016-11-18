#include "TcpClient.h"

namespace engine
{
    namespace tools
    {
        TcpClient & TcpClient::create(const string & address, const unsigned port, const unsigned loopInterval)
        {
            TcpClient & result = create();
            result.initWithLocation(address, port, loopInterval);
            return result;
        }

        const bool TcpClient::init(void)
        {
            if(!NetWork::initWithSocketType()){ return false; }
            
            return true;
        }

        const bool TcpClient::initWithLocation(const string & address, const unsigned port, const unsigned loopInterval)
        {
            if(!connect(address, port, loopInterval)){ return false; }

            return true;
        }

        void TcpClient::close(const function<void (const int client)> & callBack)
        {
            NetWork::close(callBack);
        }

        void TcpClient::recv(const function<void (const int client, const string & str)> & callBack)
        {
            NetWork::recv(callBack);
        }

        const bool TcpClient::send(const string & str) const
        {
            return NetWork::send(socket_id, str);
        }
    }
}