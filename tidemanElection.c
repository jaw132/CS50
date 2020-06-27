/* The goal of the exercise is to complete a given template for registering votes in a mock tideman election
and return the winner. CS50 provided the start of the code, I have commented which parts are template and which are 
my work */

// Template starts here.
#include <cs50.h>
#include <stdio.h>
#include <string.h>

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

// Define a structure to hold the difference between pairs and the index of the pair
typedef struct
{
    int diff;
    int index;
}
difference;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

void mergeSort(difference array[], int l, int r);
void merge(difference array[], int l, int m, int r);

//void merge(difference array[], int size);
bool is_cycle(int index, int target);

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
    int voter_count = get_int("Number of voters: ");

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
/Template ends here.

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{

    for (int i=0; i < candidate_count; i++){
        if (strcmp(name, candidates[i]) == 0){
            ranks[rank] = i;
            return true;
        }
    }
    return false;

}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count-1; i++){
        for (int j = i+1; j < candidate_count; j++){
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int index = 0;
    for (int i = 0; i<candidate_count-1; i++){
        for(int j = i+1; j<candidate_count; j++){
            if (preferences[i][j] > preferences[j][i]){
                pairs[index].winner = i;
                pairs[index].loser = j;
                index += 1;
                pair_count += 1;
            }
            else if (preferences[i][j] < preferences[j][i]){
                pairs[index].winner = j;
                pairs[index].loser = i;
                index += 1;
                pair_count += 1;
            }
        }
    }

    for (int i=0; i < pair_count; i++){
        printf("%i, %i\n", pairs[i].winner, pairs[i].loser);
    }



    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    difference pair_diff[pair_count];
    for (int i=0; i< pair_count; i++){
        pair_diff[i].diff = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        pair_diff[i].index = i;
    }


    mergeSort(pair_diff, 0, pair_count-1);

    printf("%i\n", pair_diff[0].index);
    printf("%i\n", pair_diff[1].index);
    printf("%i\n", pair_diff[2].index);


    pair temp_pairs[pair_count];
    int sort_index;
    for (int i=0; i < pair_count; i++){
        sort_index = pair_diff[i].index;

        temp_pairs[i].winner = pairs[sort_index].winner;
        temp_pairs[i].loser = pairs[sort_index].loser;
    }

    for (int i=0; i < pair_count; i++){
        pairs[i].winner = temp_pairs[i].winner;
        pairs[i].loser = temp_pairs[i].loser;
    }


    for (int i=0; i < pair_count; i++){
        printf("%i, %i\n", pairs[i].winner, pairs[i].loser);
    }


    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++){
        bool cycle = is_cycle(pairs[i].loser , pairs[i].winner);
        if (!cycle){
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++){
        bool win = true;
        for (int j = 0; j< candidate_count; j++){
            if (locked[j][i]){
                win = false;
            }
        }
        if (win){
            printf("%s\n", candidates[i]);
        }
    }
    return;
}


// Implementation of merge Sort
void mergeSort(difference array[], int left, int right){
    if (left < right){
        int m = left+(right-l)/2;
        mergeSort(array, left, m);
        mergeSort(array, m+1, right);

        merge(array, left, m, right);
    }
}

void merge(difference array[], int l, int m, int r){
    int lind = 0, rind = 0, merind = l;
    int n1 = m-l+1;
    int n2 = r-m;

    difference L[n1], R[n2];

    for (int i = 0; i < n1; i++){
        L[i].diff = array[l+i].diff;
        L[i].index = array[l+i].index;
    }

    for (int j = 0; j < n2; j++){
        R[j].diff = array[m+1+j].diff;
        R[j].index = array[m+1+j].index;
    }

    while(lind < n1 && rind < n2){
        if (L[lind].diff<R[rind].diff){
            array[merind].diff = L[lind].diff;
            array[merind].index = L[lind].index;
            lind++;
        }
        else{
            array[merind].diff = R[rind].diff;
            array[merind].index = R[rind].index;
            rind++;
        }
        merind++;
    }

    while (lind < n1){
        array[merind].diff = L[lind].diff;
        array[merind].index = L[lind].index;
        lind++;
        merind++;
    }

    while (rind < n2){
        array[merind].diff = R[rind].diff;
        array[merind].index = R[rind].index;
        rind++;
        merind++;
    }

}


// Check if adding edge to the graph will create a cycle
bool is_cycle(int index, int target){

    if (locked[index][target]){
        return true;
    }

    for (int i = 0; i < candidate_count; i++){
        if (locked[index][i]){
            bool connect = is_cycle(i, target);
            if (connect){
                return true;
            }
        }
    }

    return false;


}
