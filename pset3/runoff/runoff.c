#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
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
    for (int x = 0; x < candidate_count; x++) //create loop to go through candidates
    {
        if (strcmp (candidates[x].name, name) == 0) //check to see if the candidates name matches what was inserted by the user
        {
            preferences[voter][rank] = x; //assign that candidates name to the preference
            
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++) //loop to go through voters
    {

    int j = 0; //set j to zero to indicate the first preference

    while (candidates[preferences[i][j]].eliminated == true) //loop to be sure that the preference isn't eliminated
    {
        j++; //if the preference has been eliminated add 1 to j in order to move on to next preference
    }

    candidates[preferences[i][j]].votes++; //add a vote to the candidates tally

    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int majority = (voter_count / 2) + .5; //declare majority and add .5 so that it doesn't round down

    for (int i = 0; i < candidate_count; i++) //loop to go through candidates
    {
        if (candidates[i].votes > majority) //check to see if they have the majority
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int minimum = 100; //decalre a varioable called minimum and set it to 0

    for (int i = 0; i < candidate_count; i++) //create a loop to go through candidates
    {
        if ((candidates[i].votes <= minimum) && (candidates[i].eliminated == false)) //compare candidates to find a new minimum, making sure they have not been eliminated
        {
            minimum = candidates[i].votes; //if found update minimum
        }
       
    }
    return minimum;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int not_eliminated = 0; //set variable to keep track of how many are not eliminated
    int has_minimum = 0; //set variable to keep track of candidates with minimum

    //if not_eliminated = has_minimum then there is a tie and the function returns true

    for (int i = 0; i < candidate_count; i++) //loop to go through all candidates
    {
        if (candidates[i].eliminated == false) //check to see if they aren't eliminated
        {
            not_eliminated++; //add to counter
        }

        if (candidates[i].votes == min) //check to see if they have the minimum
        {
            has_minimum++; //add to counter
        }
    }

    if (not_eliminated == has_minimum) //compare the two variabales. If they are the same then there is a tie
    {
        return true;
    }

    return false;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if(candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
