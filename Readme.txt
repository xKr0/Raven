8IAR125 - Rendu Projet Final Raven

Groupe de travail :
CORFA S�bastien 	- CORS22119501
CROS Camille 		- CROC02529501
DECHAUX Quentin 	- DECQ24049507
COX Alexandre		- COXA03069302

#Ajout d'une nouvelle arme (Knife - arme corps � corps):
Fonctionnalit� assez complexe � mettre en place car fait appel � des fonctionnalit�s dans tous le projet. De la cr�ation de l'arme sur la map � l'utilisation par un bot en passant par les buts.
#Ajout d'un nouveau spawn d'arme � la position x=190 y=340 (Noeud index=121).
#Ajout d'un nouveau but atomique, Goal_DodgeOnce, qui est une man�uvre d'esquive dans une direction al�atoire. Lors d'un d�placement vers un item, le bot v�rifie, avant chaque d�placement vers un nouveau n�ud, si un ennemi est visible et ajoute un Goal_DodgeOnce.
#Ajout d'un r�seau de neurone, tir� du livre AI Techniques For Game Programming de Mat Buckland. Contient 1 couche cach� de 6 neurones et un neurone d'output.
->Entrainement du r�seau: Le r�seau est entrain� avec un ex�cutable � l'exterieur de Raven. L'ex�cutable r�cup�re des donn�s de jeu et la r�ponse cons�quente (tirer ou ne pas tirer) � partir dun fichier dataIn.txt et les envoie au r�seau. Le r�seau produit une r�ponse � partir des donn�s de jeu, la compare � la r�ponse attendu (1 ou 0) et ajuste ses poids en cons�quences. Les poids sont r�guli�rement sauvegard� dans un fichier RNweights.txt.
->Utilisation du r�seau dans Raven: On peut cr�er un bot int�ligent, � laquel on fournit une instance d'un r�seau pr�entrain� (touche 'N' au clavier). Lorsqu'un bot est confront� � un ennemeis, les donn�s de jeu sont envoy� au r�seau, puis la r�ponse est r�cup�r� et d�termine si le bot tire ou non.
