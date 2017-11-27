Programme stand alone, modifications apporter a CData pour fetch les données
dans data.txt, CControler pour être utilisé en Console simple, défine.h pour
modifier le nombre d'inputs/outputs attendu, NeuralNet pour prendres le nouveau
nombres d'entrée. Le reste est là juste parce que je voulais pas vérifier s'ils
étaient nécessaires ou non.

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
//	Le réseau va prendre les inputs, produire un output, puis comparer
//	cette output avec celui dans SetOut (l'output qu'il aurais du obtenir) et
//	procede à l'ajustement des poids du réseau.
//	
//	Yadayada calcul d'erreur, continue d'ajuster les poids jusqu'à ce que
//	l'erreur soit dessendu à 0.003 (Il est tard)
//------------------------------------------------------------------------