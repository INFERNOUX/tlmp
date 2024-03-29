#pragma once

#include "Common.h"
#include "UI.h"
using namespace TLMP;

namespace TLMP {

  namespace Network {

    class Server {
    public:
      typedef void (*OnListening)(void *);
      typedef void (*OnShutdown)(void *);
      typedef void (*OnClientConnect)(void *);
      typedef void (*OnClientDisconnect)(void *);
      

      static Server& getSingleton();

      void Listen(u16 port, u16 maxconnections);
      void Shutdown();

      void Reset();

      void SetCallback_OnListening(OnListening callback);
      void SetCallback_OnShutdown(OnShutdown callback);
      void SetCallback_OnClientConnect(OnClientConnect callback);
      void SetCallback_OnClientDisconnect(OnClientDisconnect callback);

      inline void SetGameStarted(bool value)  { m_bGameStarted = value; if (m_bGameStarted) m_bWaitingForGame = true; }
      inline bool GetGameStarted()            { return m_bGameStarted; }
      inline bool HasGameStarted()            { return m_bWaitingForGame; }

      inline bool GetSuppressed_SetDestination()            { return m_bSuppressNetwork_SetDestination; }
      inline void SetSuppressed_SetDestination(bool value)  { m_bSuppressNetwork_SetDestination = value; }

      inline bool GetSuppressed_SendCharacterCreation()            { return m_bSuppressNetwork_SendCharacterCreation; }
      inline void SetSuppressed_SendCharacterCreation(bool value)  { m_bSuppressNetwork_SendCharacterCreation = value; }
      
      inline bool GetSuppressed_SendEquipmentCreation()            { return m_bSuppressNetwork_SendEquipmentCreation; }
      inline void SetSuppressed_SendEquipmentCreation(bool value)  { m_bSuppressNetwork_SendEquipmentCreation = value; }

      inline bool GetSuppressed_SendEquipmentEquip()              { return m_bSuppressNetwork_SendEquipmentEquip; }
      inline void SetSuppressed_SendEquipmentEquip(bool value)    { m_bSuppressNetwork_SendEquipmentEquip = value; }

      inline bool GetSuppressed_SendEquipmentUnequip()            { return m_bSuppressNetwork_SendEquipmentUnequip; }
      inline void SetSuppressed_SendEquipmentUnequip(bool value)  { m_bSuppressNetwork_SendEquipmentUnequip = value; }

      inline bool GetSuppressed_SendEquipmentStack()              { return m_bSuppressNetwork_SendEquipmentStack; }
      inline void SetSuppressed_SendEquipmentStack(bool value)    { m_bSuppressNetwork_SendEquipmentStack = value; }

      inline bool GetSuppressed_SendCharacterAttack()              { return m_bSuppressNetwork_SendCharacterAttack; }
      inline void SetSuppressed_SendCharacterAttack(bool value)    { m_bSuppressNetwork_SendCharacterAttack = value; }

      inline bool GetSuppressed_SendAddExperience()                { return m_bSuppressNetwork_AddExperience; }
      inline void SetSuppressed_SendAddExperience(bool value)      { m_bSuppressNetwork_AddExperience = value; }

      


      void Helper_PopulateEquipmentMessage(TLMP::NetworkMessages::Equipment* msgEquipment, CEquipment *equipment, NetworkEntity *netEquipment);

      void ReceiveMessages();

      template<typename T>
      void BroadcastMessage(Message msg, ::google::protobuf::Message *message);
      template<typename T>
      void BroadcastMessage(const AddressOrGUID systemIdentifier, Message msg, ::google::protobuf::Message *message);
      template<typename T>
      void SendMessage(const AddressOrGUID systemIdentifier, Message msg, ::google::protobuf::Message *message);

      u32 GetSeed() const { return m_Seed; }

    protected:
      Server();
      Server(const Server&);
      Server& operator=(const Server&);
      ~Server();

    private:
      template<typename T>
      T* ParseMessage(RakNet::BitStream *bitStream);

      /** Work on received packet data. */
      void WorkMessage(const SystemAddress clientAddress, Message msg, RakNet::BitStream *bitStream);

      u32 m_Seed;

      /** Functions for handling individual messages. */
      void HandleVersion(const SystemAddress clientAddress, u32 version);
      void HandleHasGameStarted(const SystemAddress clientAddress);
      void HandleGameEnter(const SystemAddress clientAddress, NetworkMessages::GameEnter *);
      void HandleGameExited(const SystemAddress clientAddress);
      void HandleReplyCharacterInfo(const SystemAddress clientAddress, NetworkMessages::ReplyCharacterInfo *msgReplyCharacterInfo);
      void HandleInventoryAddEquipment(const SystemAddress clientAddress, u32 ownerId, u32 equipmentId, u32 slot, u32 unk0, u64 guid);
      void HandleInventoryRemoveEquipment(const SystemAddress clientAddress, u32 ownerId, u32 equipmentId);
      void HandleCharacterDestination(const SystemAddress clientAddress, u32 commonId, Vector3 current, Vector3 destination, u8 running, u8 attacking);
      void HandleEquipmentDrop(const SystemAddress client, u32 equipmentId, Vector3 position, bool unk0);
      void HandleEquipmentCreation(const SystemAddress clientAddress, TLMP::NetworkMessages::Equipment *msgEquipment);
      void HandleEquipmentPickup(u32 characterId, u32 equipmentId, const Vector3&);
      void HandleCharacterSetAction(const SystemAddress clientAddress, NetworkMessages::CharacterAction*);
      void HandleCharacterAttack(NetworkMessages::CharacterAttack*);
      void HandleCharacterUseSkill(NetworkMessages::CharacterUseSkill*);
      void HandleEquipmentUpdateStack(const SystemAddress, NetworkMessages::EquipmentUpdateStackSize*);
      void HandleEquipmentAddGem(const SystemAddress, NetworkMessages::EquipmentAddGem*);
      void HandleEquipmentRemoveGems(NetworkMessages::EquipmentRemoveGems *);
      void HandleChatMessage(NetworkMessages::ChatPlayerText *msgChatPlayerText);
      void HandleBreakableTriggered(NetworkMessages::BreakableTriggered *);
      void HandleTriggerUnitTriggered(NetworkMessages::TriggerUnitTriggered *);
      void HandleCharacterSetTarget(NetworkMessages::CharacterSetTarget *);
      void HandleEquipmentIdentify(NetworkMessages::EquipmentIdentify *);
      void HandleCharacterResurrect(NetworkMessages::CharacterResurrect *);
      void HandleCharacterOrientation(NetworkMessages::CharacterOrientation *);
      void HandlePlayerWeaponSwap(NetworkMessages::PlayerSwapWeapons *);
      void HandleCharacterSetSkillPoints(NetworkMessages::CharacterSetSkillPoints *);
      void HandleCharacterStrike(NetworkMessages::CharacterStrikeCharacter *);
      void HandleEquipmentAutoEquip(NetworkMessages::EquipmentAutoEquip *);
      
      void Helper_SendEquipmentToClient(const SystemAddress clientAddress, CEquipment *equipment, NetworkEntity *netEquipment);
      void Helper_SendGroundEquipmentToClient(const SystemAddress clientAddress, CEquipment *equipment, TLMP::NetworkEntity *netEquipment);
      void Helper_SendCurrentLevel(const SystemAddress clientAddress);
      void Helper_SendTriggerUnitSync(const SystemAddress clientAddress);
      bool Helper_CheckEquipmentPermission(const SystemAddress client, CCharacter *characterOwner, CEquipment *equipment);
      bool Helper_CheckCharacterPermission(const SystemAddress client, CCharacter *character);
      
      static void Helper_RemoveClient(const SystemAddress clientAddress);
      static void Helper_RemoveBaseUnit(CBaseUnit*);

      void RemoveDestroyedEquipmentFromNetwork();

      void HandleAddClientCharacter(const SystemAddress address, CCharacter* character);

      TLMP::NetworkMessages::Equipment* Helper_CreateEquipmentMessage(TLMP::NetworkMessages::Equipment* msgEquipment, CEquipment *equipment, NetworkEntity *netEquipment);
     

      static void OnClientConnected(void *args);
      static void OnClientDisconnected(void *args);

      void SendClientEntities();

      RakPeerInterface *m_pServer;
      RakNet::BitStream *m_pBitStream;
      
      vector<OnListening>         m_pOnListening;
      vector<OnShutdown>          m_pOnShutdown;
      vector<OnClientConnect>     m_pOnClientConnect;
      vector<OnClientDisconnect>  m_pOnClientDisconnect;

      bool m_bWaitingForGame;
      bool m_bGameStarted;

      bool m_bSuppressNetwork_SetDestination;
      bool m_bSuppressNetwork_SendCharacterCreation;
      bool m_bSuppressNetwork_SendEquipmentCreation;
      bool m_bSuppressNetwork_SendEquipmentEquip;
      bool m_bSuppressNetwork_SendEquipmentUnequip;
      bool m_bSuppressNetwork_SendEquipmentStack;
      bool m_bSuppressNetwork_SendCharacterAttack;
      bool m_bSuppressNetwork_AddExperience;
    };

  };

};

// Template parameterization, needs to be defined after above declarations
#include "Server.cpp.h"
