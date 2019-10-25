
#include "page.h"

int Page::IDcount = 0;

Page::Page()
{
    pTime = time(NULL);
    pageID = IDcount;
    IDcount++;
}
Page::Page(Requete* req)
{
    name = req->get_chemin();
    URL  = req->get_chemin();
    pTime= time(NULL);
    pageID = IDcount;
    IDcount++;
}

Page::Page(QString chem)
{
    name = chem;
    URL  = chem;
    pTime= time(NULL);
    pageID = IDcount;
    IDcount++;
}

Page::~Page(){}


QString Page::get_name()
{
    return name;
}

QString Page::get_URL()
{
    return URL;
}

time_t Page::get_page_time()
{
    return pTime;
}

Page::type Page::get_page_type()
{
    return pageType;
}

int Page::get_page_ID()
{
    return pageID;
}

int Page::get_page_taille()
{
    return taille;
}

QByteArray Page::get_page_data()
{
    return data;
}


File::File()
{
    pageType = file;
}

File::File(Requete *req, QFile* fichier) : Page(req)
{
    pageType = file;
    taille = (int)(fichier->size());

    if (!fichier->open(QIODevice::ReadOnly)) //En cas d'echec d'ouverture
    {
        delete fichier;
        req->force_error(500);
        return;
    }

    data = QByteArray(fichier->readAll());
    fichier->close();


}

Directory::Directory()
{
    pageType = directory;
}

Directory::Directory(Requete *req, QDir* rep) : Page(req)
{
    pageType = directory;
    data.append("<html>\n<head>\n<title>\n");
    data.append("Notre Repertoire");
    data.append("</title>\n</head>\n<body>\n");

    for (int i = 0 ; i < rep->entryList().size() ; i++)
    {
        QString c = "./public_html" + req->get_chemin()+ rep->entryList().at(i) + '/';
        QDir d(c);
        data.append("<a href=\""); //Afficher en élément d'ancre
        data.append(rep->entryList().at(i));
        if(d.exists()){ //Si l'élement est un dossier on rajoute "/"
            data.append("/");
        }
        data.append("\">");
        data.append(rep->entryList().at(i));
        data.append("</a>\n<br>\n");
    }
    data.append("</body>\n</html>");
    taille = data.size();
}

Text::Text(QString chem) : Page(chem)
{
    pageType = text;
}

void Text::tpage_title(QString title)
{
    QString h("<html>\n<head>\n<title>");
    h.append(title);
    h.append("</title>\n<body>\n");
    data.append(h);
}



void Text::tpage_end()
{
    QString h("</body>\n</html>");
    data.append(h);
}

void Text::tpage_text_br(QString txt)
{
    data.append(txt);
    data.append("\n<br>\n");
}

void Text::tpage_text_nobr(QString txt)
{
    data.append(txt);
}
void Text::tpage_text_br(QString txt, int val)
{
    data.append(txt + " : ");
    data.append(QString::number(val));
    data.append("\n<br>\n");
}
void Text::tpage_text_nobr(QString txt, int val)
{
    data.append(txt + " : ");
    data.append(QString::number(val) + '\n');

}

void Text::break_line()
{
    data.append("<br>\n");
}
