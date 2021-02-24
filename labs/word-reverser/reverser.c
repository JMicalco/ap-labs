#include <stdio.h>

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */

/* count lines, words, and characters in input */
void reverse(int lenght, char arr[]) {

    int i, tmp;

    for (i = 0;  i < lenght/2; i++) {
	tmp = arr[i];
	arr[i] = arr[lenght - i - 1];
	arr[lenght - i - 1] = tmp;
    }
}

int main()

{
    int i, state, nw;
    char c, word[100];
    state = OUT;

    i = 0;
    nw=0;

    while ((c = getchar()) != EOF) {    
    word[i] = c;
    i++;
	if (c == ' ') {
	    state = OUT; 
    }
	else if (state == OUT) {
	    state = IN;
	} else{

    }
    }
    reverse(i,word);
    for (int j = 0; j < i; j++)
        printf("%c", word[j]);   
    return 0;
}