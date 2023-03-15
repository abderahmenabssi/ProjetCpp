#ifndef PATIENT_H
#define PATIENT_H

#include <QDialog>

namespace Ui {
class Patient;
}

class Patient : public QDialog
{
    Q_OBJECT

public:
    explicit Patient(QWidget *parent = nullptr);
    ~Patient();

private:
    Ui::Patient *ui;
};

#endif // PATIENT_H
