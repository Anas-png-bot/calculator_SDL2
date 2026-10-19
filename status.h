/* ---------- Etat de la calculatrice ---------- */

/* Valeurs utilisées pour l'affichage et le suivi du calcul en cours. */
static char disp[32] = "0";
static double acc = 0.0;
static char pending_op = 0;
static int new_input = 1;

/* Correspondance entre les caractères des touches et leur position dans la grille. */
static const char LAYOUT[ROWS][COLS] = {
    {'C', '<', '%', '/'},
    {'7', '8', '9', '*'},
    {'4', '5', '6', '-'},
    {'1', '2', '3', '+'},
    {'~', '0', '.', '='},
};

static void set_error(void) {
    /* Affiche une erreur et réinitialise l'opération en cours. */
    strcpy(disp, "Error");
    pending_op = 0;
    acc = 0.0;
    new_input = 1;
}

static void show(double v) {
    /* Évite l'affichage de « -0 », puis formate le résultat dans disp. */
    if (v == 0.0) v = 0.0;
    snprintf(disp, sizeof disp, "%.10g", v);
}

static int compute(double a, double b, char op, double *out) {
    /* Calcule le résultat et signale les opérations impossibles, comme la division par zéro. */
    switch (op) {
        case '+': *out = a + b; return 1;
        case '-': *out = a - b; return 1;
        case '*': *out = a * b; return 1;
        case '/': if (b == 0.0) return 0; *out = a / b; return 1;
        case '%': if (b == 0.0) return 0; *out = fmod(a, b); return 1;
    }
    return 0;
}

static void press(char k) {
    /* Détermine si l'affichage contient une erreur avant de traiter la touche. */
    int is_error = strcmp(disp, "Error") == 0;

    /* Saisie d'un chiffre : commence un nouveau nombre ou complète le nombre courant. */
    if (k >= '0' && k <= '9') {
        if (new_input || strcmp(disp, "0") == 0) {
            disp[0] = k; disp[1] = '\0';
            new_input = 0;
        } else if (strlen(disp) < 12) {
            size_t n = strlen(disp);
            disp[n] = k; disp[n + 1] = '\0';
        }
    } else if (k == '.') {
        /* Ajoute le séparateur décimal s'il n'est pas déjà présent. */
        if (new_input) {
            strcpy(disp, "0.");
            new_input = 0;
        } else if (!strchr(disp, '.') && strlen(disp) < 12) {
            strcat(disp, ".");
        }
    } else if (k == 'C') {
        /* Efface l'affichage et remet à zéro l'état du calculateur. */
        strcpy(disp, "0");
        acc = 0.0; pending_op = 0; new_input = 1;
    } else if (k == '<') {
        /* Supprime le dernier caractère saisi, ou rétablit zéro si nécessaire. */
        if (!new_input) {
            size_t n = strlen(disp);
            if (n > 1 && !(n == 2 && disp[0] == '-')) disp[n - 1] = '\0';
            else strcpy(disp, "0");
        }
    } else if (k == '~') {
        /* Inverse le signe du nombre affiché, sauf s'il vaut zéro ou indique une erreur. */
        if (!is_error && strcmp(disp, "0") != 0) {
            char tmp[32];
            if (disp[0] == '-') snprintf(tmp, sizeof tmp, "%s", disp + 1);
            else snprintf(tmp, sizeof tmp, "-%s", disp);
            snprintf(disp, sizeof disp, "%s", tmp);
        }
    } else if (k == '+' || k == '-' || k == '*' || k == '/' || k == '%') {
        /* Enregistre une opération; calcule d'abord le résultat précédent si besoin. */
        if (is_error) return;
        double cur = atof(disp);
        if (pending_op && !new_input) {
            double res;
            if (!compute(acc, cur, pending_op, &res)) { set_error(); return; }
            acc = res;
            show(acc);
        } else if (!pending_op) {
            acc = cur;
        }
        pending_op = k;
        new_input = 1;
    } else if (k == '=') {
        /* Effectue l'opération en attente lorsque le deuxième nombre est saisi. */
        if (is_error) return;
        if (pending_op && !new_input) {
            double res;
            if (!compute(acc, atof(disp), pending_op, &res)) { set_error(); return; }
            acc = res;
            show(acc);
            pending_op = 0;
            new_input = 1;
        }
    }
}