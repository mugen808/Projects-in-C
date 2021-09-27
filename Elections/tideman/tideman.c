#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int voter_count;
int edges[MAX];


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int counter, int edge_count, int loser_pair);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Loop between the voter ranks
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // Place each votes on the preferences array
        for (int j = 1; j < candidate_count; j++)
        {
            // Store as 0 if preferred over itself
            if (ranks[i] == ranks[j])
            {
                preferences[ranks[i]][ranks[j]] = 0;
            }
            else
            {
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i] && i != j)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[j][i] && i != j)
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int check_sort = 0;
    int winner_holder = 0;
    int loser_holder = 0;

    for (int i = 0; i < pair_count - 1; i++)
    {
        if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
        {
            winner_holder = pairs[i].winner;
            pairs[i].winner = pairs[i + 1].winner;
            pairs[i + 1].winner = winner_holder;

            loser_holder = pairs[i].loser;
            pairs[i].loser = pairs[i + 1].loser;
            pairs[i + 1].loser = loser_holder;
        }
        else if (preferences[pairs[i].winner][pairs[i].loser] >= preferences[pairs[i + 1].winner][pairs[i + 1].loser])
        {
            check_sort++;
        }
    }
    if (check_sort == pair_count - 1)
    {
        return;
    }
    sort_pairs();
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int j = 0; j < pair_count; j++)
    {
        edges[j] = 0;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        if (check_cycle(pair_count, edges[pairs[i].loser], pairs[i].loser) == false)
        {
            edges[pairs[i].loser]++;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}

// Check if the lock is going to create a cycle
bool check_cycle(int counter, int edge_count, int loser_pair)
{
    int check_cycle2 = 0;
    if (edge_count > 0)
    {
        return false;
    }
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[pairs[j].winner][pairs[j].loser] == true)
        {
            check_cycle2++;
        }
        else if (locked[pairs[j].loser][pairs[j].winner] == true)
        {
            return true;
        }

    }
    for (int j = 0; j < candidate_count; j++)
    {
        if (locked[loser_pair][pairs[j].loser] == true)
        {
            return true;
        }

    }
    if (check_cycle2 == counter)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == false)
            {
                printf("%s\n", candidates[j]);
            }
        }
    }
    return;
}