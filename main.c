#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INSTRUCTION_TAILLE 11 /* Définit la taille d'une instruction */
#define CONVERTION_BASE 10 /* Définit la basse de convertion */


int err = 0; /* Variable global */

typedef struct pile
{
    int value;
    struct pile *under;
}pile_t;

pile_t* level_create(int value, pile_t* under) /* Ajoute un niveau au dessus de la pile donné en paramètre */
{
    pile_t* level = (pile_t*)malloc(sizeof(pile_t));
    if ( level == NULL ) /* Problème de mémoire */
    {
        fprintf(stderr, "Mémoire insuffisante à la creation d'un nouveau level de la pile\n");
        exit(1);
    }
    level->value = value;
    level->under = under;
    return level;
}

void pile_reset(pile_t* level) /* Libère l'espace mémoire de chaque level de la pile */
{
    if ( level != NULL ) /* Pile vidé de ses niveau, plus de niveau */
    {
        pile_reset(level->under);
        free(level);
    }
}

void pile_show(pile_t* level) /* Affiche les valeurs des différents niveau de la pile, en partant du BAS vers le HAUT */
{
    if ( level != NULL ) /* Plus de niveau inférieur, on a fini de parcourir la pile */
    {
        pile_show(level->under);
        printf("%d ", level->value); /* du bas vers le haut, à passer au dessus de l'instruction pile_show(level->under); pour inverser */
    }
}

pile_t* level_add(pile_t* pile, pile_t* level) /* Ajoute un nouveau level à la pile et l'ajoute AU DESSUS DE LA PILE */
{
    if ( pile == NULL ) /* Aucune pile, deviens la pile */
    {
        level->under = NULL;
        return level;
    }
    //pile->under = level_add(pile->under, level); /* AJOUT EN BAS DE LA PILE */
    level->under = pile; /* AJOUT AU DESSUS DE LA PILE */
    return /*pile*/level;
}

int est_un_int (char instruction[INSTRUCTION_TAILLE])
{
    int i;
    for ( i = 0; i < INSTRUCTION_TAILLE && instruction[i] != '\0'; i++ )
        if ( !isdigit(instruction[i]) )
            return 0;
    return 1;
}

int cast_en_int (char instruction[INSTRUCTION_TAILLE])
{
    char *ptr;
    return strtol(instruction, &ptr, CONVERTION_BASE);
}

int pile_rol ( pile_t * back, pile_t* pile, int i ) /* selectionne l'element i de la pile et le remonte au haut de la pile */
{
    if ( pile == NULL ) /* la pile est vide */
        return NULL;
    if ( i == 1 )
    {
        pile_t* under = pile->under;
        int value = pile->value;
        free(pile); /* supprime le level actuel */
        back->under = under;
        return value;
    }
    else
         return pile_rol ( pile, pile->under, i-1 );
}

pile_t* pile_swp ( pile_t* pile ) /* change les positions du premier et second element de la pile */
{
    if ( pile == NULL ) /* la pile est vide */
        return NULL;
    if ( pile->under != NULL ) /* il existe un deuxième élément dans la pile */
    {
        int temp = pile->value;
        pile->value = pile->under->value;
        pile->under->value = temp;
    }
    return pile;
}

pile_t* pile_dup ( pile_t* level ) /* duplique le premier element de la pile */
{
    if ( level == NULL ) /* la pile est vide */
        return NULL;
    return level_add(level, level_create(level->value, level));
}

pile_t* pile_pop ( pile_t* level ) /* retire le premier element de la pile */
{
    pile_t* under = NULL;
    if ( level != NULL ) /* la pile n'est pas vide */
    {
        under = level->under;
        free(level);
    }
    return under;
}

pile_t* operation ( pile_t* pile, char instruction[INSTRUCTION_TAILLE] )
{
    if ( strcmp("ROL", instruction) == 0 )
    {
        //fprintf(stderr, "rol\n");
        int i = pile->value;
        pile = pile_pop(pile);
        i = pile_rol(pile, pile->under, i-1);
        pile = level_add(pile, level_create(i, pile));
    }
    else if ( strcmp("SWP", instruction) == 0 )
    {
        //fprintf(stderr, "swp\n");
        pile = pile_swp(pile);
    }
    else if ( strcmp("DUP", instruction) == 0 )
    {
        fprintf(stderr, "dup\n");
        pile = pile_dup(pile);
    }
    else if ( strcmp("POP", instruction) == 0 )
    {
        fprintf(stderr, "pop\n");
        pile = pile_pop(pile);
    }
    else if ( strcmp("MOD", instruction) == 0 )
        fprintf(stderr, "mod\n");
    else if ( strcmp("DIV", instruction) == 0 )
        fprintf(stderr, "div\n");
    else if ( strcmp("MUL", instruction) == 0 )
        fprintf(stderr, "mul\n");
    else if ( strcmp("SUB", instruction) == 0 )
        fprintf(stderr, "sub\n");
    else if ( strcmp("ADD", instruction) == 0 )
        fprintf(stderr, "add\n");
    else
        err = 1;
    return pile;
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int N, i;
    pile_t* pile = NULL;
    scanf("%d", &N);
    for ( i = 0; i < N && err != 1; i++ )
    {
        char instruction[INSTRUCTION_TAILLE];
        scanf("%s", instruction);
        // int j;
        // for ( j = 0; j < 11, instruction[j] != '\0'; j++ ){fprintf(stderr, "%c; ", instruction[j]);}
        if ( est_un_int(instruction) )
        {
            // fprintf(stderr, "C'est un int : %d; ", cast_en_int(instruction));
            pile = level_add(pile, level_create(cast_en_int(instruction), pile));
        }
        else
        {
            // fprintf(stderr, "Ce n'est pas un int : %s; ", instruction);
            pile = operation(pile, instruction);
        }
    }
    /* Write an action using printf(). DON'T FORGET THE TRAILING \n
       To debug: fprintf(stderr, "Debug messages...\n"); */
    pile_show(pile);
    if ( err )
        printf("ERREUR");
    printf("\n");
    pile_reset(pile);
    return 0;
}
