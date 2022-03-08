#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QObject *parent)
    : QObject { parent }
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
                this, SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);

}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}

void FileDownloader::fileDownloaded(QNetworkReply *pReply)
{
    m_DownloadedData = pReply->readAll();
    pReply->deleteLater();
    emit downloaded();
}
