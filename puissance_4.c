#include "puissance_4.h"

/** Cols = x, lines = y **/
/** On va malloc la grid 
 * 	
 * 	grid[lignes][colonnes] **/
/** Fonction ou on va malloc notre tableau en 2d et récupérer dans notre struct cols et lines **/
struct game *new_game(int cols, int lines)
{
	struct game *game;
	if (!(game = malloc(sizeof(game))))
	{
		free(game);
		return (NULL);
	}
	if (!(game->grid = (int**)malloc(sizeof(int*) * lines)))
	{
		free(game->grid);
		free(game);
		return (NULL);
	}
	for (int x = 0; x < cols; x++)
	{
		if (!(game->grid[x] = (int*)malloc(sizeof(int) * cols)))
		{
			free(game->grid[x]);
			free(game->grid);
			free(game);
			return (NULL);
		}
	}
	for (int y = 0; y < lines; y++)
	{
		for (int x = 0; x < cols; x++)
			game->grid[y][x] = 2;
	}
	game->cols = cols;
	game->lines = lines;
	return (game);
}

/** Fonction qui permet de free toute la mémoire allouer par les mallocs **/
void	free_game(struct game *game)
{
	for (int x = 0; x < game->lines; x++) // ATTENTION LA FONCTION RENVOIE UNE ERREUR DOUBLE FREE CORUPTION ca doit venir de cette fonction je pense
		free(game->grid[x]);
	free(game->grid);
	free(game);
}

/** Fonction qui permet d'afficher la grille du jeu **/
void	print_game(struct game *game)
{

	for(int y = game->lines - 1; y >= 0; y--)
	{
		for(int x = 0; x < game->cols; x++)
		{
			if (game->grid[y][x] == 2)
				printf("· ");
			else if (game->grid[y][x] == ROUGE)
				printf("X ");
			else if (game->grid[y][x] == JAUNE)
				printf("O ");
		}
		printf("\n");
	}
	for (int i = 0; i < (game->cols * 2 - 1); i++)
		printf("-");
	printf("\n");
	for(int j = 0; j < game->cols; j++)
		printf("%d ", j);
	printf("\n");
}

int		col_height(struct game *game, int col)
{
	int count = 0;

	for (int y = 0; y < game->lines; y++)
	{
		if (game->grid[y][col] == ROUGE || game->grid[y][col] == JAUNE)
			count++;
	}
	return (count);
}

/** Fonction qui permet de placer le jeton du joueur dans la grille dans la colonne sélectionner **/
int		play(struct game *game, int player, int col)
{
	int full = 0;

	if (col < 0 || game->cols < col)
		return (0);
	for (int y = 0; y < game->lines; y++)
	{
		if (game->grid[y][col] == 2)
		{
			game->grid[y][col] = player;
			full = 1;
			break ;
		}
	}
	if (full == 0)
		return (0);
	return (1);
}


/** Fonction qui permet de checker si un joueur à gagner **/
int		player_has_won(struct game *game, int player)
{
	int count = 0; // variable qui permet de compter le nombre de jetons d'affiler

	for (int y = (game->lines - 1); y >= 0; y--) // On fait une boucle qui parcours les lignes du tableau
	{
		count = 0; // on remet count a 0 pour chaque tour de boucle
		for (int x = 0; x < game->cols; x++) // Boucle pour parcourir les colonnes du tableau
		{
			if (game->grid[y][x] == player) // a case du tableau ou l'on se trouve est égale à la variable player
				count++; // On ajoute 1 à count
			else
				count = 0; // Sinon on met count à 0 parce que le jeton n'est pas le même que celui d'avant
			if (count == 4) // Si count = 4 alors ça veut dire qu'un joueur a gagner alors on renvoie 1
				return (1);
		}
	}
	count = 0;
	for (int x = 0; x < game->cols; x++) // Meme boucle qu'au dessus et même fonctionnement sauf que cette fois on va faire la vérification en verticale
	{
		count = 0;
		for (int y = (game->lines - 1); y >= 0; y--)
		{
			if (game->grid[y][x] == player)
				count++;
			else
				count = 0;
			if (count == 4)
				return (1);
		}
	}
	//Ici il faut implémenter la vérification des diagonales
	return (0); // On renvoie 0 si le joueur n'a pas gagné
}

/** Fonction qui check si la grille est remplie **/
int		grid_is_full(struct game *game)
{
	int ret = 0;

	for (int x = 0; x < game->cols; x++) // On fait une boucle pour parcourir chaque colonne
	{
		ret = col_height(game, x); // On appel la fonctin col_height et on stocke son résultat dans ret
		if (ret < game->lines) // Si la valeur de ret est plus petite que le nombre de lignes alors ça veut dire que la grille n'est pas remplie. On renvoie 0
			return (0);
	}
	return (1);
}

/** Fonction principale qui contient une boucle while qui fait tourner la partie
 * tant qu'aucun joueur à gagner ou qu'il n'y a pas match null **/
int		main(void)
{
	struct game *game;
	int player = ROUGE, coup, win = 0, nul = 0; // Variable player sert a savoir quel joueur est entrain de jouer, coup contient le coup qui est joué,
	if ((game = new_game(10, 9)) == NULL)       // Win est un booléen qui passe à 1 quand un joueur à gagner et nul passe à 1 si la grille est remplie
	{
		printf("Malloc error...\n"); //Ici on initialise la grille et on renvoie une erreur si il y a une erreur dans les mallocs et on renvoie -1
		return (-1);
	}
	while (win == 0 && nul == 0) // Boucle qui fait tourner le jeu. Tant que win et nul sont à 0 (Pas de gagnant et la grille pas encore remplie) on continue à jouer
	{
		print_game(game); // On affiche la grille
		if (player == ROUGE)
			printf("Next move for X (0-%d): ", game->cols - 1);
		else
			printf("Next move for O (0-%d): ", game->cols - 1);
		scanf("%d", &coup); // On récupère le coup du joueur qui est saisi au clavier
		while (!play(game, player, coup)) // Boucle qui permet de redemander au joueur de jouer si il joue un coup non valide. ca permet de pas réafficher 
		{
			printf("Not a valid move\n");
			if (player == ROUGE)
				printf("Next move for X (0-%d): ", game->cols - 1);
			else
				printf("Next move for O (0-%d): ", game->cols - 1);
			scanf("%d", &coup);
		}
		if (player_has_won(game, player)) // La on regarde si le joueur à gagner
		{
			print_game(game);
			if (player == ROUGE)
				printf("X won.\n");
			else
				printf("O won.\n");
			win = 1; // Si un joueur gagne on passe win a 1 pour pouvoir quitter la partie principale et mettre fin au jeu
		}
		else if (grid_is_full(game)) // Même principe qu'au dessus
		{
			printf("NULL\n");
			nul = 1;
		}
		if (player == ROUGE) // Le if else permet de changer la variable player pour passer au joueur suivant
				player = JAUNE;
		else
			player = ROUGE;
	}
	printf("End game\n"); // Quand on quitte la boucle on affiche un message et on appelle la fonction free_game pour libérer la mémoire utilisée
	free_game(game);
	return (0);
}