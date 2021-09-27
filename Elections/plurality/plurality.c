#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;



// Array of candidates
candidate candidates[MAX];
candidate candidates_winners[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    int x = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) != 0)
        {
            x++;
        }
        else
        {
            candidates[i].votes++;
        }
    }
    if (x == candidate_count)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int winners = 0;
    int maxvotes = 0;

    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; j < candidate_count; j++)
        {
            if (candidates[i].votes > candidates[j].votes && candidates[i].votes > maxvotes)
            {
                candidates_winners[winners].name = candidates[i].name;
                printf("If: %s\n", candidates[i].name);
                candidates_winners[winners].votes = candidates[i].votes;
                maxvotes = candidates[i].votes;
            }
            else if (candidates[i].votes < candidates[j].votes && candidates[j].votes > maxvotes)
            {
                candidates_winners[winners].name = candidates[j].name;
                printf("Else If: %s\n", candidates[j].name);
                candidates_winners[winners].votes = candidates[j].votes;
                maxvotes = candidates[j].votes;
            }
            else if (candidates[i].votes == candidates[j].votes && candidates[j].votes >= maxvotes)
            {
                candidates_winners[winners].name = candidates[i].name;
                candidates_winners[winners].votes = candidates[i].votes;
                maxvotes = candidates[i].votes;
                winners ++;
                candidates_winners[winners].name = candidates[j].name;
                candidates_winners[winners].votes = candidates[j].votes;
            }
        }
    }
    for (int i = 0; i <= winners; i++)
    {
        printf("%s\n", candidates_winners[i].name);
    }

    return;
}

