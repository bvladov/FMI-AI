#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ctime>
#include <unordered_set>
using namespace std;

//chromosome.first[0] = A
//chromosome.first[1] = B
//chromosome.first[2] = C
//chromosome.first[3] = D
//chromosome.first[4] = E
//chromosome.first[5] = F
//chromosome.first[6] = G
//chromosome.first[7] = H
//chromosome.first[8] = I

///
const int POPULATION_SIZE = pow(2, 15);
///

typedef pair<vector<int>, int> chromosome;

bool pairSort(const chromosome &a, const chromosome &b)
{
	return a.second < b.second;
}

void printAnswer(vector<int> chromosome)
{
	char a = 'A';
	cout << "Solution found!" << endl;
	for (int i = 0; i < chromosome.size(); i++)
	{
		cout << a++ << ": " << chromosome[i] << endl;
	}
}

//Fitness function
//ABC * DEB = EDBDFC
int fitnessValue(vector<int> chromosome)
{
	int num1 = 100 * chromosome[0] + 10 * chromosome[1] + chromosome[2];
	int num2 = 100 * chromosome[3] + 10 * chromosome[4] + chromosome[1];

	int num6 = 100000 * chromosome[4] + 10000 * chromosome[3] + 1000 * chromosome[1] + 100 * chromosome[3] + 10 * chromosome[5] + chromosome[2];

	return abs(num1*num2 - num6);
}

bool checkSol(vector<int> chromosome)
{
	int num1 = 100 * chromosome[0] + 10 * chromosome[1] + chromosome[2];
	int num2 = 100 * chromosome[3] + 10 * chromosome[4] + chromosome[1];
	int num3 = num2;
	int num4 = 1000 * chromosome[8] + 100 * chromosome[0] + 10 * chromosome[6];
	int num5 = 100000 * chromosome[4] + 10000 * chromosome[7] + 1000 * chromosome[5] + 100 * chromosome[0];
	int num6 = 100000 * chromosome[4] + 10000 * chromosome[3] + 1000 * chromosome[1] + 100 * chromosome[3] + 10 * chromosome[5] + chromosome[2];
	return num1 * num2 == num6 && num3 + num4 + num5 == num6;
}

//Generate chromosome
vector<int> randChromosome()
{
	vector<int> chromosome;
	std::unordered_set<int> numbers;
	while (numbers.size() < 9)
	{
		numbers.insert(rand() % 9 + 1);
	}
	for (auto i = numbers.begin(); i != numbers.end(); i++)
	{
		chromosome.push_back(*i);
	}

	return chromosome;
}

//Modify the genes which have a duplicate number
//to a number which does not appear in the chromosome
vector<int> mutate(vector<int> child, unordered_set<int> set)
{
	bool used[9];
	for (auto i = set.begin(); i != set.end(); i++)
	{
		used[*i - 1] = true;
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = i; j < 9; j++)
		{
			if (child[i] == child[j])
			{
				for (int k = 0; k < 9; k++)
				{
					if (!used[k])
					{
						child[i] = k;
						used[k] = true;
						break;
					}
				}
			}
		}
	}
	return child;
}

//Child takes the first half of the first parent 
//and the second half of the second parent
chromosome reproduce(chromosome x, chromosome y)
{
	chromosome child;
	unordered_set<int> set;
	for (int i = 0; i < 5; i++)
	{
		child.first.push_back(x.first[i]);
		set.insert(x.first[i]);
	}
	for (int i = 5; i < 9; i++)
	{
		child.first.push_back(x.first[i]);
		set.insert(y.first[i]);
	}

	//If there are duplicate numbers, mutate
	if (set.size() < 9)
	{
		child.first = mutate(child.first, set);
	}

	child.second = fitnessValue(child.first);

	return child;
}

int main()
{
	char a = 'A';
	vector<int> solution;
	int bestFitness;

	//Create first population
	vector<chromosome> population;
	population.reserve(POPULATION_SIZE);

	vector<int> temp = randChromosome();
	int fitness = fitnessValue(temp);
	solution = temp;
	bestFitness = fitness;
	if (fitness == 0)
	{
		if (checkSol(solution))
		{
			printAnswer(solution);
			return 0;
		}
	}
	population.push_back({ temp, fitness });

	srand(time(NULL));
	for (int i = 1; i < POPULATION_SIZE; i++)
	{
		vector<int> temp = randChromosome();
		int fitness = fitnessValue(temp);
		//Keep closest solution according to fitness function
		if (fitness < bestFitness)
		{
			bestFitness = fitness;
			solution = temp;
		}
		//Check if it's a solution
		if (fitness == 0)
		{
			if (checkSol(temp))
			{
				printAnswer(temp);
				return 0;
			}
		}
		population.push_back({ temp, fitness });
	}

	sort(population.begin(), population.end(), pairSort);


	//Create new population with half the size from current one 
	//by reproducing adjacent chromosomes in a sorted population
	while (population.size() != 1)
	{
		vector<chromosome> new_population;
		new_population.reserve(population.size() / 2);
		for (int i = 0; i < population.size(); i += 2)
		{
			chromosome x = population[i];
			chromosome y = population[i + 1];
			chromosome child = reproduce(x, y);
			//Check if child is a solution
			if (child.second == 0)
			{
				if (checkSol(child.first))
				{
					printAnswer(child.first);
					return 0;
				}
			}

			if (child.second < bestFitness)
			{
				bestFitness = child.second;
				solution = child.first;
			}
			new_population.push_back(child);
		}
		population = new_population;
		sort(population.begin(), population.end(), pairSort);
	}

	cout << "No solution found. Closest solution is:" << endl;
	for (int i = 0; i < 9; i++)
	{
		cout << a++ << ':' << solution[i] << ' ';
	}
	cout << "Fitness: " << bestFitness;
	return 0;
}