#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Definit la taille d'une instruction */
#define INSTRUCTION_TAILLE 11

/* Definit la base de convertion (decimal) */
#define CONVERTION_BASE 10

/* Definit le nom des differente(s) opperation(s) */
#define SWP "SWP"
#define DUP "DUP"
#define POP "POP"
#define MOD "MOD"
#define DIV "DIV"
#define MUL "MUL"
#define SUB "SUB"
#define ADD "ADD"

/* liste chainee */
typedef struct pile
{
    int value;
    struct pile *under;
}pile_t;

/* Libere l'espace memoire de chaque level de la pile */
void pile_reset(pile_t* level)
{
    if ( level != NULL )
    {
        pile_reset(level->under);
        free(level);
    }
}

/* Affiche les valeurs de chaque level de la pile */
void pile_show(pile_t* level, int i)
{
    if ( level != NULL )
    {
        /* Affichage LIFO */
        // printf("%d", level->value);

        pile_show(level->under, i+1);

        /* Affichage FIFO */
        printf("%d", level->value);

        /* N'affiche pas le dernier espace */
        /* En mode LIFO */
        // if ( level->under == 0 )
        /* En mode FIFO */
        if ( i != 0 )
            printf(" ");
    }
}

/* cree un nouveau level contenant une valeur */
pile_t* level_create(int value)
{
    pile_t* level = (pile_t*)malloc(sizeof(pile_t));

    if ( level == NULL )
    {
        fprintf(stderr, "Mémoire insuffisante à la creation d'un nouveau level de la pile\n");
        exit(1);
    }

    level->value = value;

    /* N'est relie a aucune pile pour l'instant */
    pile ->under = NULL;

    return level;
}

/* Ajoute un level à la pile */
pile_t* level_add(pile_t* pile, pile_t* level)
{
    /* Necessaire quand on ajoute en dessous de la pile
    if ( pile == NULL )
    {
        level->under = NULL;
        return level;
    }*/

    /* ajout au dessus de la pile */
    level->under = pile;
    return level;
}

/* Permet de savoir si une instruction est un nombre */
int est_un_int (char instruction[INSTRUCTION_TAILLE])
{
    int i;
    for ( i = 0; i < INSTRUCTION_TAILLE && instruction[i] != '\0'; i++ )
        if ( instruction[0] != '-' && !isdigit(instruction[i]) ) /* Est un char, hors signe - devant un nombre */
            return 0;
    return 1;
}

/* Permet de convertir une chaine designant un nombre, en nombre de base CONVERTION_BASE */
int cast_en_int (char instruction[INSTRUCTION_TAILLE])
{
    char *ptr;
    return strtol(instruction, &ptr, CONVERTION_BASE);
}

int pile_rol ( pile_t * back, pile_t* pile, int i ) /* selectionne l'element i de la pile et le remonte au haut de la pile */
{
    if ( back == NULL || pile == NULL ) /* la pile est vide */
    {
        err = 1;
        return 0;
    }
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

pile_t* pile_mod ( pile_t* level ) /* effectue le modulo */
{
    if ( level == NULL ) /* la pile est vide */
        return NULL;
    if ( level->under == NULL ) /* la pile comporte qu'un element */
    {
        err = 1;
        free(level);
        return level;
    }
    int gauche = level->under->value, droit = level->value;
    pile_t* under = level->under->under;
    free(level->under);
    free(level);
    return level_add(under, level_create(gauche%droit, under));
}

pile_t* pile_div ( pile_t* level ) /* effectue la division */
{
    if ( level == NULL ) /* la pile est vide */
        return NULL;
    if ( level->under == NULL ) /* la pile comporte qu'un element */
    {
        err = 1;
        level = pile_pop(level);
        level = pile_pop(level);
        return level;
    }
    int gauche = level->under->value, droit = level->value;
    if ( droit == 0 )
    {
        err = 1;
        level = pile_pop(level);
        level = pile_pop(level);
        return level;
    }
    pile_t* under = level->under->under;
    free(level->under);
    free(level);
    return level_add(under, level_create(gauche/droit, under));
}

pile_t* pile_mul ( pile_t* level ) /* effectue la multiplication */
{
    if ( level == NULL ) /* la pile est vide */
        return NULL;
    if ( level->under == NULL ) /* la pile comporte qu'un element */
    {
        err = 1;
        level = pile_pop(level);
        return level;
    }
    int gauche = level->under->value, droit = level->value;
    pile_t* under = level->under->under;
    free(level->under);
    free(level);
    return level_add(under, level_create(gauche*droit, under));
}

pile_t* pile_sub ( pile_t* level ) /* effectue la soustraction */
{
    if ( level == NULL ) /* la pile est vide */
        return NULL;
    if ( level->under == NULL ) /* la pile comporte qu'un element */
    {
        err = 1;
        level = pile_pop(level);
        return level;
    }
    int gauche = level->under->value, droit = level->value;
    pile_t* under = level->under->under;
    free(level->under);
    free(level);
    return level_add(under, level_create(gauche-droit, under));
}

pile_t* pile_add ( pile_t* level ) /* effectue une addition */
{
    if ( level == NULL ) /* la pile est vide */
        return NULL;
    if ( level->under == NULL ) /* la pile comporte qu'un element */
    {
        err = 1;
        level = pile_pop(level);
        return level;
    }
    int gauche = level->under->value, droit = level->value;
    pile_t* under = level->under->under;
    free(level->under);
    free(level);
    return level_add(under, level_create(gauche+droit, under));
}

pile_t* operation ( pile_t* pile, char instruction[INSTRUCTION_TAILLE] )
{
    if ( !strcmp("ROL", instruction) )
    {
        int n = pile->value;
        pile = pile_pop(pile);
        n = pile_rol(pile, pile->under, n-1);
        pile = level_add(pile, level_create(n, pile));
    }
    else if ( !strcmp("SWP", instruction) )
        pile = pile_swp(pile);
    else if ( !strcmp("DUP", instruction) )
        pile = pile_dup(pile);
    else if ( !strcmp("POP", instruction) )
        pile = pile_pop(pile);
    else if ( !strcmp("MOD", instruction) )
        pile = pile_mod(pile);
    else if ( !strcmp("DIV", instruction) )
        pile = pile_div(pile);
    else if ( !strcmp("MUL", instruction) )
        pile = pile_mul(pile);
    else if ( !strcmp("SUB", instruction) )
        pile = pile_sub(pile);
    else if ( !strcmp("ADD", instruction) )
        pile = pile_add(pile);

    return pile;
}

/* Permet de savoir si l'instruction recupere est une instruction reconnu */
int operation_connue ( char instruction[INSTRUCTION_TAILLE] )
{
    return strcmp(ROL, instruction)
        && strcmp(SWP, instruction)
        && strcmp(DUP, instruction)
        && strcmp(POP, instruction)
        && strcmp(MOD, instruction)
        && strcmp(DIV, instruction)
        && strcmp(MUL, instruction)
        && strcmp(SUB, instruction)
        && strcmp(ADD, instruction);
}

/* Permet de verifier qu'on possede bien deux nombres pour effectuer l'operation */
int manque_nombre ( pile_t* pile )
{
    return pile == NULL
        || pile->under == NULL;

}

/* Verifie la division par 0 */
int division_par_zero ( pile_t* pile, char instruction[INSTRUCTION_TAILLE] )
{
    return !strcmp(DIV, instruction)
        && pile->value == 0;
}

/* Effectue une suite de verification pour trouver d'eventuelle erreurs */
int verification ( pile_t* pile, char instruction[INSTRUCTION_TAILLE] )
{
    return operation_connue(instruction)
        && !manque_nombre(pile)
        && !division_par_zero(pile, instruction);

    return pile;
}

int main()
{
    /* recuperation du nombre d'instrution(s) */
    int N;
    scanf("%d", &N);

    /* creation de la pile vide */
    pile_t* pile = NULL;

    /* variable utilise pour savoir s'il y a une erreur */
    int erreur = 0;

    /* boucle de récupération des instructions */
    int i;
    for ( i = 0; i < N && err != 1; i++ )
    {
        /* recupere l'instruction */
        char instruction[INSTRUCTION_TAILLE];
        scanf("%s", instruction);

        /* verifie si l'instruction est un nombre ou une operation */
        if ( est_un_int(instruction) )
            pile = level_add(pile, level_create(cast_en_int(instruction), pile));
        else
        {
            /* verifie si il existe une erreur sur l'operation */
            if ( verification(pile, instruction) )
            {
                erreur = verification(pile, instruction);
                break;
            }

            pile = operation(pile, instruction);
        }
    }

    /* affiche la pile */
    pile_show(pile, 0);

    /* ajoute ERROR a la fin de la pile si il y a une erreur */
    if ( erreur )
    {
        if ( pile != NULL )
            printf(" ");
        printf("ERROR");
    }

    printf("\n");

    /* libere la memoire */
    pile_reset(pile);

    return 0;
}
