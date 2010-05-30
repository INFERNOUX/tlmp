#include "Test.h"

void TLMP::test0_pre STDARG
{
  log("[TEST 0 PRE] %p %#x %#x %#x %#x", e->_this, Pz[0], Pz[1], Pz[2], Pz[3]);

  if (NetworkState::getSingleton().GetState() == CLIENT && !ClientAllowSpawn) {
    log("[CLIENT] Stopping spawn");
    e->calloriginal = false;
    e->retval = 0;
  }
}

void TLMP::test0_post STDARG
{
  log("[TEST 0 POST]");
}

void TLMP::test1_pre STDARG
{
  wstring &s1 = *(wstring*)&Pz[0];
  wstring &s2 = *(wstring*)&Pz[1];

  log("[TEST 1 PRE]: %p %ls %ls %i %i", e->_this, s1.c_str(), s2.c_str(), Pz[2], Pz[3]);
}

void TLMP::test1_post STDARG
{
  log("[TEST 0 POST]");
}

void TLMP::test2_pre STDARG
{
  //log("[[[[TEST 2 PRE]: %p %i", e->_this, Pz[0]);
  //e->calloriginal = false;
}

void TLMP::test2_post STDARG
{
  //log("[[[[TEST 2 POST]: %p retVal = %i", e->_this, e->retval);
}

void TLMP::test3_pre STDARG
{
  //log("============== TEST 3 PRE: %p %p %i %i %p", e->_this, Pz[0], Pz[1], Pz[2], Pz[3]);
}

void TLMP::test3_post STDARG
{
  //log("============== TEST 3 POST: %p retVal = %i", e->_this, e->retval);
}

void TLMP::test4_pre STDARG
{
  log("============== TEST 4 PRE: %p %i %i %i", e->_this, Pz[0], Pz[1], Pz[2]);
}

void TLMP::test4_post STDARG
{
  log("============== TEST 4 POST: %p retVal = %i", e->_this, e->retval);
}

void TLMP::test5_pre STDARG
{
  //log("============== TEST 5 PRE: %p %p %p %p %p %p", e->_this, Pz[0], Pz[1], Pz[2], Pz[3], Pz[4]);
}

void TLMP::test5_post STDARG
{
  //log("============== TEST 5 POST: %p retVal = %i", e->_this, e->retval);
}

void TLMP::test6_pre STDARG
{
  //log("============== TEST (delete) 6 PRE: %p", Pz[0]);
}

void TLMP::test6_post STDARG
{
  //log("============== TEST (delete) 6 POST: %p", Pz[0]);
  PVOID addr = (PVOID)Pz[0];

  //MemoryManager::getSingleton().removeBlock(addr);

}

void TLMP::test7_pre STDARG
{
  //log("============== TEST (new) 7 PRE: %i, retval = %p", Pz[0]);
}

void TLMP::test7_post STDARG
{
  PVOID addr = (PVOID)e->retval;
  u32 size = Pz[0];
  PVOID retInst = (PVOID)(*(u32*)e->retaddress);

  //if (Pz[0] > 200)
  MemoryManager::getSingleton().addBlock(addr, size, retInst);

  //if (Pz[0] > 96)
  //  log("============== TEST (new) 7 PRE: %i, retval = %p", Pz[0], e->retval);
}

void TLMP::test8_pre STDARG
{
  //log("pre-Player Ctor? : %p\n", e->_this);
}

void TLMP::test8_post STDARG
{
  log("post-Player Ctor? : %p %p\n", e->_this, e->retval);
}

void TLMP::test9_pre STDARG
{
  /* Works
  CCharacterSaveState *pCCharacterSaveState = (CCharacterSaveState*)e->_this;
  log("*** CCharacterSaveState Created: %p", pCCharacterSaveState);
  log("        Guid2: %016I64X", pCCharacterSaveState->guid2);

  if (pCCharacterSaveState->name.length > 7) {
    log(L"         Name: %s", pCCharacterSaveState->name.stringPtr[0]);
  } else {
    log(L"         Name: %s", &pCCharacterSaveState->name.string);
  }

  if (pCCharacterSaveState->unk91.length > 7) {
    log(L"         Skill0: %s", pCCharacterSaveState->unk91.stringPtr[0]);
  } else {
    log(L"         Skill0: %s", &pCCharacterSaveState->unk91.string);
  }

  log("");
  */
}

void TLMP::test10_pre STDARG
{
  CCharacter *pCCharacter = (CCharacter*)e->_this;
  log("*** CCharacter Created: %p", pCCharacter);
  log("        Guid2: %016I64X", pCCharacter->guid2);

  log("");
}

void TLMP::test11_pre STDARG
{
  PVOID pCResourceManager = (PVOID)e->_this;
  //log("*** CResourceManager Created: %p", pCResourceManager);

  //log("");
}

void TLMP::testItemGold_pre STDARG
{
  CItemGold *pItemGold = (CItemGold *)e->_this;

  /*
  log("");
  log("*** CItemGold Created: %p", pItemGold);
  log("    guid0: %016I64X", pItemGold->guid0);
  log("    guid1: %016I64X", pItemGold->guid1);
  log("    guid2: %016I64X", pItemGold->guid2);
  log("");
  */
}

void TLMP::testItem_pre STDARG
{
  CItem *pItem = (CItem*)e->_this;

  /*
  log("");
  log("*** CItem Created: %p", pItem);
  log("    guid0: %016I64X", pItem->guid0);
  log("    guid1: %016I64X", pItem->guid1);
  log("    guid2: %016I64X", pItem->guid2);
  log("");
  */
}

void TLMP::testFireMessage_pre STDARG
{
  CSettings *settings = (CSettings*)e->_this;
  u32 index = Pz[0];
  u32 &value = settings->SettingIntIndexLow[index];

  settings->enableInternalLogging();

  /*
  logColor(B_GREEN, "FireMessage (%p, %p)", settings, Pz[0]);
  logColor(B_GREEN, "  CSettings: %p", settings);
  logColor(B_GREEN, L"    LocalDirectory: %s", settings->LocalSettingsDirectory.getString());
  logColor(B_GREEN, L"    SettingsFile: %s", settings->SettingsFile.getString());
  logColor(B_GREEN, "    Index High: %p", settings->SettingIntIndexHigh);
  logColor(B_GREEN, "    Index Low:  %p", settings->SettingIntIndexLow);
  logColor(B_GREEN, "    Index Value:%i", value);
  logColor(B_GREEN, "  index: %#x", index);
  */

  // Turns on internal logging
  //if ((index == 0x3c) && !value)
  //  value = 1;

  //settings->dumpSettings();
  //settings->dumpSettingsFile();
}

void TLMP::testSaveGame_pre STDARG
{
  CGameClient *gameClient = (CGameClient*)e->_this;
  u32 unk0 = Pz[0];
  u32 unk1 = Pz[1];

  logColor(B_RED, "Pre Game Save: CGameClient: %p (%i, %i)", gameClient, unk0, unk1);
}

void TLMP::testSaveGame_post STDARG
{
  CGameClient *gameClient = (CGameClient*)e->_this;
  u32 unk0 = Pz[0];
  u32 unk1 = Pz[1];

  logColor(B_RED, "Post Game Save: CGameClient: %p (%i, %i)", gameClient, unk0, unk1);
}

void TLMP::testSaveSharedStash_pre STDARG
{
  logColor(B_RED, "Save Shared Stash Pre: %p", e->_this);
}

void TLMP::testSaveSharedStash_post STDARG
{
  logColor(B_RED, "Save Shared Stash Post: %p", e->_this);
}

void TLMP::testCGameLoad_pre STDARG
{
  logColor(B_RED, "CGame Load pre: %p, %i", e->_this, Pz[0]);
}

void TLMP::testCGameLoad_post STDARG
{
  logColor(B_RED, "CGame Load post: %p, %i", e->_this, Pz[0]);
}

void TLMP::testCGameClient_LoadGame_pre STDARG
{
  logColor(B_RED, "CGameClient LoadGame pre: %p, %p %p %p %p", e->_this, Pz[0], Pz[1], Pz[2], Pz[3]);
}

void TLMP::testCGameClient_LoadGame_post STDARG
{
  CPlayer *player = (CPlayer*)e->retval;

  logColor(B_RED, "CGameClient LoadGame post: %p, %p %p %p %p (Player: %p)", e->_this, Pz[0], Pz[1], Pz[2], Pz[3], player);
}

void TLMP::testCGameClient_CreateLevel_pre STDARG
{
  CGameClient *gameClient = (CGameClient*)e->_this;
  
  logColor(B_RED, "CGameClient CreateLevel pre: (CGameClient: %p) (%p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p)", 
    gameClient,
    Pz[0], Pz[1], Pz[2], Pz[3], Pz[4], Pz[5], Pz[6], Pz[7], Pz[8], Pz[9], Pz[10], Pz[11], Pz[12], 
    Pz[13], Pz[14], Pz[15], Pz[16], Pz[17], Pz[18], Pz[19], Pz[20], Pz[21], Pz[22], Pz[23]);
}

void TLMP::testCGameClient_CreateLevel_post STDARG
{
  CGameClient *gameClient = (CGameClient*)e->_this;
  
  logColor(B_RED, "CGameClient CreateLevel post: (CGameClient: %p) (%p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p)", 
    gameClient,
    Pz[0], Pz[1], Pz[2], Pz[3], Pz[4], Pz[5], Pz[6], Pz[7], Pz[8], Pz[9], Pz[10], Pz[11], Pz[12], 
    Pz[13], Pz[14], Pz[15], Pz[16], Pz[17], Pz[18], Pz[19], Pz[20], Pz[21], Pz[22], Pz[23]);
}

void TLMP::testCGameClient_LoadLevel_pre STDARG
{
  CGameClient *gameClient = (CGameClient*)e->_this;

  logColor(B_RED, "CGameClient LoadLevel pre: %p", gameClient);
  logColor(B_RED, "Previous level transfer: %i", gameClient->level);

  // -99 acts like a TP
  //gameClient->level = -99;
}

void TLMP::testCGameClient_LoadLevel_post STDARG
{
  CGameClient *gameClient = (CGameClient*)e->_this;

  logColor(B_RED, "CGameClient LoadLevel post: %p", gameClient);
  logColor(B_RED, "Previous level transfer: %i", gameClient->level);
}
