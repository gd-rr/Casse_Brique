# Mini Casse Brique

Mon projet est un casse brique, le but est de toucher une brique trois fois pour qu'elle disparaisse et ainsi toutes les détruires. 
Pour cela il faut utiliser les flèches gauche et droite du clavier pour faire rebondir à nouveau la bille quand celle-ci redescend.

La bille est une particule avec une vitesse et soumis à la gravité.
La particule rebondis quand les coordonnées de son centre vont se retrouver à l'intérieur du brique ou de la barre de jeu.
Chaque brique commence avec un état de trois et diminue de 1 à chaque collision. Elles sont affichées uniquement si l'état est supérieur à 0.
Pour poursuivre le projet il faudrait utiliser un sdf pour améliorer les collisions et créer des bonus dans certaines briques qui s'activerait 
une fois celle-ci détruite.

## Référence: 
- Un jeu mobile pour avoir une idée de l'esthétique (Bricks n Balls)
- https://www.ronja-tutorials.com/post/034-2d-sdf-basics/
- https://iquilezles.org/articles/distfunctions2d/

Projet réalisé en première année dans le cadre de l'UE LIFAMI.
