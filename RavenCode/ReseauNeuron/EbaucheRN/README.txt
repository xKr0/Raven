Programme stand alone, modifications apporter a CData pour fetch les donn�es
dans data.txt, CControler pour �tre utilis� en Console simple, d�fine.h pour
modifier le nombre d'inputs/outputs attendu, NeuralNet pour prendres le nouveau
nombres d'entr�e. Le reste est l� juste parce que je voulais pas v�rifier s'ils
�taient n�cessaires ou non.

main.cpp simple pour le faire marcher dans une console simple.

Tirer de CData.cpp:
//--------------------------------- Init ---------------------------------
//
//  Fetch entries from data.txt and initialize Input/Output vectors.
//
//	Chaques lignes contiennent 4 input + l'output obtenu (bot avait tirer ou non).
//	Les 4 input sont mis dans un vector, puis push_back dans SetIn. 
//	L'output est mis dans un vector, puis push_back dans SetOut.
//
//	Le r�seau va prendre les inputs, produire un output, puis comparer
//	cette output avec celui dans SetOut (l'output qu'il aurais du obtenir) et
//	procede � l'ajustement des poids du r�seau.
//	
//	Yadayada calcul d'erreur, continue d'ajuster les poids jusqu'� ce que
//	l'erreur soit dessendu � 0.003 (Il est tard)
//------------------------------------------------------------------------

UPDATE 27/11/17

Ajout du Momentum au calcul des poids (de Recognizeit 2.0), ajout options d'affichage de donn�s � l'�crans:
//#define DISPLAY_DESIRED_VS_OBTAINED  "ON"
� d�commenter pour afficher l'output d�sirer comparer � l'output donn� par le r�seau de neurone.

//#define DISPLAY_TO_CONSOLE "ON"
� d�commenter pour afficher des informations tel que l'output, l'erreur, les poids et etc par neurones, par couches. (ATTENTION, beaucoup de donn�s � afficher, ralenti le calcul)