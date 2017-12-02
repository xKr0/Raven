8IAR125 - Rendu Projet Final Raven

Groupe de travail :
CORFA Sébastien 	- CORS22119501
CROS Camille 		- CROC02529501
DECHAUX Quentin 	- DECQ24049507
COX Alexandre		- COXA03069302

#Ajout d'une nouvelle arme (Knife - arme corps à corps):
Fonctionnalité assez complexe à mettre en place, car fait appel à des fonctionnalités dans tous le projet. De la création de l'arme sur la map à l'utilisation par un bot en passant par les buts.
#Ajout d'un nouveau spawn d'arme pour le Knife à la position x=190 y=340 (Noeud index=121)

#Ajout d'un nouveau but atomique, Goal_DodgeOnce, qui est une manœuvre d'esquive dans une 
direction aléatoire. Lors d'un déplacement vers un item, le bot vérifie, avant chaque déplacement vers un nouveau nœud, si un ennemi est visible et ajoute un Goal_DodgeOnce.

#Ajout d'un réseau de neurone, tiré du livre AI Techniques For Game Programming de Mat Buckland. Contient 1 couche caché de 6 neurones et un neurone d'output.
->Entrainement du réseau: Le réseau est entrainé avec un exécutable à l'exterieur de Raven. L'exécutable récupère des donnés de jeu et la réponse conséquente (tirer ou ne pas tirer) à partir dun fichier dataIn.txt et les envoie au réseau. Le réseau produit une réponse à partir des donnés de jeu, la compare à la réponse attendu (1 ou 0) et ajuste ses poids en conséquences. Les poids sont régulièrement sauvegardé dans un fichier RNweights.txt.
->Utilisation du réseau dans Raven: On peut créer un bot intéligent, à laquel on fournit une instance d'un réseau préentrainé (touche 'N' au clavier). Lorsqu'un bot est confronté à un ennemeis, les donnés de jeu sont envoyé au réseau, puis la réponse est récupéré et détermine si le bot tire ou non.


#La modification de la visée en ajoutant un comportement flou a été fait en prenant compte les paramètres suivants : La distance avec la cible, le temps écoulé depuis que la cible est en vue et la direction par rapport à la cible. Prendre en compte la vitesse du bot est inutile car il se déplace toujours à la même vitesse (0.999999).

#Bonus sur la Qualité du code
Nous avons tenu à respecter la logique d'implémentation présentée, mais aussi à commenter le code ajouté.

