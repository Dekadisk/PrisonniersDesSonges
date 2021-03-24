// Fill out your copyright notice in the Description page of Project Settings.


#include "ChalkDrawDecalActor.h"

AChalkDrawDecalActor::AChalkDrawDecalActor()
{
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialCircle(TEXT("/Game/Assets/SelectionWheel/SW_C.SW_C"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialCross(TEXT("/Game/Assets/SelectionWheel/SW_Cr.SW_Cr"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialInterogation(TEXT("/Game/Assets/SelectionWheel/SW_I.SW_I"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialLeft(TEXT("/Game/Assets/SelectionWheel/SW_L.SW_L"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialRight(TEXT("/Game/Assets/SelectionWheel/SW_R.SW_R"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialFront(TEXT("/Game/Assets/SelectionWheel/SW_F.SW_F"));


	if (FoundMaterialCircle.Succeeded()) CircleMaterial = FoundMaterialCircle.Object;
	if (FoundMaterialCross.Succeeded()) CrossMaterial = FoundMaterialCross.Object;
	if (FoundMaterialInterogation.Succeeded()) InterrogationMaterial = FoundMaterialInterogation.Object;
	if (FoundMaterialLeft.Succeeded()) LeftArrowMaterial = FoundMaterialLeft.Object;
	if (FoundMaterialRight.Succeeded()) RightArrowMaterial = FoundMaterialRight.Object;
	if (FoundMaterialFront.Succeeded()) FrontArrowMaterial = FoundMaterialFront.Object;

	SetDecalMaterial(FrontArrowMaterial);
}

void AChalkDrawDecalActor::OnRep_UpdateMaterial()
{
	switch (kind) {
	case TypeDraw::FRONT_ARROW:
		SetDecalMaterial(FrontArrowMaterial);
		break;
	case TypeDraw::LEFT_ARROW:
		SetDecalMaterial(LeftArrowMaterial);
		break;
	case TypeDraw::RIGHT_ARROW:
		SetDecalMaterial(RightArrowMaterial);
		break;
	case TypeDraw::QUESTION_MARK:
		SetDecalMaterial(InterrogationMaterial);
		break;
	case TypeDraw::CROSS:
		SetDecalMaterial(CrossMaterial);
		break;
	case TypeDraw::CIRCLE:
		SetDecalMaterial(CircleMaterial);
		break;
	default:
		Destroy();
		break;

	}
}

void AChalkDrawDecalActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChalkDrawDecalActor, kind);

}