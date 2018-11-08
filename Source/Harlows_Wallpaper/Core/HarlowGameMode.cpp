#include "HarlowGameMode.h"



AHarlowGameMode::AHarlowGameMode()
{
	// http://api.unrealengine.com/INT/API/Runtime/Engine/GameFramework/AGameModeBase/index.html
	DefaultPawnClass = AHarlowPawn::StaticClass();

	// GameSessionClass = ...

	GameStateClass = AHarlowGameState::StaticClass();

	// PlayerControllerClass = ...
	
	// PlayerStateClass = ...

	// SpectatorClass = ...
}