#include "SharedState.h"

namespace TLMP
{

  vector<NetworkEntity*> *TLMP::NetworkSharedEquipment  = new vector<NetworkEntity*>();
  vector<NetworkEntity*> *TLMP::NetworkSharedCharacters = new vector<NetworkEntity*>();
  vector<NetworkEntity*> *TLMP::ClientTemporaryEquipment = new vector<NetworkEntity*>();
  
};