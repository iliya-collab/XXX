#include "Notification/Notification.hpp"

CacheData* Notification::cacher = nullptr;

QString Notification::styleSheet = "";

Notification::Notification(QWidget* parent) : QWidget(parent) {

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(styleSheet);

    auto listCacheFiles = cacher->get_cache_files();
    auto cache_cur_date = listCacheFiles[0];
    auto cache_last_date = listCacheFiles[1];

    QList<CurrencyRateCB> clist;
    QList<CurrencyRateCB> llist;

    const ssize_t szStruct = sizeof(CurrencyRateCB);
    const int szList = cache_cur_date.sz / szStruct;

    char buf[szStruct] = "\0";
    clist.resize(szList);

    lseek(cache_cur_date.fd, 0, SEEK_SET);
    for (auto& i : clist) {
        cacher->read_cache_data(cache_cur_date.fd, buf, szStruct);
        ParserCB::deserializeCurrencyRate(QByteArray(buf, szStruct), i);
    }

    llist.resize(szList);

    lseek(cache_last_date.fd, 0, SEEK_SET);
    for (auto& i : llist) {
        cacher->read_cache_data(cache_last_date.fd, buf, szStruct);
        ParserCB::deserializeCurrencyRate(QByteArray(buf, szStruct), i);
    }

    setupUI(clist, llist);
    connectionSignals();
    setupAnimation();

}

void Notification::setupUI(const QList<CurrencyRateCB>& listCurDate, const QList<CurrencyRateCB>& listLastDate) {

    label = new QLabel(NotificationConfig::getConfig().title, this);
    label->setWordWrap(true);

    text_edit = new QTextEdit(this);
    text_edit->setReadOnly(true);

    for (int i = 0; i < listCurDate.size() && i < listLastDate.size(); i++) {
        double currentValue = listCurDate[i].Value;
        double lastValue = listLastDate[i].Value;
        double difference = currentValue - lastValue;
        
        QString trend = (difference > 0) ? "↑" : (difference < 0) ? "↓" : "→";
        QString colorStyle = (difference > 0) ? "color: green;" : (difference < 0) ? "color: red;" : "";
        
        QString currencyInfo = QString("<b>%1</b>: %2 %3 <span style='%4'>(%5%6)</span>")
            .arg(listCurDate[i].CharCode)
            .arg(currentValue, 0, 'f', 4)
            .arg(trend)
            .arg(colorStyle)
            .arg(difference > 0 ? "+" : "")
            .arg(difference, 0, 'f', 4);
        
        text_edit->append(currencyInfo);
    }

    QHBoxLayout* row = new QHBoxLayout;
    btn = new QPushButton(this);
    btn->setText("OK");
    
    row->addWidget(btn);
    row->addStretch();
    row->addStretch();

    layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(text_edit);
    layout->addLayout(row);

    auto shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 4);

    label->setGraphicsEffect(shadow);
    text_edit->setGraphicsEffect(shadow);
    btn->setGraphicsEffect(shadow);

    adjustSize();
}

void Notification::connectionSignals() {
    connect(btn, &QPushButton::clicked, this, &Notification::deleteLater);
}

void Notification::setupAnimation() {
    QPropertyAnimation* showAnim = new QPropertyAnimation(this, "windowOpacity");
    showAnim->setDuration(500);
    showAnim->setStartValue(0);
    showAnim->setEndValue(1);

    QPropertyAnimation* hideAnim = new QPropertyAnimation(this, "windowOpacity");
    hideAnim->setDuration(500);
    hideAnim->setStartValue(1);
    hideAnim->setEndValue(0);

    connect(showAnim, &QPropertyAnimation::finished, [this, hideAnim]() {
        QTimer::singleShot(NotificationConfig::getConfig().duration, [this, hideAnim]() {
            hideAnim->start();
        });
    });
    connect(hideAnim, &QPropertyAnimation::finished, this, &Notification::deleteLater);

    showAnim->start();
}

void Notification::show() {
    QWidget::show();
    positionToast();
}

void Notification::positionToast() {
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    move(screenGeometry.width() - width() - 20, screenGeometry.height() - height() - 100);
}


void Notification::show(QWidget* parent) {
    if (NotificationConfig::getConfig().enable) {
        Notification* notif = new Notification(parent);
        notif->show();
    }
}

void Notification::loadStyleSheet(const QString& style) {
    styleSheet = style;
}
