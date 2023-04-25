#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "patients.h"
#include"facture.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
      void reactivateModifierButton();
      void afficherStats();
      bool statsAdded = false;
      void afficherStatsAge();
      bool statsAgeAdded;
      void ecrirefichier();
      void readfile();






private slots:




    void on_pb_ajouter_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_tabPatient_clicked(const QModelIndex &index);


    void on_pb_valider_clicked();

    void on_pb_annuler_clicked();





    /*void on_pb_chercher_clicked();*/



    /*void on_pb_tri_clicked(); */

    void on_pb_stat_clicked();



    void on_pb_pdf_clicked();

    void on_le_cherche_textChanged(const QString &arg1);
     void on_RB_CIN_clicked();
     void on_RB_PRENOM_clicked();
     void on_RB_NOM_clicked();
     void on_pb_historique_clicked();
     void on_pb_ajouterF_clicked();
     void on_pb_genererF_clicked();
    /* void on_readfile_clicked();*/




private:
    Ui::MainWindow *ui;
    Patients P;
    facture F;

};
#endif // MAINWINDOW_H
