#pragma once

#include "DataTypes.h"
#include "Common.h"
#include "Messages.h"

#include "../ExternalLibs/raknet/include/RakPeerInterface.h"
#include "../ExternalLibs/raknet/include/RakNetworkFactory.h"
#include "../ExternalLibs/raknet/include/MessageIdentifiers.h"
#include "../ExternalLibs/raknet/include/BitStream.h"

#include "network.pb.h"


namespace TLMP {

  namespace Network {

    class Client {
    public:
      typedef void (*OnConnected)(void *);
      typedef void (*OnDisconnected)(void *);
      typedef void (*OnConnectFailed)(void *);

      static Client& getSingleton();

      void Connect(const char* address, u16 port);
      void Disconnect();

      void SetCallback_OnConnected(OnConnected callback);
      void SetCallback_OnDisconnected(OnDisconnected callback);
      void SetCallback_OnConnectFailed(OnConnectFailed callback);

      void ReceiveMessages();

      template<typename T>
      void SendMessage(::google::protobuf::Message *message);

    protected:
      Client();
      Client(const Client&);
      Client& operator=(const Client&);
      ~Client();

    private:
      void OnConnect(void *);

      template<typename T>
      T* ParseMessage(u8 *packetData, u32 length);

      RakPeerInterface *m_pClient;
      RakNet::BitStream *m_pBitStream;

      OnConnected       m_pOnConnected;
      OnDisconnected    m_pOnDisconnected;
      OnConnectFailed   m_pOnConnectFailed;
    };

  };

};

// Template parameterization, needs to be defined after above declarations
#include "Client.cpp.h"
