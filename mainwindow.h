#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include "filedownloader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct WebImageView{
    QUrl url;
    QGraphicsScene *graphicsScene;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_loadImage(QGraphicsScene*, const FileDownloader*);

    void on_loadPage(const FileDownloader*);

private:
    Ui::MainWindow *ui;

    QVector<WebImageView> m_webImageViewItems;

    void getImage(const WebImageView&);
    void getPage(const QString&);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;


};
#endif // MAINWINDOW_H
