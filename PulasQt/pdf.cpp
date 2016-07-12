#include "pdf.h"
#include "poppler/qt5/poppler-qt5.h"
#include <QImage>
#include <QDebug>

Pdf::Pdf(QObject *parent) :
    QObject(parent),
    mDocument(NULL)
{

}

bool Pdf::load(const QString &str)
{
    if(mDocument)
        delete mDocument;

    const QByteArray &ba = QByteArray::fromBase64(str.toUtf8());
    const QByteArray ow;
    const QByteArray uw;
    mDocument = Poppler::Document::loadFromData(ba, ow, uw);
    if(!mDocument && mDocument->isLocked())
        return false;
    mDocument->setRenderBackend(Poppler::Document::ArthurBackend);
    return true;
}

int Pdf::getNumPage()
{
    if(mDocument)
        return mDocument->numPages();
    return 0;
}

QImage Pdf::getImage(int page)
{
    Poppler::Page *p = getPage(page);
    if(p)
        return p->renderToImage();
    return QImage();
}

bool Pdf::paint(int page, QPainter *painter, double res, int x, int y)
{
    Poppler::Page *p = getPage(page);
    if(p)
        return p->renderToPainter(painter, res, res, x, y);
    return false;
}

QSizeF Pdf::getPageSize(int page)
{
    Poppler::Page *p = getPage(page);
    if(p)
        return p->pageSizeF();
    return QSizeF();
}

Poppler::Page *Pdf::getPage(int page)
{
    if(!mDocument)
        return NULL;
    if(page >= getNumPage() || page < 0)
        return NULL;
    return mDocument->page(page);
}
