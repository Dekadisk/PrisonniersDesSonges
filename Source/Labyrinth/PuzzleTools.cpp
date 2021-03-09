#include "PuzzleTools.h"
#include "SolvableActor.h"
#include "BellPuzzleActor.h"

void ExecuteAction(APuzzleActor* instigator, ASolvableActor* target, EPuzzleActions action) {
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
		case EPuzzleActions::Open:
			target->Open();
			break;
		case EPuzzleActions::Close:
			target->Close();
			break;
		case EPuzzleActions::Ring:
			target->Ring(Cast<ABellPuzzleActor>(instigator)->note);
			break;
		default :
			break;
	}
}