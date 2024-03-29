#include <windows.h>

#include "Multiplayer.h"
using namespace TLMP;


TLMP::Logger TLMP::multiplayerLogger;
TLAPI::CGameClient *TLMP::gameClient = NULL;
bool TLMP::LevelLoading = true;

void TLMP::SetupNetwork()
{
  multiplayerLogger.SetLoggingLevel(Info);
  multiplayerLogger.WriteLine(Info, L"Torchlight Multiplayer Log");
  multiplayerLogger.WriteLine(Info, L"Registering Events...");

  //
  // Register event callbacks we need
  _GLOBAL::RegisterEvent_WndProc(WndProcPre, NULL);

  CInventoryMenu::RegisterEvent_InventoryMenu_OpenClose(InventoryMenu_OpenClosePre, NULL);
  CInventoryMenu::RegisterEvent_InventoryMenu_MouseEvent(InventoryMenu_MouseEventPre, NULL);

  CCharacterSaveState::RegisterEvent_CharacterSaveState_LoadFromFile(NULL, CharacterSaveState_ReadFromFile);

  CPath::RegisterEvent_Path_GetNextNode(Path_GetNextNodePre, Path_GetNextNodePost);

  CGame::RegisterEvent_GameCtor(Game_Ctor, NULL);
  CGame::RegisterEvent_Game_CreateUI(NULL, GameClient_CreateUI);

  CBaseUnit::RegisterEvent_BaseUnit_AddSkill(BaseUnit_AddSkillPre, BaseUnit_AddSkillPost);

  CEffect::RegisterEvent_Effect_Effect_Something(Effect_EffectSomethingPre, Effect_EffectSomethingPost);
  CEffect::RegisterEvent_Effect_Something0(Effect_Something0Pre, NULL);
  CEffect::RegisterEvent_Effect_ParamCtor(Effect_Effect_ParamCtorPre, NULL);
  //CEffect::RegisterEvent_Effect_CopyCtor(Effect_CtorPre, Effect_CtorPost);
  //CEffect::RegisterEvent_Effect_Character_Unk0(Effect_Character_Unk0Pre, Effect_Character_Unk0Post);

  CEffectManager::RegisterEvent_EffectManagerCreateEffect(Effect_EffectManagerCreateEffectPre, Effect_EffectManagerCreateEffectPost);
  CEffectManager::RegisterEvent_EffectManager_AddEffectToEquipment(EffectManager_AddEffectToEquipmentPre, EffectManager_AddEffectToEquipmentPost);

  CResourceManager::RegisterEvent_ResourceManagerCreatePlayer(CreatePlayer, NULL);
  CResourceManager::RegisterEvent_ResourceManagerCreateMonster(CreateMonsterPre, CreateMonsterPost);
  CResourceManager::RegisterEvent_ResourceManagerCreateItem(CreateItemPre, CreateItemPost);

  CLevel::RegisterEvent_LevelCharacterInitialize(Level_CharacterInitializePre, Level_CharacterInitializePost);
  CLevel::RegisterEvent_LevelDropItem(Level_DropItemPre, Level_DropItemPost);
  CLevel::RegisterEvent_Level_Dtor(Level_Dtor, NULL);
  CLevel::RegisterEvent_Level_Ctor(Level_Ctor, NULL);
  CLevel::RegisterEvent_Level_Update(Level_UpdatePre, Level_UpdatePost);
  CLevel::RegisterEvent_Level_CharacterKilledCharacter(Level_Level_CharacterKilledCharacterPre, Level_Level_CharacterKilledCharacterPost);
  CLevel::RegisterEvent_Level_RemoveEquipment(Level_Level_RemoveEquipmentPre, Level_Level_RemoveEquipmentPost);
  CLevel::RegisterEvent_Level_CheckCharacterProximity(Level_CheckCharacterProximityPre, Level_CheckCharacterProximityPost);
  CLevel::RegisterEvent_Level_RemoveCharacter(Level_RemoveCharacterPre, NULL);
  CLevel::RegisterEvent_Level_RemoveItem(Level_RemoveItemPre, Level_RemoveItemPost);

  CGameClient::RegisterEvent_GameClientCtor(NULL, GameClient_Ctor);
  CGameClient::RegisterEvent_GameClientProcessObjects(GameClient_ProcessObjects, NULL);
  CGameClient::RegisterEvent_GameClientProcessTitleScreen(NULL, GameClient_TitleProcessObjects);
  CGameClient::RegisterEvent_GameClient_CreateLevel(GameClient_CreateLevelPre, GameClient_CreateLevelPost);
  CGameClient::RegisterEvent_GameClient_LoadLevel(GameClient_LoadLevelPre, GameClient_LoadLevelPost);
  CGameClient::RegisterEvent_GameClient_LoadMap(GameClient_LoadMapPre, GameClient_LoadMapPost);
  CGameClient::RegisterEvent_GameClient_SaveGame(GameClientSaveGamePre, GameClientSaveGamePost);
  CGameClient::RegisterEvent_GameClientGamePaused(NULL, GameClientGamePausedPost);
  CGameClient::RegisterEvent_GameClient_ChangeLevel(GameClient_ChangeLevelPre, GameClient_ChangeLevelPost);

  CMainMenu::RegisterEvent_MainMenu_Event(MainMenuEventPre, NULL);

  CEquipment::RegisterEvent_EquipmentDtor(Equipment_DtorPre, Equipment_DtorPost);
  CEquipment::RegisterEvent_EquipmentInitialize(EquipmentInitialize, NULL);
  CEquipment::RegisterEvent_EquipmentAddStackCount(NULL, EquipmentAddStackCountPost);
  CEquipment::RegisterEvent_Equipment_AddGem(EquipmentAddGemPre, NULL);
  CEquipment::RegisterEvent_EquipmentIdentify(EquipmentIdentifyPre, NULL);
  CEquipment::RegisterEvent_EquipmentEnchant(Equipment_EnchantPre, NULL);

  CEquipmentRef::RegisterEvent_EquipmentRef_Dtor(EquipmentRefDtorPre, EquipmentRefDtorPost);

  CEnchantMenu::RegisterEvent_EnchantMenu_EnchantItem(EnchantMenuEnchantItemPre, NULL);

  CItemGold::RegisterEvent_ItemGold_Ctor(ItemGold_CtorPre, ItemGold_CtorPost);

  CMonster::RegisterEvent_MonsterProcessAI(Monster_ProcessAIPre, NULL);
  CMonster::RegisterEvent_MonsterProcessAI2(Monster_ProcessAI2Pre, NULL);
  CMonster::RegisterEvent_MonsterProcessAI3(Monster_ProcessAI3Pre, NULL);
  CMonster::RegisterEvent_MonsterIdle(Monster_Idle, NULL);
  CMonster::RegisterEvent_MonsterGetCharacterClose(Monster_GetCharacterClosePre, Monster_GetCharacterClosePost);

  CCharacter::RegisterEvent_CharacterDtor(Character_DtorPre, Character_DtorPost);
  CCharacter::RegisterEvent_CharacterSetAction(Character_SetActionPre, NULL);
  CCharacter::RegisterEvent_CharacterSetAlignment(Character_SetAlignmentPre, Character_SetAlignmentPost);
  CCharacter::RegisterEvent_CharacterAttack(Character_AttackPre, Character_AttackPost);
  CCharacter::RegisterEvent_CharacterSetTarget(Character_SetTarget, NULL);
  CCharacter::RegisterEvent_CharacterUseSkill(Character_UseSkillPre, Character_UseSkillPost);
  CCharacter::RegisterEvent_CharacterSetDestination(NULL, Character_SetDestination);
  CCharacter::RegisterEvent_CharacterPickupEquipment(Character_PickupEquipmentPre, Character_PickupEquipmentPost);
  CCharacter::RegisterEvent_Character_Update(Character_Character_UpdatePre, Character_Character_UpdatePost);
  CCharacter::RegisterEvent_Character_SetOrientation(Character_SetOrientationPre, NULL);
  CCharacter::RegisterEvent_Character_UpdateOrientation(Character_UpdateOrientationPre, NULL);
  CCharacter::RegisterEvent_CharacterSetupSkills(Character_SetupSkillsPre, NULL);
  CCharacter::RegisterEvent_CharacterAddSkill(Character_AddSkillPre, Character_AddSkillPost);
  CCharacter::RegisterEvent_CharacterAddSkill2(Character_AddSkill2Pre, NULL);
  CCharacter::RegisterEvent_CharacterUpdateHealth(Character_UpdateHealthPre, NULL);
  CCharacter::RegisterEvent_CharacterStrike(Character_StrikePre, NULL);
  CCharacter::RegisterEvent_PlayerResurrect(Character_ResurrectPre, NULL);
  CCharacter::RegisterEvent_Character_Update_Level(Character_Update_LevelPre, NULL);
  CCharacter::RegisterEvent_Character_Update_Character(Character_Update_CharacterPre, NULL);
  CCharacter::RegisterEvent_Player_KillMonsterExperience(Character_Player_KillMonsterExperiencePre, Character_Player_KillMonsterExperiencePost);
  CCharacter::RegisterEvent_Character_Killed(Character_KilledPre, Character_KilledPost);
  CCharacter::RegisterEvent_Player_SwapWeapons(Player_SwapWeaponsPre, NULL);

  CPlayer::RegisterEvent_PlayerLevelUp(Player_LevelUpPre, Player_LevelUpPost);

  CAutomap::RegisterEvent_Automap_AddBillboard(Automap_AddBillboardPre, NULL);

  CTriggerUnit::RegisterEvent_TriggerUnitTriggered(TriggerUnit_TriggeredPre, NULL);
  CTriggerUnit::RegisterEvent_TriggerUnit_Ctor(NULL, TriggerUnit_CtorPost);
  //CTriggerUnit::RegisterEvent_TriggerUnit_Triggered2(TriggerUnit_Triggered2Pre, NULL);

  CBreakable::RegisterEvent_BreakableTriggered(Breakable_TriggeredPre, NULL);
  
  //_GLOBAL::RegisterEvent_SetSeedValue0(Global_SetSeedValue0Pre, Global_SetSeedValue0Post);
  //_GLOBAL::RegisterEvent_SetSeedValue0(Global_SetSeedValue0Pre, NULL);
  _GLOBAL::RegisterEvent_SetSeedValue0(Global_SetSeedValue0Pre, Global_SetSeedValue0Post);
  _GLOBAL::RegisterEvent_SetSeedValue2(Global_SetSeedValue2Post, Global_SetSeedValue2Post);

  CSkillManager::RegisterEvent_SkillManagerAddSkill(SkillManager_AddSkillPre, NULL);
  CSkillManager::RegisterEvent_SkillManagerSetSkillLevel(SkillManager_SetSkillLevelPre, SkillManager_SetSkillLevelPost);

  CQuestManager::RegisterEvent_QuestManagerSetQuestCompleted(QuestManager_SetQuestCompletedPre, QuestManager_SetQuestCompletedPost);

  CInventory::RegisterEvent_InventoryAddEquipment(Inventory_AddEquipmentPre, Inventory_AddEquipmentPost);
  CInventory::RegisterEvent_InventoryRemoveEquipment(Inventory_RemoveEquipmentPre, Inventory_RemoveEquipmentPost);
  CInventory::RegisterEvent_Inventory_EquipmentAutoEquip(Inventory_EquipmentAutoEquipPre, NULL);

  CGameUI::RegisterEvent_GameUI_TriggerPause(GameUI_TriggerPausePre, NULL);
  CGameUI::RegisterEvent_GameUI_HandleKeyboardInput(GameUI_HandleKeyboardInputPre, GameUI_HandleKeyboardInputPost);
  CGameUI::RegisterEvent_GameUI_WindowResized(NULL, GameUI_WindowResizedPost);  

  CKeyManager::RegisterEvent_KeyManager_InjectKey(KeyManager_HandleInputPre, NULL);
  CMouseManager::RegisterEvent_MouseManagerInput(MouseManager_HandleInputPre, NULL);

  CPositionableObject::RegisterEvent_PositionableObject_SetNearPlayer(PositionableObject_SetNearPlayerPre, NULL);

  CParticleCache::RegisterEvent_ParticleCache_Dtor2(ParticleCache_Dtor2Pre, ParticleCache_Dtor2Post);

  CGenericModel::RegisterEvent_GenericModel_Dtor(GenericModel_DtorPre, GenericModel_DtorPost);
  // --

  multiplayerLogger.WriteLine(Info, L"Registering Events... Done.");

  Server::getSingleton().SetCallback_OnClientConnect(&ServerOnClientConnected);
  Server::getSingleton().SetCallback_OnClientDisconnect(&ServerOnClientDisconnect);
  Client::getSingleton().SetCallback_OnConnected(&ClientOnConnect);

  Hook(GetProcAddress(GetModuleHandle("OgreMain.dll"), "?isActive@RenderWindow@Ogre@@UBE_NXZ"), OgreIsActive, 0, HOOK_THISCALL, 0);

  const char *mangledResourceLocationFunc = "?addResourceLocation@ResourceGroupManager@Ogre@@QAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@00_N@Z";
  void (__thiscall *addResourceLocation)(PVOID, Ogre::String&, Ogre::String&, Ogre::String&, bool) = 
    (void (__thiscall *)(PVOID, Ogre::String&, Ogre::String&, Ogre::String&, bool))GetProcAddress(GetModuleHandle("OgreMain.dll"), mangledResourceLocationFunc);
  Hook(addResourceLocation, 0, OgreAddResourceLocation, HOOK_THISCALL, 4);

  log(L"Done.");
}

void TLMP::OgreAddResourceLocation STDARG
{
  static bool setup = false;
  Ogre::String *name = *(Ogre::String**)&Pz[0];
  Ogre::String *loc = *(Ogre::String**)&Pz[1];
  Ogre::String *resGroup = *(Ogre::String**)&Pz[2];
  
  /*
  printf("DEBUG:\n");
  for (int i = 0; i < 16; i++) {
	char *temp = (char*)name + i;
	printf("%c %x\n", *temp, *temp);
  }
  printf("\n");
  */

  /*
  // Setup the resource location for our TLAPI folder to correctly load our UI layouts
  if (!setup) {
    const char *mangledResourceLocationFunc = "?addResourceLocation@ResourceGroupManager@Ogre@@QAEXABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@00_N@Z";
    void (__thiscall *addResourceLocation)(PVOID, std::string&, std::string&, std::string&, bool) = 
      (void (__thiscall *)(PVOID, std::string&, std::string&, std::string&, bool))GetProcAddress(GetModuleHandle("OgreMain.dll"), mangledResourceLocationFunc);

    setup = true;
    addResourceLocation(e->_this, std::string("TLAPI/"), std::string("FileSystem"), std::string("General"), false);
  }
  */

  //log("AddResourceLocation: (%p) (%s %s %s)", e->_this, name->c_str(), loc->c_str(), resGroup->c_str());

  if (!setup) {
	  setup = true;

	  //log(L"Adding Ogre Resource Location...");

	  try {
		  Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(Ogre::String("TLAPI/"), Ogre::String("FileSystem"));
      Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(Ogre::String("TLAPI/ogretray"), Ogre::String("FileSystem"));
	  } catch (Ogre::ItemIdentityException &ex) {
		  log("Ogre Exception: %s", ex.what());
	  }
	  
	  //log(L"Done.\n");
  }

 
}

void TLMP::CharacterSaveState_ReadFromFile(CCharacterSaveState* saveState, PVOID file, u32 unk)
{
  /*
  // Dump character save state
  multiplayerLogger.WriteLine(Info, L"CharacterSaveState_ReadFromFile: %s (GUID: %016I64X)",
    saveState->name.c_str(), saveState->guid2);

  multiplayerLogger.WriteLine(Info, L"  Strength: %i", saveState->strength);
  multiplayerLogger.WriteLine(Info, L"  Dexterity: %i", saveState->dexterity);
  multiplayerLogger.WriteLine(Info, L"  Defense: %i", saveState->defense);
  multiplayerLogger.WriteLine(Info, L"  Magic: %i", saveState->magic);
  multiplayerLogger.WriteLine(Info, L"  Gold: %i", saveState->gold);

  multiplayerLogger.WriteLine(Info, L"  Spell 1: %s", saveState->nameSpell1.c_str());
  multiplayerLogger.WriteLine(Info, L"  Spell 2: %s", saveState->nameSpell2.c_str());
  multiplayerLogger.WriteLine(Info, L"  Spell 3: %s", saveState->nameSpell3.c_str());
  multiplayerLogger.WriteLine(Info, L"  Spell 4: %s", saveState->nameSpell4.c_str());

  // Dump character's item save states
  CItemSaveState **itr = saveState->itemSaveStateBegin;
  CItemSaveState **itrEnd = saveState->itemSaveStateEnd1;

  while (itr != itrEnd) {
    multiplayerLogger.WriteLine(Info, L"    Item: %s (GUID: %016I64X) Slot: %i",
      (*itr)->name.c_str(), (*itr)->guid2, (*itr)->slot);

    multiplayerLogger.WriteLine(Info, L"      Prefix: %s",
      (*itr)->name2.c_str());
    multiplayerLogger.WriteLine(Info, L"      Suffix: %s",
      (*itr)->name3.c_str());

    for (int i = 0; i < 6; i++) {
      multiplayerLogger.WriteLine(Info, L"      Unk1[%i]: %x",
        i, (*itr)->unk1[i]);
    }

    for (int i = 0; i < 30; i++) {
      multiplayerLogger.WriteLine(Info, L"      Unk2[%i]: %x",
        i, (*itr)->unk2[i]);
    }

    multiplayerLogger.WriteLine(Info, L"");

    itr++;
  }
  */
}

void TLMP::Equipment_DtorPre(CEquipment* equipment)
{
  //log(L"Equipment::DtorPre = %p Removing from network list... %s", equipment, equipment->nameReal.c_str());

  vector<NetworkEntity*>::iterator itr;
  for (itr = NetworkSharedEquipment->begin(); itr != NetworkSharedEquipment->end(); itr++) {
    CEquipment *equipment2 = (CEquipment*)(*itr)->getInternalObject();
    if (equipment == equipment2) {
      NetworkSharedEquipment->erase(itr);
      break;
    }
  }

  // Attempt to duct-tape a bug with item duping, move across our player's inventory
  // and cleanup the equipment in any equipmentRef's that are tied to the equipment
  if (gameClient) {
    CPlayer* player = gameClient->pCPlayer;

    // Check dups against player inventory
    if (player) {
      CInventory *inventory = player->pCInventory;
      if (inventory) {
        for (u32 i = 0; i < inventory->equipmentList.size; i++) {
          CEquipmentRef *equipmentRef = inventory->equipmentList[i];
          if (equipmentRef) {
            if (equipmentRef->pCEquipment == equipment) {
              // Null it so the destructor plays nice
              equipmentRef->pCEquipment = NULL;
            }
          }
        }
      }
    }
  }
}

void TLMP::Equipment_DtorPost(CEquipment* equipment)
{
  // Clear the vtable just in case to aid in debugging - it will should crash outright if accessing later
  *((u32*)equipment) = NULL;

  //log(L"Equipment::DtorPost = %p", equipment);
}

void TLMP::Character_DtorPre(CCharacter* character)
{
  log(L"Character::Dtor = %p", character);
  log(L"  %s", character->characterName.c_str());
  multiplayerLogger.WriteLine(Info, L"Character::Dtor = %p", character);
  multiplayerLogger.WriteLine(Info, L"  %s", character->characterName.c_str());

  // If we're the server, send the client that the character has been destroyed
  if (NetworkState::getSingleton().GetState() == SERVER) {
    NetworkEntity *entity = searchCharacterByInternalObject(character);
    if (entity) {
      NetworkMessages::CharacterDestroy msgCharacterDestroy;
      msgCharacterDestroy.set_characterid(entity->getCommonId());

      Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterDestroy>(S_PUSH_CHAR_DESTROY, &msgCharacterDestroy);
    } else {
      //log(L"Server: Error could not find network ID for character destroyed: %p", character);
    }
  }

  // Remove it from our list
  vector<NetworkEntity*>::iterator itr;
  for (itr = NetworkSharedCharacters->begin(); itr != NetworkSharedCharacters->end(); itr++) {
    CCharacter* charItem = (CCharacter*)((*itr)->getInternalObject());
    if (charItem == character) {
      //log(L"  Found and removed character");
      NetworkSharedCharacters->erase(itr);
      break;
    }
  }
}

void TLMP::Character_DtorPost(CCharacter* character)
{
  log(L"Character::DtorPost = %p", character);
  multiplayerLogger.WriteLine(Info, L"Character::DtorPost = %p", character);
}

void TLMP::Character_SetAlignmentPre(CCharacter* character, u32 alignment, bool& calloriginal)
{
  //log(L"CharacterAlignment Pre: %x", character->alignment);
}

void TLMP::Character_SetAlignmentPost(CCharacter* character, u32 alignment, bool& calloriginal)
{
  //log(L"CharacterAlignment Post: %x", character->alignment);
  //log(L"Setting Character Alignment = %x", alignment);

  multiplayerLogger.WriteLine(Info, L"Setting Character (%s) Alignment = %x",
    character->characterName.c_str(), alignment);
  //log(L"Setting Character (%s) Alignment = %x",
  //  character->characterName.c_str(), alignment);

  // Client would never send alignment, just worry about the server
  if (NetworkState::getSingleton().GetState() == SERVER) {
    NetworkEntity *entity = searchCharacterByInternalObject(character);

    if (entity) {
      NetworkMessages::CharacterAlignment msgCharacterAlignment;
      msgCharacterAlignment.set_characterid(entity->getCommonId());
      msgCharacterAlignment.set_alignment(alignment);

      Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterAlignment>(S_PUSH_CHARACTER_ALIGNMENT, &msgCharacterAlignment);
    } else {
      //log(L"Error: Cannot send alignment, could not find entity ID");
    }
  }

  else if (NetworkState::getSingleton().GetState() == CLIENT) {
  }
}

void TLMP::Game_Ctor(CGame* game)
{
  //log(L"Game::Ctor = %p", game);
}

void TLMP::GameClient_Ctor(CGameClient* client)
{
  //log(L"GameClient::Ctor = %p", client);

  gameClient = client;
}

void TLMP::CreatePlayer(CPlayer* character, CResourceManager* resourceManager, wchar_t* type, u32 unk0, bool & calloriginal)
{
  //logColor(B_RED, L"CreatePlayer: %s %x", type, unk0);
}

void TLMP::Character_UpdateHealthPre(CCharacter* character, float amount, bool& calloriginal)
{
  if (amount < 0 || (character->type__ == 0x1C && amount > 0)) {
    //logColor(B_RED, L"Character (%p) update health", character);
    //logColor(B_RED, L"Character (%s) Update health (%f)", character->characterName.c_str(), amount);

    // Ignore the following to display updated health correctly for client
    // Don't update the health on the client unless the server has sent it
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      if (!Client::getSingleton().GetAllow_HealthUpdate()) {
        //calloriginal = false;
      }
    } else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
      NetworkEntity *entity = searchCharacterByInternalObject(character);

      if (entity) {
        NetworkMessages::CharacterUpdateHealth msgCharacterUpdateHealth;
        msgCharacterUpdateHealth.set_amount(amount);
        msgCharacterUpdateHealth.set_current_health(character->healthCurrent);
        msgCharacterUpdateHealth.set_max_health(character->healthMax);
        msgCharacterUpdateHealth.set_characterid(entity->getCommonId());

        Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterUpdateHealth>(S_PUSH_CHAR_UPDATE_HEALTH, &msgCharacterUpdateHealth);
      } else {
        //log(L"Error: Could not find network ID for character");
      }
    }
  }
}

void TLMP::CreateMonsterPre(CMonster*& character, CResourceManager* resourceManager, u64 guid, u32 level, bool noItems, bool & calloriginal)
{
  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    multiplayerLogger.WriteLine(Info, L"Creating character: %016I64X %i", guid, noItems);
    //log(L"Creating character: %016I64X %i", guid, noItems);

    if (Client::getSingleton().GetSuppressed_CharacterCreation()) {
      if (guid != CAT && guid != DOG && guid != DESTROYER && guid != ALCHEMIST && 
        guid != VANQUISHER && guid != BRINK && guid != STASH && guid != SHAREDSTASH)
      {
        //calloriginal = false;
        //character = NULL;
      }
    }
  }
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    multiplayerLogger.WriteLine(Info, L"Created character with guid of: %016I64X (%p)", guid, character);
  }


  if (calloriginal) {
    multiplayerLogger.WriteLine(Info, L"Creating character: %016I64X %i", guid, gameClient->flagLevelLoading);
    //log(L"Creating character: %016I64X %i %i %i %i", guid,
    //  gameClient->flagLevelLoading, gameClient->unkFlag1, gameClient->unkFlag2, gameClient->unkFlag3);
  }
}

void TLMP::CreateMonsterPost(CMonster*& character, CResourceManager* resourceManager, u64 guid, u32 level, bool unk0, bool &)
{
}

void TLMP::CreateItemPre(CItem* item, CResourceManager* resourceManager, u64 guid, u32 level, u32 unk0, u32 unk1, bool & calloriginal)
{
  //
  if (calloriginal) {
    multiplayerLogger.WriteLine(Info, L"Creating equipment with guid of: %016I64X Level: %i (%p) %x %x",
      guid, level, item, unk0, unk1);
    //logColor(B_RED, L"Creating equipment with guid of: %016I64X Level: %i (%p) %x %x",
    //  guid, level, item, unk0, unk1);
  } else {
    multiplayerLogger.WriteLine(Info, L"Suppressing equipment creation with guid of: %016I64X Level: %i (%p) %x %x",
      guid, level, item, unk0, unk1);
    //logColor(B_RED, L"Suppressing equipment creation with guid of: %016I64X Level: %i (%p) %x %x",
    //  guid, level, item, unk0, unk1);
  }
}

void TLMP::CreateItemPost(CItem* item, CResourceManager* resourceManager, u64 guid, u32 level, u32 unk0, u32 unk1, bool & calloriginal)
{
  if (item) {
    //logColor(B_RED, L"Created equipment with guid of: %016I64X Level: %i (%p, %s) Type = %x",
    //  guid, level, item, item->nameReal.c_str(), item->type__);
    multiplayerLogger.WriteLine(Info, L"Created equipment with guid of: %016I64X Level: %i (%p, %s) Type = %x",
      guid, level, item, item->nameReal.c_str(), item->type__);

    multiplayerLogger.WriteLine(Info, L"Created equipment with guid of: %016I64X Level: %i (%p, %s) Type = %x",
      guid, level, item, item->nameReal.c_str(), item->type__);

    // If we are real equipment (not Interactable, Openable, Breakable, Gold, ...)
    if (item->type__ != BREAKABLE     && item->type__ != OPENABLE     &&
        item->type__ != GOLD          && item->type__ != INTERACTABLE &&
        item->type__ != WAYPOINTNODE  && item->type__ != TOWNPORTAL   &&
        item->type__ != TEMPPORTAL)
    {
      // --
      if (Network::NetworkState::getSingleton().GetState() == SERVER) {
        if (!Server::getSingleton().GetSuppressed_SendEquipmentCreation()) {
          //logColor(B_RED, L"Adding Regular Item of Type: %x  %s", item->type__, item->nameReal.c_str());

          NetworkEntity *newEntity = addEquipment(item);

          NetworkMessages::Equipment msgEquipment;
          Server::getSingleton().Helper_PopulateEquipmentMessage(&msgEquipment, (CEquipment*)item, newEntity);

          Server::getSingleton().BroadcastMessage<NetworkMessages::Equipment>(S_PUSH_NEWEQUIPMENT, &msgEquipment);
        }
      }
    }

    // Just send the standard Item Creation
    else {
      if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
        if (!Client::getSingleton().GetSuppressed_EquipmentCreation()) {
          // Assume the above is set to true when the server sends the LevelCreateItem message
          // For assigning to NetworkSharedLevelItems CItem* is returned on the call
        } else {
          /*
          if (item->type__ == OPENABLE) {
            item->Destroy();
            item = NULL;
            return;
          }
          */
        }
      }
      else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
        //if (item->type__ != OPENABLE && item->type__ != INTERACTABLE) {
          if (!Server::getSingleton().GetSuppressed_SendEquipmentCreation()) {
            //logColor(B_RED, L"Adding Special Item of Type: %x  %s", item->type__, item->nameReal.c_str());

            NetworkEntity *entity = addItem(item);

            NetworkMessages::LevelCreateItem msgLevelCreateItem;
            msgLevelCreateItem.set_itemid(entity->getCommonId());
            msgLevelCreateItem.set_guid(item->GUID);
            msgLevelCreateItem.set_level(level);
            msgLevelCreateItem.set_unk0(unk0);
            msgLevelCreateItem.set_unk1(unk1);
            Server::getSingleton().BroadcastMessage<NetworkMessages::LevelCreateItem>(S_PUSH_LEVEL_CREATE_ITEM, &msgLevelCreateItem);
          }
        //}
      }
    }
  }
}

void TLMP::EquipmentInitialize(CEquipment* equipment, CItemSaveState* itemSaveState, bool & calloriginal)
{
  //log(L"Equipment initializing: %p", equipment);

  //log(L"  ItemSaveState: %s %s %s",
  //  itemSaveState->name.c_str(), itemSaveState->name2.c_str(), itemSaveState->name3.c_str());

  //log(L"Equipment Initialized (GUID: %016I64X  %s)",
  //  equipment->GUID, equipment->nameReal.c_str());
  multiplayerLogger.WriteLine(Info, L"Equipment Initialized (GUID: %016I64X  %s)",
    equipment->GUID, equipment->nameReal.c_str());

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    if (Client::getSingleton().GetSuppressed_EquipmentCreation()) {
      //calloriginal = false;
    }
  }

  if (calloriginal) {
  } else {
    multiplayerLogger.WriteLine(Info, L"Suppressing Equipment Initialize (GUID: %016I64X  %s)",
      equipment->GUID, equipment->nameReal.c_str());
  }
}

// This is really an equipment use
void TLMP::EquipmentIdentifyPre(CEquipment* equipment, CCharacter *player, CBaseUnit* target, bool& calloriginal)
{
  log(L"Equipment Identify Pre (%p, player: %p, target: %p)", equipment, player, target);
  log(L"  Equipment: %s, Player: %s", equipment->nameReal.c_str(), player->characterName.c_str());

  NetworkEntity *entity = searchEquipmentByInternalObject(equipment);
  if (!entity) {
    log(L"Error: Could not find Network Entity for Equipment");
    return;
  }

  NetworkEntity *netCharacter = searchCharacterByInternalObject(player);
  if (!netCharacter) {
    log(L"Error: Could not find Network Entity for Player");
    return;
  }

  NetworkMessages::EquipmentIdentify msgEquipmentIdentify;

  // Search characters first for the target
  NetworkEntity *netTarget = searchCharacterByInternalObject(target);
  if (!netTarget) {
    // If not, we might be targetting an item, search
    netTarget = searchEquipmentByInternalObject(target);
    if (!netTarget) {
      log(L"Error: Could not find Network Entity for Target");
      return;
    } else {
      // Set item type
      msgEquipmentIdentify.set_target_player_type(false);
    }
  } else {
    // Set player type
    msgEquipmentIdentify.set_target_player_type(true);
  }

  msgEquipmentIdentify.set_equipmentid(entity->getCommonId());
  msgEquipmentIdentify.set_characterid(netCharacter->getCommonId());
  msgEquipmentIdentify.set_targetid(netTarget->getCommonId());

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetAllow_EquipmentIdentify()) {
      calloriginal = false;

      Client::getSingleton().SendMessage<NetworkMessages::EquipmentIdentify>(C_REQUEST_EQUIPMENT_IDENTIFY, &msgEquipmentIdentify);
    }
  } else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentIdentify>(S_PUSH_EQUIPMENT_IDENTIFY, &msgEquipmentIdentify);
  }
}

void TLMP::Level_CharacterInitializePre(CCharacter* retval, CLevel* level, CCharacter* character, Vector3* position, u32 unk0, bool & calloriginal)
{
  log(L"Level_CharacterInitializePre: Level = %p Character = %p", level, character);
  multiplayerLogger.WriteLine(Info, L"Level_CharacterInitializePre: Level = %p Character = %p", level, character);

  if (!character) {
    calloriginal = false;
    return;
  }

  u64 guid = character->GUID;

  multiplayerLogger.WriteLine(Info, L"Level::CharacterInitialize: %s (%f %f %f) unk0: %x",
    character->characterName.c_str(), position->x, position->y, position->z, unk0);
  log(L"Level::CharacterInitialize: %s (%f %f %f) unk0: %x",
    character->characterName.c_str(), position->x, position->y, position->z, unk0);

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    // Not suppressed, server sent it
    if (!Client::getSingleton().GetSuppressed_CharacterCreation()) {
      if (guid == STASH || guid == SHAREDSTASH) {
        // Delete the stash the server sent
        character->SetPosition(&Ogre::Vector3(10000, 0, 10000));
        character->destroy = true;
      }
    // True, attempt to suppress the character initialization
    } else if (Client::getSingleton().GetSuppressed_CharacterCreation()) {
      log("Client's suppressed character initialize guid: %016I64X", guid);
      
      // Hacky character checks for specific characters and whether we're in town or not
      if (guid == STASH || guid == SHAREDSTASH) {
        // Remove the stash or shared stash if we're not in the town
        if (wcscmp(level->levelName.c_str(), L"TOWN")) {
          character->destroy = true;
        }
        /*
      } else if (guid == DESTROYER || guid == VANQUISHER || guid == ALCHEMIST ||
        guid == DOG || guid == CAT || guid == BRINK)
      {
        //log("Client: Detected CPlayer addition to CLevel, not suppressing load");
      } else {
        // Town characters won't be destroyed, so just don't call this
        if (!wcscmp(level->levelName.c_str(), L"TOWN")) {
          log(L"Client: Suppressing Monster load into level: %016I64X %s", character->GUID, character->characterName.c_str());
          calloriginal = false;
          retval = NULL;
        }
        */
      } else {
        const u32 townsFolkSize = sizeof(TOWNSFOLK) / sizeof(TOWNSFOLK[0]);

        for (u32 i = 0; i < townsFolkSize; ++i) {
          if (guid == TOWNSFOLK[i]) {
            log(L"Client: Suppressing TownsFolk load into level: %016I64X %s", character->GUID, character->characterName.c_str());
            calloriginal = false;
            retval = NULL;
          }
        }
      }

      //log("Character destroy = %s", character->destroy ? "true" : "false");
    }
  }
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    if (gameClient->inGame) {
      multiplayerLogger.WriteLine(Info, L"Level Character Initialized: %p %s (%f %f %f) unk0: %x",
        character, character->characterName.c_str(), position->x, position->y, position->z, unk0);
      //log(L"Level Character Initialized: %p %s (%f %f %f) unk0: %x",
      //  character, character->characterName.c_str(), position->x, position->y, position->z, unk0);

      // Create a network ID to identify this character later
      NetworkEntity *newEntity = addCharacter(character);

      if (!newEntity) {
        //log(L"NetworkEntity is NULL");
        return;
      }

      // If the server isn't in the process of changing levels, we're safe to send this out
      if (!LevelLoading) {
        // Send this character to be created on the clients if it isn't suppressed
        if (!Server::getSingleton().GetSuppressed_SendCharacterCreation()) {
          if (guid != STASH && guid != SHAREDSTASH) {
            multiplayerLogger.WriteLine(Info, L"Server: Pushing Initialized Character out to clients...");
            //log(L"Server: Pushing Initialized Character out to clients...");

            string characterName(character->characterName.begin(), character->characterName.end());
            characterName.assign(character->characterName.begin(), character->characterName.end());

            // Create a new network message for all clients to create this character
            NetworkMessages::Character msgNewCharacter;
            msgNewCharacter.set_guid(character->GUID);
            msgNewCharacter.set_name(characterName);

            NetworkMessages::Position *msgPlayerPosition = msgNewCharacter.mutable_position();
            msgPlayerPosition->set_x(character->GetPosition().x);
            msgPlayerPosition->set_y(character->GetPosition().y);
            msgPlayerPosition->set_z(character->GetPosition().z);

            msgNewCharacter.set_id(newEntity->getCommonId());
            msgNewCharacter.set_alignment(character->alignment);

            Helper_BuildInventoryTabIndexSize(msgNewCharacter, character);

            // This will broadcast to all clients except the one we received it from
            Server::getSingleton().BroadcastMessage<NetworkMessages::Character>(S_PUSH_NEWCHAR, &msgNewCharacter);
          }
        }
      }
    }
  } 
}

void TLMP::Level_CharacterInitializePost(CCharacter* retval, CLevel* level, CCharacter* character, Vector3* position, u32 unk0, bool & calloriginal)
{
  log(L"Level_CharacterInitializePost");
  multiplayerLogger.WriteLine(Info, L"Level_CharacterInitializePost");

  u64 guid = character->GUID;

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    // Client created character, not the servers'
    if (Client::getSingleton().GetSuppressed_CharacterCreation()) {
      log(" Client's suppressed character initialize guid: %016I64X", guid);
      
      // Hacky character checks for specific characters and whether we're in town or not
      if (guid == STASH || guid == SHAREDSTASH || guid == DESTROYER || guid == VANQUISHER || guid == ALCHEMIST ||
        guid == DOG || guid == CAT || guid == BRINK || guid == 0xFFFFFFFFFFFFFFFF)
      {
      } else {
        // Anything not in the town
        if (wcscmp(level->levelName.c_str(), L"TOWN")) {
          log(L"  Post Destroying character... %p %p", retval, character);

          /*
          // Destroy the character if we're not going to load it
          character->SetPosition(&Ogre::Vector3(10000, 0, 10000));
          character->destroy = true;
          character->healthCurrent = -1;
          character->healthMax = -1;
          character->state = DYING;
          */

          character->destroy = true;
          ClientDuplicateCharacters->push_back(retval);
          //level->RemoveCharacter(retval);
        }
      }
    }
  }
}

void TLMP::GameClient_ProcessObjects(CGameClient *client, float dTime, PVOID unk1, PVOID unk2)
{
  //CPlayer *player = client->pCPlayer;
  //log(L"Test: CPositionableObject: %x CBaseUnit: %x  CCharacter: %x (Player = %p)",
  //  sizeof(CPositionableObject), sizeof(CBaseUnit), sizeof(CCharacter), player);

  /*
  log(L"Test: destroy = %x", player->destroy);
  log(L"Test: offset destroy = %x", ((u32)&player->destroy - (u32)player) );
  */

  /*
  log(L"Test: health = %x", player->health);
  log(L"Test: offset health = %x", ((u32)&player->health - (u32)player) );

  log(L"Test: mana = %x", player->mana);
  log(L"Test: offset mana = %x", ((u32)&player->mana - (u32)player) );
  */

  /*
  log(L"Test: target = %x", player->target);
  log(L"Test: offset target = %x", ((u32)&player->target - (u32)player) );
  log(L"Test: running = %x", player->running);
  log(L"Test: offset running = %x", ((u32)&player->running - (u32)player) );

  log(L"Test: moving = %x", player->moving);
  log(L"Test: attacking = %x", player->attacking);
  log(L"Test: offset moving = %x", ((u32)&player->moving - (u32)player) );
  log(L"Test: offset attacking = %x", ((u32)&player->attacking - (u32)player) );

  gameClient->pCGameUI->dumpSubMenuPtrs();
  
  //log(L"TargetName: %p offset = %x (gameUI = %p)", gameClient->pCGameUI->sheetTargetName, ((u8*)&gameClient->pCGameUI->sheetTargetName - (u8*)gameClient->pCGameUI), &gameClient->pCGameUI);
  //log("  TargetName: %s", gameClient->pCGameUI->sheetTargetName->getText().c_str());
  //log(L"  Target: %p", gameClient->pCGameUI->pCTargetCharacter);
  if (gameClient->pCGameUI->pCTargetCharacter) {
    log(L"  Target: %s", gameClient->pCGameUI->pCTargetCharacter->characterName.c_str());
  }
  */

  /*
  ShowCursor(false);
  CEGUI::MouseCursor* mouseCursor = CEGUI::MouseCursor::getSingletonPtr();
  mouseCursor->setImage("logo", "destroyericon");
  mouseCursor->show();
  mouseCursor->draw();
  */

  // Set the started flag - don't bother checking, it's just as fast to set it
  switch (Network::NetworkState::getSingleton().GetState()) {
    case SERVER:
      // Only set game started when the town is the level loaded
      if (!wcscmp(gameClient->pCDungeon->name0.c_str(), L"TOWN")) {
        Server::getSingleton().SetGameStarted(true);
      } else {
        Server::getSingleton().SetGameStarted(false);
      }

      Server::getSingleton().ReceiveMessages();
      break;

    case CLIENT:
      Client::getSingleton().SetGameStarted(true);

      Client::getSingleton().ReceiveMessages();
      break;
  }

  // Process messages from the Lobby server
  LobbyClient::getSingleton().ReceiveMessages();
}

void TLMP::GameClient_TitleProcessObjects(CGameClient *client, float dTime, PVOID unk1, PVOID unk2)
{
  //log("MainMenuShown: %i", MainMenuShown());

  // Determine if we've exited an existing game in network mode
  switch (Network::NetworkState::getSingleton().GetState()) {
    case SERVER:
      // Shutdown the Server if this is the first iteration out of the game
      //if (Server::getSingleton().GetGameStarted() && Server::getSingleton().HasGameStarted()) {
      //  Server::getSingleton().Shutdown();
      //}

      Server::getSingleton().SetGameStarted(false);

      Server::getSingleton().ReceiveMessages();
      break;

    case CLIENT:
      // Shutdown the Client if this is the first iteration out of the game
      //if (Client::getSingleton().GetGameStarted() && Client::getSingleton().HasGameStarted()) {
      //  Client::getSingleton().Disconnect();
      //}

      Client::getSingleton().SetGameStarted(false);

      Client::getSingleton().ReceiveMessages();
      break;
  }

  // Process messages from the Lobby server
  LobbyClient::getSingleton().ReceiveMessages();
}

void TLMP::GameClient_CreateLevelPre(CGameClient* client, wstring unk0, wstring unk1, u32 unk2, u32 unk3, u32 unk4, wstring unk5, bool & calloriginal)
{
  multiplayerLogger.WriteLine(Info, L"GameClient::CreateLevel Pre (%s, %s, %s   %x %x %x)",
    unk0.c_str(), unk1.c_str(), unk5.c_str(), unk2, unk3, unk4);
  //logColor(B_GREEN, L"GameClient::CreateLevel Pre (%s, %s, %s   %x %x %x)",
  //  unk0.c_str(), unk1.c_str(), unk5.c_str(), unk2, unk3, unk4);

  /*
  logColor(B_GREEN, L"Level: %i", client->level);
  logColor(B_GREEN, L"levelAbsolute: %x", client->levelAbsolute);
  logColor(B_GREEN, L"DungeonName: %s", client->dungeonName.c_str());
  logColor(B_GREEN, L"Dungeon: %p", client->pCDungeon);
  logColor(B_GREEN, L"  Name0: %s", client->pCDungeon->name0.c_str());
  logColor(B_GREEN, L"  Name1: %s", client->pCDungeon->name1.c_str());
  logColor(B_GREEN, L"  Name2: %s", client->pCDungeon->name2.c_str());

  static bool firstLevel = true;
  if (firstLevel) {
    firstLevel = false;
    unk5.assign(L"TOWN");
    gameClient->pCDungeon->name0.assign(L"TOWN");
  }
  */
}

void TLMP::GameClient_CreateLevelPost(CGameClient* client, wstring unk0, wstring unk1, u32 unk2, u32 unk3, u32 unk4, wstring unk5, bool & calloriginal)
{
  //log(L"CreateLevel Post");

  // Do this in Post to ensure it loads at the right time
  // Send a Message to clients that the Server's game has started
  if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    // Only set game started when the town is the level loaded
    if (!wcscmp(gameClient->pCDungeon->name0.c_str(), L"TOWN")) {
      Server::getSingleton().SetGameStarted(true);
    } else {
      Server::getSingleton().SetGameStarted(false);
    }

    NetworkMessages::GameStarted msgGameStarted;
    Server::getSingleton().BroadcastMessage<NetworkMessages::GameStarted>(S_PUSH_GAMESTARTED, &msgGameStarted);
  }

  // Send a Message to the Server that the client has entered the game
  else if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    Client::getSingleton().SetGameStarted(true);

    NetworkMessages::GameEnter msgGameEnter;
    NetworkMessages::CurrentLevel *msgCurrentLevel = msgGameEnter.mutable_currentlevel();
    
    string sDungeonSection(gameClient->pCDungeon->name0.begin(), gameClient->pCDungeon->name0.end());
    sDungeonSection.assign(gameClient->pCDungeon->name0.begin(), gameClient->pCDungeon->name0.end());

    //logColor(B_GREEN, L"Client: Sending GAME_ENTER: %s %i %i",
    //  gameClient->pCDungeon->name0.c_str(), gameClient->level, gameClient->levelAbsolute);

    msgCurrentLevel->set_dungeonsection(sDungeonSection);
    msgCurrentLevel->set_relativelevel(gameClient->level);
    msgCurrentLevel->set_absolutelevel(gameClient->levelAbsolute);

    Client::getSingleton().SendMessage<NetworkMessages::GameEnter>(C_PUSH_GAMEENTER, &msgGameEnter);
  }
}

void TLMP::Level_Dtor(CLevel* level, u32, bool&)
{
  logColor(B_RED, L"Level Dtor (%p)", level);
  multiplayerLogger.WriteLine(Info, L"Level Dtor (%p)", level);

  //level->DumpCharacters1();
  //level->DumpTriggerUnits();
  //level->DumpCharacters2();
  level->DumpItems();

  log(L"Level::Dtor Done Dumping Level Info");
}

void TLMP::Level_Ctor(wstring name, CSettings* settings, CGameClient* gameClient, CResourceManager* resourceManager, PVOID OctreeSM, CSoundManager* soundManager, u32 unk0, u32 unk1, bool&)
{
  //logColor(B_RED, L"Level Ctor");
  multiplayerLogger.WriteLine(Info, L"Level Ctor");

  /*
  log(L"  Name: %s", name.c_str());
  log(L"  Settings: %p", settings);
  log(L"  GameClient: %p", gameClient);
  log(L"  ResourceManager: %p", resourceManager);
  log(L"  SoundManager: %p", soundManager);
  */
}

void TLMP::GameClient_LoadLevelPre(CGameClient* client, bool & calloriginal)
{
  logColor(B_GREEN, L"LoadLevelPre (GameClient = %p, player = %p)", client, client->pCPlayer);
  multiplayerLogger.WriteLine(Info, L"LoadLevelPre (GameClient = %p)", client);

  /* Player may be deleted at this point?
  log("Player state: %i", client->pCPlayer->state);
  if (client->pCPlayer->state == DEAD || client->pCPlayer->state == DYING) {
    client->pCPlayer->Resurrect();
    client->pCPlayer->healthCurrent = client->pCPlayer->healthMax;
  }
  */

  // Clear the duplicate character list that will be created by this level load
  ClientDuplicateCharacters->clear();

  /*
  // Suppress level changes
  {
    calloriginal = false;
    logColor(B_GREEN, L"Level changes suppressed");
    multiplayerLogger.WriteLine(Info, L"Level changes suppressed");
  }
  */
  
  CLevel *level = client->pCLevel;
  //level->DumpCharacters1();
  //level->DumpTriggerUnits();
  //level->DumpCharacters2();
  //level->DumpItems();

  // Dump level items
  {
    multiplayerLogger.WriteLine(Info, L"");
    multiplayerLogger.WriteLine(Info, L"Level Item Dump:");

    LinkedListNode* itr = *(level->itemsAll);
    while (itr != NULL) {
      //log(L"  Level Item: itr = %p", itr);

      CItem* item = (CItem*)itr->pCBaseUnit;
      multiplayerLogger.WriteLine(Info, L"   itrNext: %p, Item: %p, Name: %s", itr->pNext, item, item->nameReal.c_str());

      itr = itr->pNext;
    }
    
    multiplayerLogger.WriteLine(Info, L"Done dumping items.");
    multiplayerLogger.WriteLine(Info, L"");
  }

  // Dump level trigger items
  {
    multiplayerLogger.WriteLine(Info, L"");
    multiplayerLogger.WriteLine(Info, L"Level Trigger Dump:");

    LinkedListNode* itr = *(level->itemsProximity);
    while (itr != NULL) {
      //log(L"  Level Item: itr = %p", itr);

      CItem* item = (CItem*)itr->pCBaseUnit;
      multiplayerLogger.WriteLine(Info, L"   itrNext: %p, Item: %p, Name: %s", itr->pNext, item, item->nameReal.c_str());

      itr = itr->pNext;
    }
    
    multiplayerLogger.WriteLine(Info, L"Done dumping triggers.");
    multiplayerLogger.WriteLine(Info, L"");
  }

  // Dump level characters
  {
    multiplayerLogger.WriteLine(Info, L"Level Character2 Dump:");

    LinkedListNode* itr = *(level->charactersAll);
    while (itr != NULL) {
      CCharacter* character = (CCharacter*)itr->pCBaseUnit;
      CInventory* inv = character->pCInventory;

      multiplayerLogger.WriteLine(Info, L"  Character (%p %s): Inventory (%p)", character, character->characterName.c_str(), inv);
      multiplayerLogger.WriteLine(Info, L"  Inventory Dump: %p for character %p", inv, inv->pCCharacter);
      multiplayerLogger.WriteLine(Info, L"  Inventory MaxSize: %i", inv->maxSize);
      multiplayerLogger.WriteLine(Info, L"  Inventory Equipment Size: %i", inv->equipmentList.size);
        
      for (u32 i = 0; i < inv->equipmentList.size; ++i) {
        multiplayerLogger.WriteLine(Info, L"     Inventory Equipment [%i]: %p  slot: %i", i, inv->equipmentList[i]->pCEquipment, inv->equipmentList[i]->slot);
        multiplayerLogger.WriteLine(Info, L"       Name: (%s)", inv->equipmentList[i]->pCEquipment->nameReal.c_str());
      }

      itr = itr->pNext;
    }
  }

  /*
  logColor(B_GREEN, L"Level: %i", client->level);
  logColor(B_GREEN, L"levelAbsolute: %x", client->levelAbsolute);
  logColor(B_GREEN, L"DungeonName: %s", client->dungeonName.c_str());
  logColor(B_GREEN, L"Dungeon: %p", client->pCDungeon);
  logColor(B_GREEN, L"  Name0: %s", client->pCDungeon->name0.c_str());
  logColor(B_GREEN, L"  Name1: %s", client->pCDungeon->name1.c_str());
  logColor(B_GREEN, L"  Name2: %s", client->pCDungeon->name2.c_str());
  */

  LevelLoading = true;

  // Suppress level changes
  if (NetworkState::getSingleton().GetSuppressed_LevelChange()) {
    //calloriginal = false;
    //client->flagLevelLoading = 0;
  }

  if (!calloriginal) {
    multiplayerLogger.WriteLine(Info, L"GameClient::LoadLevel Suppressed");
    //logColor(B_GREEN, L"GameClient::LoadLevel Suppressed");
  }

  log(L"GameClient_LoadLevelPre Done Dumping Info.");
}

void TLMP::GameClient_LoadLevelPost(CGameClient* client, bool & calloriginal)
{
  logColor(B_GREEN, L"GameClient_LoadLevelPost");
  multiplayerLogger.WriteLine(Info, L"GameClient_LoadLevelPost");

  CLevel *level = client->pCLevel;
  log(L"Level: %p", level);
  level->DumpCharacters1();
  level->DumpTriggerUnits();
  level->DumpCharacters2();
  level->DumpItems();

  /*
  logColor(B_GREEN, L"  Level: %i", client->level);
  logColor(B_GREEN, L"  levelAbsolute: %x", client->levelAbsolute);
  logColor(B_GREEN, L"  DungeonName: %s", client->dungeonName.c_str());
  logColor(B_GREEN, L"  Dungeon: %p", client->pCDungeon);
  logColor(B_GREEN, L"    Name0: %s", client->pCDungeon->name0.c_str());
  logColor(B_GREEN, L"    Name1: %s", client->pCDungeon->name1.c_str());
  logColor(B_GREEN, L"    Name2: %s", client->pCDungeon->name2.c_str());
  */

  LevelLoading = false;

  log(L"GameClient_LoadLevelPost Done Dumping Info.");

  if (NetworkState::getSingleton().GetState() == CLIENT) {
    log(L"Client destroying duplicate characters...");
    multiplayerLogger.WriteLine(Info, L"Client destroying duplicate characters...");

    vector<CCharacter*>::iterator itr;
    for (itr = ClientDuplicateCharacters->begin(); itr != ClientDuplicateCharacters->end(); ++itr) {
      log(L"  Destroying %s %016I64X", (*itr)->characterName.c_str(), (*itr)->GUID);
      multiplayerLogger.WriteLine(Info, L"  Destroying %s %016I64X", (*itr)->characterName.c_str(), (*itr)->GUID);

      (*itr)->destroy = true;
    }
    ClientDuplicateCharacters->clear();
    log(L"Done destroying.");
  }
}

void TLMP::GameClient_LoadMapPre(CGameClient*, u32 unk0, u32 unk1, bool & calloriginal)
{
  multiplayerLogger.WriteLine(Info, L"GameClient::LoadMap Pre(%x %x)", unk0, unk1);
  logColor(B_GREEN, L"GameClient::LoadMap Pre(%x %x)", unk0, unk1);

  // unk0  unk1
  //   0   3      is the character load screen
  //   0   0      is the main title screen
  //   0   1      is the new player screen
  //   2   0      is the game

  // 0: Player loading into title menu from game
  // 2: Player loading into game from title menu
  if (unk0 == 0 && unk1 == 0) {
    // If we are loading into the main menu then send off our intention to leave
    //  to either the server or client, then forcefully disconnect on PostLoadMap
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      NetworkMessages::GameEnded msgGameEnded;
      Client::getSingleton().SendMessage<NetworkMessages::GameEnded>(C_PUSH_GAMEEXITED, &msgGameEnded);
    } else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
      //NetworkMessages::GameEnded msgGameEnded;
      //Server::getSingleton().BroadcastMessage<NetworkMessages::GameEnded>(S_PUSH_GAMEENDED, &msgGameEnded);
    }
  } else if (unk0 == 2) {
    // This will suppress the game load if the Server hasn't setup the game yet
    // TODO: Re-show the UI and Display an Error dialog
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      if (!Client::getSingleton().GetServerGameStarted()) {
        log("Suppressing client load - server has not started game yet.");
        calloriginal = false;

        // Display the UI message for waiting for server to start
        DisplayWaitForServerWindow();
      }
    }
  }
}

void TLMP::GameClient_LoadMapPost(CGameClient*, u32 unk0, u32 unk1, bool & calloriginal)
{
  multiplayerLogger.WriteLine(Info, L"GameClient::LoadMap Post(%x %x)", unk0, unk1);
  logColor(B_GREEN, L"GameClient::LoadMap Post(%x %x)", unk0, unk1);

  // 0: Player loading into title menu from game
  // 2: Player loading into game from title menu
  if (unk0 == 0 && unk1 == 0) {
    // Forcefully disconnect here after we've sent off our exited/ended message in LoadMapPre
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      Client::getSingleton().Disconnect();
    } else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
      Server::getSingleton().Shutdown();
    }
  } else if (unk0 == 2) {
    // Testing out speeding up Client load times and issues
    if (NetworkState::getSingleton().GetState() == SERVER) {
      // Force load the Player into the Town
      wstring Town(L"TOWN");

      if (gameClient->pCDungeon->name0 == Town) {
        multiplayerLogger.WriteLine(Info, L"Player already in town");
        //logColor(B_RED, L"Player already in town");
      } else {
        multiplayerLogger.WriteLine(Info, L"Player not in town, force loading...");
        //logColor(B_RED, L"Player not in town, force loading...");

        NetworkState::getSingleton().SetSuppressed_LevelChange(false);
        gameClient->ForceToTown();
        NetworkState::getSingleton().SetSuppressed_LevelChange(true);
      }
    }
  }
}

void TLMP::MainMenuEventPre(CMainMenu* mainMenu, u32 unk0, wstring str, bool & calloriginal)
{
  enum {
    CONTINUELASTGAME = 3,
    NEWGAME = 4,
    CONTINUEGAME = 5,     // Load Game
  };

  log(L"MainMenuEventPre: %x  %s", unk0, str.c_str());

  // New: Attempt to suppress this at the GameClient_LoadMap function
  //     -Problem: UI is hidden when failed, go ahead and stop this function

  // Suppress this event if any of these actions are taken
  if (unk0 == CONTINUELASTGAME || unk0 == NEWGAME || unk0 == CONTINUEGAME) {
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      if (!Client::getSingleton().GetServerGameStarted()) {
        calloriginal = false;
        multiplayerLogger.WriteLine(Info, L"Enter game suppressed: Client, Server not in game");

        // Display the UI message for waiting for server to start
        DisplayWaitForServerWindow();
      }
    }
  }
}

void TLMP::Monster_Idle(CMonster* monster, float dTime, bool & calloriginal)
{
  if (monster->GUID == DESTROYER || monster->GUID == ALCHEMIST || monster->GUID == VANQUISHER) {
    calloriginal = false;
  }

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    //calloriginal = false;
  }
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    //if (monster->GUID == DESTROYER || monster->GUID == ALCHEMIST || monster->GUID == VANQUISHER) {
    //  calloriginal = false;
    //}
  }
}

void TLMP::Character_SetDestination(CCharacter* character, CLevel* level, float x, float z)
{
  // Buffer character position, check if it exists
  // If the position is the same then don't push it out
  map<CCharacter*, Vector3>::iterator itr;
  itr = CharacterNetworkPositionBuffer->find(character);
  if (itr != CharacterNetworkPositionBuffer->end()) {
    Vector3& position = (*CharacterNetworkPositionBuffer)[character];
    if (fabs(position.x - x) < 0.01f && fabs(position.z - z) < 0.01f) {
      return;
    } 
  }
  (*CharacterNetworkPositionBuffer)[character].x = x;
  (*CharacterNetworkPositionBuffer)[character].z = z;

  //log(L"Character set destination: %s %p %f %f", character->characterName.c_str(), level, x, z);

  //
  if (Network::NetworkState::getSingleton().GetState() == SINGLEPLAYER) {
    return;
  }

  NetworkMessages::CharacterDestination msgCharacterDestination;
  NetworkMessages::Position *msgPositionCurrent = msgCharacterDestination.mutable_current();
  NetworkMessages::Position *msgPositionDestination = msgCharacterDestination.mutable_destination();

  msgPositionCurrent->set_x(character->GetPosition().x);
  msgPositionCurrent->set_y(character->GetPosition().y);
  msgPositionCurrent->set_z(character->GetPosition().z);

  msgPositionDestination->set_x(x);
  msgPositionDestination->set_y(0);
  msgPositionDestination->set_z(z);

  // Find the CommonID for the Character
  u32 commonId;
  NetworkEntity* entity = searchCharacterByInternalObject((PVOID)character);

  if (entity) {
    commonId = entity->getCommonId();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find Network Common ID for Character ptr: %p in SetDestination", character);
    return;
  }

  msgCharacterDestination.set_id(commonId);
  msgCharacterDestination.set_running(character->running);
  msgCharacterDestination.set_attacking(character->attacking);

  // Send a Network message off to the server if we're a client
  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetSuppressed_SetDestination()) {
      Client::getSingleton().SendMessage<NetworkMessages::CharacterDestination>(C_PUSH_CHARACTER_SETDEST, &msgCharacterDestination);
    }
  }
  // Send this message to all clients if this is the Server
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    if (!Server::getSingleton().GetSuppressed_SetDestination()) {
      //multiplayerLogger.WriteLine(Info, L" Sending Destination for Common: %x (%p)", commonId, character);
      //log(L" Sending Destination for Common: %x (%p)", commonId, character);

      Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterDestination>(S_PUSH_CHARACTER_SETDEST, &msgCharacterDestination);
    }
  }
}

void TLMP::Level_DropItemPre(CLevel* level, CItem* item, Vector3 & position, bool unk0, bool& calloriginal)
{
  multiplayerLogger.WriteLine(Info, L"Level pre dropping Item %s at (unk0: %i) %f, %f, %f Type = %x",
    item->nameReal.c_str(), unk0, position.x, position.y, position.z, item->type__);
  log(L"Level pre dropping Item %s at (unk0: %i) %f, %f, %f Type = %x",
    item->nameReal.c_str(), unk0, position.x, position.y, position.z, item->type__);

  // Iterate over the existing items on the level and ensure the same item doesn't already exist
  LinkedListNode* itr = *level->itemsAll;
  while (itr != NULL) {
    CItem* itemLevel = (CItem*)itr->pCBaseUnit;
    if (itemLevel == item) {
      log(L"Level Drop Found Identical Item already on the Ground! Suppressing the drop.");
      calloriginal = false;
      break;
    }
    itr = itr->pNext;
  }

  //
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    // Allow interactable and stash/shared stash (these may not be needed)
    if (item->type__ == OPENABLE ||
        item->type__ == INTERACTABLE)
    {
      if (!wcscmp(item->nameReal.c_str(), L"Barrel") ||
          !wcscmp(item->nameReal.c_str(), L"Stairs Up") || 
          !wcscmp(item->nameReal.c_str(), L"Stairs Down") ||
          !wcscmp(item->nameReal.c_str(), L"Return to Dungeon") ||
          !wcscmp(item->nameReal.c_str(), L"Waygate to Town") ||
          !wcscmp(item->nameReal.c_str(), L"Mine Entrance"))
      {
        calloriginal = false;
      }
    } 
    /*
    if (item->type__ != INTERACTABLE &&
        item->type__ != OPENABLE) 
    {*/
    else {
      if (!Client::getSingleton().GetAllow_LevelItemDrop()) {
        log(L"Suppressed");
        calloriginal = false;
        item->SetPosition(&Ogre::Vector3(10000, 0, 10000));
        //item->Destroy();
        item->destroy;
      }
    }
    
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
  }

  if (!calloriginal) {
    //log(L"  Suppressing level drop item: type = %i", item->type__);
  }
}

void TLMP::Level_DropItemPost(CLevel* level, CItem* item, Vector3 & position, bool unk0, bool& calloriginal)
{
  multiplayerLogger.WriteLine(Info, L"Level post dropped Item %s at %f, %f, %f (unk0: %i)",
    item->nameReal.c_str(), position.x, position.y, position.z, unk0);
  //log(L"Level post dropped Item %s at %f, %f, %f (unk0: %i)",
  //  item->nameReal.c_str(), position.x, position.y, position.z, unk0);

  // DEBUGGING SERVER CRASH
  // Iterate over the existing items on the level and dump their names
  //log(L"Current Items on Ground: %p", level->itemsAll);
  /*
  LinkedListNode* itr2 = *level->itemsAll;
  while (itr2) {
    CEquipment* itemItr = (CEquipment*)itr2->pCBaseUnit;
    // Ensure this is Equipment (Check vtable ptr) before dumping info
    if (*(u32*)itemItr == 0xA7EA8C) {
      log(L"  Item: %s (Sockets: %i, GemList Size: %i)", itemItr->nameReal.c_str(), itemItr->socketCount, itemItr->gemList.size);

      for (u32 i = 0; i < itemItr->gemList.size; ++i) {
        log(L"    Gem[%i]: %s", i, itemItr->gemList[i]->nameReal.c_str());
      }
    }

    itr2 = itr2->pNext;
  }
  */
  // --

  // Bump out before sending the network message if we're not supposed to
  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    if (Client::getSingleton().GetAllow_LevelItemDrop()) {
      return;
    }
  }

  //
  NetworkEntity *equipmentEntity = searchEquipmentByInternalObject((PVOID)item);

  if (equipmentEntity) {
    // Create a Network Message for sending off to clients the equipment addition to the inventory
    NetworkMessages::EquipmentDrop msgEquipmentDrop;

    NetworkMessages::Position *msgPosition = msgEquipmentDrop.add_position();
    msgPosition->set_x(position.x);
    msgPosition->set_y(position.y);
    msgPosition->set_z(position.z);

    msgEquipmentDrop.set_equipmentid(equipmentEntity->getCommonId());
    msgEquipmentDrop.set_unk0(unk0);

    // Client
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      Client::getSingleton().SendMessage<NetworkMessages::EquipmentDrop>(C_PUSH_EQUIPMENT_DROP, &msgEquipmentDrop);
    }
    // Server
    else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
      Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentDrop>(S_PUSH_EQUIPMENT_DROP, &msgEquipmentDrop);

      // Add this to our Equipment on the Ground list
      ServerEquipmentOnGround->push_back(equipmentEntity);
    }
  } 
  else {
    if (Network::NetworkState::getSingleton().GetState() == SERVER) {
      NetworkEntity *entity = searchItemByInternalObject(item);

      if (!entity) {
        entity = addItem(item);
      }

      if (entity) {
        NetworkMessages::LevelDropItem msgLevelDropItem;
        NetworkMessages::Position *msgPosition = msgLevelDropItem.mutable_position();
        msgPosition->set_x(position.x);
        msgPosition->set_y(position.y);
        msgPosition->set_z(position.z);
        msgLevelDropItem.set_itemid(entity->getCommonId());
        msgLevelDropItem.set_unk0(unk0);

        Server::getSingleton().BroadcastMessage<NetworkMessages::LevelDropItem>(S_PUSH_LEVEL_ITEM_DROP, &msgLevelDropItem);
      }
    }
  }
}

void TLMP::SendInventoryAddEquipmentToServer(CCharacter* owner, CEquipment* equipment, u32 slot, u32 unk)
{
  NetworkEntity *ownerEntity = searchCharacterByInternalObject((PVOID)owner);
  NetworkEntity *equipmentEntity = searchEquipmentByInternalObject((PVOID)equipment);

  multiplayerLogger.WriteLine(Info, L"Character (%s) Adding Equipment to Inventory: %s (Slot=%i) (Unk = %i)",
    owner->characterName.c_str(), equipment->nameReal.c_str(), slot, unk);
  //log(L"Character (%s) Adding Equipment to Inventory: %s (Slot=%i) (Unk = %i)",
  //  owner->characterName.c_str(), equipment->nameReal.c_str(), slot, unk);

  if (ownerEntity) {
    // If the equipment hasn't yet been created for network use
    // request the server create it and add it to the slot and reply with a network id for it
    if (!equipmentEntity) {
      Client::getSingleton().Helper_ClientPushEquipment(equipment);
    } else {
      // Create a Network Message for sending off to clients the equipment addition to the inventory
      NetworkMessages::InventoryAddEquipment msgInventoryAddEquipment;
      msgInventoryAddEquipment.set_ownerid(ownerEntity->getCommonId());
      msgInventoryAddEquipment.set_slot(slot);
      msgInventoryAddEquipment.set_unk0(unk);
      msgInventoryAddEquipment.set_guid(equipment->GUID);
      msgInventoryAddEquipment.set_equipmentid(equipmentEntity->getCommonId());

      Client::getSingleton().SendMessage<NetworkMessages::InventoryAddEquipment>(C_PUSH_EQUIPMENT_EQUIP, &msgInventoryAddEquipment);
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Could not find NetworkEntity for inventory owner: %p (%s)",
      owner, owner->characterName.c_str());
    //log(L"Could not find NetworkEntity for inventory owner: %p (%s)",
    //  owner, owner->characterName.c_str());
  }
}

void TLMP::Inventory_AddEquipmentPre(CEquipment* retval, CInventory* inventory, CEquipment* equipment, u32& slot, u32 unk0, bool& calloriginal)
{
  log(L"Inventory adding Equipment: %016I64X (%s %p) (slot = %x) (Owner = %s)",
    equipment->GUID, equipment->nameReal.c_str(), equipment, slot, inventory->pCCharacter->characterName.c_str());

  if (inventory) {
    CCharacter *owner = inventory->pCCharacter;

    // Client - Allow the equipment to be added to the inventory, but send it out to the server to create
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      if (Client::getSingleton().GetSuppressed_EquipmentCreation()) {
        // If we're suppressing this, we need to send it to the Server
        // The server will create, add the equipment and send the message back to us with the network id
        SendInventoryAddEquipmentToServer(owner, equipment, slot, unk0);

        //calloriginal = false;
      }
    }

    // Server
    else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
      // If we're not suppressed to send this out - 
      // The reason this would be suppressed if a character had sent a Pickup message,
      // That triggers an Equip then a Pickup message on the client, causing a phantom item in the inventory
      if (!Server::getSingleton().GetSuppressed_SendEquipmentEquip()) {
        NetworkEntity *ownerEntity = searchCharacterByInternalObject((PVOID)owner);
        NetworkEntity *equipmentEntity = searchEquipmentByInternalObject((PVOID)equipment);

        if (ownerEntity) {
          if (equipmentEntity) {
            // Create a Network Message for sending off to clients the equipment addition to the inventory
            NetworkMessages::InventoryAddEquipment msgInventoryAddEquipment;
            msgInventoryAddEquipment.set_ownerid(ownerEntity->getCommonId());
            msgInventoryAddEquipment.set_equipmentid(equipmentEntity->getCommonId());
            msgInventoryAddEquipment.set_slot(slot);
            msgInventoryAddEquipment.set_unk0(unk0);

            Server::getSingleton().BroadcastMessage<NetworkMessages::InventoryAddEquipment>(S_PUSH_EQUIPMENT_EQUIP, &msgInventoryAddEquipment);
          } else {
            multiplayerLogger.WriteLine(Error, L"Could not find NetworkEntity for equipment: %p",
              equipment);
            //log(L"Could not find NetworkEntity for equipment: %p",
            //  equipment);
          }
        } else {
          multiplayerLogger.WriteLine(Error, L"Could not find NetworkEntity for inventory owner: (%p) %p",
            inventory, owner);
          //log(L"Could not find NetworkEntity for inventory owner: (%p) %p",
          //  inventory, owner);
        }
      }
    }
    
    multiplayerLogger.WriteLine(Info, L"Inventory::AddEquipmentPre(%p) (%p) (%s)",
      inventory, equipment, equipment->nameReal.c_str());
    //log(L"Inventory::AddEquipmentPre(%p) (%p) (%s)",
    //  inventory, equipment, equipment->nameReal.c_str());
  } else {
    //log(L"Error: Character has no Inventory!");
    multiplayerLogger.WriteLine(Error, L"Error: Character has no Inventory!");
  }
}

void TLMP::Inventory_AddEquipmentPost(CEquipment* retval, CInventory* inventory, CEquipment* equipment, u32& slot, u32 unk0, bool& calloriginal)
{
  multiplayerLogger.WriteLine(Info, L"Inventory::AddEquipmentPost(%p) (%p) (%s) (slot = %x) (unk0 = %x)",
    inventory, equipment, equipment->nameReal.c_str(), slot, unk0);
  //log(L"Inventory::AddEquipmentPost(%p) (%p) (%s) (slot = %x) (unk0 = %x)",
  //  inventory, equipment, equipment->nameReal.c_str(), slot, unk0);
}

void TLMP::Inventory_RemoveEquipmentPre(CInventory* inventory, CEquipment* equipment)
{
  //log(L"Inventory removing Equipment Pre:");

  multiplayerLogger.WriteLine(Info, L"Inventory::RemoveEquipment(%p) (%s)", inventory, equipment->nameReal.c_str());
  log(L"Inventory::RemoveEquipment(%p) (%s)", inventory, equipment->nameReal.c_str());
}

void TLMP::Inventory_RemoveEquipmentPost(CInventory* inventory, CEquipment* equipment)
{
  //log(L"Inventory removing Equipment Post:");

  //
  CCharacter *owner = inventory->pCCharacter;
  NetworkEntity *ownerEntity = searchCharacterByInternalObject((PVOID)owner);
  NetworkEntity *equipmentEntity = searchEquipmentByInternalObject((PVOID)equipment);

  //
  if (!ownerEntity) {
    multiplayerLogger.WriteLine(Error, L"Could not find NetworkEntity for inventory owner: (%p) %p",
      inventory, owner);
    //log(L"Could not find NetworkEntity for inventory owner: (%p) %p",
    //  inventory, owner);
    return;
  } else if (!equipmentEntity) {
    multiplayerLogger.WriteLine(Error, L"Could not find NetworkEntity for equipment: %p",
      equipment);
    //log(L"Could not find NetworkEntity for equipment: %p",
    //  equipment);
    return;
  }

  // Client
  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetSuppressed_SendEquipmentUnequip() &&
      Client::getSingleton().GetSuppressed_EquipmentCreation())
    {
      // Send the server that we're unequipping the equipment
      NetworkMessages::InventoryRemoveEquipment msgInventoryRemoveEquipment;
      msgInventoryRemoveEquipment.set_ownerid(ownerEntity->getCommonId());
      msgInventoryRemoveEquipment.set_equipmentid(equipmentEntity->getCommonId());

      Client::getSingleton().SendMessage<NetworkMessages::InventoryRemoveEquipment>(C_PUSH_EQUIPMENT_UNEQUIP, &msgInventoryRemoveEquipment);
    }
  }

  // Server
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    if (!Server::getSingleton().GetSuppressed_SendEquipmentUnequip() &&
      !Server::getSingleton().GetSuppressed_SendEquipmentEquip()) 
    {
      // Create a Network Message for sending off to clients the equipment addition to the inventory
      NetworkMessages::InventoryRemoveEquipment msgInventoryRemoveEquipment;
      msgInventoryRemoveEquipment.set_ownerid(ownerEntity->getCommonId());
      msgInventoryRemoveEquipment.set_equipmentid(equipmentEntity->getCommonId());

      Server::getSingleton().BroadcastMessage<NetworkMessages::InventoryRemoveEquipment>(S_PUSH_EQUIPMENT_UNEQUIP, &msgInventoryRemoveEquipment);
    }
  }
}

void TLMP::Character_PickupEquipmentPre(CCharacter* character, CEquipment* equipment, CLevel* level, bool & calloriginal)
{
  log(L"Character picking up Equipment Pre: %p Type: %x", equipment, equipment->type__);
  log(L"  Position: %f %f %f", 
    equipment->GetPosition().x,
    equipment->GetPosition().y,
    equipment->GetPosition().z);

  // Make sure the item is in the level before attempting to pick it up
  //   halts crashes on multiple CItemGold pickups after it's been destroyed
  {
    NetworkEntity *netEquipment = searchEquipmentByInternalObject((PVOID)equipment);
    if (!netEquipment) {
      netEquipment = searchItemByInternalObject((PVOID)equipment);
    }

    if (netEquipment) {
      CItem *item = (CItem*)netEquipment->getInternalObject();
      if (!gameClient->pCLevel->containsItem(item)) {
        log("Couldn't find item in the level, jumping out before we crash...");
        calloriginal = false;
        return;
      }
    }
  }

  multiplayerLogger.WriteLine(Info, L" Character::PickupEquipmentPre(Character: %p) (%p) (Level: %p)",
    character, equipment, level);
  multiplayerLogger.WriteLine(Info, L" Character::PickupEquipmentPre(Character: %s) (%s)",
    character->characterName.c_str(), equipment->nameReal.c_str());
  //log(L" Character::PickupEquipment(Character: %p) (%p %s) (Level: %p)",
  //  character, equipment, equipment->nameReal.c_str(), level);

  //log(L"Dumping Level Items...");
  //gameClient->pCLevel->DumpItems();
  //gameClient->pCLevel->DumpTriggerUnits();

  

  // If the client is requesting a pickup, request the server first
  // The server will respond with a pickup message
  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    if (Client::getSingleton().GetSuppressed_EquipmentPickup()) {
      // Remove the IsSendingPickup since I'm putting in Requests to pickup item
      //if (!Client::getSingleton().Get_IsSendingPickup()) {
        // Send the server a message requesting our character to pickup the item
        NetworkEntity *netPlayer = searchCharacterByInternalObject((PVOID)character);
        NetworkEntity *netEquipment = searchEquipmentByInternalObject((PVOID)equipment);

        if (!netEquipment) {
          netEquipment = searchItemByInternalObject((PVOID)equipment);
        }

        if (!netPlayer) {
          multiplayerLogger.WriteLine(Error, L"Error: Could not retrieve network entity of character for equipment pickup!");
          log(L"Error: Could not retrieve network entity of character for equipment pickup!");
        /*} else *if (!netEquipment) {
          multiplayerLogger.WriteLine(Error, L"Error: Could not retrieve network entity of equipment for equipment pickup!");
          log(L"Error: Could not retrieve network entity of equipment for equipment pickup!");*/
        } else {
          NetworkMessages::EquipmentPickup msgEquipmentPickup;
          msgEquipmentPickup.set_characterid(netPlayer->getCommonId());

          if (netEquipment) {
            msgEquipmentPickup.set_equipmentid(netEquipment->getCommonId());
          } else {
            msgEquipmentPickup.set_equipmentid(-1);
          }

          NetworkMessages::Position *msgPosition = msgEquipmentPickup.mutable_position();
          msgPosition->set_x(equipment->GetPosition().x);
          msgPosition->set_y(equipment->GetPosition().y);
          msgPosition->set_z(equipment->GetPosition().z);

          Client::getSingleton().SendMessage<NetworkMessages::EquipmentPickup>(C_REQUEST_EQUIPMENT_PICKUP, &msgEquipmentPickup);
          Client::getSingleton().Set_IsSendingPickup(true);
        }
      //}

      calloriginal = false;
    }
    else {
      Client::getSingleton().SetSuppressed_EquipmentCreation(false);
    }
  }
  // Send the message off to the clients
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    NetworkEntity *netEquipment = searchEquipmentByInternalObject(equipment);
    NetworkEntity *netCharacter = searchCharacterByInternalObject(character);

    if (!netEquipment) {
      netEquipment = searchItemByInternalObject((PVOID)equipment);
    }

    if (netEquipment) {
      if (netCharacter) {
        NetworkMessages::EquipmentPickup msgEquipmentPickup;
        msgEquipmentPickup.set_characterid(netCharacter->getCommonId());
        msgEquipmentPickup.set_equipmentid(netEquipment->getCommonId());

        NetworkMessages::Position *msgPosition = msgEquipmentPickup.mutable_position();
        msgPosition->set_x(equipment->GetPosition().x);
        msgPosition->set_y(equipment->GetPosition().y);
        msgPosition->set_z(equipment->GetPosition().z);

        Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentPickup>(S_PUSH_EQUIPMENT_PICKUP, &msgEquipmentPickup);
        //Server::getSingleton().SetSuppressed_SendEquipmentEquip(true);

        // Remove the Equipment from the Ground list
        vector<NetworkEntity*>::iterator itr;
        for (itr = ServerEquipmentOnGround->begin(); itr != ServerEquipmentOnGround->end(); itr++) {
          if ((*itr) == netEquipment) {
            ServerEquipmentOnGround->erase(itr);
            break;
          }
        }
      } else {
        multiplayerLogger.WriteLine(Error, L"Error: Could not find Network Entity for Character (%s)",
          character->characterName.c_str());
        //log(L"Error: Could not find Network Entity for Character (%s)",
        //  character->characterName.c_str());
      }
    } else {
      multiplayerLogger.WriteLine(Error, L"Error: Could not find Network Entity for Equipment (%s)",
        equipment->nameReal.c_str());
      //log(L"Error: Could not find Network Entity for Equipment (%s)",
      //  equipment->nameReal.c_str());
    }
  }
}

void TLMP::Character_PickupEquipmentPost(CCharacter* character, CEquipment* equipment, CLevel* level, bool & calloriginal)
{
  log(L" Character picking up Equipment Post");

  /*
  CInventory* inv = character->pCInventory;
  multiplayerLogger.WriteLine(Info, L"  Debug sizes: %i", inv->tabIndices.size());
  log(L"  Debug sizes: %i", inv->tabIndices.size());

  for (u32 i = 0; i < inv->tabIndices.size(); ++i) {
    multiplayerLogger.WriteLine(Info, L"  Tab[%i] = %i", inv->tabIndices[i], inv->tabSizes[i]);
    log(L"  Tab[%i] = %i", inv->tabIndices[i], inv->tabSizes[i]);
  }
  */

  //log(L"Dumping Level Items...");
  //gameClient->pCLevel->DumpItems();
  //gameClient->pCLevel->DumpTriggerUnits();
  
  // Turn on suppression again if we're the client
  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    // If the original call was let through...
    if (calloriginal) {
      Client::getSingleton().SetSuppressed_EquipmentCreation(true);
      Client::getSingleton().Set_IsSendingPickup(false);
    }
  }
  // Server turn off suppression of Equipment Equips
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    Server::getSingleton().SetSuppressed_SendEquipmentEquip(false);
  }

  //log(L"Character Done Picking up Item");
}

void TLMP::Character_SetActionPre(CCharacter* character, u32 action, bool & calloriginal)
{
  //logColor(B_GREEN, L"Character::SetActionPre(%s %x)", character->characterName.c_str(), action);

  // The below doesn't work too well when transferring over the network.
  // The client will receive the state (or action) with creature deathes and the creatures will appear to be still
  //   alive but unattackable.

  /*
  multiplayerLogger.WriteLine(Info, L"Character::SetActionPre(%s, %x)", character->characterName.c_str(), action);
  log(L"Character::SetActionPre(%s %x)", character->characterName.c_str(), action);

  // Client - Requests the set action from the server
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (Client::getSingleton().GetSuppressed_CharacterAction()) {
      calloriginal = false;
    }
  }

  // Server - Always send this out to every client becuase it's request-based
  else if (NetworkState::getSingleton().GetState() == SERVER) {
    NetworkEntity *networkCharacter = searchCharacterByInternalObject(character);

    if (networkCharacter) {
      NetworkMessages::CharacterAction msgCharacterAction;
      msgCharacterAction.set_characterid(networkCharacter->getCommonId());
      msgCharacterAction.set_action(action);

      Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterAction>(S_PUSH_CHARACTER_ACTION, &msgCharacterAction);
    } else {
      multiplayerLogger.WriteLine(Error, L"Error: Could not find network id for character: %s", character->characterName.c_str());
      log(L"Error: Could not find network id for character: %s", character->characterName.c_str());
    }
  }
  */
}

void TLMP::SkillManager_AddSkillPre(CSkillManager* skillManager, CSkill* skill, bool skillAlreadyCreated, u32 unk1, bool& calloriginal)
{
  //log(L"SkillManager (%p) AddSkill: %p", skillManager, skill);
  //log(L"   skillAlreadyCreated: %x   unk1: %x  GUID: %016I64X", skillAlreadyCreated, unk1, skill->GUID);

  //log(L"   baseUnit: %p", skillManager->pCBaseUnit);
  //log(L"   name: %s", ((CCharacter*)(skillManager->pCBaseUnit))->characterName.c_str());

  //skillManager->dumpSkillManager();
}

void TLMP::SkillManager_SetSkillLevelPre(CSkillManager* skillManager, CSkill* skill, u32 skillLevel, bool& calloriginal)
{
  /*
  log(L"SkillManager (%p) SetSkillLevelPre: %p  skillLevel: %i", skillManager, skill, skillLevel);
  log(L"  Character: %016I64X", skillManager->pCBaseUnit->GUID);
  */

  multiplayerLogger.WriteLine(Info, L"SkillManager (%p) SetSkillLevelPre: %p  skillLevel: %i", 
    skillManager, skill, skillLevel);
  multiplayerLogger.WriteLine(Info, L"  Character: %016I64X", skillManager->pCBaseUnit->GUID);

  //
  CCharacter* character = (CCharacter*)skillManager->pCBaseUnit;
  NetworkEntity *netPlayer = searchCharacterByInternalObject((PVOID)character);

  if (Network::NetworkState::getSingleton().GetState() == SINGLEPLAYER) {
    return;
  }

  if (!netPlayer) {
    //log(L"Error: Could not find networkID for character: %p", character);
    return;
  }

  // Push off the network message
  NetworkMessages::CharacterSetSkillPoints msgCharacterSetSkillPoints;
  msgCharacterSetSkillPoints.set_characterid(netPlayer->getCommonId());
  msgCharacterSetSkillPoints.set_skillguid(skill->GUID);
  msgCharacterSetSkillPoints.set_skilllevel(skillLevel);

  // If the client is requesting a pickup, request the server first
  // The server will respond with a pickup message
  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetAllow_SetSkillPoints()) {
      Client::getSingleton().SendMessage<NetworkMessages::CharacterSetSkillPoints>(C_REQUEST_SKILLLEVEL, &msgCharacterSetSkillPoints);
      calloriginal = false;
    }
  }
  // Send the message off to the clients
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterSetSkillPoints>(S_PUSH_SKILLLEVEL, &msgCharacterSetSkillPoints);
  }
}

void TLMP::SkillManager_SetSkillLevelPost(CSkillManager* skillManager, CSkill* skill, u32 skillLevel, bool& calloriginal)
{
  /*
  log(L"SkillManager (%p) SetSkillLevelPost: %p  skillLevel: %i", skillManager, skill, skillLevel);
  log(L"  Level: %i", skill->skillLevel);
  */
}

void TLMP::Character_SetupSkillsPre(CCharacter* character, CDataGroup* dataGroup, u32 unk0, bool& calloriginal)
{
}

void TLMP::Character_AddSkillPre(CCharacter* character, wstring* skillName, u32 unk1, bool& calloriginal)
{
  /*
  log(L"Character::AddSkillPre: %p", character);
  log(L"   %s", skillName->c_str());
  log(L"   Character Available Skill Points: %i", character->GetAvailableSkillPoints());

  // Appears to be a flag if the character is valid
  if (unk1) {
    log(L"   %s", character->characterName.c_str());
  }
  */
}

void TLMP::Character_AddSkillPost(CCharacter* character, wstring* skillName, u32 unk1, bool& calloriginal)
{
  //log(L"Character::AddSkillPost: %p", character);
}

void TLMP::Character_AddSkill2Pre(CCharacter* character, wstring skillName, bool& calloriginal)
{
  //log(L"Character::AddSkill2: %s  %s", character->characterName.c_str(), skillName.c_str());
}

void TLMP::Character_UseSkillPre(CCharacter* character, u64 skillGUID, bool & calloriginal)
{
  if (skillGUID != 0xFFFFFFFFFFFFFFFF) {
    //log(L"Test: Character Mana max: %i", character->manaMax);

    /*
    log(L"Character (%s) used skill pre: (%016I64X)", character->characterName.c_str(), skillGUID);
    log(L"  SkillManager: %p (Offset: %x)", character->pCSkillManager, ((u32)&character->pCSkillManager - (u32)character));
    */

    multiplayerLogger.WriteLine(Info, L"Character (%s) used skill pre: (%016I64X)",
      character->characterName.c_str(), skillGUID);
    multiplayerLogger.WriteLine(Info, L"  SkillManager: %p (Offset: %x)",
      character->pCSkillManager, ((u32)&character->pCSkillManager - (u32)character));

    // Testing for Skill Graphic Effect
    if (!character->usingSkill) {
      /*
      log(L"  EffectManager: %p", character->pCEffectManager);
      log(L"  Type: %x", character->type__);
      log(L"  Skill: %p", character->pCSkill);
      log(L"  ResourceManager: %p", character->pCResourceManager);
      log(L"  pCSkillManager: %p", character->pCSkillManager);
      log(L"  UsingSkill: %i", character->usingSkill);
      */
      
      //character->pCSkillManager->dumpSkillManager();
    } else {
      // Save us the trouble of the constant keydown function spam
      calloriginal = false;
    }
    // --

    NetworkEntity *netPlayer = searchCharacterByInternalObject((PVOID)character);

    if (!netPlayer) {
      multiplayerLogger.WriteLine(Error, L"Error: Could not retrieve network entity of character for equipment pickup!");
      //log(L"Error: Could not retrieve network entity of character for equipment pickup!");
      return;
    }

    // Send the skill level before we push off the skill use,
    //
    {
      CSkill* skill = character->pCSkillManager->getSkillFromGUID(skillGUID);
      if (!skill) {
        log("Error: Could not find skill in skillmanager of guid: %016I64X", skillGUID);
        return;
      }

      NetworkMessages::CharacterSetSkillPoints msgCharacterSetSkillPoints;
      msgCharacterSetSkillPoints.set_characterid(netPlayer->getCommonId());
      msgCharacterSetSkillPoints.set_skillguid(skill->GUID);
      msgCharacterSetSkillPoints.set_skilllevel(skill->skillLevel);

      // If the client is requesting a pickup, request the server first
      // The server will respond with a pickup message
      if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
        Client::getSingleton().SendMessage<NetworkMessages::CharacterSetSkillPoints>(C_REQUEST_SKILLLEVEL, &msgCharacterSetSkillPoints);
      }
      // Send the message off to the clients
      else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
        Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterSetSkillPoints>(S_PUSH_SKILLLEVEL, &msgCharacterSetSkillPoints);
      }
    }

    //
    NetworkMessages::CharacterUseSkill msgCharacterUseSkill;
    msgCharacterUseSkill.set_characterid(netPlayer->getCommonId());
    msgCharacterUseSkill.set_skill(skillGUID);

    NetworkMessages::Position *msgPosition = msgCharacterUseSkill.mutable_direction();
    msgPosition->set_x(character->orientation.x);
    msgPosition->set_y(character->orientation.y);
    msgPosition->set_z(character->orientation.z);

    if (!character->usingSkill) {
      // If the client is requesting a pickup, request the server first
      // The server will respond with a pickup message
      if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
        if (!Client::getSingleton().GetSuppressed_SendUseSkill()) {
          Client::getSingleton().SendMessage<NetworkMessages::CharacterUseSkill>(C_REQUEST_CHARACTER_USESKILL, &msgCharacterUseSkill);
          calloriginal = false;
        }
      }
      // Send the message off to the clients
      else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
        Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentPickup>(S_PUSH_CHARACTER_USESKILL, &msgCharacterUseSkill);
      }
    }
  }
}

void TLMP::Character_UseSkillPost(CCharacter* character, u64 skillGUID, bool & calloriginal)
{
  if (skillGUID != 0xFFFFFFFFFFFFFFFF) {
    //log(L"Character_UseSkillPost: Character Mana: %f / %i", character->manaCurrent, character->manaMax);

    multiplayerLogger.WriteLine(Info, L"Character (%s) used skill post: (%016I64X)",
      character->characterName.c_str(), skillGUID);
    //log(L"Character (%s) used skill post: (%016I64X)",
    //  character->characterName.c_str(), skillGUID);

    // Turn on suppression again if we're the client
    if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
      // If the original call was let through...
      if (calloriginal) {
        Client::getSingleton().Set_IsSendingUseSkill(false);
      }
    }
    // Server turn off suppression of Equipment Equips
    else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
      Server::getSingleton().SetSuppressed_SendEquipmentEquip(false);
    }
  }
}

void TLMP::Character_SetOrientationPre(CCharacter* character, Vector3* orient1, float dTime, bool& calloriginal)
{
  /*
  if (character == gameClient->pCPlayer) {
    log(L"Character_SetOrientationPre: %s (%p)", character->characterName.c_str(), orient1);
    log(L" Current: %f, %f, %f", character->orientation.x, character->orientation.y, character->orientation.z);
    log(L" Orient1: %f %f %f", orient1->x, orient1->y, orient1->z);
    log(L" dTime: %f", dTime);
    log(L" unkBool0: %i  usingSkill: %i", character->unkBool0, character->usingSkill);
  }
  */
}

void TLMP::Character_SetTarget(CCharacter* character, CCharacter* target, bool & calloriginal)
{
  if (target != NULL) {
    //multiplayerLogger.WriteLine(Info, L"Character (%s) Set Target: %s", character->characterName.c_str(), target->characterName.c_str());
    //log(L"Character (%s) Set Target: %s", character->characterName.c_str(), target->characterName.c_str());
  } else {
    //multiplayerLogger.WriteLine(Info, L"Character (%s) Set Target: null", character->characterName.c_str());
  }

  // Setup the information
  NetworkEntity *netCharacter = searchCharacterByInternalObject(character);
  NetworkEntity *netTarget = searchCharacterByInternalObject(target);

  NetworkMessages::CharacterSetTarget msgCharacterSetTarget;

  if (netCharacter) {
    s32 targetID = -1;    // Sometimes there's no target
    if (netTarget) {
      targetID = netTarget->getCommonId();
    }

    msgCharacterSetTarget.set_characterid(netCharacter->getCommonId());
    msgCharacterSetTarget.set_targetid(targetID);
  } else {
    //log(L"Error: Could not find network entity for character: %s", character->characterName.c_str());
    return;
  }

  // Network push it out
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetAllow_CharacterSetTarget()) {
      //calloriginal = false;

      //if (character == gameClient->pCPlayer) {
      // Attempt to send AI request to server if the character has no current target but wants one
      if (!character->target && target) {
        Client::getSingleton().SendMessage<NetworkMessages::CharacterSetTarget>(C_REQUEST_CHARACTER_SETTARGET, &msgCharacterSetTarget);
      }
    }
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    // Only send out target changes
    map<CCharacter*, CCharacter*>::iterator itr;
    itr = CharacterTargetBuffer->find(character);
    if (itr != CharacterTargetBuffer->end()) {
      if (itr->second != target) {
        log(L"New target set: %p -> %p", character, target);
        //Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterSetTarget>(S_PUSH_CHARACTER_SETTARGET, &msgCharacterSetTarget);
      }
    }
    (*CharacterTargetBuffer)[character] = target;
  }
}

void TLMP::Character_AttackPost(CCharacter* character, bool & calloriginal)
{
  /*
  log(L"Character (%s) Set Attack Post", character->characterName.c_str());
  log(L"  Test: %x %x %x %x", character->destroy, character->destroy1, character->destroy2, character->destroy3);
  log(L"  Test: %x %x %x %x", character->moving, character->unkBool0, character->attacking, character->usingSkill);
  */
}

void TLMP::Character_AttackPre(CCharacter* character, bool & calloriginal)
{
  multiplayerLogger.WriteLine(Info, L"Character (%s) Set Attack Pre", character->characterName.c_str());

  /*
  log(L"Character (%s) Set Attack Pre", character->characterName.c_str());
  log(L"  Test: %x %x %x %x", character->destroy, character->destroy1, character->destroy2, character->destroy3);
  log(L"  Test: %x %x %x %x", character->moving, character->unkBool0, character->attacking, character->usingSkill);
  */

  /*
  log(L"Character (%s) Set Attack Pre", character->characterName.c_str());

  if (character->pCAttackDescription)
    character->pCAttackDescription->dumpAttack();
  log(L"Character Attack Descriptions: %i", character->attackDescriptions.size());
  vector<CAttackDescription*>::iterator itr;
  for (itr = character->attackDescriptions.begin(); itr != character->attackDescriptions.end(); itr++) {
    log(L"  Character Attack Desc:");
    (*itr)->dumpAttack();
  }
  */

  // Client - Requests the set action from the server
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (Client::getSingleton().GetSuppressed_CharacterAttack()) {
      //calloriginal = false;

      NetworkEntity *networkCharacter = searchCharacterByInternalObject(character);

      if (networkCharacter) {
        NetworkMessages::CharacterAttack msgCharacterAttack;
        msgCharacterAttack.set_characterid(networkCharacter->getCommonId());

        Client::getSingleton().SendMessage<NetworkMessages::CharacterAttack>(C_PUSH_CHARACTER_ATTACK, &msgCharacterAttack);
      } else {
        multiplayerLogger.WriteLine(Error, L"Error: Could not find network id for character: %s", character->characterName.c_str());
        //log(L"Error: Could not find network id for character: %s", character->characterName.c_str());
      }
    }
  }

  // Server - Always send this out to every client becuase it's request-based
  else if (NetworkState::getSingleton().GetState() == SERVER) {
    if (!Server::getSingleton().GetSuppressed_SendCharacterAttack()) {
      NetworkEntity *networkCharacter = searchCharacterByInternalObject(character);

      if (networkCharacter) {
        NetworkMessages::CharacterAttack msgCharacterAttack;
        msgCharacterAttack.set_characterid(networkCharacter->getCommonId());

        Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterAttack>(S_PUSH_CHARACTER_ATTACK, &msgCharacterAttack);
      }
    } else {
      multiplayerLogger.WriteLine(Error, L"Error: Could not find network id for character: %s", character->characterName.c_str());
      //log(L"Error: Could not find network id for character: %s", character->characterName.c_str());
    }
  }
}

void TLMP::EquipmentAddStackCountPost(CEquipment *equipment, u32 amount)
{
  //logColor(B_RED, L"Add StackCount: %s %i", equipment->nameReal.c_str(), amount);

  NetworkEntity *netEquipment = searchEquipmentByInternalObject(equipment);

  if (netEquipment) {
    NetworkMessages::EquipmentUpdateStackSize msgEquipmentUpdateStackSize;
    msgEquipmentUpdateStackSize.set_id(netEquipment->getCommonId());
    msgEquipmentUpdateStackSize.set_change_amount(amount);

    // Client
    if (NetworkState::getSingleton().GetState() == CLIENT) {
      if (!Client::getSingleton().GetSuppressed_SendEquipmentStack()) {
        Client::getSingleton().SendMessage<NetworkMessages::EquipmentUpdateStackSize>(C_PUSH_EQUIPMENT_STACKUPDATE, &msgEquipmentUpdateStackSize);
      }
    }

    // Server
    else if (NetworkState::getSingleton().GetState() == SERVER) {
      if (!Server::getSingleton().GetSuppressed_SendEquipmentStack()) {
        Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentUpdateStackSize>(S_PUSH_EQUIPMENT_STACKUPDATE, &msgEquipmentUpdateStackSize);
      }
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Error: Could not find network id for equipment: %s", equipment->nameReal.c_str());
    //log(L"Error: Could not find network id for equipment: %s", equipment->nameReal.c_str());
  }
}

void TLMP::GameClientSaveGamePre(CGameClient *gameClient, u32 unk0, u32 unk1, bool & calloriginal)
{
  //log(L"Suppressing Game Save: %p (%x %x)", gameClient, unk0, unk1);
  multiplayerLogger.WriteLine(Info, L"Suppressing Game Save: %p (%x %x)", gameClient, unk0, unk1);

  CLevel *level = gameClient->pCLevel;

  // Remove any characters that have been added through multiplayer before saving
  LinkedListNode* itrInv = *level->charactersAll;
  while (itrInv != NULL) {
    CCharacter* character = (CCharacter*)itrInv->pCBaseUnit;

    if (character) {
      if (character != gameClient->pCPlayer && 
          (character->GUID == DESTROYER || 
           character->GUID == DESTROYER || 
           character->GUID == DESTROYER))
      {
        vector<CCharacter*>::iterator itrMinion;
        vector<CCharacter*> *minions = character->GetMinions();
        for (itrMinion = minions->begin(); itrMinion != minions->end(); itrMinion++) {
          (*itrMinion)->Destroy();
        }

        character->Destroy();
      }
    }

    itrInv = itrInv->pNext;
  }

  // Fix bug when trying to save game on client that is loading
  if (gameClient->pCLevel->pCLevelLayout == NULL) {
    calloriginal = false;
  }

  //callOriginal = false;

  /* Removed - New characters will try to save right off and Disconnect - move this
     to a proper function where the game exits.

  if (NetworkState::getSingleton().GetState() == CLIENT) {
    // Force disconnect, crashes when rejoining
    log(L"Forcing client disconnect.");
    multiplayerLogger.WriteLine(Info, L"Forcing client disconnect.");

    Client::getSingleton().Disconnect();
    NetworkState::getSingleton().SetState(SINGLEPLAYER);
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    log(L"Forcing server disconnect.");
    multiplayerLogger.WriteLine(Info, L"Forcing server disconnect.");

    Server::getSingleton().Shutdown();
    NetworkState::getSingleton().SetState(SINGLEPLAYER);
  }
  */
}

void TLMP::GameClientSaveGamePost(CGameClient *gameClient, u32 unk0, u32 unk1, bool & callOriginal)
{
  //log(L"Suppressed Game Save Successful");
  multiplayerLogger.WriteLine(Info, L"Suppressed Game Save Successful");

}

void TLMP::GameClientGamePausedPost(bool& retval, CGameClient *gameClient, bool & calloriginal)
{
  retval = false;
}

void TLMP::GameUI_TriggerPausePre(CGameUI *gameUI, bool & calloriginal)
{
  calloriginal = false;
}

void TLMP::EnchantMenuEnchantItemPre(CEnchantMenu* enchantMenu, bool & calloriginal)
{
  // Suppress enchants
  {
    multiplayerLogger.WriteLine(Info, L"EnchantMenu suppressed.");
    log(L"EnchantMenu suppressed.");

    calloriginal = false;

    return;
  }

  multiplayerLogger.WriteLine(Info, L"EnchantMenu::EnchantItem(%p) Type = %x", enchantMenu, enchantMenu->EnchantType);
  log(L"EnchantMenu::EnchantItem(%p) Type = %x", enchantMenu, enchantMenu->EnchantType);

  multiplayerLogger.WriteLine(Info, L"EnchantMenu debug: Player: %p (%s), Character: %p (%s)",
    enchantMenu->player, enchantMenu->player->characterName.c_str(), enchantMenu->character, enchantMenu->character->characterName.c_str());
  log(L"EnchantMenu debug: Player: %p (%s), Character: %p (%s)",
    enchantMenu->player, enchantMenu->player->characterName.c_str(), enchantMenu->character, enchantMenu->character->characterName.c_str());
  
  // !!!
  // This is assuming the gameClient->Player is the Owner
  CInventory *inventory = gameClient->pCPlayer->pCInventory;
  CEquipment *equipment = inventory->GetEquipmentFromSlot(0xE);
  //log(L"  Acting on Equipment: %s", equipment->nameReal.c_str());
  multiplayerLogger.WriteLine(Info, L"  Acting on Equipment: %s", equipment->nameReal.c_str());

  if (!inventory) {
    //log(L"  Inventory is null!");
    multiplayerLogger.WriteLine(Error, L"  Inventory is null!");
  }

  NetworkEntity* netEntity = searchEquipmentByInternalObject(equipment);
  if (netEntity) {
    if (enchantMenu->EnchantType == 0x19) {
      //log(L"Destroying gems from Equipment...");
      multiplayerLogger.WriteLine(Info, L"Destroying gems from Equipment...");

      NetworkMessages::EquipmentRemoveGems msgEquipmentRemoveGems;
      msgEquipmentRemoveGems.set_equipmentid(netEntity->getCommonId());

      if (NetworkState::getSingleton().GetState() == CLIENT) {
        calloriginal = false;
        Client::getSingleton().SendMessage<NetworkMessages::EquipmentRemoveGems>(C_PUSH_EQUIPMENT_REMOVE_GEMS, &msgEquipmentRemoveGems);
      } else if (NetworkState::getSingleton().GetState() == SERVER) {
        Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentRemoveGems>(S_PUSH_EQUIPMENT_REMOVE_GEMS, &msgEquipmentRemoveGems);
      }
    } else if (enchantMenu->EnchantType == 0x1A) {
      //log(L"Removing gems and destroying equipment...");
      multiplayerLogger.WriteLine(Info, L"Removing gems and destroying equipment...");
    }
  } else {
    //log(L"Error: Could not find NetworkEntity for equipment");
    multiplayerLogger.WriteLine(Error, L"Error: Could not find NetworkEntity for equipment");
  }
}

void TLMP::EquipmentAddGemPre(CEquipment* equipment, CEquipment* gem, bool & calloriginal)
{
  //log(L"Equipment (%s) Adding Gem (%s)", equipment->nameReal.c_str(), gem->nameReal.c_str());
  multiplayerLogger.WriteLine(Info, L"Equipment (%s) Adding Gem (%s)", equipment->nameReal.c_str(), gem->nameReal.c_str());

  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (gameClient->inGame) {
      if (!Client::getSingleton().Get_IsSendingEquipmentAddGem()) {
        calloriginal = false;

        NetworkEntity *netEquipment = searchEquipmentByInternalObject(equipment);
        NetworkEntity *netGem = searchEquipmentByInternalObject(gem);

        if (netEquipment && netGem) {
          // Send the server a message requsting the Gem be Added to the equipment
          NetworkMessages::EquipmentAddGem msgEquipmentAddGem;
          msgEquipmentAddGem.set_equipmentid(netEquipment->getCommonId());
          msgEquipmentAddGem.set_gemid(netGem->getCommonId());

          Client::getSingleton().SendMessage<NetworkMessages::EquipmentAddGem>(C_PUSH_EQUIPMENT_ADD_GEM, &msgEquipmentAddGem);
        } else {
          //log(L"Error: Client could not send off EquipmentAddGem - Could not find NetworkEntity for either Equipment (%p) or Gem (%p)",
          //  netEquipment, netGem);
          multiplayerLogger.WriteLine(Error, L"Error: Client could not send off EquipmentAddGem - Could not find NetworkEntity for either Equipment (%p) or Gem (%p)",
            netEquipment, netGem);
        }
      }
    }
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    NetworkEntity *netEquipment = searchEquipmentByInternalObject(equipment);
      NetworkEntity *netGem = searchEquipmentByInternalObject(gem);

      if (netEquipment && netGem) {
        // Send the server a message requsting the Gem be Added to the equipment
        NetworkMessages::EquipmentAddGem msgEquipmentAddGem;
        msgEquipmentAddGem.set_equipmentid(netEquipment->getCommonId());
        msgEquipmentAddGem.set_gemid(netGem->getCommonId());

        Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentAddGem>(S_PUSH_EQUIPMENT_ADD_GEM, &msgEquipmentAddGem);
      } else {
        //log(L"Error: Server could not send off EquipmentAddGem - Could not find NetworkEntity for either Equipment (%p) or Gem (%p)",
        //  netEquipment, netGem);
        multiplayerLogger.WriteLine(Error, L"Error: Server could not send off EquipmentAddGem - Could not find NetworkEntity for either Equipment (%p) or Gem (%p)",
          netEquipment, netGem);
      }
  }
}

// Uses the KeyManager_HandleInput now to suppress this if needed
void TLMP::GameUI_HandleKeyboardInputPre(CGameUI* gameUI, u32 unk0, u32 unk1, u32 unk2, bool & calloriginal)
{
  /*
  //log(L"GameUI::HandleKeyboardInput (%p, %x, %x, %x)", gameUI, unk0, unk1, unk2);
  multiplayerLogger.WriteLine(Info, L"GameUI::HandleKeyboardInput (%p, %x, %x, %x)", gameUI, unk0, unk1, unk2);

  CEGUI::Editbox* pInGameChatEntry = (CEGUI::Editbox*)UserInterface::getWindowFromName("1010_ChatEntry");
  CEGUI::Window* pInGameChatEntryBackground = UserInterface::getWindowFromName("1010_ChatEntryBackground");

  if (pInGameChatEntry && pInGameChatEntryBackground) {
    // Keydown and 'Enter'
    if (unk0 == 0x100 && unk1 == 0xD) {
      if (!pInGameChatEntry->isActive()) {
        log("%p %p", pInGameChatEntry, pInGameChatEntryBackground);
        log("vis: %i", pInGameChatEntry->isVisible());

        pInGameChatEntryBackground->setVisible(true);        
        pInGameChatEntry->show();
        pInGameChatEntry->activate();
      }
    }
  }
  */
}

void TLMP::GameUI_HandleKeyboardInputPost(CGameUI* gameUI, u32 unk0, u32 unk1, u32 unk2, bool& calloriginal)
{
  // Keydown and 'Enter'
  if (unk0 == 0x100 && unk1 == 0xD) {
    CEGUI::Editbox* pInGameChatEntry = (CEGUI::Editbox*)UserInterface::getWindowFromName("1010_ChatEntry");
    CEGUI::Window* pInGameChatEntryBackground = UserInterface::getWindowFromName("1010_ChatEntryBackground");

    if (pInGameChatEntry && pInGameChatEntryBackground) {
      if (!pInGameChatEntry->isActive()) {
        pInGameChatEntryBackground->setVisible(true);        
        pInGameChatEntry->show();
        pInGameChatEntry->activate();
      }
    }
  }
}

void TLMP::KeyManager_HandleInputPre(CKeyManager* keyManager, u32 unk0, u32 unk1, bool & calloriginal)
{
  //log(L"KeyManager::HandleInput(%p %x %x)", keyManager, unk0, unk1);
  multiplayerLogger.WriteLine(Info, L"KeyManager::HandleInput(%p %x %x)", keyManager, unk0, unk1);

  CEGUI::Editbox* pInGameChatEntry = (CEGUI::Editbox*)UserInterface::getWindowFromName("1010_ChatEntry");
  if (pInGameChatEntry) {
    if (pInGameChatEntry->isVisible()) {
      calloriginal = false;
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find 1010_ChatEntry");
  }
}

void TLMP::MouseManager_HandleInputPre(CMouseManager* mouseManager, u32 wParam, u32 mouseButton, bool& calloriginal)
{
  //log(L"MouseManager::handleInput(%p %x %x)", mouseManager, wParam, mouseButton);

  if (wParam == 0x201 || wParam == 0x204) {
    CEGUI::Editbox* pInGameChatEntry = (CEGUI::Editbox*)UserInterface::getWindowFromName("1010_ChatEntry");
    if (pInGameChatEntry) {
      if (pInGameChatEntry->hasInputFocus()) {
        //log(L"Suppressing Mouse Input - ChatEntry has focus");
        calloriginal = false;
      }
    } else {
      multiplayerLogger.WriteLine(Error, L"Error could not find 1010_ChatEntry");
    }

    /*
    CEGUI::PushButton* pInGameChatSay = (CEGUI::PushButton*)UserInterface::getWindowFromName("1010_Say");
    if (pInGameChatSay) {
      if (pInGameChatSay->isPushed()) {
        log(L"Suppressing Mouse Input - ChatHistory has focus");
        calloriginal = false;
      }
    } else {
      multiplayerLogger.WriteLine(Error, L"Error could not find 1010_Say");
    }
    */
  }
}

void TLMP::GameUI_WindowResizedPost(CGameUI* game, bool & calloriginal)
{
  //log(L"Window Resized");
  multiplayerLogger.WriteLine(Info, L"Window Resized");
  
  ResizeUI();
}

void TLMP::Character_Character_UpdatePre(CCharacter* character, PVOID octree, float* unk0, float unk1, bool& calloriginal)
{
  //CharacterUpdateTime[character].reset();
  
  u32 bitFieldVisibility = (character->flagFadingIn << 2) | (character->flagHasBeenDisplayed << 1) | (character->flagFadingOut);

  // DEBUG
  bitFieldVisibility |= (character->unkTransparencyFlag1 << 6) | (character->unkTransparencyFlag2 << 5) | (character->unkTransparencyFlag3 << 4) | (character->unkTransparencyFlag5 << 3);
  // --

  map<CCharacter*, u32>::iterator itr = CharacterVisibilityBuffer->find(character);
  if (itr != CharacterVisibilityBuffer->end()) {
    // Already exists, if it differs push it out to clients
    if ((*CharacterVisibilityBuffer)[character] != bitFieldVisibility) {
       if (NetworkState::getSingleton().GetState() == SERVER) {
        NetworkEntity *netCharacter = searchCharacterByInternalObject(character);

        if (netCharacter) {
          NetworkMessages::Visibility msgVisibility;

          // DEBUG
          log(L" Character:  %p %s %f %f", character, character->characterName.c_str(), character->unkTransparency0, character->unkTransparency1);
          log(L"  Fading in: %i", character->flagFadingIn);
          log(L"  HasBeenDisplayed: %i", character->flagHasBeenDisplayed);
          log(L"  Fading out: %i", character->flagFadingOut);
          log(L"  Other: %i %i %i %i", character->unkTransparencyFlag1, character->unkTransparencyFlag2, character->unkTransparencyFlag3, character->unkTransparencyFlag5);
          // --

          msgVisibility.set_id(netCharacter->getCommonId());
          msgVisibility.set_fading_in(character->flagFadingIn);
          msgVisibility.set_has_been_displayed(character->flagHasBeenDisplayed);
          msgVisibility.set_fading_out(character->flagFadingOut);
          msgVisibility.set_transparency(character->unkTransparency1);

          Server::getSingleton().BroadcastMessage<NetworkMessages::Visibility>(S_PUSH_CHARACTER_VISIBILITY, &msgVisibility);
        }
      }
    }
  }
  (*CharacterVisibilityBuffer)[character] = bitFieldVisibility;
}

void TLMP::Character_Character_UpdatePost(CCharacter* character, PVOID octree, float* unk0, float unk1, bool& calloriginal)
{
  //log(L"  CharacterUpdateTime: %s %f  (%f, %f, %f)", character->characterName.c_str(), CharacterUpdateTime[character].getTime(),
  //  character->position.x, character->position.y, character->position.z);

  // Off weird characters that may have been setup by the client... don't know how these got created but they're all at Y=100.0
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (character->GetPosition().y >= 100.0)
      character->destroy = true;
  }

  /*
  // Testing Visibility
  if (!wcscmp(L"Shade", character->characterName.c_str())) 
  {
    static u8 oldVis = character->visibility_test2;
    static u32* petData = NULL;
    static u32 petDataSize = 0;

    log(L"    Post: unkPos[1]: %i", character->unkPositionable[1]);

    if (character->visibility_test2 != oldVis) {
      oldVis = character->visibility_test2;
      log(L"Dumping Shade, visibility changed to: %i  offset: %x", character->visibility_test2, (u32*)&character->unkPositionable - (u32*)character);
      character->dumpCharacter(petData, petDataSize);

      log(L"  Flags: %x %x %x %x", character->unkFlags[0], character->unkFlags[1], character->unkFlags[2], character->unkFlags[3]);
      character->unkPositionable[1] = 0;
    }

    //log(L"%s Update: vis: %i  state: %i  check: %x   %i %i %i %i   %i %i %i (offset: %x)", character->characterName.c_str(), character->visibility_test2,
    //  character->state,
    //  character->unk6,
    //  character->unk06[0], character->unk06[1], character->unk06[2], character->unk06[3], 
    //  character->destroy1, character->destroy2, character->destroy3,
    //  &character->unk06[1] - (u8*)character);
   
  }
  */
}

void TLMP::GameClient_ChangeLevelPre(CGameClient* client, wstring dungeonName, s32 level, u32 unk0, u32 unk1, wstring str2, u32 unk2, bool& calloriginal)
{
  logColor(B_RED, L"GameClient::ChangeLevel (%s %i %i %i %s %i)", dungeonName.c_str(), level, unk0, unk1, str2.c_str(), unk2);
  multiplayerLogger.WriteLine(Info, L"GameClient::ChangeLevel (%s %i %i %i %s %i)", dungeonName.c_str(), level, unk0, unk1, str2.c_str(), unk2);

  // Remove any other player characters and equipment or else we'll crash
  // TODO: For my Destroyer "Drivehappy"

  // Suppress the change if we're the client
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetAllow_ChangeLevel()) {
      calloriginal = false;
    }
  }
  else if (NetworkState::getSingleton().GetState() == SERVER) {
    NetworkMessages::ChangeLevel msgChangeLevel;

    string sDungeonName(dungeonName.begin(), dungeonName.end());
    sDungeonName.assign(dungeonName.begin(), dungeonName.end());
    string sUnkString(str2.begin(), str2.end());
    sUnkString.assign(str2.begin(), str2.end());

    msgChangeLevel.set_dungeon(sDungeonName);
    msgChangeLevel.set_level(level);
    msgChangeLevel.set_unk0(unk0);
    msgChangeLevel.set_unk1(unk1);
    msgChangeLevel.set_unkstring(sUnkString);
    msgChangeLevel.set_unk2(unk2);

    Server::getSingleton().BroadcastMessage<NetworkMessages::ChangeLevel>(S_PUSH_CHANGE_LEVEL, &msgChangeLevel);
  }

  // If we're going ahead with the change level, remove all the network information,
  // we'll repopulate it when the new level loads
  if (calloriginal) {
    removeAllNetworkBaseUnits();

    // Flush Network IDs
    //logColor(B_GREEN, L"Flushing item network IDs");
    clearAllNetworkIDs();
  }

  // Suppress level changes
  //calloriginal = false;
}

void TLMP::GameClient_ChangeLevelPost(CGameClient* client, wstring dungeonName, s32 level, u32 unk0, u32 unk1, wstring str2, u32 unk2, bool& calloriginal)
{
  //log(L"GameClient::ChangeLevel Post");
  multiplayerLogger.WriteLine(Info, L"GameClient::ChangeLevel Post");
}

void TLMP::Global_SetSeedValue0Pre(u32 seed, bool& calloriginal)
{
  Seed1 = (u32*)EXEOFFSET(SeedOffset1);
  Seed2 = (u32*)EXEOFFSET(SeedOffset2);
  Seed3 = (u32*)EXEOFFSET(SeedOffset3);
  Seed4 = (u32*)EXEOFFSET(SeedOffset4);

  if (NetworkState::getSingleton().GetState() == CLIENT) {
    seed = Client::getSingleton().GetSeed();
    //logColor(B_GREEN, L"Client reseting seed to:: %x (%i)", seed, seed);

    *Seed1 = seed;
    *Seed2 = 0;
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    //logColor(B_GREEN, L"Server reseting seed to:: %x (%i)", *Seed1, *Seed1);
    
    *Seed1 = Server::getSingleton().GetSeed();
    *Seed2 = 0; 
  }

  calloriginal = false;
}

void TLMP::Global_SetSeedValue0Post(u32 seed, bool &calloriginal)
{
  /*
  static u32 rand = time(NULL);

  logColor(B_GREEN, L"GameClient SetSeedValue0 Post: %x (%i)", seed, seed);
  multiplayerLogger.WriteLine(Info, L"GameClient SetSeedValue0 Post: %x (%i)", seed, seed);

  // Ugly, but fix later
  Seed1 = (u32*)EXEOFFSET(SeedOffset1);
  Seed2 = (u32*)EXEOFFSET(SeedOffset2);
  Seed3 = (u32*)EXEOFFSET(SeedOffset3);
  Seed4 = (u32*)EXEOFFSET(SeedOffset4);

  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (Client::getSingleton().GetAllow_RandomSeed()) {
      logColor(B_GREEN, L"Client setting random seed to: %x", seed);
      *Seed1 = seed;
      *Seed2 = seed;
      *Seed3 = seed;
      *Seed4 = seed;
    }
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    seed = rand;

    NetworkMessages::RandomSeed msgRandomSeed;
    msgRandomSeed.set_seed(seed);
    
    Server::getSingleton().BroadcastMessage<NetworkMessages::RandomSeed>(S_PUSH_RANDOM_SEED, &msgRandomSeed);
  }
  */
}


void TLMP::Global_SetSeedValue2Pre(u32 seed, bool& calloriginal)
{
  //logColor(B_GREEN, L"GameClient SetSeedValue2 Pre: %x (%i)", seed, seed);
  //calloriginal = false;
}

void TLMP::Global_SetSeedValue2Post(u32 seed, bool& calloriginal)
{
  //logColor(B_GREEN, L"GameClient SetSeedValue2 Post: %x (%i)", seed, seed);
  multiplayerLogger.WriteLine(Info, L"GameClient SetSeedValue2 Post: %x (%i)", seed, seed);
}

void TLMP::TriggerUnit_TriggeredPre(CTriggerUnit* triggerUnit, CPlayer* player, bool& calloriginal)
{
  log(L"TriggerUnit: %p %p (%f, %f, %f)", triggerUnit, player, 
    triggerUnit->GetPosition().x, triggerUnit->GetPosition().y, triggerUnit->GetPosition().z);

  // Breakable was null on client, adding here to be safe -
  // This can be null, e.g. when door after bad Brink opens up
  //if (!player)
  //  return;

  if (triggerUnit->type__ != OPENABLE && triggerUnit->type__ != INTERACTABLE) {
    return;
  }

  NetworkEntity *netCharacter = NULL;
  NetworkEntity *entity = searchItemByInternalObject(triggerUnit);

  if (player) {
    logColor(B_RED, L"TriggerUnit (%s)(%p) trigger by player (%s)", triggerUnit->nameReal.c_str(), triggerUnit, player->characterName.c_str());
    multiplayerLogger.WriteLine(Info, L"TriggerUnit (%s)(%p) trigger by player (%s)", triggerUnit->nameReal.c_str(), triggerUnit, player->characterName.c_str());

    netCharacter = searchCharacterByInternalObject(player);
  }

 
  NetworkMessages::TriggerUnitTriggered msgTriggerUnitTrigger;
  if (netCharacter) {
    msgTriggerUnitTrigger.set_characterid(netCharacter->getCommonId());
  } else {
    msgTriggerUnitTrigger.set_characterid(-1);
  }

  if (entity) {
    msgTriggerUnitTrigger.set_itemid(entity->getCommonId());
  }

  NetworkMessages::Position *msgPosition = msgTriggerUnitTrigger.mutable_position();
  if (triggerUnit->GetPosition().length() > EPSILON) {
    msgPosition->set_x(triggerUnit->GetPosition().x);
    msgPosition->set_y(triggerUnit->GetPosition().y);
    msgPosition->set_z(triggerUnit->GetPosition().z);
  }

  // Client
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetSuppressed_SendTriggerUnitTriggered()) {
      calloriginal = false;
      Client::getSingleton().SendMessage<NetworkMessages::TriggerUnitTriggered>(C_REQUEST_TRIGGER_TRIGGERED, &msgTriggerUnitTrigger);
    }
  }

  // Server
  else if (NetworkState::getSingleton().GetState() == SERVER) {
    Server::getSingleton().BroadcastMessage<NetworkMessages::TriggerUnitTriggered>(S_PUSH_TRIGGER_TRIGGERED, &msgTriggerUnitTrigger);
  }
}

void TLMP::TriggerUnit_Triggered2Pre(CTriggerUnit* triggerUnit, CCharacter* character, bool& calloriginal)
{
  log(L"TriggerUnit Triggered2Pre: %p %p", triggerUnit, character);
}

void TLMP::Breakable_TriggeredPre(CBreakable* breakable, CPlayer* player, bool& calloriginal)
{
  log(L"Breakable: %p %p", breakable, player);
  log(L"  Position: %f %f %f", breakable->GetPosition().x, breakable->GetPosition().y, breakable->GetPosition().z);

  if (player) {
    logColor(B_RED, L"Breakable item (%s) triggered by Player (%s)", breakable->nameReal.c_str(), player->characterName.c_str());
    multiplayerLogger.WriteLine(Info, L"Breakable item (%s) triggered by Player (%s)", breakable->nameReal.c_str(), player->characterName.c_str());
  }

  NetworkEntity *netCharacter = searchCharacterByInternalObject(player);
  NetworkEntity *entity = searchItemByInternalObject(breakable);

  //if (entity) {
    if (netCharacter) {
      NetworkMessages::BreakableTriggered msgBreakableTriggered;

      if (entity) {
        msgBreakableTriggered.set_itemid(entity->getCommonId());
      } else {
        msgBreakableTriggered.set_itemid(-1);
      }
      
      // Character can be null if skill was used to kill it
      if (player) {
        msgBreakableTriggered.set_characterid(netCharacter->getCommonId());
      } else {
        msgBreakableTriggered.set_characterid(-1);
      }

      // Build the position of the breakable object
      NetworkMessages::Position *msgPosition = msgBreakableTriggered.mutable_position();
      msgPosition->set_x(breakable->GetPosition().x);
      msgPosition->set_y(breakable->GetPosition().y);
      msgPosition->set_z(breakable->GetPosition().z);

      // Client
      if (NetworkState::getSingleton().GetState() == CLIENT) {
        if (!Client::getSingleton().GetSuppressed_SendBreakableTriggered()) {
          calloriginal = false;
          Client::getSingleton().SendMessage<NetworkMessages::BreakableTriggered>(C_REQUEST_BREAKABLE_TRIGGERED, &msgBreakableTriggered);
        }
      }

      // Server
      else if (NetworkState::getSingleton().GetState() == SERVER) {
        Server::getSingleton().BroadcastMessage<NetworkMessages::BreakableTriggered>(S_PUSH_BREAKABLE_TRIGGERED, &msgBreakableTriggered);
      }
    } else {
      //log(L"Error: Could not find network character in network shared list");
    }
  //} else {
    //log(L"Error: Could not find breakable item in network shared list");
  //}
}

void TLMP::TriggerUnit_CtorPost(CTriggerUnit* triggerUnit, CLayout* layout, bool& calloriginal)
{
  //logColor(B_RED, L"TriggerUnit Created: %p", triggerUnit);
}

void TLMP::ItemGold_CtorPost(CItemGold* itemGold, PVOID _this, CResourceManager* resourceManager, u32 amount, bool&)
{
  //logColor(B_RED, L"ItemGold Created: %p %p %i", itemGold, resourceManager, amount);
  //logColor(B_RED, L"   GUID: %016I64X", itemGold->GUID);
  //logColor(B_RED, L"   Amount: %i", itemGold->amount);

  if (NetworkState::getSingleton().GetState() == SERVER) {
    NetworkEntity *entity = addItem(itemGold);

    NetworkMessages::ItemGoldCreate msgItemGoldCreate;
    msgItemGoldCreate.set_itemid(entity->getCommonId());
    msgItemGoldCreate.set_amount(itemGold->amount);
    Server::getSingleton().BroadcastMessage<NetworkMessages::LevelCreateItem>(S_PUSH_ITEM_GOLD, &msgItemGoldCreate);

    //logColor(B_RED, L"   ID: %x", entity->getCommonId());
  }
}

void TLMP::ItemGold_CtorPre(CItemGold* itemRetval, PVOID _this, CResourceManager* resourceManager, u32 amount, bool& calloriginal)
{
  if (!calloriginal) {
    //logColor(B_RED, L"ItemGold Ctor Suppressed for Testing");
  }

  //calloriginal = false;
  //itemRetval = NULL;
}

void TLMP::Character_StrikePre(CCharacter* character, CLevel* level, CCharacter* characterOther, PVOID unk0, u32 unk1, float unk2, float unk3, u32 unk4, bool& calloriginal)
{
  logColor(B_GREEN, L"Character Strike: %p %p %p   %p %x %f %f %x", 
    character, level, characterOther, unk0, unk1, unk2, unk3, unk4);

  NetworkEntity *attacker = searchCharacterByInternalObject(character);
  NetworkEntity *defender = searchCharacterByInternalObject(characterOther);

  if (attacker && defender) {
    if (NetworkState::getSingleton().GetState() == CLIENT) {
      NetworkMessages::CharacterStrikeCharacter msgCharacterStrike;
      msgCharacterStrike.set_characteridsource(attacker->getCommonId());
      msgCharacterStrike.set_characteridtarget(defender->getCommonId());
      msgCharacterStrike.set_unk0(unk2);
      msgCharacterStrike.set_unk1(unk3);
      msgCharacterStrike.set_unk2(unk4);

      Client::getSingleton().SendMessage<NetworkMessages::CharacterStrikeCharacter>(C_PUSH_CHARACTER_STRIKE, &msgCharacterStrike);
    }
  }
}

void TLMP::Character_ResurrectPre(CCharacter* character, bool& calloriginal)
{
  log(L"PlayerResurrectPre (%p)", character);
  log(L"   Name: (%s)", character->characterName.c_str());

  NetworkEntity *entity = searchCharacterByInternalObject(character);

  NetworkMessages::CharacterResurrect msgCharacterResurrect;
  if (entity) {
    msgCharacterResurrect.set_characterid(entity->getCommonId());
  }

  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetAllow_CharacterResurrect()) {
      calloriginal = false;

      if (entity) {
        Client::getSingleton().SendMessage<NetworkMessages::CharacterResurrect>(C_REQUEST_CHARACTER_RESURRECT, &msgCharacterResurrect);
      }
    } else {
      if (character) {
        character->healthCurrent = character->healthMax;
      }
    }
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    if (entity) {
      Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterResurrect>(S_PUSH_CHARACTER_RESURRECT, &msgCharacterResurrect);
    }
  }
}

void TLMP::EquipmentRefDtorPre(CEquipmentRef* equipmentRef, u32 unk0)
{
  //log(L"EquipmentRef::Dtor Pre (%p %x Equipment: %p)", equipmentRef, unk0, equipmentRef->pCEquipment);
  multiplayerLogger.WriteLine(Info, L"EquipmentRef::Dtor Pre (%p %x Equipment: %p)", equipmentRef, unk0, equipmentRef->pCEquipment);

  // Move through the character inventory items and invalid any equipment tied to the same Ref
  CEquipment *equipment = equipmentRef->pCEquipment;
  CLevel *level = gameClient->pCLevel;
  if (level) {
    LinkedListNode* itr = *level->charactersAll;
    while (itr != NULL) {
      CCharacter* character = (CCharacter*)itr->pCBaseUnit;
      if (character) {
        CInventory* inventory = character->pCInventory;
        if (inventory) {
          for (u32 i = 0; i < inventory->equipmentList.size; ++i) {
            CEquipmentRef *equipmentRef2 = inventory->equipmentList[i];
            //log(L"DEBUG: %i %i - %p %p", i, inventory->equipmentList.size, inventory, equipmentRef2);
            if (equipmentRef2) {
              if (equipment == equipmentRef2->pCEquipment && equipmentRef2 != equipmentRef) {
                log(L"Removed duplicate equipment in equipmentRef: %p", equipmentRef2);
                multiplayerLogger.WriteLine(Info, L"Removed duplicate equipment in equipmentRef: %p", equipmentRef2);

                equipmentRef2->pCEquipment = NULL;
              }
            }
          }
        }
      }

      itr = itr->pNext;
    }
  }
}

void TLMP::EquipmentRefDtorPost(CEquipmentRef* equipmentRef, u32 unk0)
{
  //log(L"EquipmentRef::Dtor Post");
  multiplayerLogger.WriteLine(Info, L"EquipmentRef::Dtor Post");

  // Invalidate our equipment
  if (equipmentRef->pCEquipment) {
    equipmentRef->pCEquipment = NULL;
  }
}

void TLMP::Monster_GetCharacterClosePre(CCharacter*& retval, CMonster* monster, u32 unk0, float unk1, bool& calloriginal)
{
  //log(L"Monster GetCharacterClosePre: %s, %i %f",  monster->characterName.c_str(),unk0, unk1);

  //calloriginal = false;
  //retval = NULL;
}

void TLMP::Monster_GetCharacterClosePost(CCharacter*& retval, CMonster* monster, u32 unk0, float unk1, bool& calloriginal)
{
  if (retval) {
    //log("  retval: %p  monster: %p %i %f", retval, monster, unk0, unk1);
    //log(L"Monster GetCharacterClosePost: %s, %i %f  to: %s", monster->characterName.c_str(), unk0, unk1, retval->characterName.c_str());
  }
}

void TLMP::Monster_ProcessAIPre(CMonster* monster, float dTime, u32 unk0, bool & calloriginal)
{
  //log(L"Monster_ProcessAIPre (%p %s), %f %p", monster, monster->characterName.c_str(), dTime, unk0);

  if (monster->GUID == DESTROYER || monster->GUID == ALCHEMIST || monster->GUID == VANQUISHER) {
    calloriginal = false;
  }

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    //calloriginal = false;
  }
}

void TLMP::Monster_ProcessAI2Pre(CMonster* monster, float dTime, CLevel *level, u32 unk1, bool & calloriginal)
{
  //log(L"Monster_ProcessAI2Pre (%s), %f %p %i", monster->characterName.c_str(), dTime, level, unk1);

  //calloriginal = false;

  if (monster->GUID == DESTROYER || monster->GUID == ALCHEMIST || monster->GUID == VANQUISHER) {
    calloriginal = false;
  }

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    //calloriginal = false;
  }
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    //if (monster->GUID == DESTROYER || monster->GUID == ALCHEMIST || monster->GUID == VANQUISHER) {
    //  calloriginal = false;
    //}
  }
}
void TLMP::Monster_ProcessAI3Pre(CMonster* monster, u32 unk0, bool & calloriginal)
{
  //log(L"Monster_ProcessAI3Pre (%s), %i", monster->characterName.c_str(), unk0);

  if (monster->GUID == DESTROYER || monster->GUID == ALCHEMIST || monster->GUID == VANQUISHER) {
    calloriginal = false;
  }

  if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
    calloriginal = false;
  }
  else if (Network::NetworkState::getSingleton().GetState() == SERVER) {
    //if (monster->GUID == DESTROYER || monster->GUID == ALCHEMIST || monster->GUID == VANQUISHER) {
      //calloriginal = false;
    //}
  }
}

void TLMP::Character_Update_LevelPre(CCharacter* character, CLevel* level, float unk0, bool& calloriginal)
{
  //if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
  //  calloriginal = false;
  //}
}

void TLMP::Character_Update_CharacterPre(CCharacter*, CCharacter*, bool& calloriginal)
{
  //if (Network::NetworkState::getSingleton().GetState() == CLIENT) {
  //  calloriginal = false;
  //}
}

void TLMP::Level_UpdatePre(CLevel*, Vector3*, u32, float, bool& calloriginal)
{
  LevelUpdateTime.reset();
}

void TLMP::Level_UpdatePost(CLevel* level, Vector3* position, u32 unk0, float unk1, bool&)
{
  /*
  log("Level_UpdatePost: %f", LevelUpdateTime.getTime());
  log("  Position: %f %f %f", position->x, position->y, position->z);
  log("  unk0: %i   unk1: %f", unk0, unk1);
  log("  Character Count: %i", level->GetCharacterCount());
  */
}

void TLMP::Level_Level_RemoveEquipmentPre(CLevel* level, CEquipment* equipment, bool&)
{
  //log("Level_Level_RemoveEquipmentPre -- Level: %p, Equipment: %p", level, equipment);
}

void TLMP::Level_Level_RemoveEquipmentPost(CLevel* level, CEquipment* equipment, bool&)
{
  //log("Level_Level_RemoveEquipmentPost -- Level: %p, Equipment: %p", level, equipment);
}

void TLMP::Effect_EffectSomethingPre(CEffect* effect, CEffect* other, bool & calloriginal)
{
  /*
  log(L"EffectSomethingPre: %p %p", effect, other);
  log(L"  BaseUnit: %p", other->pCBaseUnit);

  // Other
  if (other) {
    log(L"    other Name: %s", other->name.c_str());
    log("    other Type: %s %i", searchForEffectName(other->effectType), other->effectType);
    log(L"    other listUnknown size: %x", other->listUnknown.size);

    u32 maxTerm = 20;
    for (u32 i = 0; i < other->listUnknown.size && maxTerm >= 0; ++i, --maxTerm) {
      log(L"     other listUnknown[%i]: %p", i, other->listUnknown[i]);
    }

    if (other->pCBaseUnit) {
      log(L"  BaseUnit2 Type: %x (%016I64X)", other->pCBaseUnit->type__, other->pCBaseUnit->GUID);

      if (other->pCBaseUnit->type__ == 0x1B) {
        CCharacter* character = (CCharacter*)other->pCBaseUnit;
        log(L"    Character Name: %s", character->characterName.c_str());
      }
    }
  }
  */
}

void TLMP::Effect_EffectSomethingPost(CEffect* effect, CEffect* other, bool & calloriginal)
{
  //log(L"EffectSomethingPost: %p %p", effect, other);
}

void TLMP::Effect_Character_Unk0Pre(CEffect* effect, CCharacter* character, bool unk)
{
  //log(L"Effect::Character_Unk0Pre: %p %p %i", effect, character, unk);
}

void TLMP::Effect_Character_Unk0Post(CEffect* effect, CCharacter* character, bool unk)
{
  //log(L"Effect::Character_Unk0Post: %p %p %i", effect, character, unk);
}

void TLMP::Level_CleanupPre(CLevel* level, u32 unk0, u32 unk1, bool& calloriginal)
{
  log(L"Level::Cleanup Pre: %s %x %x", level->levelName.c_str(), unk0, unk1);
}

void TLMP::Level_CleanupPost(CLevel* level, u32 unk0, u32 unk1, bool& calloriginal)
{
  log(L"Level::Cleanup Post: %s %x %x", level->levelName.c_str(), unk0, unk1);
}


void TLMP::Character_Player_KillMonsterExperiencePre(CCharacter* player, CLevel* level, CCharacter* monster, u32 experience, u32 unk0, bool& calloriginal)
{
  //log(L"Player Kill Monster Experience Pre");
  //log(L"Player Killed Monster Pre: %s %p", player->characterName.c_str(), monster);

  if (monster) {
    //log(L"Player Killed Monster Pre: %s worth %i experience, unk0: %x", monster->characterName.c_str(), experience, unk0);
    //log(L"  Test: %x %x %x %x", monster->destroy, monster->destroy1, monster->destroy2, monster->destroy3);
    //log(L"  Test: %x %x %x %x", monster->moving, monster->unkBool0, monster->attacking, monster->usingSkill);
  }

  if (NetworkState::getSingleton().GetState() == SINGLEPLAYER) {
    return;
  }

  // Work on network messages
  NetworkEntity *netCharacter = searchCharacterByInternalObject(player);
  if (!netCharacter) {
    //log(L"Error: Could not find network id for player: %s", player->characterName.c_str());
    return;
  }

  // Client
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetAllow_AddExperience()) {
      calloriginal = false;
    }
  }
  // Server
  else if (NetworkState::getSingleton().GetState() == SERVER) {
    if (!Server::getSingleton().GetSuppressed_SendAddExperience()) {
      // Only send experience when the player is tagged as a killer (should always happen, and only 1 player should be tagged so)
      if (player->GUID == ALCHEMIST || player->GUID == DESTROYER || player->GUID == VANQUISHER) {
        NetworkMessages::CharacterAddExperience msgCharacterAddExperience;
        msgCharacterAddExperience.set_characterid(netCharacter->getCommonId());
        msgCharacterAddExperience.set_experience(experience);
        msgCharacterAddExperience.set_unk0(unk0);

        Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterAddExperience>(S_PUSH_ADDEXPERIENCE, &msgCharacterAddExperience);

        // If the player isn't ours, go ahead and give our player the experience too
        if (gameClient->pCPlayer != player) {
          Server::getSingleton().SetSuppressed_SendAddExperience(true);
          gameClient->pCPlayer->KillMonsterExperience(level, NULL, experience, unk0);
          Server::getSingleton().SetSuppressed_SendAddExperience(false);
        }
      }
    }
  }
}

void TLMP::Character_Player_KillMonsterExperiencePost(CCharacter* player, CLevel* level, CCharacter* monster, u32 experience, u32 unk0, bool& calloriginal)
{
  //log(L"Player Killed Monster Post: %p  experience: %i", monster, experience);

  if (monster) {
    //log(L"Player Killed Monster Pre: %s worth %i experience, unk0: %x", monster->characterName.c_str(), experience, unk0);
    //log(L"  Test: %x %x %x %x", monster->destroy, monster->destroy1, monster->destroy2, monster->destroy3);
    //log(L"  Test: %x %x %x %x", monster->moving, monster->unkBool0, monster->attacking, monster->usingSkill);
  }
}

void TLMP::Level_Level_CharacterKilledCharacterPre(CLevel* level, CCharacter* character, CCharacter* other, Vector3* position, u32 unk0, bool& calloriginal)
{
  /*
  log(L"Level_CharacterKilledCharacterPre: character = %p", character);

  if (character) {
    log(L"Level_CharacterKilledCharacterPre: %p, %s, %f %f %f,  unk0: %x",
      level, character->characterName.c_str(), position->x, position->y, position->z, unk0);
  }
  */

  // Skip sending, this is probably a barrel and we sync it up anyways
  if (!other) {
    return;
  }

  // 
  if (NetworkState::getSingleton().GetState() == SERVER) {
    NetworkEntity *entityCharacter = searchCharacterByInternalObject(character);
    NetworkEntity *entityOther = searchCharacterByInternalObject(other);

    if (!entityCharacter) {
      //log(L"Error: Could not find network ID for : %s", character->characterName.c_str());
      return;
    }

    if (!entityOther) {
      //log(L"Error: Could not find network ID for character: %s", other->characterName.c_str());
      return;
    }

    NetworkMessages::CharacterKilledCharacter msgCharacterKilledCharacter;
    NetworkMessages::Position *msgPosition = msgCharacterKilledCharacter.mutable_position();

    msgPosition->set_x(position->x);
    msgPosition->set_y(position->y);
    msgPosition->set_z(position->z);

    msgCharacterKilledCharacter.set_characterid(entityCharacter->getCommonId());
    msgCharacterKilledCharacter.set_otherid(entityOther->getCommonId());
    msgCharacterKilledCharacter.set_unk0(unk0);

    Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterKilledCharacter>(S_PUSH_CHARACTERKILLED, &msgCharacterKilledCharacter);
  }
}

void TLMP::Level_Level_CharacterKilledCharacterPost(CLevel* level, CCharacter* character, CCharacter* other, Vector3* position, u32 unk0, bool& calloriginal)
{
  //log(L"Level_CharacterKilledCharacterPost");
}

void TLMP::Character_KilledPre(CCharacter* charKilled, CCharacter* charKillingBlow, Ogre::Vector3* direction, float unk0, u32 unk1, bool& calloriginal)
{
  /*
  log(L"Character KilledPre: direction: %p", direction);

  if (charKillingBlow && direction) {
    log(L"  Character KilledPre: %s   %s, %f %f %f  %f %x",
      charKilled->characterName.c_str(), charKillingBlow->characterName.c_str(), direction->x, direction->y, direction->z, unk0, unk1);
  }
  */

  //calloriginal = false;
}

void TLMP::Character_KilledPost(CCharacter* charKilled, CCharacter* charKillingBlow, Ogre::Vector3* direction, float unk0, u32 unk1, bool& calloriginal)
{
  /*
  log(L"Character KilledPost");

  if (charKillingBlow && direction) {
    log(L"  Character KilledPost: %s   %s, %f %f %f  %f %x",
      charKilled->characterName.c_str(), charKillingBlow->characterName.c_str(), direction->x, direction->y, direction->z, unk0, unk1);
  }

  log(L"Character KilledPost2");
  */
}

void TLMP::Effect_EffectManagerCreateEffectPre(CEffect* retval, CEffectManager* effectManager)
{
  //log(L"EffectManagerCreate EffectPre: %p  Effects in Manager: %i", effectManager, effectManager->effectList.size);
}

void TLMP::Effect_EffectManagerCreateEffectPost(CEffect* retval, CEffectManager* effectManager)
{
  //log(L"EffectManagerCreate EffectPost: %p  Effects in Manager: %i", effectManager, effectManager->effectList.size);
  //log(L"      BaseUnit: %p", retval->pCBaseUnit);
}

void TLMP::EffectManager_AddEffectToEquipmentPre(CEffectManager* manager, CEquipment* equipment, CEffect* effect)
{
  //log(L"EffectManager_AddEffectToEquipmentPre: %p %p %p", manager, equipment, effect);
}

void TLMP::EffectManager_AddEffectToEquipmentPost(CEffectManager* manager, CEquipment* equipment, CEffect* effect)
{
  //log(L"EffectManager_AddEffectToEquipmentPost: %p %p %p", manager, equipment, effect);
  //log(L"   Effect Name: %s", effect->name.c_str());
}

void TLMP::BaseUnit_AddSkillPre(CBaseUnit* baseUnit, wstring* skillName, u32 unk0, bool& calloriginal)
{
  const u32 PLAYER = 0x1C;
  
  if (baseUnit->type__ != PLAYER) {
    return;
  }

  if (NetworkState::getSingleton().GetState() == SINGLEPLAYER) {
    return;
  }

  //log(L"BaseUnit AddSkillPre: %p %s %x", baseUnit, skillName->c_str(), unk0);
  //log(L"  SkillManager: %p", baseUnit->pCSkillManager);
  
  // Get the character
  CCharacter *character = (CCharacter*)baseUnit;

  NetworkEntity *netCharacter = searchCharacterByInternalObject(character);
  if (!netCharacter) {
    //log("Error: Could not find network ID for character: %s", character->characterName.c_str());
    return;
  }

  // Network push it out
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    // Do Nothing for now
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    string skillNameAscii(skillName->begin(), skillName->end());
    skillNameAscii.assign(skillName->begin(), skillName->end());

    NetworkMessages::BaseUnitAddSkill msgBaseUnitAddSkill;
    msgBaseUnitAddSkill.set_characterid(netCharacter->getCommonId());
    msgBaseUnitAddSkill.set_skillname(skillNameAscii.c_str());

    Server::getSingleton().BroadcastMessage<NetworkMessages::BaseUnitAddSkill>(S_PUSH_CHARACTER_ADD_SKILL, &msgBaseUnitAddSkill);
  }
}

void TLMP::BaseUnit_AddSkillPost(CBaseUnit* baseUnit, wstring* skillName, u32 unk0, bool& calloriginal)
{
  if (NetworkState::getSingleton().GetState() == SINGLEPLAYER) {
    return;
  }

  //log(L"BaseUnit AddSkillPost: %p %s %x", baseUnit, skillName->c_str(), unk0);
}

void TLMP::Path_GetNextNodePre(CPath* path, Vector3* vec, float unk0)
{
  return;

  if (path == gameClient->pCPlayer->pCPath)
  {
    log(L"Path::GetNextNode Pre (%p) %f %f %f,  %f", path, vec->x, vec->y, vec->z, unk0);
  }
}

void TLMP::Path_GetNextNodePost(CPath* path, Vector3* vec, float unk0)
{
  return; 

  if (path == gameClient->pCPlayer->pCPath)
  {
    log(L"Path::GetNextNode Post (%p) %f %f %f,  %f", path, vec->x, vec->y, vec->z, unk0);
    
    log(L" unk3:");
    vector<Vector3>::iterator itr;
    for (itr = path->unk3.begin(); itr != path->unk3.end(); itr++) {
      log(L"  %f, %f, %f", (*itr).x, (*itr).y, (*itr).z);
    }

    log(L" unk4:");
    vector<float>::iterator itr2;
    for (itr2 = path->unk4.begin(); itr2 != path->unk4.end(); itr2++) {
      log(L"  %f", (*itr2));
    }

    log(L" unk5:");
    for (itr2 = path->unk5.begin(); itr2 != path->unk5.end(); itr2++) {
      log(L"  %f", (*itr2));
    }

    log(L" unk6:");
    vector<u32>::iterator itr3;
    for (itr3 = path->unk6.begin(); itr3 != path->unk6.end(); itr3++) {
      log(L"  %x", (*itr3));
    }

    log(L" unk7:");
    for (itr2 = path->unk7.begin(); itr2 != path->unk7.end(); itr2++) {
      log(L"  %f", (*itr2));
    }
  }
}

void TLMP::Character_UpdateOrientationPre(CCharacter* character, float x, float z, bool& calloriginal)
{
  // Check if the new values differ before bothering to continue
  if (character->orientation.x != x || character->orientation.z != z) {
    //log(L"Character updated orientation: %p  %f %f", character, x, z);

    if (NetworkState::getSingleton().GetState() == SINGLEPLAYER) {
      return;
    }

    NetworkEntity *netCharacter = searchCharacterByInternalObject(character);
    if (!netCharacter) {
      log(L"Could not find network ID for character!");
      return;
    }

    u32 characterId = netCharacter->getCommonId();

    NetworkMessages::CharacterOrientation msgCharacterOrientation;
    NetworkMessages::Position *msgOrientation = msgCharacterOrientation.mutable_orientation();
    msgCharacterOrientation.set_characterid(characterId);
    msgOrientation->set_x(x);
    msgOrientation->set_y(0);
    msgOrientation->set_z(z);

    //
    if (NetworkState::getSingleton().GetState() == CLIENT) {
      if (Client::getSingleton().GetAllow_UpdateOrientation()) {
      } else {
        calloriginal = false;
        Client::getSingleton().SendMessage<NetworkMessages::CharacterOrientation>(C_REQUEST_ORIENTATION, &msgCharacterOrientation);
      }
    } else if (NetworkState::getSingleton().GetState() == SERVER) {
      // Push it out to clients
      Server::getSingleton().BroadcastMessage<NetworkMessages::CharacterOrientation>(S_PUSH_ORIENTATION, &msgCharacterOrientation);
    }
  }
}

void TLMP::Player_LevelUpPre(CPlayer* player, bool& calloriginal)
{
  log("Player leveled up Pre %p", player);

  if (gameClient->pCPlayer != player) {
    log("  Player leveled up not my character");
    //calloriginal = false;
  }
}

void TLMP::Player_LevelUpPost(CPlayer* player, bool& calloriginal)
{
  log("Player leveled up Post %p", player);
}

void TLMP::Player_SwapWeaponsPre(CCharacter* player, bool& calloriginal)
{
  static Timer delayInput;
  static bool firstSwap = true;
  const float inputDelay = 0.1f;

  NetworkEntity *netCharacter = searchCharacterByInternalObject(player);
  if (!netCharacter) {
    log(L"Could not find network ID for character!");
    return;
  }

  u32 characterId = netCharacter->getCommonId();

  NetworkMessages::PlayerSwapWeapons msgPlayerSwapWeapons;
  msgPlayerSwapWeapons.set_characterid(characterId);

  //
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (Client::getSingleton().GetAllow_WeaponSwap()) {
    } else {
      // Limit the amount of messages for this being sent out if this is our character
      if (player == gameClient->pCPlayer) {
        if (delayInput.getTime() < inputDelay && !firstSwap) {
          calloriginal = false;
          return;
        } else if (firstSwap) {
          firstSwap = false;
        } else {
          delayInput.reset();
        }

        calloriginal = false;
        Client::getSingleton().SendMessage<NetworkMessages::PlayerSwapWeapons>(C_REQUEST_WEAPONSWAP, &msgPlayerSwapWeapons);
        return;
      } else {
        // Halt any local weapon swaps not sent by the server and not on our character
        calloriginal = false;
      }
    }
  } else if (NetworkState::getSingleton().GetState() == SERVER) {
    // Limit the amount of messages for this being sent out if this is our character
    if (player == gameClient->pCPlayer) {
      if (delayInput.getTime() < inputDelay && !firstSwap) {
        calloriginal = false;
        return;
      } else if (firstSwap) {
        firstSwap = false;
      } else {
        delayInput.reset();
      }
    }

    // Push it out to clients
    Server::getSingleton().BroadcastMessage<NetworkMessages::PlayerSwapWeapons>(S_PUSH_WEAPONSWAP, &msgPlayerSwapWeapons);
  }

  log("Player swapping weapons: %p", player);
}

void TLMP::Effect_Something0Pre(CEffect* effect, u32 unk0, bool &calloriginal)
{
  //log("Effect_Something0: %p %i", effect, unk0);
}

void TLMP::Effect_Effect_ParamCtorPre(CEffect* effect, u32 unk0, bool unk1, bool unk2, float unk3, float unk4, float unk5, bool unk6, bool& calloriginal)
{
  //log("Effect::ParamCtor (%p %i (%s) %i %i %f %f %f %i)", effect, unk0, searchForEffectName(unk0), unk1, unk2, unk3, unk4, unk5, unk6);
}

void TLMP::Effect_CtorPre(CEffect* effect)
{
  //log(L"Effect::CtorPre (%p)", effect);
}

void TLMP::Effect_CtorPost(CEffect* effect)
{
  //log(L"Effect::CtorPost (%p)", effect);

  //effect->dumpEffect();
}


// Quest Manager
void TLMP::QuestManager_SetQuestCompletedPre(CQuestManager* questManager, CQuest* quest, CCharacter* character, u32 questActive, bool& calloriginal)
{
  log(L"QuestManager::SetQuestCompletedPre (QMgr: %p, Quest: %p, Character: %p, QuestActive: %i)", questManager, quest, character, questActive);

  if (quest) {
    log(L"  Quest Dungeon: %s", quest->dungeon.c_str());
    log(L"  Quest Location: %s", quest->location.c_str());
    log(L"  Quest Name: %s", quest->name.c_str());
  }
}

void TLMP::QuestManager_SetQuestCompletedPost(CQuestManager* questManager, CQuest* quest, CCharacter* character, u32 questActive, bool& calloriginal)
{
  log(L"QuestManager::SetQuestCompletedPost (QMgr: %p, Quest: %p, Character: %p, QuestActive: %i)", questManager, quest, character, questActive);
}

void TLMP::Automap_AddBillboardPre(CAutomap* automap, u32 unk0, float* unk1, Vector3* unk2, u32 unk3, u32 unk4, bool& calloriginal)
{
  // Fix bug when Torchlight tries to call this with this == NULL
  if (!automap) {
    log("Halted this = null call on Automap, yay!");
    multiplayerLogger.WriteLine(Error, L"Halted this = null call on Automap, yay!");

    calloriginal = false;
  }
}

void TLMP::InventoryMenu_OpenClosePre(CInventoryMenu *menu, bool open, bool& calloriginal)
{
  //log(L"InventoryMenu_OpenClosePre: %p, %i", menu, open);

  // Player inspection support

  // Check if target character has CPlayer vtable
  menu->player = gameClient->pCPlayer;
  menu->weaponSwapCheckBox->enable();

  if (gameClient->pCGameUI->pCTargetCharacter) {
    if (gameClient->pCGameUI->pCTargetCharacter->GUID == DESTROYER ||
        gameClient->pCGameUI->pCTargetCharacter->GUID == ALCHEMIST ||
        gameClient->pCGameUI->pCTargetCharacter->GUID == VANQUISHER)
    {
      log(L"    New character inv: %s, old char: %s", gameClient->pCGameUI->pCTargetCharacter->characterName.c_str(), menu->player->characterName.c_str());
      menu->player = (CPlayer*)gameClient->pCGameUI->pCTargetCharacter;
      menu->weaponSwapCheckBox->disable();
    }
  }

  // Set the correct weapon set for the selected player
  if (menu->weaponSwapCheckBox->isSelected() != (bool)menu->player->weaponSetToggle) {
    menu->weaponSwapCheckBox->setSelected(!menu->weaponSwapCheckBox->isSelected());
  }

  // Strip off the extra scene nodes if there is more than our target
  Ogre::SceneManager* sm = CMasterResourceManager::getInstance()->sceneManagerPlayerInventory;
  Ogre::SceneNode* root = sm->getRootSceneNode();

  Ogre::Node* targetNode = menu->player->pCGenericModel_Inventory->pOctreeNode_Inventory;
  root->removeAllChildren();
  root->addChild(targetNode);
}

void TLMP::InventoryMenu_MouseEventPre(CInventoryMenu* invMenu, const CEGUI::MouseEventArgs* args, bool & calloriginal)
{
  log(L"InventoryMenu_MouseEventPre:: %p", invMenu);
  log("InventoryMenu_MouseEventPre:: Window: %p %s", args->window, args->window->getName().c_str());

  /*
  log("Viewport Offset: %x", (u8*)&invMenu->paperDollViewport - (u8*)invMenu);
  log("weaponSwapCheckBox Offset: %x", (u8*)&invMenu->weaponSwapCheckBox - (u8*)invMenu);
  log("Viewport: %p", invMenu->paperDollViewport);
  
  //invMenu->paperDollViewport->setBackgroundColour(Ogre::ColourValue(0.13f, 0.07f, 0.21f));
  */

  if (invMenu->player != gameClient->pCPlayer) {
    // If we're inspecting another player then ignore mouse events
    calloriginal = false;
    log(L"Suppressing inventory menu mouse event - inspecting another player");
  }
}

void TLMP::Equipment_EnchantPre(u32 retval, CEquipment* equipment, u32 unk0, u32 unk1, u32 unk2, bool & calloriginal)
{
  log(L"Equipment_Enchant: (%p, %i, %i, %i)", equipment, unk0, unk1, unk2);
  multiplayerLogger.WriteLine(Info, L"Equipment_Enchant: (%p, %i, %i, %i)", equipment, unk0, unk1, unk2);

  log(L"Suppressing equipment enchant.");
  multiplayerLogger.WriteLine(Info, L"Suppressing equipment enchant.");

  // Prevent enchanting
  calloriginal = false;
}

void TLMP::ParticleCache_Dtor2Pre(CParticleCache *cache)
{
  //log(L"ParticleCache_Dtor2Pre: %p", cache);
  //log(L"  ParticleSystems: %i", cache->listParticleSystems.size);
  //log(L"  CParticleCache children: %i", cache->listParticleCache->size);
  /*
  log(L"  List Debug:");
  log(L"    unk1 size: %i", cache->unk1.size);
  log(L"    unk2 size: %i", cache->unk2.size);
  log(L"    unk3 size: %i", cache->unk3.size);
  log(L"    unk4 size: %i", cache->unk4.size);
  log(L"    unk5 size: %i", cache->unk5.size);
  log(L"    unk6 size: %i", cache->unk6.size);
  log(L"    unk7 size: %i", cache->unk7.size); 0x489F8D
  */

  //for (u32 i = 0; i < cache->listParticleSystems.size; ++i) {
  //  log(L"    ParticleSystem: %p", cache->listParticleSystems[i]); 
  //}
}

void TLMP::ParticleCache_Dtor2Post(CParticleCache *cache)
{
  //log(L"ParticleCache_Dtor2Post: %p", cache);
}

void TLMP::PositionableObject_SetNearPlayerPre(CPositionableObject* object, bool& nearPlayer, bool& calloriginal)
{ /*
  if (nearPlayer) {
    logColor(B_GREEN, L"PositionableObject_SetNearPlayerPre: %p %i", object, nearPlayer);
    logColor(B_GREEN, L"  name: %s", object->name.c_str());
  }
  */
}

void TLMP::Level_CheckCharacterProximityPre(CCharacter* retval, CLevel* level, Vector3* normalizedVec, u32 unk0, float x, float y, float z, u32 unk1, CCharacter* character, u32 unk2, bool& calloriginal)
{
  /*
  log(L"Level_CheckCharacterProximityPre: %p %p", level, character);
  log(L"  character: %s", character->characterName.c_str());
  log(L"  normVec: %f %f %f", normalizedVec->x, normalizedVec->y, normalizedVec->z);
  log(L"  unk0: %x   unk1: %x   unk2: %x", unk0, unk1, unk2);
  log(L"  x,y,z: %f %f %f", x, y, z);
  */
}

void TLMP::Level_CheckCharacterProximityPost(CCharacter* retval, CLevel* level, Vector3* normalizedVec, u32 unk0, float x, float y, float z, u32 unk1, CCharacter* character, u32 unk2, bool& calloriginal)
{
  /*
  if (retval) {
    log(L"Level_CheckCharacterProximityPre: %p %p %p", level, character, retval);
    if (character) {
      log(L"  character: %s", character->characterName.c_str());
    }
    log(L"  normVec: %f %f %f", normalizedVec->x, normalizedVec->y, normalizedVec->z);
    log(L"  unk0: %x   unk1: %x   unk2: %x", unk0, unk1, unk2);
    log(L"  x,y,z: %f %f %f", x, y, z);
    log(L"  retval: %s", retval->characterName.c_str());
  }
  */
}

void TLMP::Inventory_EquipmentAutoEquipPre(CInventory* inventory, CEquipment* equipment, bool& calloriginal)
{
  log("Inventory_EquipmentAutoEquipPre: %p %p", inventory, equipment);

  NetworkEntity *netCharacter = searchCharacterByInternalObject(inventory->pCCharacter);
  NetworkEntity *netEquipment = searchEquipmentByInternalObject(equipment);

  if (!netCharacter || !netEquipment) {
    log(L"Error: Could not find network id for character or equipment: %p %p", netCharacter, netEquipment);
    multiplayerLogger.WriteLine(Error, L"Error: Could not find network id for character or equipment: %p %p", netCharacter, netEquipment);
    return;
  }

  u32 characterId = netCharacter->getCommonId();
  u32 equipmentId = netEquipment->getCommonId();

  NetworkMessages::EquipmentAutoEquip msgEquipmentAutoEquip;
  msgEquipmentAutoEquip.set_characterid(characterId);
  msgEquipmentAutoEquip.set_equipmentid(equipmentId);

  //
  if (NetworkState::getSingleton().GetState() == CLIENT) {
    if (!Client::getSingleton().GetAllow_AutoEquip()) {
      calloriginal = false;

      Client::getSingleton().SendMessage<NetworkMessages::EquipmentAutoEquip>(C_REQUEST_AUTOEQUIP, &msgEquipmentAutoEquip);
    }
  } else if (NetworkState::getSingleton().GetState() == SERVER) {

    // Push it out to clients
    Server::getSingleton().BroadcastMessage<NetworkMessages::EquipmentAutoEquip>(S_PUSH_AUTOEQUIP, &msgEquipmentAutoEquip);
  }
}

void TLMP::Level_RemoveCharacterPre(CLevel* level, CCharacter* character, bool& calloriginal)
{
  logColor(B_RED, L"Level_RemoveCharacterPre: %p %p (%s)", level, character, character->characterName.c_str());
}

void TLMP::Level_RemoveItemPre(CLevel* level, CItem* item, bool& callorginal)
{
  logColor(B_RED, L"Level_RemoveItemPre: %p %p", level, item);

  NetworkEntity* netEquipment = NULL;
  log(L"Removing item from equipment/item network lists...");
  
  removeItem(item);
  removeEquipment(item);
}

void TLMP::Level_RemoveItemPost(CLevel* level, CItem* item, bool& callorginal)
{
  logColor(B_RED, L"Level_RemoveItemPost: %p %p", level, item);
}

void TLMP::GenericModel_DtorPre(CGenericModel *model, bool&)
{
  log(L"GenericModel::DtorPre: %p", model);
}

void TLMP::GenericModel_DtorPost(CGenericModel *model, bool&)
{
  log(L"GenericModel::DtorPost: %p", model);
}

// Server Events
void TLMP::ServerOnClientConnected(void *arg)
{
  if (arg) {
    SystemAddress address = *(SystemAddress *)arg;
    NetworkMessages::Version msgVersion;
    msgVersion.set_version(MessageVersion);

    multiplayerLogger.WriteLine(Info, L"Client connected - sending S_VERSION push to: %x:%i", address.binaryAddress, address.port);
    //log(L"Client connected - sending S_VERSION push to: %x:%i", address.binaryAddress, address.port);

    Server::getSingleton().SendMessage<NetworkMessages::Version>(address, S_VERSION, &msgVersion);
  }
}

void TLMP::ServerOnClientDisconnect(void *arg)
{
  if (arg) {
    SystemAddress address = *(SystemAddress *)arg;

    multiplayerLogger.WriteLine(Info, L"Client disconnected: %x:%i", address.binaryAddress, address.port);
    //log(L"Client disconnected: %x:%i", address.binaryAddress, address.port);
  }
  // TODO: Cleanup client characters.
}
// --

// Client Events
void TLMP::ClientOnConnect(void *arg)
{
  if (arg) {
    SystemAddress address = *(SystemAddress *)arg;

    multiplayerLogger.WriteLine(Info, L"Client connected: %x:%i", address.binaryAddress, address.port);
    //log(L"Client connected: %x:%i", address.binaryAddress, address.port);
  }
}
// --

void TLMP::WndProcPre(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
  /*
  if (!(msg == 0x100 || msg == 0x101 || msg == 0x102 || msg == 0x201))
    return;

  switch (msg) {
  case WM_KEYUP:
    switch (wParam) {

    // T =
    //   Testing Equipment creation and crashes on level cleanup
    case 'E':
    case 'e':
      {
        static CEquipment* mainHand = NULL;
        static CEquipment* secondHand = NULL;

        CInventory* inv = gameClient->pCPlayer->pCInventory;
        CEquipment* tmp1 = inv->GetEquipmentFromSlot(0);
        CEquipment* tmp2 = inv->GetEquipmentFromSlot(1);

        CEquipmentRef* ref1 = inv->GetEquipmentRefFromEquipment(tmp1);
        CEquipmentRef* ref2 = inv->GetEquipmentRefFromEquipment(tmp2);

        log("ref1 = %p, ref2 = %p", ref1, ref2);
      }
      break;
  }
  */
}
