#ifndef _CUSTOM_QDIALOG_
#define _CUSTOM_QDIALOG_

#include <QDialog>
#include <QVBoxLayout>

class CustomQDialog : public QDialog
{
    Q_OBJECT
protected:
    virtual void setupUI() = 0;
    virtual void connectionSignals() = 0;

    QVBoxLayout* layout = nullptr;

public:
    CustomQDialog(QWidget* parent = nullptr) : QDialog(parent) {

    }

    virtual ~CustomQDialog() {
        delete layout;
    }
};


#endif