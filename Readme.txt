8IAR125 - Rendu Projet Final Raven

Groupe de travail :
CORFA Sébastien  - CORS22119501
CROS Camille   - CROC02529501
DECHAUX Quentin  - DECQ24049507

#Enrichir les règles sur le choix d'une arme
Nous avons rajouter plus de précision dans les règles pour gérer la distance (VeryClose, Close, Medium, Far, VeryFar), mais aussi pour le nombre de munition (Loads, Good, Ok, Low).
Ce qui nous a permis d'augmenter considérablement le nombre de règles pour le rocketLauncher. 

#Gestion d'équipe
Nous avons rajouter un flag (Blue ou Red) pour identifier l'équipe appartient le bot, ce qui permet d'éviter les tirs alliés.
De plus il est possible d'activer dynamique les équipes en appuyant sur la touche "T". La répartition des bots et équilibré.

#Cache d'arme de l'équipe
Nous avons rajouter deux points sur la map dans la couleur respective de chaque équipe (Blue, Red). Ces points reprèsente les caches d'armes dans lesquels les armes
des bots sont déposées lorsque le bot meurt. En outre, nous avons aussi rajouté un but permettant aux bots de l'équipe de se diriger vers leur cache d'arme si jamais
les conditions sont satisfaisantes. Nous avons aussi géré le faite que la cache d'arme de l'équipe peut être vidée par un autre membre pendant qu’un bot si rendais.

#Ajout d'une nouvelle arme (Knife - arme corps à corps):
Fonctionnalité assez complexe à mettre en place, car fait appel à des fonctionnalités dans tous le projet. De la création de l'arme sur la map à l'utilisation par un bot en passant par les buts.

#Ajout d'un nouveau spawn d'arme pour le Knife à la position x=190 y=340 (Noeud index=121).

#La modification de la visée en ajoutant un comportement flou a été fait en prenant compte les paramètres suivants : La distance avec la cible, le temps écoulé depuis que la cible est en vue et la direction par rapport à la cible. Prendre en compte la vitesse du bot est inutile car il se déplace toujours à la même vitesse (0.999999).

#Bonus sur la Qualité du code
Nous avons tenu à respecter la logique d'implémentation présentée, mais aussi à commenter le code ajouté.
