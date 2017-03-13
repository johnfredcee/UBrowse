
#include "UBrowsePrivatePCH.h"
#include "UBrowseNode.h"

namespace {
	FString GetFlagsAsText(EObjectFlags Param)
	{
		FString Result;
		if (Param == 0) {
			Result = TEXT("No Flags");
			return Result;
		}
		if (Param & RF_Public) {
			Result.Append(TEXT("|Public"));
		}
		if (Param & RF_Standalone) {
			Result.Append(TEXT("|Standalone"));
		}
		if (Param & RF_MarkAsNative) {
			Result.Append(TEXT("|Marked As Native"));
		}
		if (Param & RF_Transactional) {
			Result.Append(TEXT("|Transatctional"));
		}
		if (Param & RF_ClassDefaultObject) {
			Result.Append(TEXT("|CDO"));
		}
		if (Param & RF_ArchetypeObject) {
			Result.Append(TEXT("|Archetype"));
		}
		if (Param & RF_Transient) {
			Result.Append(TEXT("|Transient"));
		}
		if (Param & RF_MarkAsRootSet) {
			Result.Append(TEXT("|Marked as Root"));
		}
		if (Param & RF_TagGarbageTemp) {
			Result.Append(TEXT("|Tageed Garbage"));
		}
		if (Param & RF_NeedLoad) {
			Result.Append(TEXT("|Needs Loading"));
		}
		if (Param & RF_NeedPostLoad) {
			Result.Append(TEXT("|Needs Post Loading"));
		}
		if (Param & RF_NeedPostLoadSubobjects) {
			Result.Append(TEXT("|Subobjects need Post Loading"));
		}
		if (Param & RF_BeginDestroyed) {
			Result.Append(TEXT("|Being destroyed"));
		}
		if (Param & RF_FinishDestroyed) {
			Result.Append(TEXT("|Is destroyed"));
		}
		if (Param & RF_BeingRegenerated) {
			Result.Append(TEXT("|Being Regenerated"));
		}
		if (Param & RF_DefaultSubObject) {
			Result.Append(TEXT("|Default Subobject"));
		}
		if (Param & RF_WasLoaded) {
			Result.Append(TEXT("|Was Loaded"));
		}
		if (Param & RF_TextExportTransient) {
			Result.Append(TEXT("|Do not export as text"));
		}
		if (Param & RF_LoadCompleted) {
			Result.Append(TEXT("|Load Complete"));
		}
		if (Param & RF_InheritableComponentTemplate) {
			Result.Append(TEXT("|Inheritable Template"));
		}
		if (Param &	RF_StrongRefOnFrame) {
			Result.Append(TEXT("|Strong Ref on Function Frame"));
		}
		if (Param & RF_Dynamic) {
			Result.Append(TEXT("|Dynamic"));
		}
		Result.Append("|");
		return Result;
	}
}

UBrowseNode::UBrowseNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ChildrenPin = nullptr;
	ParentPin = nullptr;
	NodeObject = nullptr;
}

void UBrowseNode::AddChild(UBrowseNode* ChildNode)
{
	UEdGraphPin* ParentPinOfChild = ChildNode->GetParentPin();
	if (ensure(ParentPinOfChild)) {
		ParentPinOfChild->bHidden = false;
		ChildrenPin->bHidden = false;
		ChildrenPin->MakeLinkTo(ParentPinOfChild);
	}
}

void UBrowseNode::AllocateDefaultPins()
{
	ChildrenPin = CreatePin(EEdGraphPinDirection::EGPD_Input, TEXT(""), TEXT(""), nullptr, false, false, TEXT("Inner"));
	ParentPin = CreatePin(EEdGraphPinDirection::EGPD_Output, TEXT(""), TEXT(""), nullptr, false, false, TEXT("Outer"));
}


void UBrowseNode::SetupNode(const FIntPoint& NodePosition, UObject* NodeUObject)
{

	NodeObject = NodeUObject;
	FString Title;
	Title.Empty(256);
	UClass* ObjectClass = NodeUObject->GetClass();
	ObjectClass->AppendName(Title);
	Title += " -> ";
	NodeUObject->AppendName(Title);
	NodeTitle = FText::FromString(Title);
	ShortDesc = NodeUObject->GetDesc();
	EObjectFlags UObjectFlags = NodeUObject->GetFlags();
	LongDesc = GetFlagsAsText(UObjectFlags);
	NodePosX = NodePosition.X;
	NodePosY = NodePosition.Y;

	AllocateDefaultPins();
}

FText UBrowseNode::GetTooltipText() const
{
	FText Result;
	if (NodeObject)
	{
		Result = NodeObject->GetClass()->GetToolTipText();
	}
	return Result;
}

FText UBrowseNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NodeTitle;
}

UEdGraphPin* UBrowseNode::GetChildrenPin()
{
	return ChildrenPin;
}

UEdGraphPin* UBrowseNode::GetParentPin()
{
	return ParentPin;
}