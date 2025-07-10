#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char buffer[]){
    char keywords[32][10] = {
        "auto","break","case","char","const","continue","default",
        "do","double","else","enum","extern","float","for","goto",
        "if","int","long","register","return","short","signed",
        "sizeof","static","struct","switch","typedef","union",
        "unsigned","void","volatile","while"
    };
    for(int i = 0; i < 32; ++i){
        if(strcmp(keywords[i], buffer) == 0){
            return 1;
        }
    }
    return 0;
}
int isConstant(char buffer[]) {
    int i = 0, hasDot = 0;
    if (!isdigit(buffer[0]))
        return 0;
    for(i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '.') {
            if (hasDot) return 0;
            hasDot = 1;
        } else if (!isdigit(buffer[i])) {
            return 0;
        }
    }
    return 1;
}
int main(){
    char ch, buffer[100], operators[] = "+-*/%=<>!", separators[] = ";:(){}";
    FILE *fp;
    int i, j = 0;
    fp = fopen("inp.txt","r");
    if(fp == NULL){
        printf("Error while opening the file\n");
        exit(0);
    }
    while((ch = fgetc(fp)) != EOF){
        // Handle single-line and multi-line comments
        if(ch == '/') {
            char next = fgetc(fp);
            if(next == '/') {
                while((ch = fgetc(fp)) != EOF && ch != '\n');
                continue;
            }
            else if(next == '*') {
                while(1) {
                    ch = fgetc(fp);
                    if(ch == EOF) break;
                    if(ch == '*') {
                        if((ch = fgetc(fp)) == '/') break;
                        else ungetc(ch, fp);
                    }
                }
                continue;
            }
            else {
                ungetc(next, fp);
            }
        }
        // Check for operators (including >=, <=, ==, !=)
        for(i = 0; i < strlen(operators); ++i){
            if(ch == operators[i]) {
                char next = fgetc(fp);
                if((ch == '>' || ch == '<' || ch == '=' || ch == '!') && next == '=') {
                    printf("%c%c is operator\n", ch, next);
                } else {
                    printf("%c is operator\n", ch);
                    ungetc(next, fp);
                }
                break;
            }
        }
        // Check for separators
        for(i = 0; i < strlen(separators); ++i){
            if(ch == separators[i]) {
                printf("%c is separator\n", ch);
                break;
            }
        }
        // Collect characters for identifier/keyword/constant
        if(isalnum(ch) || (ch == '.' && j > 0 && isdigit(buffer[j - 1]))) {
            buffer[j++] = ch;
        }
        else if((ch == ' ' || ch == '\n' || ch == '\t') && j != 0){
            buffer[j] = '\0';
            j = 0;
            if(isKeyword(buffer))
                printf("%s is keyword\n", buffer);
            else if(isConstant(buffer))
                printf("%s is number\n", buffer);
            else
                printf("%s is an identifier\n", buffer);
        }
        else if(j != 0 && !isalnum(ch) && ch != '.') {
            buffer[j] = '\0';
            j = 0;
            if(isKeyword(buffer))
                printf("%s is keyword\n", buffer);
            else if(isConstant(buffer))
                printf("%s is number\n", buffer);
            else
                printf("%s is an identifier\n", buffer);
        }
    }
    fclose(fp);
    return 0;
}
