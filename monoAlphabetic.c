/* Script that implements a simple monoalphabetic substitution cipher by taking a key as a parameter and then prompting 
the user for some text to encrypt */

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int valid_check(int number, string input);
string encrypt(string text, string cipher);


int main(int argc, string argv[]){
    /* Initially check that only one key has been provided, that the key is 26 letters long it contains each letter of
    the alphabet once. */
    int valid = valid_check(argc, argv[1]);
    if(valid == 1){
        return 1;
    }

    printf("%s\n", argv[1]);
    
    /*prompt the user for text to encrypt and implements the encrytion */
    string plain = get_string("PLAINTEXT: ");
    string ciphertext = encrypt(plain, argv[1]);

    printf("Ciphertext: %s\n", ciphertext);
    return 0;

}

int valid_check(int number, string input){
    if (number != 2){
        printf("Invalid number of parameters, program takes 1\n");
        return 1;
    }
    if (strlen(input) != 26){
        printf("Invalid length of key, must be 26 characters long\n");
        return 1;
    }
    int flag;
    for (int i=0; i<26; i++){
        flag = 0;
        for (int j=0; j<26; j++){
            if (input[j] == (char) (97+i) || input[j] == (65+i)){
                flag += 1;
            }
        }
        if (flag != 1){
            printf("Invalid key, key must contain one of each alphabetic characters\n");
            return 1;
        }
    }
    return 0;
}

string encrypt(string text, string cipher){
    int n = strlen(text);
    string ciphertext = malloc(n);

    for (int i=0; i<n; i++){
        if ((int) text[i] >= 97 && (int) text[i] <= 122){
            int index = (int) text[i] - 97;
            ciphertext[i] = tolower(cipher[index]);
        }
        else if ((int) text[i] >= 65 && (int) text[i] <= 90){
            int index = (int) text[i] - 65;
            ciphertext[i] = toupper(cipher[index]);
        }
        else{
            ciphertext[i] = text[i];
        }
    }

    return ciphertext;
}
