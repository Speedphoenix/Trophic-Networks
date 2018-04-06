#include "grman/grman.h"
#include <iostream>

#include "graph.h"

#define GRAPHFILE1 "graphe1.txt"
#define GRAPHFILE2 "graphe2.txt"

using namespace std;

int main()
{
    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");
    //mis avant init pour utiliser la souris

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Un exemple de graphe. pris depuis le fichier GRAPHEFILE1
    Graph g(GRAPHFILE1);
    //g.make_test1();


//    //sauvegarde dans le nouveau fichier
//    ofstream myFile1(GRAPHFILE1, ios::out | ios::trunc);
//
//    myFile1 << g;
//    myFile1.close();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] && !g.get_quit())
    {

        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();


        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

        if( grman::key_press[KEY_SPACE] )
            g.turn();
    }

    ///NE PAS �CRIRE DANS LE GRAPHEFILE1!! (on veut garder une instance de ce fichier propre)
    //sauvegarde dans le nouveau fichier
    ofstream myFile2(GRAPHFILE2, ios::out | ios::trunc);

    myFile2 << g;
    myFile2.close();

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();



