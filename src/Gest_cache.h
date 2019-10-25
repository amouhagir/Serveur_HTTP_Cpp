#ifndef GEST_CACHE_H
#define GEST_CACHE_H

#include <QString>
//#include <QMap>
#include <QHash>
#include <ctime>

#include "page.h"
#include "stats.h"
#include "trait_requete.h"


class Gest_cache
{
    private :
        static int const memTotale = 20000000;
        int memLibre;
        stats *state;
        QString ancienne;
        QString nouvelle;
        //n-ième page
        struct page_n{ //Cette structure permet de ordonner les pages par anciennté, ainsi que manipuler les flèches -> suivant et <- précédent
            Page* page;
            QString suivante;
            QString derniere;
            ~page_n(){
                delete page;
            }
        };
        QHash<QString, page_n*> hash;

    public :

         Gest_cache();
         Gest_cache(stats *_stats_t);
        ~Gest_cache();
         void vider();
         int free_mem();
         int used_mem();
         void add_page(Page* p, QString chem);
         Page affiche_page(Requete* req);
         Text affiche_cache();


};

#endif // GEST_CACHE_H
