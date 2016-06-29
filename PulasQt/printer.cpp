#include "printer.h"
#include "constant.h"

#include <QPrinterInfo>
#include <QVariant>
#include <QTemporaryFile>
#include <QDebug>
#include <QWebFrame>
#include <QWebPage>
#include <QEventLoop>
#include <QTimer>
#include <windows.h>

Printer::Printer(QObject *parent) :
    QObject(parent),
    mPrinter(new QPrinter(QPrinterInfo::defaultPrinter())),
    mIsPaperCustomSize(false)
{
    mDefaultMargin = 20;
    mPaperSize = QPrinter::A4;
    mUnit = QPrinter::Millimeter;
    mMargin = QMarginsF(mDefaultMargin, mDefaultMargin, mDefaultMargin, mDefaultMargin);
    mOrientation = QPrinter::Portrait;
    mResolution = -1;
    mColorMode = QPrinter::Color;
    mPageOrder = QPrinter::FirstPageFirst;
    initMap();
    applySetting();
}

QVariant Printer::getListPrinter()
{
    QStringList list = QPrinterInfo::availablePrinterNames();
    QVariantList ret;
    foreach (const QString &str, list) {
        ret.append(QVariant(str));
    }
    return QVariant(ret);
}

QVariant Printer::getDefaultPrinterName()
{
    return QVariant(QPrinterInfo::defaultPrinterName());
}

QVariant Printer::setPrinter(const QString &name)
{
    mPrinter->setPrinterName(name);
    return QVariant(true);
}

QVariant Printer::getCurrentPrinter()
{
    return QVariant(mPrinter->printerName());
}

QVariant Printer::settingPrinter(const QVariant &setting)
{
    const QVariantMap &m = qvariant_cast<QVariantMap>(setting);
    //check the paper
    if(m.contains("papersize")) {
        if(m.value("papersize").type() == QVariant::String) {
            mPaperSize = mPaperSizeMap.value(m.value("paper").toString(), QPrinter::A4);
            mIsPaperCustomSize = false;
        } else if(m.value("papersize").canConvert<QVariantMap>()) {
            const QVariantMap &vm = qvariant_cast<QVariantMap>(m.value("papersize"));
            mIsPaperCustomSize = true;
            mPaperCustomSize.setWidth(vm.value("width", QVariant(0.0f)).toFloat());
            mPaperCustomSize.setHeight(vm.value("height", QVariant(0.0f)).toFloat());
        }
    }
    //check unit
    if(m.contains("unit")) {
        mUnit = mUnitMap.value(m.value("unit").toString(), QPrinter::Millimeter);
    }
    //check margin
    if(m.contains("margin")) {
        const QVariantMap &mg = qvariant_cast<QVariantMap>(m.value("margin"));
        mMargin.setTop(mg.value("top", QVariant(mDefaultMargin)).toDouble());
        mMargin.setRight(mg.value("right", QVariant(mDefaultMargin)).toDouble());
        mMargin.setBottom(mg.value("bottom", QVariant(mDefaultMargin)).toDouble());
        mMargin.setLeft(mg.value("left", QVariant(mDefaultMargin)).toDouble());
    }
    //check orientation
    if(m.contains("orientation")) {
        mOrientation = mOrientationMap.value(m.value("orientation").toString(), QPrinter::Portrait);
    }
    //resolution
    if(m.contains("resolution")) {
        mResolution = m.value("resolution").toInt();
    }
    //color mode
    if(m.contains("colormode")) {
        const QString &cm = m.value("colormode").toString();
        if(!cm.compare("grayscale"))
            mColorMode = QPrinter::GrayScale;
        else if(!cm.compare("color"))
            mColorMode = QPrinter::Color;
    }
    //page order
    if(m.contains("pageorder")) {
        const QString &po = m.value("pageorder").toString();
        if(!po.compare("firsttolast"))
            mPageOrder = QPrinter::FirstPageFirst;
        else if(!po.compare("lasttofirst"))
            mPageOrder  = QPrinter::LastPageFirst;
    }
    applySetting();
    return QVariant(true);
}

QVariant Printer::print(const QVariant &data)
{
    const QVariantMap &m = qvariant_cast<QVariantMap>(data);
    int type = HTML;
    if(m.value("printtype").toString().compare("escp") == 0) {
        type = ESCP_COMMAND;
    }
    int output = NATIVE;
    if(m.value("printoutput").toString().compare("pdf") == 0) {
        QTemporaryFile temp;
        if(temp.open())
            mPdfFileName = temp.fileName();
        output = PDF;
        mPrinter->setOutputFormat(QPrinter::PdfFormat);
        mPrinter->setOutputFileName(mPdfFileName);
    }
    if(type == HTML) {
        QTemporaryFile temp;
        QString n;
        if(temp.open()) {
            n = temp.fileName() + ".html";
        }
        QFile f(n);
        if(f.open(QFile::WriteOnly)) {
            QTextStream s(&f);
            s << m.value("data").toString();
            f.close();
        }
        QWebPage wp;
        wp.mainFrame()->load(QUrl("file:///" + n));
        waitLoad(wp.mainFrame());
        wp.mainFrame()->print(mPrinter);
    } else {
        directPrint(m.value("data").toString());
        return QVariant(true);
    }
    if(output == PDF) {
        //read file
        QFile f(mPdfFileName);
        if(f.open(QFile::ReadOnly)) {
            QByteArray ba = f.readAll().toBase64();
            return QVariant(QString(ba));
        }
        return QVariant(false);
    } else {
        return QVariant(true);
    }
}

void Printer::applySetting()
{
    if(mIsPaperCustomSize) {
        if(mPaperCustomSize.height() > 0.0f && mPaperCustomSize.width() > 0.0f)
            mPrinter->setPaperSize(mPaperCustomSize, mUnit);
    } else {
        mPrinter->setPageSize(mPaperSize);
    }
    mPrinter->setPageMargins(mMargin.left(), mMargin.top(), mMargin.right(), mMargin.bottom(), mUnit);
    mPrinter->setOrientation(mOrientation);
    if(mResolution > 0)
        mPrinter->setResolution(mResolution);
    mPrinter->setColorMode(mColorMode);
    mPrinter->setPageOrder(mPageOrder);
}

void Printer::initMap()
{
    mUnitMap.insert("mm", QPrinter::Millimeter);
    mUnitMap.insert("inch", QPrinter::Inch);
    mPaperSizeMap.insert("A4", QPrinter::A4);
    mPaperSizeMap.insert("A5", QPrinter::A5);
    mPaperSizeMap.insert("Letter", QPrinter::Letter);
    mOrientationMap.insert("portrait", QPrinter::Portrait);
    mOrientationMap.insert("landscape", QPrinter::Landscape);
}

bool Printer::waitLoad(QWebFrame *f)
{
    QEventLoop loopLoad;
    QTimer timer;
    timer.setSingleShot(true);
    timer.start(TIMEOUT);
    QObject::connect(f, SIGNAL(loadFinished(bool)), &loopLoad, SLOT(quit()));
    QObject::connect(&timer, SIGNAL(timeout()), &loopLoad, SLOT(quit()));
    loopLoad.exec();
    return true;
}

void Printer::directPrint(const QString &str)
{
    const QString pName = mPrinter->printerName();
    wchar_t printerName[128];
    pName.toWCharArray(printerName);
    printerName[pName.length()] = '\0';
    LPBYTE lpData;
    QByteArray ba = str.toUtf8();
    lpData = (unsigned char*)(ba.data());
    DWORD dwCount = ba.length();

    DOC_INFO_1 docInfo;
    wchar_t docName[16], dataType[8];
    wcscpy_s(docName, 100, L"Pulas Document");
    wcscpy_s(dataType, 100, L"RAW");
    docInfo.pOutputFile = NULL;
    docInfo.pDocName = docName;
    docInfo.pDatatype = dataType;

    BOOL bStatus = FALSE;
    HANDLE hPrinter = NULL;
    DWORD dwPrtJob = 0L;
    DWORD dwBytesWritten = 0L;

    bStatus = OpenPrinterW(printerName, &hPrinter, NULL);
    if(bStatus) {

        dwPrtJob = StartDocPrinterW (
                        hPrinter,
                        1,
                        (LPBYTE)&docInfo);

        if (dwPrtJob > 0) {
                // Send the data to the printer.
                bStatus = WritePrinter (
                hPrinter,
                lpData,
                dwCount,
                &dwBytesWritten);
        }

        EndDocPrinter (hPrinter);

        // Close the printer handle.
        bStatus = ClosePrinter(hPrinter);
    }

    if (!bStatus || (dwCount != dwBytesWritten)) {
        bStatus = FALSE;
    } else {
        bStatus = TRUE;
    }
}
