#!/bin/sh

# Exécuter autoreconf pour préparer les fichiers de configuration
autoreconf --install

# Configurer le projet avec les options de compilation et de couverture
./configure CFLAGS="-Wall -Wextra -Werror -Wvla -pedantic -std=c99 -coverage -fPIC -I../src -g3 -fsanitize=address" \
            LDFLAGS="-fsanitize=address --coverage" \
            LDLIBS="-lgcov -fsanitize=address"

# Lancer les tests (make check) et ignorer les erreurs
make check || true

# Calculer la couverture de code avec gcovr
gcovr src/ --gcov-ignore-parse-errors
