ProjetSI
========

/*----------------------------------------------------------------------------------------
----------------------------- 14/11/2012 : Prise en main
-----------------------------------------------------------------------------------------*/

Reprise en main du projet des SI de l'année dernière.
Dossiers images, include, lib et projet-si.app nécessaire, disponible dans le dossier "Ressources" de la dropbox.
OpenCV nessessaire pour compiler.

/!\ Ne compiles pas !
Documentation inexistant pour certaines classes ou insuffisantes pour d'autres.

Pas de README, n'indique pas les bibliothèques/framework utilisé.
Pas de documentation UML

Code dégueulasse  :
- Ils ont mes des using namespace dans des .h(pp)
- On mis des chemins absolus pour l'édition des liens ( donc code pas portable)
- Des classes avec beaucoup trop de membres (mauvais partage des responsabilités ?)
- Projet non-nettoyés, ils n'ont même pas fait de shadow building
- Ils ont mis des #pragma (donc code pas portable)
- Utilisent un code de google .Inc où les vargs sont "géré" de manière dégueulasse.
- Insertion de jeux de mot dans les commentaires (guéteure; gay t' heureux; cette heure.).
- N'utilisent pas d'enum typé (= nommé) utilisent donc des int à la place.
- Quand on utilise du code d'un autre langage on l'encapsule!
- On tout mis leur code dans un seul dossier

Autres remarques :
- On peut intégrer git à QtCreator
- TIOBE est un classement qui ne veut RIEN dire.
- Une classe héritant d'une classe virtuelle ne doit redifinir les méthodes virtuelles pure en
donnant le corps de la méthode et non en mettant : virtual IplImage ∗ operator ( ) ( IplImage ∗ s o u r c e ) = 0 ;

