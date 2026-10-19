# Calculatrice SDL2

Petite calculatrice graphique écrite en C avec SDL2. L'interface et les caractères
sont dessinés directement avec SDL2 : SDL_ttf n'est pas nécessaire.

## Prérequis

- Windows
- Code::Blocks avec un compilateur MinGW/GCC installé
- SDL2 **Development Libraries pour MinGW**, de la même architecture que le
  compilateur utilisé par Code::Blocks (32 ou 64 bits)

Le paquet SDL2 « Runtime » seul ne suffit pas pour compiler : il faut les
en-têtes et les bibliothèques de développement.

## Installer SDL2

1. Téléchargez l'archive de développement SDL2 pour MinGW depuis le site officiel
   de SDL : <https://www.libsdl.org/>.
2. Décompressez-la dans un emplacement stable, par exemple
   `C:\SDL2\SDL2-devel-2.x.x-mingw`.
3. Repérez dans cette archive :
   - le dossier d'en-têtes `include\SDL2` (il contient `SDL.h`) ;
   - le dossier de bibliothèques de votre architecture, par exemple
     `x86_64-w64-mingw32\lib` ou `i686-w64-mingw32\lib` ;
   - `SDL2.dll`, généralement dans le dossier `bin` de la même architecture.

Les noms exacts des dossiers dépendent de la version de SDL2 téléchargée.

## Configurer Code::Blocks

1. Ouvrez `simple_calculator_SDL.cbp` dans Code::Blocks.
2. Ouvrez **Project > Build options...**. Sélectionnez le projet (ou configurez
   les deux cibles **Debug** et **Release**).
3. Dans **Search directories > Compiler**, ajoutez le chemin complet vers
   `include\SDL2`, par exemple
   `C:\SDL2\SDL2-devel-2.x.x-mingw\include\SDL2`.
   Le code inclut SDL avec `#include <SDL.h>`.
4. Dans **Search directories > Linker**, ajoutez le dossier `lib` correspondant
   à l'architecture de MinGW, par exemple
   `C:\SDL2\SDL2-devel-2.x.x-mingw\x86_64-w64-mingw32\lib`.
5. Dans **Linker settings > Link libraries**, ajoutez, dans cet ordre :
   - `mingw32`
   - `SDL2main`
   - `SDL2`
   - `m`

   Code::Blocks accepte les noms de bibliothèques sans le préfixe `lib` ni
   l'extension `.a`.
6. Choisissez le compilateur MinGW de la même architecture que les bibliothèques
   SDL2 dans **Settings > Compiler**, puis lancez **Build > Build**.

## Fichiers du projet

Pour recréer le projet dans un nouveau dossier, copiez ces fichiers :

- `main.c` : point d'entrée et boucle d'événements ;
- `status.h` : état et opérations de la calculatrice ;
- `interface.h` : disposition et dessin des boutons et de l'écran ;
- `police_bitmap.h` : police bitmap dessinée par le programme.

Vous pouvez aussi copier `simple_calculator_SDL.cbp` pour conserver la
configuration du projet Code::Blocks. Les fichiers `.h` sont inclus depuis
`main.c` ; il n'est pas nécessaire de les ajouter séparément à la compilation.

## Lancer le programme

Après compilation, copiez `SDL2.dll` à côté de l'exécutable généré, par exemple
dans `bin\Debug` ou `bin\Release`. La DLL fournie à la racine du dépôt peut être
utilisée si elle correspond à l'architecture de l'exécutable. Lancez ensuite
`simple_calculator_SDL.exe`.

N'incluez pas les dossiers `obj` ou les exécutables déjà compilés lorsque vous
recréez le projet : Code::Blocks les génère lui-même.

## Utilisation

- Cliquez sur les boutons ou saisissez les chiffres et opérateurs au clavier.
- Opérateurs : `+`, `-`, `*`, `/` et `%` (reste de la division).
- `Entrée` ou `=` : calculer ; `Retour arrière` : effacer le dernier caractère ;
  `Échap` ou `C` : remettre à zéro.
- `.` ou `,` : séparateur décimal ; la touche `~` de l'interface inverse le signe.

---

# SDL2 Calculator

A small graphical calculator written in C with SDL2. The interface and
characters are drawn directly with SDL2, so SDL_ttf is not required.

## Requirements

- Windows
- Code::Blocks with the MinGW/GCC compiler installed
- The SDL2 **Development Libraries for MinGW**, matching the architecture of
  the compiler used by Code::Blocks (32-bit or 64-bit)

The SDL2 Runtime package alone is not sufficient for compiling; the development
headers and libraries are required.

## Installing SDL2

1. Download the SDL2 development archive for MinGW from the official SDL
   website: <https://www.libsdl.org/>.
2. Extract it to a stable location, for example
   `C:\SDL2\SDL2-devel-2.x.x-mingw`.
3. Locate the following in the extracted archive:
   - the headers folder `include\SDL2` (it contains `SDL.h`);
   - the library folder for your architecture, for example
     `x86_64-w64-mingw32\lib` or `i686-w64-mingw32\lib`;
   - `SDL2.dll`, usually in the `bin` folder for the same architecture.

The exact folder names depend on the SDL2 version you download.

## Configuring Code::Blocks

1. Open `simple_calculator_SDL.cbp` in Code::Blocks.
2. Open **Project > Build options...**. Select the project (or configure both
   the **Debug** and **Release** targets).
3. Under **Search directories > Compiler**, add the full path to
   `include\SDL2`, for example
   `C:\SDL2\SDL2-devel-2.x.x-mingw\include\SDL2`.
   The code includes SDL with `#include <SDL.h>`.
4. Under **Search directories > Linker**, add the `lib` folder matching your
   MinGW architecture, for example
   `C:\SDL2\SDL2-devel-2.x.x-mingw\x86_64-w64-mingw32\lib`.
5. Under **Linker settings > Link libraries**, add the following, in this
   order:
   - `mingw32`
   - `SDL2main`
   - `SDL2`
   - `m`

   Code::Blocks accepts library names without the `lib` prefix or the `.a`
   extension.
6. In **Settings > Compiler**, select a MinGW compiler with the same
   architecture as the SDL2 libraries, then choose **Build > Build**.

## Project files

To recreate the project in a new folder, copy these files:

- `main.c`: program entry point and event loop;
- `status.h`: calculator state and operations;
- `interface.h`: layout and drawing of the buttons and display;
- `police_bitmap.h`: bitmap font drawn by the program.

You can also copy `simple_calculator_SDL.cbp` to keep the Code::Blocks project
configuration. The `.h` files are included from `main.c`; they do not need to
be added separately to the build.

## Running the program

After building, copy `SDL2.dll` next to the generated executable, for example
in `bin\Debug` or `bin\Release`. The DLL provided in the repository root can be
used if it matches the executable's architecture. Then run
`simple_calculator_SDL.exe`.

When recreating the project, do not copy the `obj` folders or previously built
executables; Code::Blocks generates them.

## Usage

- Click the buttons or enter numbers and operators using the keyboard.
- Operators: `+`, `-`, `*`, `/`, and `%` (remainder).
- `Enter` or `=`: calculate; `Backspace`: delete the last character;
  `Escape` or `C`: clear.
- `.` or `,`: decimal separator; the `~` button on the interface changes the
  sign.
