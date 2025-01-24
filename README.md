# WORDLE

## Contributeurs
- Armand LONDEIX
- Patrick O'BRIEN
- Lucas RIOUX
- Serge TEHE

## Environnement nécessaire
- Visual Studio Code (https://code.visualstudio.com/) ;
- Un terminal (Linux de préférence).

## Lancer le jeu
1. Cloner le projet git en exécutant la commande `git clone https://gitlab.telecomnancy.univ-lorraine.fr/ppii2k22/project2-E21.git` dans votre terminal.
2. Se rendre **dans le sous-dossier `src/`** du projet git cloné. 
Les commandes suivantes doivent toutes être exécutées dans **dans le sous-dossier `src/`** :
3. Créer un environnement virtuel en exécutant la commande `python3 -m venv env` ;
4. Activer cet environnement virtuel en exécutant `source env/bin/activate`.
`(env)` doit s'afficher à gauche de votre terminale comme suit :
`(env) eleve@TNCY-Linux:~$`
5. Télécharger les dépendances nécessaires au fonctionnement du jeu en exécutant `pip install -r requirements.txt` ;
6. Sélectionnez l'application python du jeu en exécutant `FLASK_APP=app.py` ;
7. Lancer l'application en exécutant la commande `flask run`. Bon jeu !

## Arrêter le jeu
Avant de fermer le terminal, désactivez votre environnement virtuel dans avec la commande `deactivate`, peu importe le sous-dossier.

# Solveur WORDLE

## Lancer le solveur
- Se rendre **dans le sous-dossier `Solveur/`**;
- Exécuter les commandes dans l'ordre : **`make clean && make main && ./main`**.
- Suivez les instructions données par le solveur.
