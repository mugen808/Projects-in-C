#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9
typedef struct
{
    string ranking;
    int voter;
}
preference;

// preferences[i][j] is jth preference for voter i
preference preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    int first_option;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    int x = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank].ranking = name;
            rank++;
        }
        else
        {
            x++;
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

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{

    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Loops between the candidate names and the preference list for each voter
            if (strcmp(candidates[j].name, preferences[i][0].ranking) == 0 && candidates[j].eliminated == false)
            {
                candidates[j].votes++;
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one

bool print_winner(void)
{
    int x = 0;
    float half_votes = voter_count / 2;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > half_votes)
        {
            printf("Winner: %s\n", candidates[i].name);
            x = 1;
        }
    }
    if (x == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = candidates[0].votes;
    for (int i = 1; i < candidate_count; i++)
    {
        if (candidates[i].votes < candidates[(i - 1)].votes && candidates[i].eliminated == false
            && candidates[(i - 1)].eliminated == false)
        {
            min = candidates[i].votes;
        }
        else if (candidates[i].votes > candidates[(i - 1)].votes && candidates[i].eliminated == false
                 && candidates[(i - 1)].eliminated == false)
        {
            min = candidates[(i - 1)].votes;
        }
    }

    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int tie = 0;
    int remaining = 0;
    // Checks if in the remaining candidates there is a tie after a round of elimination
    for (int i = 1; i < candidate_count; i++)
    {
        if (candidates[i].eliminated != true)
        {
            remaining++;
        }
        if (candidates[i].votes == min)
        {
            tie++;
        }
    }
    if (tie == remaining)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // If tie is false, proceed to eliminate candidate(s)
        int rank_change = 1;
        if (candidates[i].votes <= min && is_tie(min) == false)
        {
            candidates[i].eliminated = true;
        }
        //When a candidate is eliminated, shif below candidates up in the ballot
        if (candidates[i].eliminated == true)
        {
            for (int k = 0; k < voter_count; k++)
            {
                if (strcmp(candidates[i].name, preferences[k][0].ranking) == 0 && rank_change <= candidate_count)
                {
                    preferences[k][0].ranking = preferences[k][rank_change].ranking;
                    preferences[k][rank_change].ranking = "ELIMINATED";
                }
                else if (strcmp(preferences[k][i].ranking, "ELIMINATED") == 0 && rank_change <= candidate_count)
                {
                    preferences[k][i].ranking = preferences[k][rank_change].ranking;
                    preferences[k][rank_change].ranking = "ELIMINATED";
                }
            }
        }
        else
        {
            candidates[i].eliminated = false;
        }
        rank_change++;
    }
    return;
}
