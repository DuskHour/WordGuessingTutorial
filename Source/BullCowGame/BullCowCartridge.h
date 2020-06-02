// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
		virtual void BeginPlay() override;
		virtual void OnInput(const FString& Input) override;
		
	// Your declarations go below!
	private:
		void InitGame();
		void EndGame(const FString& Input);
		void ProcessGuess(const FString& Input);
		void GiveHint() const;
		void CheckInput(const FString& Input);
		bool IsIsogram(const FString& Input) const;
		FBullCowCount GetBullCows(const FString& Guess) const;

		FString HiddenWord;
		int32 Lives;
		bool bWinState;
		TArray<FString> Isograms;
};
