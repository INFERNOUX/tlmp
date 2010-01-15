#pragma once

#include "Common.h"
#include "Entity.h"
#include "Network.h"
using namespace TLMP::Network;

namespace TLMP {

  void _spider_some_create_pre STDARG;
  void _spider_some_create_post STDARG;

  void _spider_process_ai_pre STDARG;

  void _spider_process_ai2_pre STDARG;

  void _spider_process_ai3_pre STDARG;
  void _spider_process_ai3_post STDARG;

  void _spider_idle_pre STDARG;

  void _spider_on_hit_pre STDARG;

  void _set_alignment_pre STDARG;

};
