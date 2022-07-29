

#pragma once

#include "UBrowse.h"
#include "ClassIconFinder.h"
#include "SlateOptMacros.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

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
			Name = FText::FromString(Obj ? Obj->GetFName().ToString() : FString("None"));
			ClassName = Obj->GetClass()->GetDisplayNameText();
			Package = FText::FromString(Obj->GetFullName());
			HighlightText = InArgs._HighlightText;
			OuterName = Obj->GetOuter() != nullptr ? FText::FromString(Obj->GetOuter()->GetName()) : LOCTEXT("NoOuter", "None");			
			// Get selection icon based on actor(s) classes and add before the selection label
			FSlateIcon Icon(FSlateIconFinder::FindIconForClass(Obj->GetClass()));
			ClassIcon = Icon.GetSmallIcon();
			Number = Obj ? Obj->GetFName().GetNumber() : 0;
			Id = Obj ? Obj->GetUniqueID() : 0;
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
		else if (ColumnName == "Number")
		{
			return SNew(STextBlock)
				   .Text(FText::AsNumber(Number));
		}
		else if (ColumnName == "Id")
		{
			return SNew(STextBlock)
				   .Text(FText::AsNumber(Id));
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
	int32 Number;
	int32 Id;
};


#undef LOCTEXT_NAMESPACE