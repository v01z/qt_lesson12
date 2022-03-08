#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i{}; i < 3; ++i) // 3 - we need only three webImageView objects
    {
        m_webImageViewItems.push_back( WebImageView{ QUrl(""), new QGraphicsScene(this) } );
        m_webImageViewItems[i].graphicsScene->addText("Empty");
    }

    ui->graphicsView1->setScene(m_webImageViewItems.at(0).graphicsScene);
    ui->graphicsView2->setScene(m_webImageViewItems.at(1).graphicsScene);
    ui->graphicsView3->setScene(m_webImageViewItems.at(2).graphicsScene);

    //Для перехвата нажатия 'Return'
    ui->plainTextEdit->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_2_clicked()
{
    QString wordToSearch { ui->plainTextEdit->toPlainText() };
    if (wordToSearch.isEmpty())
    {
        ui->statusbar->showMessage("Empty string");
        return;
    }

    getPage(wordToSearch);
    ui->plainTextEdit->clear();
    ui->statusbar->showMessage("Search query: \'" + wordToSearch + "\'");
}

void MainWindow::on_loadImage(QGraphicsScene *scene, const FileDownloader *file_downloader)
{
    QPixmap pixMap;
    pixMap.loadFromData(file_downloader->downloadedData());

    scene->clear();

    scene->addPixmap(pixMap);

    if (scene == m_webImageViewItems[0].graphicsScene)
        ui->graphicsView1->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    else if (scene == m_webImageViewItems[1].graphicsScene)
        ui->graphicsView2->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    else
        ui->graphicsView3->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_loadPage(const FileDownloader *file_dowloader)
{

    QString pageAsString  { file_dowloader->downloadedData() };

    const int lengthOfLeftJunk { 7 };
    int currentPageIndex{};
    int currentWebImageViewIndex{m_webImageViewItems.size() - 1 };

    while (currentWebImageViewIndex >= 0 &&
           (currentPageIndex = pageAsString.indexOf("\"url\":\"https://", currentPageIndex)) != -1)
    {
        int indexAtExtension { pageAsString.indexOf(".jpg", currentPageIndex + lengthOfLeftJunk) };
        if (indexAtExtension != -1)
        {
            QString tempStr { pageAsString.mid(currentPageIndex + lengthOfLeftJunk,
                                               indexAtExtension - currentPageIndex - 3)}; //3 - length of junk at the right

            QUrl imageUrl { tempStr };

            if (imageUrl.isValid() && tempStr.length() < 100) //Не будем ходить по длинным линкам
            {
                m_webImageViewItems[currentWebImageViewIndex].url = imageUrl;
                currentWebImageViewIndex--;
            }
        }

        currentPageIndex++;
    }

    for (const auto &elem : m_webImageViewItems)
    {
        getImage(elem);
    }

}

void MainWindow::getImage(const WebImageView &webImageView)
{

    FileDownloader *fileDownloader = new FileDownloader ( webImageView.url, this );

    connect(fileDownloader, &FileDownloader::downloaded,
            std::bind (&MainWindow::on_loadImage, this, webImageView.graphicsScene, fileDownloader));
}

void MainWindow::getPage(const QString &word)
{

    FileDownloader *fileDownloader = new FileDownloader(QUrl("https://images.rambler.ru/search?query="
                                                             + word) , this);

    connect(fileDownloader, &FileDownloader::downloaded,
            std::bind (&MainWindow::on_loadPage, this, fileDownloader));

}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->plainTextEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return)
        {
            on_pushButton_2_clicked();
        }
    }
}
