#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>
#include <QPrinter>
#include <QMap>

class QWebFrame;

class Printer : public QObject
{
    Q_OBJECT
public:
    enum PrintType {
        HTML,
        ESCP_COMMAND
    };
    enum PrintOutput {
        NATIVE,
        PDF
    };

    Printer(QObject *parent = 0);
    QVariant getListPrinter();
    QVariant getDefaultPrinterName();
    QVariant setPrinter(const QString &name);
    QVariant getCurrentPrinter();
    QVariant settingPrinter(const QVariant &setting);
    QVariant print(const QVariant &data);

private:
    qreal mDefaultMargin;
    QString mPdfFileName;
    QPrinter *mPrinter;
    QPrinter::PaperSize mPaperSize;
    QPrinter::Unit mUnit;
    QMarginsF mMargin;
    QPrinter::Orientation mOrientation;
    QMap<QString, QPrinter::Unit> mUnitMap;
    QMap<QString, QPrinter::PaperSize> mPaperSizeMap;
    QMap<QString, QPrinter::Orientation> mOrientationMap;

    void applySetting();
    void initMap();
    bool waitLoad(QWebFrame *f);
    void directPrint(const QString &str);
};

#endif // PRINTER_H
