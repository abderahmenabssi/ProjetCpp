#ifndef FACTURE_H
#define FACTURE_H
#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>



class facture
{
public:
    facture();                                     //constructeur par défaut
    facture(int,QString,QString,int,int);    //constructeure parametré

    int getnum();
QString getnomp();
QString getdatef();
int getprix_c();
int getprix_m();

void setnum(int);
void setnomp(QString);
void setdatef(QString);
void setprix_c(int);
void setprix_m(int);

bool ajouter();
QSqlQueryModel * afficher();
bool supprimer(int);
bool modifier(int);

private :
QString  nomp,datef;
int num,montant,prix_c,prix_m;
};

#endif // FACTURE_H
