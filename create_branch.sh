#!/bin/bash

# Nom de la nouvelle branche
BRANCH_NAME=$1

if [ -z "$BRANCH_NAME" ]; then
  echo "Le nom de la branche ne peut pas être vide."
  exit 1
fi

# Vérifier si la branche existe déjà
if git show-ref --verify --quiet "refs/heads/$BRANCH_NAME"; then
  echo "La branche $BRANCH_NAME existe déjà."
  exit 1
fi

# Vérifier si la branche a bien pour nom de format SH-XXX
if [[ ! "$BRANCH_NAME" =~ ^SH-[0-9]+$ ]]; then
  echo "Le nom de la branche doit être de la forme SH-XXX."
  exit 1
fi

USER=$2
# Vérifier si l'utilisateur a bien été renseigné
if [ -z "$USER" ]; then
  echo "Usage: ./create_branch SH-XX a/y/j/n."
  exit 1
fi

if [ "$USER" != "a" ] && [ "$USER" != "y" ] && [ "$USER" != "j" ] && [ "$USER" != "n" ]; then
  echo "Usage: ./create_branch SH-XX a/y/j/n."
  exit 1
fi

# Vérification de l'existence de la branche main
git fetch origin
git checkout main

# Création de la nouvelle branche à partir de main
git checkout -b "$BRANCH_NAME"
git push origin "$BRANCH_NAME"

GIT_API_URL="https://gitlab.cri.epita.fr/api/v4/projects/22408/merge_requests"

# https://docs.gitlab.com/ee/user/profile/personal_access_tokens.html
TOKEN="YOUR_TOKEN"

antoine=7744
jules=6277
yann=6675
noah=6076

if [ "$USER" == "a" ]; then
  USER=$antoine
elif [ "$USER" == "y" ]; then
  USER=$yann
elif [ "$USER" == "n" ]; then
  USER=$noah
else
  USER=$jules
fi

# Création de la merge request vers la branche develop
curl --request POST "$GIT_API_URL" --header "PRIVATE-TOKEN: $TOKEN" \
  --data "source_branch=$BRANCH_NAME&target_branch=develop&title=Draft: DEV: $BRANCH_NAME&assignee_id=$jules"

# Création de la merge request vers la branche main
curl --request POST "$GIT_API_URL" --header "PRIVATE-TOKEN: $TOKEN" \
  --data "source_branch=$BRANCH_NAME&target_branch=main&title=Draft: PROD: $BRANCH_NAME&assignee_id=$USER"
