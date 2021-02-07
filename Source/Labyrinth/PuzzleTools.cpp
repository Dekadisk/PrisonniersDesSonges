#include "PuzzleTools.h"

void ExecuteAction(ASolvableActor* target, EPuzzleActions action) {
	switch (action) {
		case EPuzzleActions::Unlock :
			target->Unlock();
			break;
		case EPuzzleActions::Lock :
			target->Lock();
			break;
		case EPuzzleActions::Activate :
			target->Activate();
			break;
		default :
			break;
	}
}