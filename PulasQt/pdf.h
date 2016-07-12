#ifndef PDF_H
#define PDF_H

#include <QObject>

class QPainter;
namespace Poppler {
    class Document;
    class Page;
}

class Pdf : public QObject
{
    Q_OBJECT
public:
    Pdf(QObject *parent = 0);
    bool load(const QString &str);
    int getNumPage();
    QImage getImage(int page);
    bool paint(int page, QPainter *painter, double res, int x, int y);
    QSizeF getPageSize(int page);

private:
    Poppler::Document *mDocument;

    Poppler::Page *getPage(int page);
};

#endif // PDF_H
