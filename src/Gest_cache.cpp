#include <string>
#include <iostream>
#include <ctime>

#include <QFile>

#include "Gest_cache.h"



Gest_cache::Gest_cache()
{
    memLibre = memTotale;
    ancienne  = "rien";
    nouvelle  = "rien";

}
Gest_cache::Gest_cache(stats *_stats_t)
{
    memLibre = memTotale;
    ancienne  = "rien";
    nouvelle  = "rien";
    state = _stats_t;

}

Gest_cache::~Gest_cache()
{
  vider();
  delete state;
}

int Gest_cache::free_mem()
{
    return memLibre;
}

int Gest_cache::used_mem()
{
    return memTotale - free_mem();
}

void Gest_cache::vider()
{
    qDeleteAll(hash);
    hash.clear();

    memLibre = memTotale;
    ancienne  = "rien";
    nouvelle  = "rien";
}

void Gest_cache::add_page(Page *p, QString chem)
{
    //Il faut tester au début c'est il y'a assez de place dans la mémoire cache
    //Si ce n'est pas le cas on retire autant de page que nécessaire
    while(free_mem() < p->get_page_taille())
    {
        memLibre += hash.value(ancienne)->page->get_page_taille();
        QString t = ancienne;
        ancienne = hash.value(ancienne)->suivante;

        delete hash.value(t);
        hash.remove(t);
    }
    //Ajout de la page
    struct page_n *nouv_page = new struct page_n;
    nouv_page->suivante = "rien";
    nouv_page->derniere = nouvelle;
    nouv_page->page = p;
    hash.insert(chem,nouv_page);

    //On déclare la page comme la plus récente
    //si c'est la première comme la plus ancienne
    if(hash.contains(nouvelle)){
        hash.value(nouvelle)->suivante = chem;
    }
    if (!hash.contains(ancienne)){
        ancienne = chem;
    }
    nouvelle = chem;
    memLibre -= p->get_page_taille();

}

Page Gest_cache::affiche_page(Requete *req)
{
    QFile   f("./public_html" + req->get_chemin());
    QDir    d("./public_html" + req->get_chemin());
    std::cout << " - isDirectory       : " << d.exists() << std::endl;
    QString chem1 ="./public_html" + req->get_chemin();
    QString chem = req->get_chemin();
    Text *tp,*tp1;

    switch(req->get_commande())
    {
        //Cas d'une page (fichier, répertoire)
        case Requete::get  :

            if( f.exists() == false &&  d.exists() == false)
            {
                // ERREUR 404 LE FICHIER N'EXISTE PAS...
                req->force_error(404);
                state->new_error(404);
                if(hash.contains("ERROR_404")) { //Si la page existe déja en cache
                    return *(hash.value("ERROR_404")->page);
                }else{
                    add_page(new File(req,new QFile("./error_404.html")),"ERROR_404");
                    return *(hash.value("ERROR_404")->page);
                }

            }else if (chem.contains('.') && f.exists() == true)
            {
              //un Fichier
                    state->new_error(200);
                    state->newFile(chem1);
                    if (hash.contains(chem1)){
                        return *(hash.value(chem1)->page);
                    }else if (f.size() < memTotale){
                        add_page(new File(req,&f), chem1);
                        return *(hash.value(chem1)->page);
                    }else{
                        return File(req,&f);
                    }

            }else if (d.exists() == true)
            {
              //Un Répertoire
                state->new_error(200);
                if (hash.contains(chem1)){
                    return *(hash.value(chem1)->page);

                }else{
                    add_page(new Directory(req,&d),"public_html"+req->get_chemin());
                    return *(hash.value( "public_html"+req->get_chemin())->page);
                }
            }
        break;

    case Requete::info  :


        state->new_error(200);
        if (hash.contains(chem1)){
            return *(hash.value(chem1)->page);
        }else{

            tp= new Text(chem1);
            tp->tpage_title("Welcome");

            tp->tpage_text_br("<b>Bienvenue sur notre serveur local</b>");
            tp->break_line();

            tp->tpage_text_nobr("Ce serveur a ete cree le ");
            tp->tpage_text_nobr(__DATE__);
            tp->tpage_text_nobr(" a ");
            tp->tpage_text_nobr(__TIME__);

            tp->break_line();
            tp->break_line();

            tp->tpage_text_br("Vous pouvez consulter :");
            tp->break_line();
            tp->tpage_text_br("<a href=\"/private/stats.html\">Statistiques du serveur</a>");
            tp->tpage_text_br("<a href=\"/private/cache.html\">Etat du cache</a>");
            tp->tpage_text_br("<a href=\"/\">Retour vers la racine</a>");

            tp->break_line();
            tp->tpage_text_br("Made By MOUHAGIR & FERIAT");
            tp->tpage_end();
            // std::cout << "taille = " << tp->get_page_taille() << std::endl;
            add_page(tp,chem1);
            return *(hash.value(chem1)->page);

        }

    break;

    case Requete::stats  :
        state->new_error(200);
        return state->show_stats();
    break;
    case Requete::clear_stats  :
        state->new_error(200);
        tp= new Text(chem1);
        tp->tpage_title("Stats cleared !");
        tp->tpage_text_br("<b>Stats Cleared Successfully</b>");
        tp->break_line();
        tp->tpage_end();
        state->clear_stats();
        return *tp;
    break;

    case Requete::cache :
        state->new_error(200);
        return affiche_cache();
    break;
    case Requete::clear_cache  :
        state->new_error(200);
        vider();
        tp= new Text(chem1);
        tp->tpage_title("Cache cleared !");
        tp->tpage_text_br("<b>Cache Cleared Successfully</b>");
        tp->break_line();
        tp->tpage_end();
        return *tp;
    break;

    case Requete::activate  :
        state->new_error(200);
        state->enable();
        tp= new Text(chem1);
        tp->tpage_title("Server activate");
        tp->tpage_text_br("<b>Server response activated Successfully</b>");
        tp->break_line();
        tp->tpage_end();
        return *tp;

    break;


    case Requete::deactivate  :
        state->disable();
        req->force_error(500);
        state->new_error(500);

        if(hash.contains("ERROR_500")) { //Si la page existe déja en cache
            return *(hash.value("ERROR_500")->page);
        }else{
            tp1= new Text(chem1);
            tp1->tpage_title("Server response deactivate");
            tp1->tpage_text_br("<b>Server response is deactivated</b>");
            tp1->tpage_text_nobr("<b>Please reactivate it entering this command : </b>");
            tp1->tpage_text_br("<a href=\"/private/activate.html\">Server response reactivation</a>");
            tp1->break_line();
            tp1->tpage_end();
            add_page(tp1,"ERROR_500");
            return *(hash.value("ERROR_500")->page);
        }

    break;

    default:
        state->new_error(200);
        return Text(chem);
    }
}

Text Gest_cache::affiche_cache()
{
    Text tp("./public_html/cache.html");
    tp.tpage_title("Cache Info");

    tp.tpage_text_br("<b>Memoire Cache :</b>");
    tp.break_line();
    tp.break_line();

    tp.tpage_text_br("Nombre de pages dans le cache",hash.size());
    tp.tpage_text_nobr("Memoire utilisee",used_mem());
    tp.tpage_text_br(" octets");
    tp.tpage_text_nobr("Memoire libre",free_mem());
    tp.tpage_text_br(" octets");

    tp.break_line();
    tp.break_line();

    tp.tpage_text_br("<b>Pages en cache en ordre chronologique </b>");
    tp.tpage_text_nobr("<div style=\"height:300; overflow: auto;\">");
            QString pc = nouvelle;
            while (pc != ancienne)
            {
                tp.tpage_text_br(pc);
                pc = hash.value(pc)->derniere;
            }
            tp.tpage_text_nobr(pc);

    tp.tpage_text_nobr("</div>");
    tp.tpage_end();
    return tp;
}
