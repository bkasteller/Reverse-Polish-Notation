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
    level->under = NULL;

    return level;
}

/* Ajoute un level à la pile */
pile_t* level_add(pile_t* pile, pile_t* level)
{
    /* ajout au dessus de la pile */
    level->under = pile;
    return level;
}

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












/* Change les positions du premier et second element de la pile */
pile_t* pile_swp ( pile_t* pile )
{
    if ( !verif_pile(pile, 2) )
        erreur(pile);

    int temp = pile->value;
    pile->value = pile->under->value;
    pile->under->value = temp;
    return pile;
}

/* Duplique le premier element de la pile */
pile_t* pile_dup ( pile_t* level )
{
    if ( !verif_pile(pile, 1) )
        erreur(pile);

    return level_add(level, level_create(level->value));
}

/* Retire le premier element de la pile */
pile_t* pile_pop ( pile_t* level )
{
    if ( !verif_pile(pile, 1) )
        erreur(pile);

    pile_t* under = level->under;
    free(level);
    return under;
}

/* Modulo */
pile_t* pile_mod ( pile_t* pile, int gauche, int droit )
{
    if ( !verif_pile(pile, 2) )
        erreur(pile);

    return level_add(pile, level_create(gauche%droit));
}

/* Division */
pile_t* pile_div ( pile_t* pile, int gauche, int droit )
{
    if ( !verif_pile(pile, 2) )
        erreur(pile);

    return level_add(pile, level_create(gauche/droit));
}

/* Multiplication */
pile_t* pile_mul ( pile_t* pile, int gauche, int droit )
{
    if ( !verif_pile(pile, 2) )
        erreur(pile);

    return level_add(pile, level_create(gauche*droit));
}

/* Soustraction */
pile_t* pile_sub ( pile_t* pile, int gauche, int droit )
{
    if ( !verif_pile(pile, 2) )
        erreur(pile);

    return level_add(pile, level_create(gauche-droit));
}

/* Addition */
pile_t* pile_add ( pile_t* pile, int gauche, int droit )
{
    if ( !verif_pile(pile, 2) )
        erreur(pile);

    return level_add(pile, level_create(gauche+droit));
}

/* Affiche une erreur et stop le programme */
void erreur( pile_t* pile )
{
    printf("ERROR\n");
    pile_reset(pile);
    exit(1);
}

pile_t* operation ( pile_t* pile, char instruction[INSTRUCTION_TAILLE] )
{
    int gauche = level->under->value, droit = level->value;

    if ( !strcmp(ROL, instruction) )
    {
        int n = pile->value;
        pile = pile_pop(pile);
        n = pile_rol(pile, pile->under, n-1);
        pile = level_add(pile, level_create(n));
    }
    else if ( !strcmp(SWP, instruction) )
        pile = pile_swp(pile);
    else if ( !strcmp(DUP, instruction) )
        pile = pile_dup(pile);
    else if ( !strcmp(POP, instruction) )
        pile = pile_pop(pile);
    else if ( !strcmp(MOD, instruction) )
        pile = pile_mod(pile_pop(pile_pop(pile)), gauche, droit);
    else if ( !strcmp(DIV, instruction) )
        pile = pile_div(pile_pop(pile_pop(pile)), gauche, droit);
    else if ( !strcmp(MUL, instruction) )
        pile = pile_mul(pile_pop(pile_pop(pile)), gauche, droit);
    else if ( !strcmp(SUB, instruction) )
        pile = pile_sub(pile_pop(pile_pop(pile)), gauche, droit);
    else if ( !strcmp(ADD, instruction) )
        pile = pile_add(pile_pop(pile_pop(pile)), gauche, droit);
    else
        erreur();

    return pile;
}

/* Permet de verifier si l'on possede bien n nombre(s) dans la pile */
int verif_pile ( pile_t* pile, int n )
{
    if ( pile == NULL )
        return 0;

    if ( pile > 1 )
        return verif_pile(pile->under, n--)
    else
        return 1;
}

/* Verifie la division par 0 */
int verif_div ( pile_t* pile, char instruction[INSTRUCTION_TAILLE] )
{
    pile_show(pile, 0);

    /* dans le cas d'une division par zero il faut pop les 2 elements superieur de la pile avant de l'afficher */
    if ( !manque_nombre(pile) )
        pile = pile_pop(pile_pop(pile));


    return !strcmp(DIV, instruction)
        && pile->value == 0;
}

/* Verifie que le nombre a roll existe */
void verif_roll ( pile_t* pile, char instruction[INSTRUCTION_TAILLE] )
{
    /*
    pile_t* under = level->under->under;
    free(level->under);
    free(level);*/
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
    for ( i = 0; i < N && !erreur; i++ )
    {
        /* recupere l'instruction */
        char instruction[INSTRUCTION_TAILLE];
        scanf("%s", instruction);

        /* verifie si l'instruction est un nombre ou une operation */
        if ( est_un_int(instruction) )
            pile = level_add(pile, level_create(cast_en_int(instruction)));
        else
            pile = operation(pile, instruction);
    }

    pile_show(pile, 0);
    printf("\n");

    /* libere la memoire */
    pile_reset(pile);

    return 0;
}
