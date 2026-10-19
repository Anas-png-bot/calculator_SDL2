/* ---------- Police bitmap 5x7 ---------- */
/*
 * Chaque caractère est représenté par une matrice de 5 colonnes sur 7 lignes.
 * Dans chaque valeur de ligne, les cinq bits de poids faible indiquent les
 * pixels à allumer, du bit de poids fort (à gauche) au bit de poids faible.
 */

/* Associe un caractère à ses sept lignes de pixels. */
typedef struct { char c; uint8_t rows[7]; } Glyph;

/* Formes bitmap des caractères utilisés par la calculatrice et ses messages. */
static const Glyph FONT[] = {
    {'0', {0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110}},
    {'1', {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}},
    {'2', {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111}},
    {'3', {0b11110, 0b00001, 0b00001, 0b01110, 0b00001, 0b00001, 0b11110}},
    {'4', {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010}},
    {'5', {0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110}},
    {'6', {0b00110, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110}},
    {'7', {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000}},
    {'8', {0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110}},
    {'9', {0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00010, 0b01100}},
    {'.', {0, 0, 0, 0, 0, 0b01100, 0b01100}},
    {'+', {0, 0b00100, 0b00100, 0b11111, 0b00100, 0b00100, 0}},
    {'-', {0, 0, 0, 0b11111, 0, 0, 0}},
    {'*', {0, 0b10101, 0b01110, 0b11111, 0b01110, 0b10101, 0}},
    {'/', {0b00001, 0b00010, 0b00010, 0b00100, 0b01000, 0b01000, 0b10000}},
    {'%', {0b11001, 0b11010, 0b00010, 0b00100, 0b01000, 0b01011, 0b10011}},
    {'=', {0, 0, 0b11111, 0, 0b11111, 0, 0}},
    {'C', {0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110}},
    {'<', {0b00010, 0b00100, 0b01000, 0b10000, 0b01000, 0b00100, 0b00010}},
    {'~', {0b00100, 0b00100, 0b11111, 0b00100, 0b00100, 0, 0b11111}}, /* +/- */
    {'E', {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111}},
    {'r', {0, 0, 0b10110, 0b11001, 0b10000, 0b10000, 0b10000}},
    {'o', {0, 0, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110}},
};

/* Recherche la définition du caractère dans la police ; NULL s'il est absent. */
static const Glyph *find_glyph(char c) {
    for (size_t i = 0; i < sizeof FONT / sizeof FONT[0]; i++)
        if (FONT[i].c == c) return &FONT[i];
    return NULL;
}

/* Dessine les pixels actifs du glyphe à la position (x, y), agrandis par s. */
static void draw_char(SDL_Renderer *r, char c, int x, int y, int s) {
    const Glyph *g = find_glyph(c);
    if (!g) return;
    for (int row = 0; row < 7; row++)
        for (int col = 0; col < 5; col++)
            /* Teste le bit correspondant à la colonne courante. */
            if (g->rows[row] & (1 << (4 - col))) {
                SDL_Rect px = {x + col * s, y + row * s, s, s};
                SDL_RenderFillRect(r, &px);
            }
}

/* Calcule la largeur d'une chaîne, avec un pixel d'espacement entre glyphes. */
static int text_width(const char *t, int s) {
    int n = (int)strlen(t);
    return n ? n * 6 * s - s : 0;
}
