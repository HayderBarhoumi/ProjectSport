#include<stdio.h>
#include<string.h>


typedef enum { GENDER_HOMME = 1, GENDER_FEMME = 2 } Gender;
typedef struct { 
int day; int month; int year; 
} Date;

typedef struct {
    int     CIN;
    char    nom[30];
    char    prénom[30];
    Gender  genre;
    Date    date_naissance;    
    char    phone[20];
} Member;
