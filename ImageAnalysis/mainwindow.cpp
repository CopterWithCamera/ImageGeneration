#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <QImage>

#define IMAGE_WIDTH     80
#define IMAGE_HEIGHT    48
#define IMAGE_SIZE      (IMAGE_WIDTH * IMAGE_HEIGHT)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_Open_clicked()
{
    //获取源文件
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("hexfile(*.hex);;Allfile(*.*)"));
    ui->label_filename->setText(fileName);

    //获取保存文件目录
    QString file_path = QFileDialog::getExistingDirectory(this,"请选择数据保存文件夹","./");
    ui->label_savepath->setText(file_path);

    //打开文件处理
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    ui->label_status->setText("处理中...");

    int size = file.size(); //获取文件字节数
    qDebug() << size;

    unsigned char data;
    int image_counter = 0;

    int imagenum = size/IMAGE_SIZE;
    for(int i=0;i<imagenum;i++)  //循环生成图片
    {
        //生成图片文件名
        image_counter++;
        QString strtmp;
        strtmp.setNum(image_counter);
        QString ImageFileName = file_path + "/" + strtmp + ".bmp";

        //灰度数据临时BUFF
        unsigned char imageTmpArray[IMAGE_SIZE];
        for(int j=0;j<IMAGE_SIZE;j++)
        {
            in >> data;
            imageTmpArray[j] = data;
        }

        //拷贝到图像缓存数组
        unsigned char imageByteArray[IMAGE_SIZE*3];
        for(int j=0;j<IMAGE_SIZE;j++)
        {
            imageByteArray[j*3] = imageByteArray[j*3+1] = imageByteArray[j*3+2] = imageTmpArray[j];
        }

        //生成QImage对象
        QImage DisImage = QImage(imageByteArray, IMAGE_WIDTH, IMAGE_HEIGHT, QImage::Format_RGB888);

        //保存图片
        QPixmap imgtmp = QPixmap::fromImage(DisImage);
        imgtmp.save(ImageFileName,"bmp",100);
    }

    ui->label_status->setText("完成！");
}
