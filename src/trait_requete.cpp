#include "trait_requete.h"
#include "stats.h"
#include <QtNetwork>


Requete::Requete()
{
    rTime=time(NULL);
    chemin="None";
    error=200;


}
Requete::Requete(std::string cmd, QString chem)
{
    chemin = chem;
    rTime=time(NULL);
    error=200;

    //commande = cmd;

    if (cmd == std::string("GET"))
    {
        if (chemin == "/private/stats.html")
        {
            commande = stats;
        }
        else if (chemin == "/private/cache.html")
        {
            commande = cache;
        }
        else if (chemin == "/info.html")
        {
            commande = info;
        }
        else if (chemin == "/private/deactivate.html")
        {
            commande = deactivate;
        }
        else if (chemin == "/private/activate.html")
        {
            commande = activate;
        }
        else if (chemin == "/private/clear_cache.html")
        {
            commande = clear_cache;
        }
        else if (chemin == "/private/clear_stats.html")
        {
            commande = clear_stats;
        }
        else {
            commande = get;
        }

    }

}

void Requete::force_error(int err)
{
    error = err;

}


QString Requete::get_chemin()
{
    return chemin;
}

enum Requete::commands Requete::get_commande()
{
    return commande;
}


time_t Requete::get_time()
{
    return rTime;
}

int Requete::get_error()
{
    return error;
}
const char* Requete::http_code()
{

    switch (Requete::get_error()) {
    case 200:
        return "HTTP/1.1 200 OK\n\n";
        break;
    case 404:
        return "HTTP/1.1 404 Not Found\n\n";
        break;
    case 403:
        return "HTTP/1.1 403 Forbidden";
        break;
    default:
        return "HTTP/1.1 500 Internal Server Error\n\n";
        break;
    }

}
