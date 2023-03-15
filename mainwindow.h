#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "patients.h"
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


private slots:




    void on_pb_ajouter_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_tabPatient_clicked(const QModelIndex &index);


    void on_pb_valider_clicked();

    void on_pb_annuler_clicked();





private:
    Ui::MainWindow *ui;
    Patients P;
};
#endif // MAINWINDOW_H
