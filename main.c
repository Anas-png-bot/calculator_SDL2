/*
 * Calculatrice SDL2 en C (sans SDL_ttf : police bitmap integree)
 *
 * Utilisation : souris ou clavier (0-9 . + - * / % = Entree, Retour arriere, Echap = C)
 */

#include <SDL.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN_W 320
#define WIN_H 480
#define MARGIN 10
#define GAP 8
#define ROWS 5
#define COLS 4

#include "status.h"
#include "interface.h"
#include "police_bitmap.h"


int main(int argc, char *argv[]) {
    // Les arguments de la ligne de commande ne sont pas utilisés.
    (void)argc; (void)argv;

    // Initialise le sous-système vidéo de SDL avant toute création de fenêtre.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    // Crée la fenêtre de la calculatrice, centrée à l'écran.
    SDL_Window *win = SDL_CreateWindow("Calculatrice SDL",
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Tente d'abord un rendu accéléré, puis utilise le rendu logiciel si nécessaire.
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    if (!ren) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // Mémorise la touche survolée et contrôle la boucle principale du programme.
    int hover_row = -1, hover_col = -1;
    int running = 1;
    // Active la réception de texte pour gérer les chiffres et les opérateurs saisis.
    SDL_StartTextInput();
    render(ren, hover_row, hover_col);

    SDL_Event e;
    // Attend et traite les événements jusqu'à la fermeture de la fenêtre.
    while (running && SDL_WaitEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                // Une demande de fermeture met fin à la boucle.
                running = 0;
                break;

            case SDL_MOUSEMOTION:
                // Détermine quelle touche est sous le pointeur pour actualiser son survol.
                if (!hit_test(e.motion.x, e.motion.y, &hover_row, &hover_col))
                    hover_row = hover_col = -1;
                break;

            case SDL_MOUSEBUTTONDOWN:
                // Un clic gauche active la touche située aux coordonnées du clic.
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int row, col;
                    if (hit_test(e.button.x, e.button.y, &row, &col))
                        press(LAYOUT[row][col]);
                }
                break;

            case SDL_TEXTINPUT: {
                // Convertit le texte saisi en touche de calculatrice, puis la transmet au calcul.
                char k = map_text(e.text.text);
                if (k) press(k);
                break;
            }

            case SDL_KEYDOWN:
                // Gère les touches du clavier qui ne sont pas envoyées comme texte.
                switch (e.key.keysym.sym) {
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:  press('='); break;
                    case SDLK_BACKSPACE: press('<'); break;
                    case SDLK_ESCAPE:    press('C'); break;
                }
                break;
        }
        // Redessine l'interface après chaque événement pour afficher son nouvel état.
        render(ren, hover_row, hover_col);
    }

    // Arrête la saisie de texte et libère toutes les ressources SDL avant de quitter.
    SDL_StopTextInput();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
