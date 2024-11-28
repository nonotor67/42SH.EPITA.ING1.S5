#!/bin/bash

check_clang_format() {
    local file="$1"

    if ! diff -q <(clang-format "$file") "$file" > /dev/null 2>&1; then
        echo "Le fichier '$file' ne respecte pas le formatage clang-format."
        return 1
    fi
    return 0
}

check_all_files_in_src() {
    local src_dir="$1"

    # Vérification si le dossier existe
    if [[ ! -d "$src_dir" ]]; then
        echo "Erreur : Le dossier '$src_dir' n'existe pas."
        exit 1
    fi

    # Trouver tous les fichiers dans src (vous pouvez adapter l'extension si nécessaire)
    local files
    files=$(find "$src_dir" -type f -name "*.c" -o -name "*.h")
    flag=0
    for file in $files; do
        if ! check_clang_format "$file"; then
            flag=1
        fi
    done

    if [[ $flag -eq 1 ]]; then
        exit 1
    fi
}

# Appel de la fonction avec le dossier src
check_all_files_in_src "./src"

