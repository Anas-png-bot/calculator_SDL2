/* ---------- Interface ---------- */

/* Calcule la position et la taille d'un bouton à partir de sa ligne et de sa colonne. */
static SDL_Rect button_rect(int row, int col) {
    /* La grille commence sous l'écran d'affichage. */
    int grid_y = 115;
    int bw = (WIN_W - 2 * MARGIN - (COLS - 1) * GAP) / COLS;
    int bh = (WIN_H - grid_y - MARGIN - (ROWS - 1) * GAP) / ROWS;
    SDL_Rect r = {MARGIN + col * (bw + GAP), grid_y + row * (bh + GAP), bw, bh};
    return r;
}

/* Recherche le bouton situé sous le curseur et renseigne sa position dans la grille. */
static int hit_test(int mx, int my, int *row, int *col) {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) {
            SDL_Rect b = button_rect(i, j);
            SDL_Point p = {mx, my};
            /* Le point appartient à ce bouton : renvoie ses coordonnées. */
            if (SDL_PointInRect(&p, &b)) { *row = i; *col = j; return 1; }
        }
    /* Aucun bouton ne se trouve à cet emplacement. */
    return 0;
}

/* Définit la couleur de dessin avec une opacité maximale. */
static void set_color(SDL_Renderer *r, int R, int G, int B) {
    SDL_SetRenderDrawColor(r, R, G, B, 255);
}

/* Dessine l'écran, son contenu et les boutons de la calculatrice. */
static void render(SDL_Renderer *r, int hover_row, int hover_col) {
    /* Efface l'image précédente. */
    set_color(r, 24, 24, 28);
    SDL_RenderClear(r);

    /* Dessine l'écran d'affichage. */
    SDL_Rect screen = {MARGIN, MARGIN, WIN_W - 2 * MARGIN, 90};
    set_color(r, 38, 40, 46);
    SDL_RenderFillRect(r, &screen);

    /* Réduit le texte si nécessaire pour qu'il tienne dans l'écran. */
    int scale = 5;
    while (scale > 1 && text_width(disp, scale) > screen.w - 20) scale--;
    int tw = text_width(disp, scale);
    /* Aligne le texte à droite et le centre verticalement. */
    int tx = screen.x + screen.w - 10 - tw;
    int ty = screen.y + (screen.h - 7 * scale) / 2;
    set_color(r, 240, 240, 240);
    /* Dessine chaque caractère de la valeur affichée. */
    for (int i = 0; disp[i]; i++)
        draw_char(r, disp[i], tx + i * 6 * scale, ty, scale);

    /* Affiche l'opérateur mémorisé en attendant le second nombre. */
    if (pending_op) {
        set_color(r, 255, 159, 10);
        draw_char(r, pending_op, screen.x + 8, screen.y + 8, 2);
    }

    /* Dessine chaque bouton et met en évidence celui survolé. */
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) {
            char k = LAYOUT[i][j];
            SDL_Rect b = button_rect(i, j);
            int hover = (i == hover_row && j == hover_col);
            int d = hover ? 25 : 0;

            /* Choisit une couleur selon le type de bouton. */
            if (k == '=')                                  set_color(r, 48 + d, 170 + d / 2, 90 + d);
            else if (k == 'C')                             set_color(r, 200 + d / 2, 60 + d, 60 + d);
            else if (strchr("+-*/%", k))                   set_color(r, 255, 159 + d / 2, 10 + d);
            else if (k == '<' || k == '~')                 set_color(r, 80 + d, 82 + d, 90 + d);
            else                                           set_color(r, 58 + d, 60 + d, 68 + d);
            SDL_RenderFillRect(r, &b);

            /* Centre le symbole dans le rectangle du bouton. */
            set_color(r, 255, 255, 255);
            int s = 4;
            draw_char(r, k, b.x + (b.w - 5 * s) / 2, b.y + (b.h - 7 * s) / 2, s);
        }

    /* Affiche l'image dessinée. */
    SDL_RenderPresent(r);
}

/* Convertit un caractère saisi en symbole reconnu par la calculatrice. */
static char map_text(const char *t) {
    char c = t[0];
    /* Accepte la virgule comme séparateur décimal et la convertit en point. */
    if (c == ',') return '.';
    /* Accepte les chiffres, le point décimal et les opérateurs. */
    if ((c >= '0' && c <= '9') || strchr(".+-*/%=", c)) return c;
    /* Ignore les caractères non pris en charge. */
    return 0;
}