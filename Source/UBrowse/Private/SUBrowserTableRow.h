// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ClassIconFinder.h"

#define LOCTEXT_NAMESPACE "SUBrowserTableRow"


/**
* Implements a row widget for a live uobject.
*/
class SUBrowserTableRow
	: public SMultiColumnTableRow<TSharedPtr<int32> >
{
public:

	SLATE_BEGIN_ARGS(SUBrowserTableRow) { }
		SLATE_ARGUMENT(TSharedPtr<FBrowserObject>, Object)
		SLATE_ARGUMENT(FText, HighlightText)
	SLATE_END_ARGS()

public:

	SUBrowserTableRow()
		: ClassIcon(nullptr)
	{
	}

	/**
	* Constructs the widget.
	*
	* @param InArgs The construction arguments.
	*/
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
	{
		Object = InArgs._Object;

		if (UObject* Obj = InArgs._Object->Object.Get())
		{
			Name = FText::FromString(Obj->GetName());
			ClassName = Obj->GetClass()->GetDisplayNameText();
			Package = FText::FromString(Obj->GetFullName());
			HighlightText = InArgs._HighlightText;
			OuterName = Obj->GetOuter() != nullptr ? FText::FromString(Obj->GetOuter()->GetName()) : LOCTEXT("NoOuter", "None");			
			// Get selection icon based on actor(s) classes and add before the selection label
			ClassIcon = FClassIconFinder::FindIconForClass(Obj->GetClass());
		}

		SMultiColumnTableRow<TSharedPtr<int32> >::Construct(FSuperRowType::FArguments(), InOwnerTableView);
	}

public:

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
		if (ColumnName == "Class")
		{
			return SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SImage)
					.Image(ClassIcon)
				]

			+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(ClassName)
				];
		}
		else if (ColumnName == "Name")
		{
			return SNew(STextBlock)
				.Text(Name)
				.HighlightText(HighlightText);
		}
		else if (ColumnName == "Path")
		{
			return SNew(STextBlock)
				.Text(Package);
		}
		else if (ColumnName == "Outer")
		{
			return SNew(STextBlock)
				.Text(OuterName);
		}

		return SNullWidget::NullWidget;
	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION

private:

	TSharedPtr<FBrowserObject> Object;

	const FSlateBrush* ClassIcon;

	// Holds the transaction's title text.
	FText Name;
	FText ClassName;
	FText Package;
	FText OuterName;
	FText HighlightText;
};


#undef LOCTEXT_NAMESPACE