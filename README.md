# Subuteo Evolution

Un jeu de simulation de football de table (Subbuteo) en 2D, développé en **C++17** avec **SFML 3**.

## Aperçu

Deux joueurs s'affrontent en tour par tour. Chaque joueur sélectionne un de ses pions et lui applique une impulsion via un mécanisme de **slingshot** (glisser-relâcher) pour frapper la balle vers le but adverse.

Le projet repose sur un **moteur physique entièrement custom** — aucune bibliothèque physique externe.

---

## Fonctionnalités

- Simulation physique complète : intégration Euler semi-implicite, frottement visqueux, collisions par impulsion
- Détection et résolution de collisions cercle-cercle
- Buts avec ouverture réelle dans les murs (la balle traverse)
- Système de score avec affichage "BUT !" à chaque point
- Indicateur du tour en cours (couleur de l'équipe active)
- Sélection visuelle du joueur (contour jaune)
- Ligne de visée lors du tir
- L'équipe qui concède remet en jeu

---

## Structure du projet

```
├── Vecteur2D.hpp    # Classe mathématique 2D (position, vitesse, forces)
├── Settings.hpp     # Toutes les constantes du jeu (physique, rendu, buts)
├── Objets.hpp       # Classes physiques : Objet, Balle, Joueur, Terrain
├── Physique.hpp     # Moteur physique : frottement, collisions, murs
├── Jeu.hpp          # Logique de jeu : tours, buts, machine à états
├── main.cpp         # Boucle principale, rendu SFML, entrées utilisateur
├── Makefile         # Compilation
├── grass.jpg        # Texture du terrain
└── arial.ttf        # Police d'affichage
```

---

## Prérequis

- **g++** avec support C++17
- **SFML 3** installé sur la machine

### Installation de SFML (macOS)
```bash
brew install sfml
```

### Installation de SFML (Ubuntu/Debian)
```bash
sudo apt-get install libsfml-dev
```

---

## Compilation et lancement

```bash
make        # Compile le projet
./subuteo   # Lance le jeu
make clean  # Supprime l'exécutable
```

---

## Comment jouer

| Action | Description |
|--------|-------------|
| **Clic gauche** sur un pion | Sélectionne le joueur (contour jaune) |
| **Glisser + relâcher** | Tire le joueur dans la direction opposée au glisser |
| Attendre | Le tour change automatiquement quand tout s'arrête |

> Le joueur qui **concède un but** remet en jeu.

---

## Physique

| Paramètre | Valeur | Rôle |
|-----------|--------|------|
| `FRICTION` | 0.7 | Frottement visqueux — ralentissement des objets |
| `RESTITUTION` | 0.7 | Coefficient de rebond (1 = élastique pur) |
| `PUISSANCE_TIR` | 3.0 | Multiplicateur de force au tir |
| `SEUIL_ARRET` | 0.05 m/s | Vitesse en dessous de laquelle un objet est immobile |
| `DT` | 1/60 s | Pas de temps de simulation (60 FPS) |

Tous ces paramètres sont modifiables dans `Settings.hpp`.

---

## Joueurs

| Équipe | Joueur |
|--------|--------|
| Rouge | Leo |
| Bleue | Karim |

---

## Auteurs

Projet réalisé par **Leo**, **Karim** et **Mehdi**.