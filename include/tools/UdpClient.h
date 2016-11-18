#pragma once
#ifndef __UDP_CLIENT_H__
#define __UDP_CLIENT_H__

#include "NetWork.h"

namespace engine
{
    namespace tools
    {
        using namespace core;
        class UdpClient : public NetWork
        {
        public:
            static UdpClient & create(const string & address, const unsigned port);
            const bool send(const string & str) const;
        protected:
            
            const bool initWithLocation(const string & address, const unsigned port);

        private:
            struct sockaddr_in sendToAddr;
        };
    }
}

#endif //__UDP_CLIENT_H__