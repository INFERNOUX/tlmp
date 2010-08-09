#include "UI.h"

bool TLMP::suppressInGameMouse = false;

void TLMP::GameClient_CreateUI(CGame* game)
{
  UserInterface::init();

  multiplayerLogger.WriteLine(Info, L"Creating User Interface...");

  // Added the main game GUI elements
  CEGUI::Window *pRoot = NULL, *pMainMenuButton = NULL, *pMainMenuOptions = NULL;
  CEGUI::Window *pSheet = NULL;
  CEGUI::WindowManager* wm = UserInterface::getManager();

  // Sanity check on the window manager
  if (!wm) {
    multiplayerLogger.WriteLine(Error, L"Error getting WindowManager: NULL");
    return;
  }

  // Find the root window for attaching our UI elements
  if (wm->isWindowPresent(CEGUI::String("25_Root"))) {
    pRoot = wm->getWindow(CEGUI::String("25_Root"));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error finding CEGUI Window \"25_Root\" - cannot add Multiplayer Options window to MainMenu.");
    return;
  }

  // Load the MainMenu Multiplayer Button
  try {
    pMainMenuButton = wm->loadWindowLayout(CEGUI::String("MainMenu_Multiplayer.layout"), CEGUI::String("1001_"));
  } catch (exception &e) {
    log(e.what());
    multiplayerLogger.WriteLine(Error, L"Exception occurred when reading MainMenu_Multiplayer.layout");
    return;
  }

  // Load the MainMenu Multiplayer Options Window
  try {
    pMainMenuOptions = wm->loadWindowLayout(CEGUI::String("MainMenu_MultiplayerOptions.layout"), CEGUI::String("1002_"));
  } catch (exception &e) {
    log(e.what());
    multiplayerLogger.WriteLine(Error, L"Exception occurred when reading MainMenu_MultiplayerOptions.layout");
    return;
  }

  // Hookup button events
  CEGUI::Window *pButton = pMainMenuButton->recursiveChildSearch("1001_OpenMultiplayerOptions");
  if (pButton) {
    pButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_OpenMultiplayerOptions));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: OpenMultiplayerOptions");
    return;
  }

  CEGUI::Window *pOptionsCancel = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_CancelButton");
  if (pOptionsCancel) {
    pOptionsCancel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_CloseMultiplayerOptions));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_CancelButton");
    return;
  }

  CEGUI::Window *pOptionsHost = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_HostButton");
  if (pOptionsHost) {
    pOptionsHost->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_MultiplayerOptions_Host));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_HostButton");
    return;
  }
  
  CEGUI::Window *pOptionsJoin = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_JoinButton");
  if (pOptionsJoin) {
    pOptionsJoin->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_MultiplayerOptions_Join));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_JoinButton");
    return;
  }
    
  CEGUI::Window *pOptionsJoinCancel = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_Join_CancelButton");
  if (pOptionsJoinCancel) {
    pOptionsJoinCancel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_MultiplayerOptions_Join_Cancel));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_Join_CancelButton");
    return;
  }
    
  CEGUI::Window *pOptionsHostCancel = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_Host_CancelButton");
  if (pOptionsHostCancel) {
    pOptionsHostCancel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_MultiplayerOptions_Host_Cancel));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_Host_CancelButton");
    return;
  }
    
  CEGUI::Window *pOptionsHostHost = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_Host_HostButton");
  if (pOptionsHostHost) {
    pOptionsHostHost->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_MultiplayerOptions_Host_Host));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_Host_HostButton");
    return;
  }
    
  CEGUI::Window *pOptionsJoinJoin = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_Join_JoinButton");
  if (pOptionsJoinJoin) {
    pOptionsJoinJoin->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ButtonEvent_MultiplayerOptions_Join_Join));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_Join_JoinButton");
    return;
  }

  onNetworkStateChange();
  
  
  
  // Grab the Sheet window
  if (wm->isWindowPresent(CEGUI::String("Sheet"))) {
    pSheet = wm->getWindow(CEGUI::String("Sheet"));
  } else {
    multiplayerLogger.WriteLine(Error, L"Error finding CEGUI Window \"Sheet\" - cannot add Multiplayer Options window to MainMenu.");
    return;
  }

  // Hookup everything to the existing main menu
  pRoot->addChildWindow(pMainMenuButton);
  pSheet->addChildWindow(pMainMenuOptions);
  pMainMenuOptions->setVisible(false);
  

  multiplayerLogger.WriteLine(Info, L"Creating User Interface... Done.");
}

bool TLMP::ButtonEvent_OpenMultiplayerOptions(const CEGUI::EventArgs& args)
{
  CEGUI::Window *pWindow = UserInterface::getWindowFromName("1002_MultiplayerOptions");

  if (pWindow) {
    pWindow->setVisible(true);
    pWindow->moveToFront();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Multiplayer Options Window");
  }

  return true;
}

bool TLMP::ButtonEvent_CloseMultiplayerOptions(const CEGUI::EventArgs& args)
{
  CEGUI::Window *pWindow = UserInterface::getWindowFromName("1002_MultiplayerOptions");

  if (pWindow) {
    pWindow->setVisible(false);
    pWindow->moveToBack();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Multiplayer Options Window");
  }

  return true;
}

bool TLMP::ButtonEvent_MultiplayerOptions_Host(const CEGUI::EventArgs& args)
{
  CEGUI::Window *pWindow = UserInterface::getWindowFromName("1002_MultiplayerOptions_Host");

  if (pWindow) {
    pWindow->setVisible(true);
    pWindow->moveToFront();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find 1002_MultiplayerOptions_Host");
  }

  return true;
}

bool TLMP::ButtonEvent_MultiplayerOptions_Join(const CEGUI::EventArgs& args)
{
  CEGUI::Window *pWindow = UserInterface::getWindowFromName("1002_MultiplayerOptions_Join");

  if (pWindow) {
    pWindow->setVisible(true);
    pWindow->moveToFront();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Multiplayer Options Join Window");
  }

  return true;
}

bool TLMP::ButtonEvent_MultiplayerOptions_Join_Cancel(const CEGUI::EventArgs& args)
{
  CEGUI::Window *pWindow = UserInterface::getWindowFromName("1002_MultiplayerOptions_Join");
  CEGUI::Window *pWindowOptions = UserInterface::getWindowFromName("1002_MultiplayerOptions");

  if (pWindowOptions && pWindow) {
    pWindow->setVisible(false);
    pWindow->moveToBack();

    pWindowOptions->setVisible(true);
    pWindowOptions->moveToFront();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Multiplayer Options Join Window *or* 1002_MultiplayerOptions");
  }

  return true;
}

bool TLMP::ButtonEvent_MultiplayerOptions_Host_Cancel(const CEGUI::EventArgs& args)
{
  CEGUI::Window *pWindow = UserInterface::getWindowFromName("1002_MultiplayerOptions_Host");
  CEGUI::Window *pWindowOptions = UserInterface::getWindowFromName("1002_MultiplayerOptions");

  if (pWindowOptions && pWindow) {
    pWindow->setVisible(false);
    pWindow->moveToBack();

    pWindowOptions->setVisible(true);
    pWindowOptions->moveToFront();
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Multiplayer Options Host Window *or* 1002_MultiplayerOptions");
  }

  return true;
}

bool TLMP::ButtonEvent_MultiplayerOptions_Host_Host(const CEGUI::EventArgs& args)
{
  u16 port = 0;
  CEGUI::Window *pWindowOptions = UserInterface::getWindowFromName("1002_MultiplayerOptions");

  if (pWindowOptions) {
    pWindowOptions->setVisible(false);
    pWindowOptions->moveToBack();

    CEGUI::Window *pWindow = pWindowOptions->recursiveChildSearch("1002_MultiplayerOptions_Host");

    // Remove the window
    if (pWindow) {
      pWindow->setVisible(false);
      pWindow->moveToBack();

      CEGUI::Editbox *pHostPort = (CEGUI::Editbox *)pWindow->recursiveChildSearch("MultiplayerOptions_Host_HostPortEdit");

      // Grab the text
      if (pHostPort) {
        const char* pPortString = pHostPort->getText().c_str();
        port = (u16)atoi(pPortString);
      } else {
        multiplayerLogger.WriteLine(Error, L"Error could not find MultiplayerOptions_Host_HostPortEdit");
        return false;
      }
    } else {
      multiplayerLogger.WriteLine(Error, L"Error could not find 1002_MultiplayerOptions_Host");
      return false;
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find 1002_MultiplayerOptions");
    return false;
  }

  // Finally, host it
  multiplayerLogger.WriteLine(Info, L"Hosting game on port %i...", port);
  Network::HostGame(port);

  onNetworkStateChange();
  multiplayerLogger.WriteLine(Info, L"Server setup completed.");

  return true;
}

bool TLMP::ButtonEvent_MultiplayerOptions_Join_Join(const CEGUI::EventArgs& args)
{
  u16 port = 0;
  const char* hostName = NULL;

  CEGUI::Window *pWindowOptions = UserInterface::getWindowFromName("1002_MultiplayerOptions");

  if (pWindowOptions) {
    pWindowOptions->setVisible(false);
    pWindowOptions->moveToBack();

    CEGUI::Window *pWindow = pWindowOptions->recursiveChildSearch("1002_MultiplayerOptions_Join");

    // Remove the window
    if (pWindow) {
      pWindow->setVisible(false);
      pWindow->moveToBack();

      CEGUI::Editbox *pHostPort = (CEGUI::Editbox *)pWindow->recursiveChildSearch("MultiplayerOptions_Join_HostPortEdit");
      CEGUI::Editbox *pHostName = (CEGUI::Editbox *)pWindow->recursiveChildSearch("MultiplayerOptions_Join_HostNameEdit");

      // Grab the text
      if (pHostPort) {
        const char* pPortString = pHostPort->getText().c_str();
        port = (u16)atoi(pPortString);
      } else {
        multiplayerLogger.WriteLine(Error, L"Error could not find MultiplayerOptions_Join_HostPortEdit");
        return false;
      }

      if (pHostName) {
        hostName = pHostName->getText().c_str();
      } else {
        multiplayerLogger.WriteLine(Error, L"Error could not find MultiplayerOptions_Join_HostNameEdit");
        return false;
      }
    } else {
      multiplayerLogger.WriteLine(Error, L"Error could not find 1002_MultiplayerOptions_Join");
      return false;
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find 1002_MultiplayerOptions");
    return false;
  }

  // Finally, attempt to connect
  multiplayerLogger.WriteLine(Info, L"Joining game at %s on port %i...", hostName, port);
  Network::JoinGame(hostName, port);
  onNetworkStateChange();
  multiplayerLogger.WriteLine(Info, L"Joining game completed.");

  return true;
}






void TLMP::onNetworkStateChange()
{
  CEGUI::Window *pMainMenuOptions = UserInterface::getWindowFromName("1002_MultiplayerOptions");
  CEGUI::Window *pOptionsStatusText = pMainMenuOptions->recursiveChildSearch("1002_MultiplayerOptions_MultiplayerStateText");

  multiplayerLogger.WriteLine(Info, L"Updating network state");

  if (pOptionsStatusText) {
    switch (NetworkState::getSingleton().GetState()) {
    case SINGLEPLAYER:
      pOptionsStatusText->setText("Singleplayer.");
      break;
    case SERVER:
      pOptionsStatusText->setText("Hosting game.");
      break;
    case CLIENT:
      pOptionsStatusText->setText("Joining game.");
      break;
    }
  } else {
    multiplayerLogger.WriteLine(Error, L"Error could not find Button: 1002_MultiplayerOptions_MultiplayerStateText");
    return;
  }
}
