#include<stdio.h>
#include<iostream>
#include<vector>

bool Vectorize(int* input)
{

	char str[22];
	fgets(str, 22, stdin);
	std::vector <std::string> vec;
	int score = 0;
	for (auto c : str)
	{
		vec.push_back("");
		vec.back().push_back(c);
	}

	if (vec.size() > 1)
	{


		for (int i = 0; i < 19; ++i)
		{
			if (i % 2 == 1 && (str[i] == 'x' || str[i] == 'X'))
			{
				printf("Invalid Input");
				return 0;
			}
		}
		for (int i = 0; i < 21; ++i)
		{
			if (i % 2 == 0 && str[i] == '/')
			{
				printf("Invalid Input");
				return 0;
			}
		}
		for (int i = 0; i < 21; ++i)
		{
			if (str[i] == 10)
			{
				printf("Too short\n");
				return 0;
			}
			else if (str[i] == ' ' || str[i] == '-')
			{
				input[i] = 0;
			}
			else if (str[i] == 'X' || str[i] == 'x')
			{
				input[i] = 10;
			}
			else if (str[i] == '/')
			{
				input[i] = (10 - input[i - 1]);
			}

			else if (str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' || str[i] == '4' || str[i] == '5' || str[i] == '6' || str[i] == '7' || str[i] == '8' || str[i] == '9')
			{
				input[i] = atoi(vec[i].c_str());
			}
			else
			{
				printf("Wrong Character\n");
				return 0;
			}
		}
		return input;
	}

}

int BowlingScore(int* input)
{
	int score = 0;

	for (int i = 0; i < 21; ++i)
	{
		if (i < 17)
		{
			if (input[i] == 10 && input[i + 2] == 10 && input[i + 4] == 10)
			{
				{
					input[i] = 30;
				}
			}


			else if (i != 17)
			{
				if (input[i] == 10 && i % 2 == 0)
				{
					if (input[i + 2] != 10)
					{
						input[i] = 10 + input[i + 2] + input[i + 3];
					}
					else
					{
						input[i] = 10 + input[i + 2] + input[i + 4];
					}
				}
			}
		}

		if ((input[i] + input[i - 1]) == 10 && i % 2 != 0)
		{
			input[i] = input[i] + input[i + 1];
		}

		score = score + input[i];
	}

	return score;
}

int main()
{
	int input[22];
	int final_score = 0;

	bool x = Vectorize(input);

	if (x == 1)
	{

		final_score = BowlingScore(input);
		printf("%d\n", final_score);
	}


	return 0;
}
