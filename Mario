/* Script that creates mock Mario pyramid structures using # given a height input by user */
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    /* Prompt user for height and keep prompting until the height is between 1 and 8 */
    int height = get_int("Please enter height of blocks\n");
    while(height < 1 || height > 8){
        height = get_int("Please enter height of blocks\n");
    }
    
    char *blocks = "#";
    char str[8];
    char revstr[8];
    
    /* for each row add to str how spaces we need followed by how many left hastags we need, 
    then add to revstr how many right hashtags we need*/
    for (int i = 1; i <= height; i++){
        strcpy(str, "");
        strcpy(revstr, "");
        for (int j = 0; j < height-i; j++){
            strcat(str, " ");
        }
        for (int j = 0; j < i; j++){
            strcat(str, blocks);
        }
        for (int j = 0; j < i; j++){
            strcat(revstr, blocks);
        }
       
        printf("%s  %s\n", str, revstr);
    }

}

/* example if height = 4, output = 
   #  #
  ##  ##
 ###  ###
####  ####
*/
