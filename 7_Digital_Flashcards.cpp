#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

class FlashCard
{
public:
	int a, b;

	void Present(void);
	int Solution(void);
};

void FlashCard::Present(void)
{
	printf("%dx%d=", a, b);
};
int FlashCard::Solution(void)
{
	return a * b;
};

class FlashCardSet
{
public:
	FlashCard card[144];
	int nCardToWork;

	void SwapFlashcard(FlashCard& x, FlashCard& y);
	void Shuffle(int nCard, FlashCard card[]);
};

void FlashCardSet::SwapFlashcard(FlashCard& x, FlashCard& y)
{
	FlashCard c = x;
	x = y;
	y = c;
}

void FlashCardSet::Shuffle(int nCard, FlashCard card[])
{
	for (int i = 0; i < nCard; ++i)
	{
		int idx = rand() % nCard;
		SwapFlashcard(card[i], card[idx]);
	}
}

int main(void)
{
	int nCard = 144;
	FlashCardSet cardset;

	//int card[144];
	//for (auto &c:cardset.card)

	for (int i = 0; i < 144; ++i)
	{
		cardset.card[i].a = 1 + (i % 12);
		cardset.card[i].b = 1 + (i / 12);

	}

	srand(time(NULL));

	cardset.Shuffle(nCard, cardset.card);
	char str[256];

	//int nCardToWork = 0;
	while (cardset.nCardToWork < 1 || 144 < cardset.nCardToWork)
	{
		printf("How many cards to work on? :");
		//scanf("%d", &cardset.nCardToWork); // Can be std::cin >> nCardToWork;
		fgets(str, 255, stdin);
		str[255] = 0;
		auto i = atoi(str);
		cardset.nCardToWork = i;
		if (cardset.nCardToWork < 1 || 144 < cardset.nCardToWork)
		{
			printf("The number needs to be between 1 and 144.\n");
		}
	}

	unsigned int t0 = time(NULL);
	char str1[256];
	int nWrong = 0;
	for (int i = 0; i < cardset.nCardToWork; ++i)
	{
		//int a = 1 + (card[i] % 12);
		//int b = 1 + (card[i] / 12);
		//printf("%dx%d=", a, b);
		cardset.card[i].Present();
		int userInput;
		fgets(str, 255, stdin);
		str[255] = 0;
		auto j = atoi(str);
		userInput = j;
		//scanf("%d", &userInput); // Can be std::cin >> userInput;

		//if (userInput != a * b)
		if (userInput != cardset.card[i].Solution())
		{
			printf("Wrong!  Correct answer is %d\n", cardset.card[i].Solution());
			++nWrong;
		}

	}

	printf("You answered %d problems in %d seconds.\n", cardset.nCardToWork, time(NULL) - t0);
	printf("You answered %d correctly (%d%%)\n", cardset.nCardToWork - nWrong, 100 * (cardset.nCardToWork - nWrong) / cardset.nCardToWork);

	return 0;
}

