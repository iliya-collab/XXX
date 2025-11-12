#ifndef _NOTIFICATION_
#define _NOTIFICATION_

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QRect>

#include "Parser/ParserCB.hpp"

#include "NotificationConfig.hpp"

class Notification : public QWidget
{
public:

    static void show(QWidget* parent);
    static void loadStyleSheet(const QString& style);
    static CacheData* cacher;
    
private:

    Notification(QWidget* parent);

    void setupUI(const QList<CurrencyRateCB>& listCurDate, const QList<CurrencyRateCB>& listLastDate);
    void connectionSignals();
    void setupAnimation();

    void show();

    void positionToast();

    QVBoxLayout* layout;
    QTextEdit* text_edit;
    QLabel* label;
    QPushButton* btn;

    static QString styleSheet;

};

#endif
