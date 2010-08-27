#include "Server.h"

using namespace TLMP::Network;

Server& Server::getSingleton()
{
  static Server m_pSingleton;
  return m_pSingleton;
}

Server::Server()
{
  m_pServer = NULL;
  m_pBitStream = new RakNet::BitStream(1024);

  Reset();

  m_bWaitingForGame = true;

  SetCallback_OnClientConnect(&OnClientConnected);
  SetCallback_OnClientDisconnect(&OnClientDisconnected);
}

Server::~Server()
{
  delete m_pBitStream;
}

void Server::Reset()
{
  m_bSuppressNetwork_SetDestination = false;
  m_bSuppressNetwork_SendCharacterCreation = false;
  m_bSuppressNetwork_SendEquipmentEquip = false;
  m_bSuppressNetwork_SendEquipmentUnequip = false;
  m_bSuppressNetwork_SendEquipmentCreation = false;
  m_bSuppressNetwork_SendEquipmentStack = false;
}

void Server::Listen(u16 port, u16 maxconnections)
{
  multiplayerLogger.WriteLine(Info, L"Starting server...");

  if (m_pServer) {
    multiplayerLogger.WriteLine(Info, L"Shutting down server");
    m_pServer->Shutdown(0);
  }

  m_pServer = RakNetworkFactory::GetRakPeerInterface();
  multiplayerLogger.WriteLine(Info, L"Got server: %p", m_pServer);

  // Debugging - allow 2 minutes until disconnect
  //m_pServer->SetTimeoutTime(120000, UNASSIGNED_SYSTEM_ADDRESS);
  //--

  SocketDescriptor socketDescriptor(port, 0);
  m_pServer->Startup(maxconnections, 30, &socketDescriptor, 1);
  m_pServer->SetMaximumIncomingConnections(maxconnections);

  multiplayerLogger.WriteLine(Info, L"Server listening on port %i (max connections = %i)", port, maxconnections);

  vector<OnListening>::iterator itr;
  for (itr = m_pOnListening.begin(); itr != m_pOnListening.end(); itr++) {
    (*itr)(NULL);
  }
}

void Server::Shutdown()
{
  if (m_pServer) {
    Reset();

    m_pServer->Shutdown(0);
    m_pServer = NULL;

    multiplayerLogger.WriteLine(Info, L"Server shutdown");

    vector<OnShutdown>::iterator itr;
    for (itr = m_pOnShutdown.begin(); itr != m_pOnShutdown.end(); itr++) {
      (*itr)(NULL);
    }
  }
}

void Server::SetCallback_OnListening(OnListening callback)
{
  m_pOnListening.push_back(callback);
}

void Server::SetCallback_OnShutdown(OnShutdown callback)
{
  m_pOnShutdown.push_back(callback);
}

void Server::SetCallback_OnClientConnect(OnClientConnect callback)
{
  m_pOnClientConnect.push_back(callback);
}

void Server::SetCallback_OnClientDisconnect(OnClientDisconnect callback)
{
  m_pOnClientDisconnect.push_back(callback);
}

void Server::ReceiveMessages()
{
  if (!m_pServer)
    return;

  // Round up any packets we've received
  Packet *packet = m_pServer->Receive();
  while (packet) {
    switch(packet->data[0]) {
    case ID_NEW_INCOMING_CONNECTION:
      {
        multiplayerLogger.WriteLine(Info, L"Client connected");
        logColor(B_GREEN, L"Client connected");

        vector<OnClientConnect>::iterator itr;
        for (itr = m_pOnClientConnect.begin(); itr != m_pOnClientConnect.end(); itr++) {
          SystemAddress *sysAddress = new SystemAddress();
          *sysAddress = packet->systemAddress;
          (*itr)((void*)sysAddress);
        }
      }
      break;

    case ID_DISCONNECTION_NOTIFICATION:
      {
        multiplayerLogger.WriteLine(Info, L"Client disconnected");
        logColor(B_GREEN, L"Client disconnected");

        vector<OnClientDisconnect>::iterator itr;
        for (itr = m_pOnClientDisconnect.begin(); itr != m_pOnClientDisconnect.end(); itr++) {
          SystemAddress *sysAddress = new SystemAddress();
          *sysAddress = packet->systemAddress;
          (*itr)((void*)sysAddress);
        }
      }
      break;

    case ID_CONNECTION_LOST:
      {
        multiplayerLogger.WriteLine(Info, L"Client disconnected");
        logColor(B_GREEN, L"Client disconnected");

        vector<OnClientDisconnect>::iterator itr;
        for (itr = m_pOnClientDisconnect.begin(); itr != m_pOnClientDisconnect.end(); itr++) {
          SystemAddress *sysAddress = new SystemAddress();
          *sysAddress = packet->systemAddress;
          (*itr)((void*)sysAddress);
        }
      }
      break;

    case ID_USER_PACKET_ENUM+1:
      u32 msg;
      m_pBitStream->Reset();
      m_pBitStream->Write((const char *)packet->data, packet->length);
      m_pBitStream->IgnoreBits(8);
      m_pBitStream->Read<u32>(msg);

      WorkMessage(packet->systemAddress, (Message)msg, m_pBitStream);

      break;
    }

    m_pServer->DeallocatePacket(packet);
    packet = m_pServer->Receive();
  }
}

void Server::WorkMessage(const SystemAddress address, Message msg, RakNet::BitStream *bitStream)
{
  if (msg >= sizeof(MessageString) / sizeof(char*)) {
    log("Error: Message out of range.");
    multiplayerLogger.WriteLine(Info, L"Error: Message out of range.");
  }

  wstring msgString = convertAcsiiToWide(MessageString[msg]);

  multiplayerLogger.WriteLine(Info, L"Server Received Message: %s", msgString.c_str());
  logColor(B_GREEN, L"Server Received Message: %s", msgString.c_str());

  switch (msg) {
  case C_VERSION:
    {
      NetworkMessages::Version *msgVersion = ParseMessage<NetworkMessages::Version>(m_pBitStream);
      u32 version = (u32)(msgVersion->version());

      HandleVersion(address, version);
    }
    break;

  case C_REQUEST_HASGAMESTARTED:
    {
      NetworkMessages::GameHasStarted *msgGameHasStarted = ParseMessage<NetworkMessages::GameHasStarted>(m_pBitStream);

      HandleHasGameStarted(address);
    }
    break;

  case C_PUSH_GAMEENTER:
    {
      NetworkMessages::GameEnter *msgGameEnter = ParseMessage<NetworkMessages::GameEnter>(m_pBitStream);

      HandleGameEnter(address);
    }
    break;

  case C_PUSH_GAMEEXITED:
    {
      NetworkMessages::GameExited *msgGameExited = ParseMessage<NetworkMessages::GameExited>(m_pBitStream);

      HandleGameExited(address);
    }
    break;

  case C_REPLY_CHARINFO:
    {
      NetworkMessages::ReplyCharacterInfo *msgReplyCharacterInfo = ParseMessage<NetworkMessages::ReplyCharacterInfo>(m_pBitStream);

      HandleReplyCharacterInfo(address, msgReplyCharacterInfo);
    }
    break;

  case C_PUSH_EQUIPMENT:
    {
      NetworkMessages::Equipment *msgEquipment = ParseMessage<NetworkMessages::Equipment>(m_pBitStream);
      
      HandleEquipmentCreation(address, msgEquipment); 
    }
    break;

  case C_PUSH_EQUIPMENT_DROP:
    {
      NetworkMessages::EquipmentDrop *msgEquipmentDrop = ParseMessage<NetworkMessages::EquipmentDrop>(m_pBitStream);
      NetworkMessages::Position msgPosition = msgEquipmentDrop->position().Get(0);

      Vector3 position;
      position.x = msgPosition.x();
      position.y = msgPosition.y();
      position.z = msgPosition.z();

      u32 equipmentId = msgEquipmentDrop->equipmentid();
      bool unk0 = msgEquipmentDrop->unk0();

      HandleEquipmentDrop(equipmentId, position, unk0);
    }
    break;

  case C_PUSH_EQUIPMENT_PICKUP:
    {
      NetworkMessages::EquipmentPickup *msgEquipmentPickup = ParseMessage<NetworkMessages::EquipmentPickup>(m_pBitStream);

      u32 equipmentId = msgEquipmentPickup->equipmentid();
      u32 characterId = msgEquipmentPickup->characterid();

      HandleEquipmentPickup(characterId, equipmentId);
    }
    break;

  case C_PUSH_EQUIPMENT_EQUIP:
    {
      NetworkMessages::InventoryAddEquipment *msgInventoryAddEquipment = ParseMessage<NetworkMessages::InventoryAddEquipment>(m_pBitStream);
      u32 ownerId = msgInventoryAddEquipment->ownerid();
      u32 equipmentId = msgInventoryAddEquipment->equipmentid();
      u32 slot = msgInventoryAddEquipment->slot();
      u32 unk0 = msgInventoryAddEquipment->unk0();
      u64 guid = msgInventoryAddEquipment->guid();

      HandleInventoryAddEquipment(address, ownerId, equipmentId, slot, unk0, guid);
    }
    break;

  case C_PUSH_EQUIPMENT_UNEQUIP:
    {
      NetworkMessages::InventoryRemoveEquipment *msgInventoryRemoveEquipment = ParseMessage<NetworkMessages::InventoryRemoveEquipment>(m_pBitStream);
      u32 ownerId = msgInventoryRemoveEquipment->ownerid();
      u32 equipmentId = msgInventoryRemoveEquipment->equipmentid();

      HandleInventoryRemoveEquipment(address, ownerId, equipmentId);
    }
    break;

  case C_PUSH_EQUIPMENT_USE:
    break;

  case C_PUSH_EQUIPMENT_IDENTIFY:
    break;

  case C_PUSH_EQUIPMENT_ADDENCHANT:
    break;

  case C_PUSH_EQUIPMENT_ADDSOCKET:
    break;

  case C_PUSH_CHARACTER_SETDEST:
    {
      NetworkMessages::CharacterDestination *msgCharacterDestination = ParseMessage<NetworkMessages::CharacterDestination>(m_pBitStream);
      NetworkMessages::Position msgDestination = msgCharacterDestination->destination().Get(0);

      u32 commonId = msgCharacterDestination->id();

      Vector3 destination;
      destination.x = msgDestination.x();
      destination.z = msgDestination.z();

      u8 running = msgCharacterDestination->running();
      u8 attacking = msgCharacterDestination->attacking();

      HandleCharacterDestination(address, commonId, destination, running, attacking);
    }
    break;

  case C_PUSH_CHARACTER_ACTION:
    {
      NetworkMessages::CharacterAction *msgCharacterAction = ParseMessage<NetworkMessages::CharacterAction>(m_pBitStream);

      HandleCharacterSetAction(address, msgCharacterAction);
    }
    break;

  case C_PUSH_CHARACTER_ATTACK:
    {
      /* Crashes the server when receiving
      NetworkMessages::CharacterAttack *msgCharacterAttack = ParseMessage<NetworkMessages::CharacterAttack>(m_pBitStream);

      HandleCharacterAttack(msgCharacterAttack);
      */
    }

  case C_PUSH_CHARACTER_USESKILL:
    {
      NetworkMessages::CharacterUseSkill *msgCharacterUseSkill = ParseMessage<NetworkMessages::CharacterUseSkill>(m_pBitStream);

      HandleCharacterUseSkill(msgCharacterUseSkill);
    }
    break;
    
  case C_PUSH_EQUIPMENT_STACKUPDATE:
    {
      NetworkMessages::EquipmentUpdateStackSize *msgEquipmentUpdateStackSize = ParseMessage<NetworkMessages::EquipmentUpdateStackSize>(m_pBitStream);

      HandleEquipmentUpdateStack(address, msgEquipmentUpdateStackSize);
    }
    break;

  case C_PUSH_EQUIPMENT_ADD_GEM:
    {
      NetworkMessages::EquipmentAddGem *msgEquipmentAddGem = ParseMessage<NetworkMessages::EquipmentAddGem>(m_pBitStream);

      HandleEquipmentAddGem(address, msgEquipmentAddGem);
    }
    break;

  case C_PUSH_EQUIPMENT_REMOVE_GEMS:
    {
      NetworkMessages::EquipmentRemoveGems *msgEquipmentRemoveGems = ParseMessage<NetworkMessages::EquipmentRemoveGems>(m_pBitStream);

      HandleEquipmentRemoveGems(msgEquipmentRemoveGems);
    }
    break;

  }
}

//
// Below are message handlers

// Server receives a version message from Client.
void Server::HandleVersion(const SystemAddress clientAddress, u32 version)
{
  multiplayerLogger.WriteLine(Info, L"Server received Client Version: %i", version);

  if (version != MessageVersion) {
    multiplayerLogger.WriteLine(Info, L"Disconnecting the Client: Version Mismatch: (Client = %i  Server = %i)",
      version, MessageVersion);

    // TODO Disconnect Client
  }
}

// Server receives game started request from Client upon initial login.
void Server::HandleHasGameStarted(const SystemAddress clientAddress)
{
  NetworkMessages::GameHasStarted msgGameHasStarted;
  msgGameHasStarted.set_started(m_bGameStarted);

  SendMessage<NetworkMessages::GameHasStarted>(clientAddress, S_REPLY_HASGAMESTARTED, &msgGameHasStarted);
}

// Server receives game entrance from Client
void Server::HandleGameEnter(const SystemAddress clientAddress)
{
  multiplayerLogger.WriteLine(Info, L"Client has Entered the Game");

  vector<NetworkEntity*>::iterator itr;

  // Check for destroyed equipment and remove them
  RemoveDestroyedEquipmentFromNetwork();

  // Send all of the existing equipment in the game to the client
  for (itr = NetworkSharedEquipment->begin(); itr != NetworkSharedEquipment->end(); itr++) {
    CEquipment *equipment = (CEquipment*)((*itr)->getInternalObject());

    // Suppress Waypoint and Return to Dungeon
    if (equipment->GUID != 0x761772BDA01D11DE &&
      equipment->GUID != 0xD3A8F99E2FA111DE) 
    {
      Helper_SendEquipmentToClient(clientAddress, equipment, (*itr));
    }
  }
  
  // Send all of the Equipment on the Ground in the game
  for (itr = ServerEquipmentOnGround->begin(); itr != ServerEquipmentOnGround->end(); itr++) {
    CEquipment *equipment = (CEquipment*)((*itr)->getInternalObject());

    // Suppress Waypoint and Return to Dungeon
    if (equipment->GUID != 0x761772BDA01D11DE &&
      equipment->GUID != 0xD3A8F99E2FA111DE) 
    {
      Helper_SendGroundEquipmentToClient(clientAddress, equipment, (*itr));
    }
  }

  // Send all of the existing characters in the game to the client
  for (itr = NetworkSharedCharacters->begin(); itr != NetworkSharedCharacters->end(); itr++) {
    CCharacter* character = (CCharacter*)((*itr)->getInternalObject());

    multiplayerLogger.WriteLine(Info, L"Server: Pushing existing Character (%016I64X %i %s) out to client that just joined...",
      character->GUID, (*itr)->getCommonId(), character->characterName.c_str());
    log(L"Server: Pushing existing Character (%p %016I64X %i %s) out to client that just joined...",
      character, character->GUID, (*itr)->getCommonId(), character->characterName.c_str());

    string characterName(character->characterName.begin(), character->characterName.end());
    characterName.assign(character->characterName.begin(), character->characterName.end());

    // Create a new network message for all clients to create this character
    NetworkMessages::Character msgNewCharacter;
    msgNewCharacter.set_guid(character->GUID);
    msgNewCharacter.set_name(characterName);

    NetworkMessages::Position *msgPlayerPosition = msgNewCharacter.mutable_position();
    msgPlayerPosition->set_x(character->position.x);
    msgPlayerPosition->set_y(character->position.y);
    msgPlayerPosition->set_z(character->position.z);

    msgNewCharacter.set_id((*itr)->getCommonId());
    msgNewCharacter.set_health(character->healthMax);
    msgNewCharacter.set_mana(character->manaMax);
    msgNewCharacter.set_level(character->level);

    // This allows a workaround for the modified attributes needed for equipment
    /*
    msgNewCharacter.set_strength(character->baseStrength);
    msgNewCharacter.set_dexterity(character->baseDexterity);
    msgNewCharacter.set_defense(character->baseDefense);
    msgNewCharacter.set_magic(character->baseMagic);
    */
    msgNewCharacter.set_strength(10000);
    msgNewCharacter.set_dexterity(10000);
    msgNewCharacter.set_defense(10000);
    msgNewCharacter.set_magic(10000);

    // This will broadcast to all clients except the one we received it from
    Server::getSingleton().SendMessage<NetworkMessages::Character>(clientAddress, S_PUSH_NEWCHAR, &msgNewCharacter);

    // Old: Move through the body slots and send an equip message to show the initial equipment
    ///// New: Move throug all inventory slots and add equipment
    if (character->pCInventory) {
      for (u32 i = 0; i <= 0x0C; i++) {
        CEquipment* equipment = character->pCInventory->GetEquipmentFromSlot(i);

        if (equipment) {
          NetworkEntity* netEquipment = searchEquipmentByInternalObject(equipment);

          if (netEquipment) {
            NetworkMessages::InventoryAddEquipment msgInventoryAddEquipment;
            msgInventoryAddEquipment.set_equipmentid(netEquipment->getCommonId());
            msgInventoryAddEquipment.set_guid(equipment->GUID);
            msgInventoryAddEquipment.set_ownerid((*itr)->getCommonId());
            msgInventoryAddEquipment.set_slot(i);
            msgInventoryAddEquipment.set_unk0(1);

            Server::getSingleton().SendMessage<NetworkMessages::InventoryAddEquipment>(clientAddress, S_PUSH_EQUIPMENT_EQUIP, &msgInventoryAddEquipment);
          }
        }
      }
    } else {
      log(L"Error: Character has no Inventory!");
      multiplayerLogger.WriteLine(Error, L"Error: Character has no Inventory!");
    }
  }

  // Send a request for character info
  NetworkMessages::RequestCharacterInfo msgRequestCharacterInfo;
  SendMessage<NetworkMessages::RequestCharacterInfo>(clientAddress, S_REQUEST_CHARINFO, &msgRequestCharacterInfo);
}

// Server receives game exit from client
void Server::HandleGameExited(const SystemAddress clientAddress)
{
  multiplayerLogger.WriteLine(Info, L"Client has Exited the Game");
}

void Server::HandleReplyCharacterInfo(const SystemAddress clientAddress, NetworkMessages::ReplyCharacterInfo *msgReplyCharacterInfo)
{
  NetworkMessages::Character msgPlayer;
  NetworkMessages::Position  msgPosition;
  NetworkEntity *entityCharacter;

  msgPlayer = msgReplyCharacterInfo->player();

  // This will broadcast to all clients except the one we received it from
  Server::getSingleton().BroadcastMessage<NetworkMessages::Character>(clientAddress, S_PUSH_NEWCHAR, &msgPlayer);

  Vector3 posCharacter;
  Vector3 posPlayer;
  posPlayer.x = msgPlayer.position().x();
  posPlayer.y = msgPlayer.position().y();
  posPlayer.z = msgPlayer.position().z();

  float health = msgPlayer.health();
  float mana = msgPlayer.mana();
  u32 levelCharacter = msgPlayer.level();
  u32 strength = msgPlayer.strength();
  u32 dexterity = msgPlayer.dexterity();
  u32 defense = msgPlayer.defense();
  u32 magic = msgPlayer.magic();

  // Create this Player on this instance
  CResourceManager *resourceManager = (CResourceManager *)gameClient->pCPlayer->pCResourceManager;
  CMonster *clientCharacter = resourceManager->CreateMonster(msgPlayer.guid(), levelCharacter, true);

  if (clientCharacter) {
    clientCharacter->characterName.assign(convertAcsiiToWide(msgPlayer.name()));
    clientCharacter->SetAlignment(1);

    // Lock the creation so we don't resend to all the clients
    Server::getSingleton().SetSuppressed_SendCharacterCreation(true);
    gameClient->pCLevel->CharacterInitialize(clientCharacter, &posPlayer, 0);
    Server::getSingleton().SetSuppressed_SendCharacterCreation(false);

    clientCharacter->healthMax = health;
    clientCharacter->manaMax = mana;
    clientCharacter->baseStrength = strength;
    clientCharacter->baseDexterity = dexterity;
    clientCharacter->baseDefense = defense;
    clientCharacter->baseMagic = magic;

    // Send this ID to the Client that created the character
    entityCharacter = searchCharacterByInternalObject(clientCharacter);
    if (!entityCharacter) {
      multiplayerLogger.WriteLine(Error, L"Error: Could not find Shared NetworkEntity for the Client Character.");
      log(L"Error: Could not find Shared NetworkEntity for the Client Character.");
    } else {
      multiplayerLogger.WriteLine(Info, L"Server sending client character ID reply: %i", entityCharacter->getCommonId());

      NetworkMessages::ReplyCharacterId msgReplyCharacterId;
      msgReplyCharacterId.set_id(entityCharacter->getCommonId());
      msgReplyCharacterId.set_guid(clientCharacter->GUID);

      Server::getSingleton().SendMessage<NetworkMessages::ReplyCharacterId>(clientAddress, S_REPLY_CHARID, &msgReplyCharacterId);

      // Add this character to our ClientAddressCharacter map
      HandleAddClientCharacter(clientAddress, clientCharacter);
    }
  } else {
    log("Error: clientCharacter is null!");
    multiplayerLogger.WriteLine(Error, L"Error: clientCharacter is null!");
  }

  log(L"Minion Count: %i", msgPlayer.minion_size());

  // Create the pets
  for (int i = 0; i < msgPlayer.minion_size(); i++) {
    NetworkMessages::Character msgMinion = msgPlayer.minion().Get(i);
    posCharacter.x = msgMinion.position().x();
    posCharacter.y = msgMinion.position().y();
    posCharacter.z = msgMinion.position().z();

    // Create this Player on this instance
    CMonster *clientMinion = resourceManager->CreateMonster(msgMinion.guid(), msgMinion.level(), true);
    if (clientMinion) {
      clientMinion->characterName.assign(convertAcsiiToWide(msgMinion.name()));
      clientMinion->SetAlignment(1);

      // Lock the creation so we don't resend to all the clients
      Server::getSingleton().SetSuppressed_SendCharacterCreation(true);
      gameClient->pCLevel->CharacterInitialize(clientMinion, &posCharacter, 0);
      Server::getSingleton().SetSuppressed_SendCharacterCreation(false);  

      // Add the minion to the player
      clientCharacter->AddMinion(clientMinion);
      
      // Send this ID to the Client that created the character
      entityCharacter = searchCharacterByInternalObject(clientMinion);
      if (!entityCharacter) {
        multiplayerLogger.WriteLine(Error, L"Error: Could not find Shared NetworkEntity for the Client Minion Character.");
        log(L"Error: Could not find Shared NetworkEntity for the Client Minion Character.");
      } else {
        multiplayerLogger.WriteLine(Info, L"Server sending client character ID reply: %i", entityCharacter->getCommonId());

        NetworkMessages::ReplyCharacterId msgReplyCharacterId;
        msgReplyCharacterId.set_id(entityCharacter->getCommonId());
        msgReplyCharacterId.set_guid(clientMinion->GUID);

        Server::getSingleton().SendMessage<NetworkMessages::ReplyCharacterId>(clientAddress, S_REPLY_CHARID, &msgReplyCharacterId);

        // Add this character to our ClientAddressCharacter map
        HandleAddClientCharacter(clientAddress, clientMinion);
      }
    } else {
      log("Error: clientMinion is null!");
      multiplayerLogger.WriteLine(Error, L"Error: clientMinion is null!");
    }
  }
}


void Server::HandleCharacterDestination(const SystemAddress clientAddress, u32 commonId, Vector3 destination, u8 running, u8 attacking)
{
  multiplayerLogger.WriteLine(Info, L"Server received character setDestination (CommonID = %x), Position = %f, %f",
    commonId, destination.x, destination.z);

  // Search for our Character given the CommonID
  NetworkEntity *entity = searchCharacterByCommonID(commonId);

  if (!entity) {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find internal object in shared characters from commonId: %x when receiving character destination.", commonId);
    return;
  }

  CCharacter *character = (CCharacter *)entity->getInternalObject();

  character->SetDestination(gameClient->pCLevel, destination.x, destination.z);
  character->running = running;
  character->attacking = attacking;
}


void Server::HandleInventoryAddEquipment(const SystemAddress clientAddress, u32 ownerId, u32 equipmentId, u32 slot, u32 unk0, u64 guid)
{
  multiplayerLogger.WriteLine(Info, L"Server received inventory add equipment: (CharacterID = %x, EquipmentID = %x, slot = %x, guid = %016I64X)",
    ownerId, equipmentId, slot, guid);
  log(L"Server received inventory add equipment: (CharacterID = %x, EquipmentID = %x, slot = %x, guid = %016I64X)",
    ownerId, equipmentId, slot, guid);

  NetworkEntity* owner = searchCharacterByCommonID(ownerId);
  NetworkEntity* equipment = searchEquipmentByCommonID(equipmentId);
  CResourceManager *resourceManager = (CResourceManager *)gameClient->pCPlayer->pCResourceManager;

  if (owner) {
    CCharacter *characterOwner = (CCharacter*)owner->getInternalObject();
    CInventory *inventory = characterOwner->pCInventory;
    CEquipment *equipmentReal;

    if (equipment) {
      equipmentReal = (CEquipment*)equipment->getInternalObject();
    } else {
      multiplayerLogger.WriteLine(Info, L"Server: Could not find Equipment with ID = %x, creating it", equipmentId);
      log(L"Server: Could not find Equipment with ID = %x, creating it", equipmentId);

      equipmentReal = resourceManager->CreateEquipment(guid, 1, 1, 0);
  
      log(L"Server: Adding equipment to shared network equipment...");
      NetworkEntity *newEntity = addEquipment(equipmentReal, equipmentId);
    }

    multiplayerLogger.WriteLine(Info, L"Server: Added equipment.. now adding to inventory...");
    log(L"Server: Added equipment.. now adding to inventory...");

    // Suppress network send
    Server::getSingleton().SetSuppressed_SendEquipmentEquip(true);
    inventory->AddEquipment(equipmentReal, slot, unk0);
    Server::getSingleton().SetSuppressed_SendEquipmentEquip(false);

    // And send it to the other clients
    NetworkMessages::InventoryAddEquipment msgInventoryAddEquipment;
    msgInventoryAddEquipment.set_ownerid(owner->getCommonId());
    msgInventoryAddEquipment.set_equipmentid(equipment->getCommonId());
    msgInventoryAddEquipment.set_slot(slot);
    msgInventoryAddEquipment.set_unk0(unk0);

    Server::getSingleton().BroadcastMessage<NetworkMessages::InventoryAddEquipment>(clientAddress, S_PUSH_EQUIPMENT_EQUIP, &msgInventoryAddEquipment);

    //
    multiplayerLogger.WriteLine(Info, L"Server: Done adding equipment to inventory.");
    log(L"Server: Done adding equipment to inventory.");
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find Character with ID = %x", ownerId);
    log(L"Error: Could not find Character with ID = %x", ownerId);
  }

  log(L"HandleInventoryAddEquipment Done.");
}

void Server::Helper_SendEquipmentToClient(const SystemAddress clientAddress, CEquipment *equipment, TLMP::NetworkEntity *netEquipment)
{
  multiplayerLogger.WriteLine(Info, L"Server: Pushing existing Equipment (%016I64X %i) out to client that just joined...",
    equipment->GUID, netEquipment->getCommonId());
  log(L"Server: Pushing existing Equipment (%016I64X %i) out to client that just joined...",
    equipment->GUID, netEquipment->getCommonId());

  // Create a new network message for all clients to create this character
  NetworkMessages::Equipment msgEquipment;
  Helper_PopulateEquipmentMessage(&msgEquipment, equipment, netEquipment);

  log(L"  DEBUG: Equipment gemList: %p (Offset = %x)",
    &equipment->gemList, ((u32)this - (u32)&equipment->gemList));
  multiplayerLogger.WriteLine(Info, L"  Equipment contains %i Gems", equipment->gemList.size);
  log(L"  Equipment contains %i Gems", equipment->gemList.size);

  // Add the Gem Equipment
  for (u32 i = 0; i < equipment->gemList.size; i++) {
    multiplayerLogger.WriteLine(Info, L"Server: Adding gem...");
    log(L"Server: Adding gem...");

    NetworkMessages::Equipment *msgGem = msgEquipment.add_gems();
    CEquipment *gem = equipment->gemList[i];
    NetworkEntity *netGem = searchEquipmentByInternalObject(gem);
    
    // 
    if (netGem) {
      Helper_PopulateEquipmentMessage(msgGem, gem, netGem);
    } else {
      multiplayerLogger.WriteLine(Info, L"Could not find NetworkEntity for Gem, skipping equipment");
      log(L"Could not find NetworkEntity for Gem, skipping equipment");
      continue;
    }
  }

  // This will broadcast to all clients except the one we received it from
  Server::getSingleton().SendMessage<NetworkMessages::Equipment>(clientAddress, S_PUSH_NEWEQUIPMENT, &msgEquipment);
}

void Server::Helper_SendGroundEquipmentToClient(const SystemAddress clientAddress, CEquipment *equipment, TLMP::NetworkEntity *netEquipment)
{
  multiplayerLogger.WriteLine(Info, L"Server: Pushing existing Equipment (%016I64X %i) out to client that just joined...",
    equipment->GUID, netEquipment->getCommonId());
  log(L"Server: Pushing existing Equipment (%016I64X %i) out to client that just joined...",
    equipment->GUID, netEquipment->getCommonId());

  // Create a new network message for all clients to create this character
  NetworkMessages::EquipmentDrop msgEquipmentDrop;
  NetworkMessages::Position *msgPosition = msgEquipmentDrop.add_position();
  msgPosition->set_x(equipment->position.x);
  msgPosition->set_y(equipment->position.y);
  msgPosition->set_z(equipment->position.z);
  msgEquipmentDrop.set_equipmentid(netEquipment->getCommonId());
  msgEquipmentDrop.set_unk0(1);

  // This will broadcast to all clients except the one we received it from
  Server::getSingleton().SendMessage<NetworkMessages::EquipmentDrop>(clientAddress, S_PUSH_EQUIPMENT_DROP, &msgEquipmentDrop);
}

//
// Populates the msgEquipment with the given Equipment vars
void Server::Helper_PopulateEquipmentMessage(NetworkMessages::Equipment* msgEquipment, CEquipment *equipment, NetworkEntity *netEquipment)
{
  multiplayerLogger.WriteLine(Info, L"  Setting up regular variables for Equipment: %s", equipment->nameReal.c_str());
  log(L"  Setting up regular variables for Equipment: %s", equipment->nameReal.c_str());

  // Fill out the easy stuff
  msgEquipment->set_guid(equipment->GUID);
  msgEquipment->set_id(netEquipment->getCommonId());
  msgEquipment->set_stacksize(equipment->stackSize);
  msgEquipment->set_stacksizemax(equipment->stackSizeMax);
  msgEquipment->set_socketcount(equipment->socketCount);

  msgEquipment->set_physical_damage_min(equipment->minimumPhysicalDamage);
  msgEquipment->set_physical_damage_max(equipment->maximumPhysicalDamage);

  // Check if we're a:
  //  Waypoint Portal
  //  Return To Dungeon
  // If so skip this
  if (equipment->GUID != 0x761772BDA01D11DE &&
    equipment->GUID != 0xD3A8F99E2FA111DE) 
  {
    string nameUnidentified(equipment->nameUnidentified.begin(), equipment->nameUnidentified.end());
    nameUnidentified.assign(equipment->nameUnidentified.begin(), equipment->nameUnidentified.end());
    msgEquipment->set_name_unidentified(nameUnidentified);

    string namePrefix(equipment->namePrefix.begin(), equipment->namePrefix.end());
    namePrefix.assign(equipment->namePrefix.begin(), equipment->namePrefix.end());
    msgEquipment->set_name_prefix(namePrefix);
    
    string nameSuffix(equipment->nameSuffix.begin(), equipment->nameSuffix.end());
    nameSuffix.assign(equipment->nameSuffix.begin(), equipment->nameSuffix.end());
    msgEquipment->set_name_suffix(nameSuffix);

    msgEquipment->set_req_level(equipment->requirements[0]);
    msgEquipment->set_req_strength(equipment->requirements[1]);
    msgEquipment->set_req_dexterity(equipment->requirements[2]);
    msgEquipment->set_req_magic(equipment->requirements[3]);
    msgEquipment->set_req_defense(equipment->requirements[4]);

    multiplayerLogger.WriteLine(Info, L"  Setting up Regular Enchants... size = %x",
      equipment->enchantList.size());
    log(L"  Setting up Regular Enchants... size = %x",
      equipment->enchantList.size());

    // New
    // Work on Regular Enchants
    for (size_t index = 0; index < equipment->enchantList.size(); index++) {
      log(L"  Adding Equipment Enchant: REGULAR");
      log(L"    (%i: %i)", equipment->enchantTypeList[index], equipment->enchantList[index]);
      multiplayerLogger.WriteLine(Info, L"  Adding Equipment Enchant: REGULAR");
      multiplayerLogger.WriteLine(Info, L"    (%i: %i)", equipment->enchantTypeList[index], equipment->enchantList[index]);

      NetworkMessages::EnchantType *msgEnchantType = msgEquipment->add_enchants();
      msgEnchantType->set_type(REGULAR);
      msgEnchantType->set_subtype(equipment->enchantTypeList[index]);
      msgEnchantType->set_value((float)equipment->enchantList[index]);
    }


    // Add the Effect Enchants
    CEffectManager *effectManager = equipment->pCEffectManager;
    if (effectManager) {
      multiplayerLogger.WriteLine(Info, L"  Setting up Effect Enchants...");
      log(L"  Setting up Effect Enchants...");

      // Work on Effect Enchants
      for (u32 i = 0; i < effectManager->effectList.size; i++) {
        CEffect *effect = effectManager->effectList[i];

        multiplayerLogger.WriteLine(Info, L"  Adding Equipment Effect Enchant: (%x: %f)", effect->effectType, effect->effectValue);
        log(L"  Adding Equipment Effect Enchant: (%x: %f)", effect->effectType, effect->effectValue);

        NetworkMessages::EnchantType *msgEnchantType = msgEquipment->add_enchants();
        msgEnchantType->set_type(effect->effectType);
        msgEnchantType->set_subtype(-1);  // Not used in this instance
        msgEnchantType->set_value(effect->effectValue);
      }
    }
  }
}

void Server::HandleEquipmentDrop(u32 equipmentId, Vector3 position, bool unk0)
{
  multiplayerLogger.WriteLine(Info, L"Server received Equipment Drop: (EquipmentId = %x, Position: %f, %f, %f, Unk0: %i",
    equipmentId, position.x, position.y, position.z, unk0);
  log(L"Server received Equipment Drop: (EquipmentId = %x, Position: %f, %f, %f, Unk0: %i",
    equipmentId, position.x, position.y, position.z, unk0);

  NetworkEntity *netEquipment = searchEquipmentByCommonID(equipmentId);

  if (netEquipment) {
    CEquipment *equipment = (CEquipment *)netEquipment->getInternalObject();
    CLevel *level = gameClient->pCLevel;

    level->EquipmentDrop(equipment, position, unk0);
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find Equipment from CommonId: %x", equipmentId);
    log(L"Error: Could not find Equipment from CommonId: %x", equipmentId);
  }
}

void Server::HandleEquipmentCreation(const SystemAddress clientAddress, TLMP::NetworkMessages::Equipment *msgEquipment)
{
  u32 id = msgEquipment->id();
  u64 guid = msgEquipment->guid();
  u32 stacksize = msgEquipment->stacksize();
  u32 stacksizemax = msgEquipment->stacksizemax();
  u32 socketcount = msgEquipment->socketcount();
  u32 gemCount = msgEquipment->gems_size();
  u32 enchantCount = msgEquipment->enchants_size();
  u32 client_id = msgEquipment->client_id();

  u32 minPhysicalDamage = msgEquipment->physical_damage_min();
  u32 maxPhysicalDamage = msgEquipment->physical_damage_max();

  wstring nameUnidentified = convertAcsiiToWide(msgEquipment->name_unidentified());
  wstring namePrefix = convertAcsiiToWide(msgEquipment->name_prefix());
  wstring nameSuffix = convertAcsiiToWide(msgEquipment->name_suffix());

  u32 reqLevel = msgEquipment->req_level();
  u32 reqStrength = msgEquipment->req_strength();
  u32 reqDexterity = msgEquipment->req_dexterity();
  u32 reqMagic = msgEquipment->req_magic();
  u32 reqDefense = msgEquipment->req_defense();


  multiplayerLogger.WriteLine(Info, L"Server received equipment creation: (CommonID = %x) (GUID = %016I64X) (Stack: %i/%i) (SocketCount: %i)",
    id, guid, stacksize, stacksizemax, socketcount);
  log(L"Server received equipment creation: (CommonID = %x) (GUID = %016I64X) (Stack: %i/%i) (SocketCount: %i)",
    id, guid, stacksize, stacksizemax, socketcount);

  // DEBUGGING
  log(L"  Test Equipment Received:");
  log(L"    min damage: %i", minPhysicalDamage);
  log(L"    max damage: %i", maxPhysicalDamage);
  log(L"    nameUn: %s", nameUnidentified.c_str());
  log(L"    namePr: %s", namePrefix.c_str());
  log(L"    nameSu: %s", nameSuffix.c_str());
  log(L"    req Level: %i", reqLevel);
  log(L"    req Str: %i", reqStrength);
  log(L"    req Dex: %i", reqDexterity);
  log(L"    req Magic: %i", reqMagic);
  log(L"    req Def: %i", reqDefense);

  // --

  CResourceManager *resourceManager = (CResourceManager *)gameClient->pCPlayer->pCResourceManager;

  if (resourceManager) {
    // Suppress the server from sending this out, we do it afterwards only for the other clients
    Server::getSingleton().SetSuppressed_SendEquipmentCreation(true);
    CEquipment *equipment = resourceManager->CreateEquipment(guid, 1, 1, 0);
    Server::getSingleton().SetSuppressed_SendEquipmentCreation(false);

    equipment->socketCount = socketcount;
    equipment->stackSize = stacksize;
    equipment->stackSizeMax = stacksizemax;
    equipment->gemList.size = 0;

    equipment->minimumPhysicalDamage = minPhysicalDamage;
    equipment->maximumPhysicalDamage = maxPhysicalDamage;
    equipment->changeMeForDamageUpdate = maxPhysicalDamage;

    equipment->nameUnidentified = nameUnidentified;
    equipment->namePrefix = namePrefix;
    equipment->nameSuffix = nameSuffix;
    equipment->requirements[0] = reqLevel;
    equipment->requirements[1] = reqStrength;
    equipment->requirements[2] = reqDexterity;
    equipment->requirements[3] = reqMagic;
    equipment->requirements[4] = reqDefense;

    multiplayerLogger.WriteLine(Info, L"Server: Adding gems to new equipment...");
    log(L"Server: Adding gems to new equipment...");

    // Add the gems
    for (u32 i = 0; i < gemCount; i++) {
      NetworkMessages::Equipment msgGem = msgEquipment->gems().Get(i);
      CEquipment *gem = resourceManager->CreateEquipment(msgGem.guid(), 1, 1, 0);
      
      if (gem) {
        multiplayerLogger.WriteLine(Info, L"Server: Adding gem to shared network equipment: %i", msgGem.id());
        log(L"Server: Adding gem to shared network equipment: %i", msgGem.id());
        NetworkEntity *newEntity = addEquipment(gem, msgGem.id());

        for (int j = 0; j < msgGem.enchants_size(); j++) {
          NetworkMessages::EnchantType msgGemEnchantType = msgGem.enchants().Get(j);
          gem->AddEnchant((EffectType)msgGemEnchantType.type(), (EnchantType)msgGemEnchantType.subtype(), msgGemEnchantType.value());
        }

        equipment->gemList.push(gem);

        //
        // Same as below, broadcast and send this gem ID to our client and others
        //
        multiplayerLogger.WriteLine(Info, L"Server: Broadcasting new gem to all other clients.");
        log(L"Server: Broadcasting new gem to all other clients.");

        // Send the Equipment w/ CommonID back to the clients
        // Simply change the slot ID on the message we received and send it back out to all other clients
        msgGem.set_id(newEntity->getCommonId());
        Server::getSingleton().BroadcastMessage<NetworkMessages::Equipment>(clientAddress, S_PUSH_NEWEQUIPMENT, &msgGem);

        multiplayerLogger.WriteLine(Info, L"Server: Sending gem common ID to owner Client of equipment.");
        log(L"Server: Sending gem common ID to owner Client of equipment.");

        // Send a special equipment common ID message to the client that sent us their equipment
        NetworkMessages::EquipmentSetID msgEquipmentSetID;
        msgEquipmentSetID.set_guid(msgGem.guid());
        msgEquipmentSetID.set_client_id(msgGem.client_id());
        msgEquipmentSetID.set_id(newEntity->getCommonId());
        Server::getSingleton().SendMessage<NetworkMessages::Equipment>(clientAddress, S_REPLY_EQUIPMENT_ID, &msgEquipmentSetID);
      } else {
        multiplayerLogger.WriteLine(Error, L"Could not create gem of GUID: %016I64X", msgGem.guid());
        log(L"Could not create gem of GUID: %016I64X", msgGem.guid());
      }
    }

    multiplayerLogger.WriteLine(Info, L"Server: Adding enchants to new equipment...: %i", enchantCount);
    log(L"Server: Adding enchants to new equipment...: %i", enchantCount);

    // Add the enchants
    for (u32 i = 0; i < enchantCount; i++) {
      NetworkMessages::EnchantType msgEnchantType = msgEquipment->enchants().Get(i);

      multiplayerLogger.WriteLine(Info, L"  EnchantType: %x %x  Value: %f",
        (EffectType)msgEnchantType.type(), (EnchantType)msgEnchantType.subtype(), msgEnchantType.value());
      log(L"  EnchantType: %x %x  Value: %f",
        (EffectType)msgEnchantType.type(), (EnchantType)msgEnchantType.subtype(), msgEnchantType.value());

      equipment->AddEnchant((EffectType)msgEnchantType.type(), (EnchantType)msgEnchantType.subtype(), msgEnchantType.value());
    }

    multiplayerLogger.WriteLine(Info, L"Server: Adding equipment to shared network equipment");
    log(L"Server: Adding equipment to shared network equipment");
    NetworkEntity *newEntity = addEquipment(equipment);

    multiplayerLogger.WriteLine(Info, L"Server: Broadcasting new equipment to all other clients.");
    log(L"Server: Broadcasting new equipment to all other clients.");

    // Send the Equipment w/ CommonID back to the clients
    // Simply change the slot ID on the message we received and send it back out to all other clients
    msgEquipment->set_id(newEntity->getCommonId());
    Server::getSingleton().BroadcastMessage<NetworkMessages::Equipment>(clientAddress, S_PUSH_NEWEQUIPMENT, msgEquipment);

    multiplayerLogger.WriteLine(Info, L"Server: Sending equipment common ID to owner Client of equipment.");
    log(L"Server: Sending equipment common ID to owner Client of equipment.");

    // Send a special equipment common ID message to the client that sent us their equipment
    NetworkMessages::EquipmentSetID msgEquipmentSetID;
    msgEquipmentSetID.set_guid(guid);
    msgEquipmentSetID.set_client_id(client_id);
    msgEquipmentSetID.set_id(newEntity->getCommonId());
    Server::getSingleton().SendMessage<NetworkMessages::Equipment>(clientAddress, S_REPLY_EQUIPMENT_ID, &msgEquipmentSetID);
  }
}

void Server::HandleEquipmentPickup(u32 characterId, u32 equipmentId)
{
  NetworkEntity *netEquipment = searchEquipmentByCommonID(equipmentId);
  NetworkEntity *netCharacter = searchCharacterByCommonID(characterId);

  if (netEquipment) {
    if (netCharacter) {
      CEquipment *equipment = (CEquipment *)netEquipment->getInternalObject();
      CCharacter *character = (CCharacter *)netCharacter->getInternalObject();

      character->PickupEquipment(equipment, gameClient->pCLevel);
    } else {
      multiplayerLogger.WriteLine(Error, L"Error: Could not find Character from CommonId: %x", characterId);
      log(L"Error: Could not find Character from CommonId: %x", characterId);
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find Equipment from CommonId: %x", equipmentId);
    log(L"Error: Could not find Equipment from CommonId: %x", equipmentId);
  }
}

void Server::HandleInventoryRemoveEquipment(const SystemAddress clientAddress, u32 ownerId, u32 equipmentId)
{
  multiplayerLogger.WriteLine(Info, L"Server received inventory remove equipment: (CharacterID = %x, EquipmentID = %x)",
    ownerId, equipmentId);
  log(L"Server received inventory remove equipment: (CharacterID = %x, EquipmentID = %x)",
    ownerId, equipmentId);

  NetworkEntity* owner = searchCharacterByCommonID(ownerId);
  NetworkEntity* equipment = searchEquipmentByCommonID(equipmentId);

  if (owner) {
    if (equipment) {
      CCharacter *characterOwner = (CCharacter*)owner->getInternalObject();
      CEquipment *equipmentReal = (CEquipment*)equipment->getInternalObject();

      CInventory *inventory = characterOwner->pCInventory;

      // Suppress this and send it manually
      Server::getSingleton().SetSuppressed_SendEquipmentUnequip(true);
      inventory->RemoveEquipment(equipmentReal);
      Server::getSingleton().SetSuppressed_SendEquipmentUnequip(false);

      // And send to other clients
      // Create a Network Message for sending off to clients the equipment addition to the inventory
      NetworkMessages::InventoryRemoveEquipment msgInventoryRemoveEquipment;
      msgInventoryRemoveEquipment.set_ownerid(owner->getCommonId());
      msgInventoryRemoveEquipment.set_equipmentid(equipment->getCommonId());

      Server::getSingleton().BroadcastMessage<NetworkMessages::InventoryRemoveEquipment>(clientAddress, S_PUSH_EQUIPMENT_UNEQUIP, &msgInventoryRemoveEquipment);
    } else {
      multiplayerLogger.WriteLine(Error, L"Error: Could not find Equipment with ID = %x", equipmentId);
      log(L"Error: Could not find Equipment with ID = %x", equipmentId);
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find Character with ID = %x", ownerId);
    log(L"Error: Could not find Character with ID = %x", ownerId);
  }
}

void Server::HandleCharacterSetAction(const SystemAddress clientAddress, NetworkMessages::CharacterAction* msgCharacterAction)
{
  u32 id = msgCharacterAction->characterid();
  u32 action = msgCharacterAction->action();

  NetworkEntity *networkCharacter = searchCharacterByCommonID(id);

  if (networkCharacter) {
    CCharacter* character = (CCharacter*)networkCharacter->getInternalObject();
    character->SetAction(action);
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find character with common id = %x", id);
    log(L"Error: Could not find character with common id = %x", id);
  }
}

void Server::HandleCharacterAttack(NetworkMessages::CharacterAttack* msgCharacterAttack)
{
  u32 id = msgCharacterAttack->characterid();

  NetworkEntity *networkCharacter = searchCharacterByCommonID(id);

  if (networkCharacter) {
    CCharacter* character = (CCharacter*)networkCharacter->getInternalObject();

    character->Attack();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find character with common id = %x", id);
    log(L"Error: Could not find character with common id = %x", id);
  }
}

void Server::HandleCharacterUseSkill(NetworkMessages::CharacterUseSkill* msgCharacterUseSkill)
{
  u32 id = msgCharacterUseSkill->characterid();
  u64 skill = msgCharacterUseSkill->skill();

  NetworkEntity *networkCharacter = searchCharacterByCommonID(id);

  if (networkCharacter) {
    CCharacter* character = (CCharacter*)networkCharacter->getInternalObject();

    // Stop the suppress and network request
    character->UseSkill(skill);
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find character with common id = %x", id);
    log(L"Error: Could not find character with common id = %x", id);
  }
}

void Server::RemoveDestroyedEquipmentFromNetwork()
{
  vector<NetworkEntity*>::iterator itr;

  // Search for destroyed equipment
  bool clean = false;
  while (!clean) {
    clean = true;

    for (itr = NetworkSharedEquipment->begin(); itr != NetworkSharedEquipment->end(); itr++) {
      CEquipment *equipment = (CEquipment*)(*itr)->getInternalObject();

      if (equipment->destroy) {
        NetworkSharedEquipment->erase(itr);
        clean = false;
        break;
      }
    }
  }

}

void Server::HandleEquipmentUpdateStack(const SystemAddress clientAddress, NetworkMessages::EquipmentUpdateStackSize *msgEquipmentUpdateStackSize)
{
  u32 id = msgEquipmentUpdateStackSize->id();
  u32 amount = msgEquipmentUpdateStackSize->change_amount();

  NetworkEntity *netEquipment = searchEquipmentByCommonID(id);

  if (netEquipment) {
    CEquipment *equipment = (CEquipment*)netEquipment->getInternalObject();

    // Suppress the server from sending it
    SetSuppressed_SendEquipmentStack(true);
    EquipmentAddStackCount(equipment, amount);
    SetSuppressed_SendEquipmentStack(false);

    // Manually send it to the other clients
    Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentUpdateStackSize>(clientAddress, S_PUSH_EQUIPMENT_STACKUPDATE, msgEquipmentUpdateStackSize);

  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find equipment with common id = %x", id);
    log(L"Error: Could not find equipment with common id = %x", id);
  }
}

void Server::HandleEquipmentAddGem(const SystemAddress address, NetworkMessages::EquipmentAddGem* msgEquipmentAddGem)
{
  u32 equipmentId = msgEquipmentAddGem->equipmentid();
  u32 gemId = msgEquipmentAddGem->gemid();

  NetworkEntity *netEquipment = searchEquipmentByCommonID(equipmentId);
  NetworkEntity *netGem = searchEquipmentByCommonID(gemId);

  if (netEquipment && netGem) {
    CEquipment *equipment = (CEquipment*)netEquipment->getInternalObject();
    CEquipment *gem = (CEquipment*)netGem->getInternalObject();

    equipment->AddGem(gem);
  } else {
    log(L"Equipment or Gem commonId not found: %x %x  ptrs: %p %p",
      equipmentId, gemId, netEquipment, netGem);
    multiplayerLogger.WriteLine(Error, L"Equipment or Gem commonId not found: %x %x  ptrs: %p %p",
      equipmentId, gemId, netEquipment, netGem);
  }
}

void Server::HandleEquipmentRemoveGems(NetworkMessages::EquipmentRemoveGems *msgEquipmentRemoveGems)
{
  u32 equipmentId = msgEquipmentRemoveGems->equipmentid();

  NetworkEntity *netEntity = searchEquipmentByCommonID(equipmentId);

  if (netEntity) {
    CEquipment* equipment = (CEquipment*)netEntity->getInternalObject();
    equipment->gemList.size = 0;

    // Broadcast it back out
    Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentRemoveGems>(S_PUSH_EQUIPMENT_REMOVE_GEMS, msgEquipmentRemoveGems);
  } else {
    log(L"Error: Could not find NetworkEntity for equipment of id: %x", equipmentId);
    multiplayerLogger.WriteLine(Error, L"Error: Could not find NetworkEntity for equipment of id: %x", equipmentId);
  }
}

void Server::Helper_RemoveBaseUnit(CBaseUnit* unit)
{
  log(L"Server: Removing BaseUnit (%p)  %016I64X", unit, unit->GUID);
  multiplayerLogger.WriteLine(Info, L"Server: Removing BaseUnit (%p)  %016I64X", unit, unit->GUID);

  unit->destroy = true;
}

void Server::OnClientConnected(void *args)
{
  
}

void Server::OnClientDisconnected(void *args)
{
  SystemAddress address = *(SystemAddress*)args;

  map<SystemAddress, vector<NetworkEntity*>*>::iterator itr;

  // Search for an existing address
  for (itr = Server_ClientCharacterMapping->begin(); itr != Server_ClientCharacterMapping->end(); itr++) {
    if ((*itr).first == address) {
      vector<NetworkEntity*>::iterator itr2;
      for (itr2 = (*itr).second->begin(); itr2 != (*itr).second->end(); itr2++) {
        if (*itr2) {
          CCharacter *character = (CCharacter*)(*itr2)->getInternalObject();
          if (character) {
            Helper_RemoveBaseUnit(character);
          }
        }
      }
      break;
    }
  }
}

void Server::HandleAddClientCharacter(const SystemAddress address, CCharacter* character)
{
  map<SystemAddress, vector<NetworkEntity*>*>::iterator itr;

  // Search for an existing address
  bool found = false;
  for (itr = Server_ClientCharacterMapping->begin(); itr != Server_ClientCharacterMapping->end(); itr++) {
    if ((*itr).first == address) {
      found = true;
      break;
    }
  }

  // Add the new key
  if (!found) {
    (*Server_ClientCharacterMapping)[address] = new vector<NetworkEntity*>();
  }

  // Find the NetworkEntity for the character
  NetworkEntity *netCharacter = searchCharacterByInternalObject(character);

  // We now have a vector at the key, add the character
  if (netCharacter) {
    (*Server_ClientCharacterMapping)[address]->push_back(netCharacter);
  } else {
    log(L"Error: Could not add character to Server_ClientCharacterMapping - Could not find network entity.");
    multiplayerLogger.WriteLine(Error, L"Error: Could not add character to Server_ClientCharacterMapping - Could not find network entity.");
  }
}