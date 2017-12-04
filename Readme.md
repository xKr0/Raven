# 8IAR125 - Rendu Projet Final Raven

### Groupe de travail :
CORFA Sébastien: CORS22119501
-- CROS Camille: CROC02529501
-- DECHAUX Quentin: DECQ24049507
-- COX Alexandre: COXA03069302
-- GUILLET Natacha: GUIN14619507
-- BOCHÉ Dorian: BOCD27119509

## Ajout d'une nouvelle arme (Knife - arme corps à corps)
Fonctionnalité assez complexe à mettre en place, car fait appel à des fonctionnalités dans tout le projet. De la création de l'arme sur la map à l'utilisation par un bot en passant par les buts.

## Ajout d'un nouveau spawn d'arme
Ajout d'un nouveau spawn d'arme pour le Knife à la position x=190 y=340 (Noeud index=121)

## Ajout d'un nouveau but atomique, Goal_DodgeOnce
Ajout d'un nouveau but atomique, Goal_DodgeOnce, qui est une manœuvre d'esquive dans une direction aléatoire. Lors d'un déplacement vers un item, le bot vérifie, avant chaque déplacement vers un nouveau nœud, si un ennemi est visible et ajoute un Goal_DodgeOnce.

## Enrichir les règles sur le choix d'une arme
Nous avons rajouté plus de précision dans les règles pour gérer, avec plus de finesse, la distance (VeryClose, Close, Medium, Far, VeryFar), mais aussi pour le nombre de munition (Loads, Good, Ok, Low).
Ce qui nous a permis d'augmenter considérablement le nombre de règles de FuzzyLogic pour le RocketLauncher.

## Gestion d'équipe
Nous avons rajouté un flag (Blue ou Red) pour identifier à quelle équipe appartient le bot, ce qui permet d'éviter les tirs alliés.
De plus, il est possible d'activer (ou désactiver) dynamiquement les équipes et les comportements associés en appuyant sur la touche "T".
La répartition des bots est équilibré (et le reste même s'il on ajoute ou supprime des bots).

## Cache d'arme de l'équipe
Nous avons rajouté deux points sur la map dans la couleur respective de chaque équipe (Blue, Red). Ces points représentent les caches d'armes dans lesquelles les armes
des bots sont déposées lorsque ces derniers meurent. En outre, nous avons aussi rajouté un but composite (Goal_GoToCache) permettant aux bots de l'équipe de se diriger vers leur cache d'arme si jamais
les conditions sont satisfaisantes (grâce à un goal evaluator spécifique, GoToCacheGoal_Evaluator). Nous avons aussi géré le fait que la cache d'arme de l'équipe peut être vidée par un autre membre pendant qu’un bot si rende.

## Ajout d'un réseau de neurone
Réseau de neurone, tiré du livre AI Techniques For Game Programming de Mat Buckland.Contient 1 couche cachée de 6 neurones et un neurone d'output.

#### Entrainement du réseau:
Le réseau est entrainé avec un exécutable à l'exterieur de Raven. L'exécutable récupère des données de jeu et la réponse conséquente (tirer ou ne pas tirer) à partir d'un fichier dataIn.txt et les envoie au réseau. Le réseau produit une réponse à partir des données de jeu, la compare à la réponse attendu (1 ou 0) et ajuste ses poids en conséquence. Les poids sont régulièrement sauvegardés dans un fichier RNweights.txt.
#### Utilisation du réseau dans Raven:
On peut créer un bot intelligent, auquel on fournit une instance d'un réseau pré-entrainé (touche 'N' au clavier). Lorsqu'un bot est confronté à un ennemei, les données de jeu sont envoyées au réseau, puis la réponse est récupérée et détermine si le bot tire ou non.

## La modification de la visée
La modification de la visée en ajoutant un comportement flou a été fait en prenant compte les paramètres suivants : La distance avec la cible, le temps écoulé depuis que la cible est en vue et la direction par rapport à la cible. Prendre en compte la vitesse du bot est inutile car il se déplace toujours à la même vitesse (0.999999).

## Control d'un bot par un joueur
L'utilisateur peut appuyer sur 'P' pour posséder un bot. Le deplacement s'effectue avec les touches WASD, la visée avec la souris et le tir avec un clic gauche.

## Stratégie d'attaque guidé par le leader
Lorsque les équipes sont activées on peux choisir dans le menu la stratégie guidée par un leader: 'Follow leader'. Chaque équipe à un leader que les autres membres de l'équipe vont suivre lorsqu'ils l'aperçoivent grâce au but Goal_FollowLeader ce qui va permettre de créer une cohésion logique dans l'équipe.

## Bonus sur la Qualité du code
Nous avons tenu à respecter la logique d'implémentation présentée, mais aussi à commenter le code ajouté quand cela était nécessaire.
