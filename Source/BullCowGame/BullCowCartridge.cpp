// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();

	InitGame();
}

void UBullCowCartridge::InitGame()
{
	const FString PossibleWordsPath = FPaths::ProjectContentDir() / TEXT("IsogramWords/HiddenWordList.txt");
	if (FFileHelper::LoadFileToStringArray(Isograms, *PossibleWordsPath))
	{
		int32 RandomElement = FMath::RandRange(0, Isograms.Num()-1);

		HiddenWord = Isograms[RandomElement];
	}
	Lives = HiddenWord.Len();
	bWinState = false;

	PrintLine(TEXT("Welcome to Bull Cows!"));
	PrintLine(TEXT("Guess the %i letter isogram."), HiddenWord.Len());
	PrintLine(TEXT("Press tab then enter to continue..."));
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
	ClearScreen();

	if (Lives > 0 && !bWinState)
	{
		ProcessGuess(Input);
	}
	else if (Lives == 0)
	{
		ClearScreen();

		PrintLine(TEXT("You lost!"));
		PrintLine(TEXT("The word was '%s'."), *HiddenWord);

		EndGame(Input);
	}

	if (bWinState)
	{
		EndGame(Input);
	}
}

void UBullCowCartridge::ProcessGuess(const FString& PlayerInput)
{
	PrintLine(PlayerInput);

	if (PlayerInput == HiddenWord)
	{
		bWinState = true;
		PrintLine(TEXT("You won!"));
	}
	else if (PlayerInput != HiddenWord)
	{
		CheckInput(PlayerInput);

		PrintLine(TEXT("It's a %i letter word."), HiddenWord.Len());
		//PrintLine(TEXT("It's a " + FString::FormatAsNumber(HiddenWord.Len()) + " letter word."));
		//PrintLine(FString::Printf(TEXT("It's a %i letter word."), HiddenWord.Len()));

		GiveHint();
		FBullCowCount Score = GetBullCows(PlayerInput);

		PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
		PrintLine(TEXT("Try again! You have %i lives left!"), Lives);
	}
}

void UBullCowCartridge::CheckInput(const FString& PlayerInput)
{
	if (PlayerInput == "")
	{
		Lives++;
		PrintLine(TEXT("You need to enter a word!"));
	}
	else if (!IsIsogram(PlayerInput))
	{
		Lives++;
		PrintLine(TEXT("No repeating letters, guess again!"));
	}
	else if (PlayerInput.Len() != HiddenWord.Len())
	{
		if (PlayerInput.Len() > HiddenWord.Len())
		{
			PrintLine(TEXT("Your word was too long!"));
		}
		else
		{
			PrintLine(TEXT("Your word was too short!"));
		}
	}

	Lives--;
}

bool UBullCowCartridge::IsIsogram(const FString& PlayerInput) const
{
	TCHAR Temp = PlayerInput[0];
	int32 RowIndex = 1;
	int32 CurrentIndex = RowIndex;

	if (PlayerInput.Len() == 1)
	{
		return true;
	}

	while (Temp != PlayerInput[CurrentIndex])
	{
		CurrentIndex++;

		if (RowIndex == PlayerInput.Len() - 1)
		{
			return true;
		}
		else if (Temp == PlayerInput[CurrentIndex])
		{
			return false;
		}
		else if (CurrentIndex == PlayerInput.Len() - 1)
		{
			Temp = PlayerInput[RowIndex];
			RowIndex++;
			CurrentIndex = RowIndex;
		}
	}

	return false;
}

void UBullCowCartridge::GiveHint() const
{
	int32 WordHalf = (int)(HiddenWord.Len() / 2);

	if (Lives <= WordHalf)
	{
		FString LetterHints = "";
		for (int i = 0; i <= (WordHalf - Lives); i++)
		{
			LetterHints.AppendChar(HiddenWord[i]);
		}
		PrintLine(TEXT("The word starts with '") + LetterHints + TEXT("'."));
	}
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
	FBullCowCount Count;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			Count.Bulls++;
			continue;
		}

		for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
		{
			if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
			{
				Count.Cows++;
				break;
			}
		}
	}

	return Count;
}

void UBullCowCartridge::EndGame(const FString& Input)
{
	PrintLine(TEXT("Do you want to play again?"));
	PrintLine(TEXT("Enter 'Yes' or 'No'"));

	if (Input == "Yes")
	{
		ClearScreen();

		InitGame();
	}
	else if (Input == "No")
	{
		ClearScreen();
	}
}