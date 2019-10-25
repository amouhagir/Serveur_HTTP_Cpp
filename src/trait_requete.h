#ifndef TRAIT_REQUETE_H
#define TRAIT_REQUETE_H
#include <string>
#include <cstdlib>
#include <ctime>

#include <QThread>
#include <QString>
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QtNetwork>
#include <QMessageBox>




class Requete
{
    public:
    enum commands{stats, cache, info, get,deactivate,activate,clear_cache,clear_stats};

    private:
        QString chemin;
        enum commands commande;
        int error;
        time_t rTime;




    public:

        Requete();
        Requete(std::string cmd , QString chem);

        commands get_commande();
        QString get_chemin();
        time_t get_time();
        int get_error();
        void force_error(int err);
        const char* http_code();

};


#endif // TRAIT_REQUETE_H
