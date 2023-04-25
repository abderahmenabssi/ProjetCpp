#ifndef PATIENTS_H
#define PATIENTS_H
#include <QString>
#include<QSqlQuery>
#include<QSqlQueryModel>


class Patients
{
public:
    Patients();
    Patients(int,QString,QString,QString,QString,int);
    int getCIN();
    QString getnom();
    QString getprenom();
    QString getdate();
    QString getsexe();
    int getNtelephone();
    void setCIN(int);
    void setnom(QString);
    void setprenom(QString);
    void setdate(QString);
    void setsexe(QString);
    void setNtelephone(int);
    bool ajouter();
    QSqlQueryModel* afficher();
    bool rechercher(int);
    bool supprimer(int);
    bool modifier(int, QString, QString, QString , QString ,int );
    Patients obtenirPatient(int);
   /* QSqlQueryModel* chercher(int CIN,bool triCroissant);*/
    QSqlQueryModel*  filtrerPatients(const QString &text);
    QSqlQueryModel* trierNoms();
    QString pourcentageHommesFemmes();
    void enregistrerAjoutHistorique(int);
    void enregistrerModifierHistorique(int);
    void enregistrerSupprimerHistorique(int);
    QSqlQueryModel* afficherH();

private:
    int CIN;
    QString nom , prenom , date , sexe ;
    int Ntelephone;
    bool triCroissant;
};

#endif // PATIENTS_H
