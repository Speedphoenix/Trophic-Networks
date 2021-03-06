#include "vertex.h"

//pas de problème dans un .cpp
#include "edge.h"
#include "graph.h"

/**
    Ce fichier est un adaptation du code fourni par M. Robin Fercoq
    Adapté par le groupe n°2 du TD09, ING2 PROMO 2021 à l'ECE Paris
        Benoit Coville
        Leonardo Jeanteur
        Louis Galibert

    Merci de citer vos sources
*/




/***************************************************
                    VERTEX
****************************************************/

using namespace std;


Vertex_type int_to_vertex(int what)
{
    switch (what)
    {
        case 1:
        return Vertex_type::Exp;
    break;

        case 2:
        return Vertex_type::Fossil;
    break;

        case 3:
        return Vertex_type::Logistic;
    break;

        default:
        case 0:
        throw "tried making a vertex of unknown type";
    break;

    }
}


int vertex_to_int(Vertex_type what)
{
    switch (what)
    {
        case Vertex_type::Exp:
        return 1;
    break;

        case Vertex_type::Fossil:
        return 2;
    break;

        case Vertex_type::Logistic:
        return 3;
    break;

        default:
        return 0;
    break;

    }
}


/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string name, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, max_value);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,71);
    m_slider_value.set_gravity_x(grman::GravityX::Left);
    m_slider_value.set_posy(9);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( grman::to_string_prec(idx) );

    //Le nom
    m_name.set_message( name );

    m_top_box.add_child( m_box_name );
    m_box_name.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Up);
    m_box_name.set_margin(0);
    m_box_name.set_dim(10 * m_name.get_message().size(), 9);
    m_box_name.set_bg_color(BLANC);

    m_top_box.add_child( m_name );
    m_name.set_gravity_xy(grman::GravityX::Center, grman::GravityY::Up);


    m_top_box.add_child( m_box_label_comp );
    m_box_label_comp.set_gravity_y(grman::GravityY::Down);
    m_box_label_comp.set_dim(20,12);
    m_box_label_comp.set_bg_color(BLANC);

    m_box_label_comp.add_child( m_label_comp);
    m_label_comp.set_message( grman::to_string_prec(-1) );


    m_top_box.add_child( m_selection );
    m_selection.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_selection.set_dim(20, 20);
}



/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (m_interface)
    {
        /// Copier la valeur locale de la donnée m_value vers le slider associé
        m_interface->m_slider_value.set_value(m_value);

        /// Copier la valeur locale de la donnée m_value vers le label sous le slider
        m_interface->m_label_value.set_message( grman::to_string_prec( (int)m_value) );
        m_interface->m_label_value.set_value( m_value );

        m_interface->m_label_comp.set_message( grman::to_string_prec(m_compNum));

        //m_interface->m_selection.set_value(m_selected);
    }
}



/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();

    //si la valeur est en train d'être entrée en texte, on prend celle là
    if (m_interface->m_label_value.is_typing())
        m_value = m_interface->m_label_value.get_value();

    //selectionné ou pas
    m_selected = m_interface->m_selection.get_value();
}



//ci dessous des fonctions pour les tours


void Vertex::turn2(Graph& g, double t)
{
    double k = 0;
    double n = m_value;

    Edge temp;
    double coef_proie = 0;
    double coef_pred = 0;
    double pred_tot = 0;
    double n_proie = 0;
    double n_pred = 0;
    double change = 0;


    ///Actualisation de m_value

    if(m_type == Vertex_type::Logistic)
    {
        ///Ce que notre sommet mange
        for(unsigned i = 0 ; i < m_in.size() ; i++)
        {
            //temps correspond à l'arete entrante
            temp = g.getEdge(m_in[i]);

            ///si l'arete est de type trophique
            //if(temp.m_type == Edge_type::Trophic)
            {
                // k = Coef_herbe->lapin * N_herbe + Coef_carrottes->lapin *N_carrottes
                coef_proie = temp.m_weight;
                n_proie = g.getVertex(temp.m_from).m_value;
                k += coef_proie * n_proie;
            }

        }
    }
    else
    {
        if(m_in.empty())//Sommet exponentiel
            k = 100; ///mettre des valeurs pour chacun ?
        else
        {
            k = 0;
            for(unsigned i = 0; i < m_in.size() ; i++)
            {
                //temps correspond à l'arete entrante
                temp = g.getEdge(m_in[i]);

                if(temp.m_type == Edge_type::Non_Trophic)
                {
                    // k = Coef_herbe->lapin * N_herbe + Coef_carrottes->lapin *N_carrottes
                    coef_proie = temp.m_weight;
                    n_proie = g.getVertex(temp.m_from).m_value;
                    k += coef_proie * n_proie;
                }
                else
                    ;//throw "gros probleme dans Vertex::turn2() ligne 184. i = " + i;
            }
        }

    }


    ///Si notre vertex a des prédateurs
    if(!m_out.empty())
    {
        ///Notre vertex est mangé par ses prédateurs
        for(unsigned i = 0 ; i < m_out.size() ; i++ )
        {
            temp = g.getEdge(m_out[i]);

            ///si l'arête est de type trophique
            if(temp.m_type == Edge_type::Trophic)
            {
                //pred_tot = coef_lapin->renard * n_renard - coef_lapin->loup * n_loup...
                coef_pred = temp.m_weight;
                n_pred = g.getVertex(temp.m_to).m_value;

                //k = k - coef_pred * n_pred;
                pred_tot += coef_pred * n_pred;
            }
        }
    }


    if(k > 0)
    {
        change = t * ( m_r * n *(1 - n / k) - pred_tot * 0.5 ); //0.5 amortissement
        m_value = n + change;
    }
    else
        m_value = 0;

    if(m_value < 0)
        m_value = 0;


    //std::cerr << m_name << " " << m_value << "           k : " << k << std::endl;
}

/*


void Vertex::turn(Graph& g)
{
    double k = 0;
    double n = m_value;

    Edge temp;
    double coef_proie = 0;
    double coef_pred = 0;
    double pred_tot = 0;
    double n_proie = 0;
    double n_pred = 0;

    ///Actualisation de m_value
    if(m_type == Vertex_type::Logistic)
    {
        ///Ce que notre sommet mange
        for(unsigned i = 0 ; i < m_in.size() ; i++)
        {
            //temps correspond à l'arete entrante
            temp = g.getEdge(m_in[i]);

            ///si l'arete est de type trophique
            if(temp.m_type == Edge_type::Trophic)
            {
                // k = Coef_herbe->lapin * N_herbe + Coef_carrottes->lapin *N_carrottes
                coef_proie = temp.m_weight;
                n_proie = g.getVertex(temp.m_from).m_value;
                k += coef_proie * n_proie;
            }
            ///Si l'arete est non trophique
            ///...
        }
    }
    else
        k = 100; ///mettre des valeurs pour chacun ?

    if(k != 0)
        m_value = n + m_r * n *(1 - n / k);
    else
        m_value = 0;

    if(m_value < 0)
        m_value = 0;

    ///Si notre vertex a des prédateurs
    if(!m_out.empty())
    {
        ///Notre vertex est mangé par ses prédateurs
        for(unsigned i = 0 ; i < m_out.size() ; i++ )
        {
            temp = g.getEdge(m_out[i]);

            ///si l'arête est de type trophique
            if(temp.m_type == Edge_type::Trophic)
            {
                //pred_tot = coef_lapin->renard * n_renard - coef_lapin->loup * n_loup...
                coef_pred = temp.m_weight;
                n_pred = g.getVertex(temp.m_to).m_value;

                pred_tot += coef_pred * n_pred;
            }

        }
        m_value = m_value - pred_tot;
    }

    if(m_value < 0)
        m_value = 0;

    std::cout << m_name << " " << m_value << std::endl;

}

void Vertex::turn_dif(Graph& g)
{
    turn_exp(g);
        E(m_name) E(m_value)
    turn_logistic(g);
    std::cerr << std::endl;
}

void Vertex::turn_exp(Graph& g)
{
    if(m_type == Vertex_type::Exp)
    {
        std::cout << m_name << " r : " << m_r << endl;
        double N = m_value;
        double i_non_tro = 0;//Influence non_trophique
        Edge temp;

        for(unsigned i = 0 ; i < m_in.size() ; i ++)
        {
            temp = g.getEdge(m_in[i]);

            if(temp.m_type == Edge_type::Non_Trophic)
            {
                std::cout << "test" << std::endl;
                i_non_tro += g.getVertex(temp.m_from).m_value * temp.m_weight;//Ni * m_weight : nombre d'individu * le nombre d'influence par individu = influence totale sur notre vertex
            }
            if(temp.m_type == Edge_type::Trophic)
                std::cout << "erreur de logique, un sommet de type exp est prédateur" << std::endl;
        }
        //Si on va obtenir un nombre négatif (peut être inutile...)
        if( (N + m_r * N + i_non_tro) < 0 )
            m_value = 0;///alors on met zéro
        else if( (N + m_r * N + i_non_tro) > 100)
            m_value = 100;
        else
            m_value = N + m_r * N + i_non_tro;



    }
}

void Vertex::turn_logistic(Graph& g)
{
    if(m_type == Vertex_type::Logistic)
    {

        double N = m_value;
        double mort = 0;
        double bebe = 0;
        Edge temp;
        //Pour chaque proie on attribue un nbr de proie nécéssaire (m_weight) pour nourire un de nos vertex
        //exemple un renard doit manger un lapin ou 3 souris pour être rassasié
        double nbr_nourrit = 0;

        ///Calcul le nombre d'individu qu'on peut nourrir
        //Pour toutes les proies
        for (unsigned i = 0 ; i < m_in.size() ; i++)
        {
            temp = g.getEdge(m_in.at(i));

            if(temp.m_type == Edge_type::Trophic)
            {
                nbr_nourrit = nbr_nourrit + g.getVertex(temp.m_from).m_value * 0.5 * temp.m_weight;//a cet endroit m_value(voisin) vaut le double de sa valeur supposée
                std::cerr << "voisin " << g.getVertex(temp.m_from).m_value  << " weight " << temp.m_weight << std::endl;
            }
        }

        ///Si on peut en nourrir plus qu'il n'y en a
        if(nbr_nourrit > m_value)
            nbr_nourrit = m_value;//On les nourrit tous

E(nbr_nourrit)

        ///On tue ceux qu'on ne peut pas nourrir
        mort = (m_value - nbr_nourrit);
        m_value -= mort;

        ///Les survivants font des bébés
        bebe =  m_r * m_value; //Ajouter influences non trophiques
        m_value += bebe;

E(bebe)
E(mort)

        //Maintenant on mange les proies
        for(unsigned i = 0 ; i < m_in.size(); i++)
        {
            temp = g.getEdge(m_in[i]);

            if(temp.m_type == Edge_type::Trophic)
            {

                g.getVertex(temp.m_from).m_value -=  nbr_nourrit/ (m_in.size() * temp.m_weight );
            }
        }

    }

}
*/

