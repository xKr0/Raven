8IAR125 - Rendu Projet Final Raven

Groupe de travail :
CORFA S�bastien  - CORS22119501
CROS Camille   - CROC02529501
DECHAUX Quentin  - DECQ24049507

#Enrichir les r�gles sur le choix d'une arme
Nous avons rajouter plus de pr�cision dans les r�gles pour g�rer la distance (VeryClose, Close, Medium, Far, VeryFar), mais aussi pour le nombre de munition (Loads, Good, Ok, Low).
Ce qui nous a permis d'augmenter consid�rablement le nombre de r�gles pour le rocketLauncher. 

#Gestion d'�quipe
Nous avons rajouter un flag (Blue ou Red) pour identifier l'�quipe appartient le bot, ce qui permet d'�viter les tirs alli�s.
De plus il est possible d'activer dynamique les �quipes en appuyant sur la touche "T". La r�partition des bots et �quilibr�.

#Cache d'arme de l'�quipe
Nous avons rajouter deux points sur la map dans la couleur respective de chaque �quipe (Blue, Red). Ces points repr�sente les caches d'armes dans lesquels les armes
des bots sont d�pos�es lorsque le bot meurt. En outre, nous avons aussi rajout� un but permettant aux bots de l'�quipe de se diriger vers leur cache d'arme si jamais
les conditions sont satisfaisantes. Nous avons aussi g�r� le faite que la cache d'arme de l'�quipe peut �tre vid�e par un autre membre pendant qu�un bot si rendais.

#Ajout d'une nouvelle arme (Knife - arme corps � corps):
Fonctionnalit� assez complexe � mettre en place, car fait appel � des fonctionnalit�s dans tous le projet. De la cr�ation de l'arme sur la map � l'utilisation par un bot en passant par les buts.

#Ajout d'un nouveau spawn d'arme pour le Knife � la position x=190 y=340 (Noeud index=121).

#La modification de la vis�e en ajoutant un comportement flou a �t� fait en prenant compte les param�tres suivants : La distance avec la cible, le temps �coul� depuis que la cible est en vue et la direction par rapport � la cible. Prendre en compte la vitesse du bot est inutile car il se d�place toujours � la m�me vitesse (0.999999).

#Bonus sur la Qualit� du code
Nous avons tenu � respecter la logique d'impl�mentation pr�sent�e, mais aussi � commenter le code ajout�.
