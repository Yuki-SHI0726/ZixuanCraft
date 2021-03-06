// Fill out your copyright notice in the Description page of Project Settings.
// ZixuanCraftMainGameWidget.h
// Author: Zixuan Shi
// Main game widget during gameplay

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "ZixuanCraftWidgetBase.h"
#include "ZixuanCraftMainGameWidget.generated.h"

class AZixuanCraftCharacter;
class ACraftingManager;
class ALoot;
class UZixuanCraftCraftingResultButton;

class UPanelWidget;
class UButton;
class UImage;
class UTextBlock;

/**
 * User Widget used for main game.
 * - Health
 * - Starveness
 * - Inventory
 * - Crafting
 * - Mobile input if on Mobile
 */
UCLASS()
class ZIXUANCRAFT_API UZixuanCraftMainGameWidget : public UZixuanCraftWidgetBase
{
	GENERATED_BODY()
	
protected:
	//------------------------------------------------------------------------------------------------------------------------------------
	// Instructions
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UPanelWidget* Instructions_Panel = nullptr;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Gameplay
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UPanelWidget* Gameplay_Panel = nullptr;

	/** All platforms UI */
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UProgressBar* HealthBar_ProgressBar = nullptr;

	/** This panel is at the bottom of the game UI, indicates a portion of inventory that allows the player quick select */
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UPanelWidget* GameplayInventoryItems_Panel = nullptr;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Inventory & Crafting
	UPROPERTY(BlueprintReadWrite, Category = Gameplay, meta = (BindWidget))
	UPanelWidget* InventoryCrafting_Panel = nullptr;

	/** Hiden in gameplay, will show up if the player pressed the show key, indicates all the inventory this owning player has */
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UPanelWidget* BackpackInventoryItems_Panel = nullptr;

	/** Panel for crafting. 9 or 4 buttons depending on if crafting through a toolbox for crafting sources and 1 for result */
	UPROPERTY(BlueprintReadWrite, Category = Crafting, meta = (BindWidget))
	UPanelWidget* Crafting_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Crafting)
	ACraftingManager* CraftingManager = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Crafting, meta = (BindWidget))
	UZixuanCraftCraftingResultButton* CraftingResult_Button = nullptr;

	UZixuanCraftButton* LastHoveredButton = nullptr;

	/** The index to select an item */
	int32 SelectedIndex = InvalidIndex;

	/** :( shhhhhh this will be refactored once I have time */
	int32 LastClickedInventoryButtonIndex = InvalidIndex;

	//------------------------------------------------------------------------------------------------------------------------------------
	// PC/Console dedicated inventory UI 
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UPanelWidget* SelectedItem_Panel = nullptr;

	/** Image of the item got selected */
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UImage* SelectedItem_Image = nullptr;

	/** Text of the count of the selected item */
	UPROPERTY(BlueprintReadWrite, Category = Inventory, meta = (BindWidget))
	UTextBlock* SelectedItemCount_TextBlock = nullptr;

	/** Instruction prompt of input */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* InstructionsPrompt_TextBlock = nullptr;

	FLootSlot SelectedSlot;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Mobile dedicated UI
	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UPanelWidget* Mobile_Panel = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* Jump_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* DestroyAttack_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* PlaceUseItem_Mobile_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Mobile, meta = (BindWidget))
	UButton* ToggleInventory_Mobile_Button = nullptr;

public:
	virtual void NativeConstruct() override final;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Instructions
	virtual void ToggleInstructions() override final;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Gameplay
	virtual void UpdateHealthBarPercent(float Percent) override final { HealthBar_ProgressBar->SetPercent(Percent); }
	
	UFUNCTION()
	void OnDestoryAttackButtonPressed();

	UFUNCTION()
	void OnPlaceUseItemButtonPressed();

	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void OnJumpButtonPressed();

	UFUNCTION()
	void OnJumpButtonReleased();

	//------------------------------------------------------------------------------------------------------------------------------------
	// Inventory & Crafting
	/**
	 * Set the selected item (the one always follows mouse's position)
	 * @param LootSlot		Loot slot data to set
	 */
	virtual void SetSelectedItemPanel(const FLootSlot& LootSlot) override final;

	/** 
	 * Takes in an index of a button of this widget, convert and return the index in backpack inventory
	 * @param WidgetIndex		Index in widget
	 * @return					Index in backpack inventory
	 */
	virtual int32 ToBackpackIndex(int32 WidgetIndex) const override final { return WidgetIndex - GameplayInventoryItems_Panel->GetAllChildren().Num(); }

	/**
	* Takes in an index of a button of this widget, convert and return the index in crafting panel
	* @param WidgetIndex		Index in widget
	* @return					Index in crafting panel
	*/
	virtual int32 ToCraftingIndex(int32 WidgetIndex) const override final { return WidgetIndex - GameplayInventoryItems_Panel->GetAllChildren().Num() - BackpackInventoryItems_Panel->GetAllChildren().Num(); }

	/** 
	 * Behavior to perform when player scrolls the mouse wheel 
	 * @param bIsScrollingDown		True if scrolling mouse wheel down, false if up
	 */
	virtual void ScrollInventory(bool bIsScrollingDown) override final;

	/** Turn on/off Inventory and Crafting panel */
	virtual void IToggleInventoryCrafting() override final { ToggleInventory(); }

	/** 
	 * Reset an button at Widget Index 
	 * @param WidgetIndex		Index in widget
	 */
	virtual void ResetItemAt(int32 WidgetIndex) override final;

	//------------------------------------------------------------------------------------------------------------------------------------
	// Mobile
	virtual void IOnJumpButtonPressed() override final { OnJumpButtonPressed(); }
	virtual void IOnJumpButtonReleased() override final { OnJumpButtonReleased(); }
	virtual void IOnDestoryAttackButtonPressed() override final { OnDestoryAttackButtonPressed(); }
	virtual void IOnPlaceUseItemButtonPressed() override final { OnPlaceUseItemButtonPressed(); }

	//------------------------------------------------------------------------------------------------------------------------------------
	// Utils
	virtual int32 GetGameplayInventoryNum() const override final { return GameplayInventoryItems_Panel->GetAllChildren().Num(); }
	virtual int32 GetTotalInventoryNum() const override final { return GetGameplayInventoryNum() + BackpackInventoryItems_Panel->GetAllChildren().Num(); }
	virtual bool IsDisplayingInventoryPanel() const override final { return InventoryCrafting_Panel->GetIsEnabled(); }
	virtual UZixuanCraftButton* GetButtonAt(int32 WidgetIndex) const override final;
	virtual bool IsGameplayInventory(int32 WidgetIndex) const { return WidgetIndex >= 0 && WidgetIndex < GetGameplayInventoryNum(); }
	virtual bool IsBackpackInventory(int32 WidgetIndex) const { return WidgetIndex >= GetGameplayInventoryNum() && WidgetIndex < GetTotalInventoryNum(); }
	virtual bool IsCraftingButton(int32 WidgetIndex) const { return WidgetIndex >= GetTotalInventoryNum(); }

	//------------------------------------------------------------------------------------------------------------------------------------
	// Getter Setters
	UZixuanCraftButton* GetSelectedInventory() const;
	virtual void SetSelectIndex(int32 Index) override final;
	virtual int32 IGetSelectIndex() const override final { return SelectedIndex; }	
	virtual void SetButtonDataAt(const FLootSlot& Data, int32 Index) override final;
	virtual UZixuanCraftButton* GetLastHoveredButton() const override final { return LastHoveredButton; }
	virtual void SetLastHoveredButton(UZixuanCraftButton* Button)  override final { LastHoveredButton = Button; }
	virtual const FLootSlot& GetSelectedSlotData() const { return SelectedSlot; }
	virtual FLootSlot& GetSelectedSlotData() { return SelectedSlot; }
	virtual int32 IGetTempHackSelectIndex() const override final { return LastClickedInventoryButtonIndex; }
	virtual void SetLastSelectedInventoryIndex(int32 Index) override final { LastClickedInventoryButtonIndex = Index; }
	virtual UPanelWidget* GetCraftingPanel() const override final { return Crafting_Panel; }

private:
	void InitButtons();
};
