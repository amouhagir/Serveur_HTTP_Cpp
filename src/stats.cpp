#include "stats.h"



stats::stats()
{
    m_nbrErreur     = 0;
    m_nbrReqRecu    = 0;
    m_nbrReqTraitee = 0;
    m_nbrOctetsRecu = 0;
    m_nbrOctetsEnv  = 0;
    actif = true;

    list_req = new QVector<Requete>;
    list_clients = new QVector<QString>;

    list_errors[200] = 0;
    list_errors[403] = 0;
    list_errors[404] = 0;
    list_errors[500] = 0;
}

stats::~stats()
{
    delete list_req;
    delete list_clients;
}

Page stats::show_stats()
{
    Text *page;
    page= new Text("./public_html/stats.html");
    page->tpage_title("Statistques");
    page->tpage_text_br("<b>Statistiques du serveur HTTP</b>");
    page->break_line();
    page->tpage_text_br("Nombre de requetes recues", get_nbrReqRecu());
    page->tpage_text_br("Nombre de requetes traitees", get_nbrReqTraitee());
    page->tpage_text_br("Nombre de de clients", get_nbrClients());
    page->tpage_text_br("Taille de donnees recues en octet", get_OctetsRecu());
    page->tpage_text_br("Taille de donnees transmise en octet", get_OctetsEnv());
    page->tpage_text_br("Nombre d'erreurs total", get_nbrErreur());
    page->tpage_text_br("Nombre d'erreurs 200", get_Erreur(200));
    page->tpage_text_br("Nombre d'erreurs 403", get_Erreur(403));
    page->tpage_text_br("Nombre d'erreurs 404", get_Erreur(404));
    page->tpage_text_br("Nombre d'erreurs 500", get_Erreur(500));
    page->break_line();

    page->tpage_text_br("<b>Liste des requetes traitees :</b>");
    page->break_line();
    for (int i=0; i < list_req->size(); i++)
    {
        page->tpage_text_br(show_rq(i));
    }
    page->break_line();

    page->tpage_text_br("<b>Liste des fichiers telecharges :</b>");
    page->break_line();
    QList<QString> files = list_files.keys();
    for (int i=0; i < list_files.size(); i++)
    {
        page->tpage_text_br(show_nbrFile(files.at(i)));
    }
    page->break_line();

    page->tpage_end();

    return *page;
}

QString stats::show_rq(int i)
{
    QString str = "GET ";
    Requete xx = list_req->at(i);
    str.append(xx.get_chemin());
    str.append("\n");
    return str;
}

QString stats::show_nbrFile(QString file)
{
    QString str;
    str.append(file);
    str.append(" est telecharge ");
    str.append(QString::number(list_files.value(file)));
    str.append(" fois");
    str.append("\n");
    return str;
}
void stats::clear_stats()
{
    m_nbrErreur     = 0;
    m_nbrReqRecu    = 0;
    m_nbrReqTraitee = 0;
    m_nbrOctetsRecu = 0;
    m_nbrOctetsEnv  = 0;

    list_req->clear();
    list_clients->clear();
    list_errors.clear();
    list_files.clear();

    list_errors[200] = 0;
    list_errors[403] = 0;
    list_errors[404] = 0;
    list_errors[500] = 0;
}

int stats::get_nbrErreur()
{
    return m_nbrErreur;
}

int stats::get_OctetsEnv()
{
    return m_nbrOctetsEnv;
}

int stats::get_OctetsRecu()
{
    return m_nbrOctetsRecu;
}

int stats::get_nbrClients()
{
    return list_clients->size();
}

int stats::get_nbrReqRecu()
{
    return m_nbrReqRecu;
}

int stats::get_nbrReqTraitee()
{
    return m_nbrReqTraitee;
}

int stats::get_Erreur(int erreur)
{
    return list_errors.value(erreur);
}

void stats::newFile(QString file)
{
    if(list_files.contains(file)){
        list_files[file]++;
    }
    list_files.insert(file, 1);
}

void stats::new_error(int erreur)
{
    list_errors[erreur]++;
    m_nbrErreur++;
}

void stats::new_req(Requete req)
{
    list_req->append(req);
    new_reqTraitee();
}

void stats::new_reqRecu()
{
    m_nbrReqRecu +=1;
}

void stats::new_reqTraitee()
{
    m_nbrReqTraitee++;
}

void stats::new_octetsEnv(int dt_size)
{
    m_nbrOctetsEnv += dt_size;
}

void stats::new_octetsRecu(int dt_size)
{
    m_nbrOctetsRecu += dt_size;
}

void stats::new_client(QString adress)
{
    list_clients->append(adress);
}

void stats::enable()
{
    actif = true;
}

void stats::disable()
{
    actif = false;
}

bool stats::get_etat()
{
    return actif;
}
bool stats::etat_serveur()
{
    return actif;
}

bool stats::connected(QString addr_client)
{
    return list_clients->contains(addr_client);
}
