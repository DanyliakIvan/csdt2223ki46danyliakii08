#ifndef LAB_2_H
#define LAB_2_H

#endif // LAB_2_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTextBrowser>

#include <QString>

class CSDT : public QWidget {

  public:
    CSDT(QWidget *parent = 0);
    QLabel *label1, *label2;
    QLineEdit *textWrite;
    QPushButton *sendButton, *openButton, *closeButton;
    QComboBox *comboBox;
    QTextBrowser *textShow;

private slots:
    void OnSendPressed();
    void OnOpenPressed();
    void OnClosePressed();
};
