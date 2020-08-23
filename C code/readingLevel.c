/* prompts user for excerpt of book and determines reading level */
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

void counting(string text, int total[]);

int main(void){
    string input = get_string("TEXT:");
    int total[] = {0, 0, 0};
    /* Function to return number of letters, words, and sentences in text provided */
    counting(input, total);
    
    /* Compute number of letters per 100 words and the number of sentences per 100 words and calculate the Coleman-Liau index */
    float letter_avg = ((float) total[0] / total[1] ) * 100;
    float sent_avg = ((float) total[2] / total[1] ) * 100;

    float index = round(0.0588 * letter_avg - 0.296 * sent_avg - 14.8);
    
    /* print the level as output */
    if(index < 1){
        printf("Before Year 1\n");
    }
    else if (index > 17){
        printf("Year 17+\n");
    }
    else{
        printf("Year: %i\n", (int) index);
    }

}

void counting(string text, int total[]){


    for(int i=0, n=strlen(text); i<n; i++){
        if ((text[i]>='a' && text[i]<='z') || (text[i]>='A' && text[i]<='Z')){
            total[0] += 1;
        }

        if (text[i] == ' '){
            total[1] += 1;
        }

        if (text[i] == '.' || text[i] == '?' || text[i] == '!'){
            total[2] += 1;
        }
    }

    total[1] += 1;

}
