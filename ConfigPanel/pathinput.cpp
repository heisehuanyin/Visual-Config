#include "pathinput.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QtDebug>

using namespace CustomWidget;

PathInput::PathInput(WSFoundation::ConfigV1::PathInput *path_in, QWidget *parent)
    : QWidget(parent), path_in(path_in)
{
    auto layout = new QHBoxLayout(this);
    layout->setMargin(2);

    auto x = new QLabel(path_in->configTitle(), this);
    x->setFrameStyle(QFrame::Sunken|QFrame::Box);
    layout->addWidget(x);

    auto input = new QLineEdit(this);
    layout->addWidget(input);

    QPushButton *btn=nullptr;
    if(path_in->type() == WSFoundation::ConfigV1::PathInput::FILE){
        btn = new QPushButton("选择文件", this);
        layout->addWidget(btn);
    }else {
        btn = new QPushButton("选择文件夹", this);
        layout->addWidget(btn);
    }

    connect(input,  QOverload<const QString&>::of(&QLineEdit::textChanged), [=](const QString &p){
        QFileInfo f(input->text());
        if(f.exists() &&
           ((path_in->type()==WSFoundation::ConfigV1::PathInput::FILE && f.isFile())||
            (path_in->type()==WSFoundation::ConfigV1::PathInput::DIRECTORY && f.isDir()))){
            input->setStyleSheet("color:\"black\"");
            path_in->resetPath(p);
        }else {
            input->setStyleSheet("color:\"red\"");
        }
    });

    connect(btn,    QOverload<bool>::of(&QPushButton::clicked), [=](bool){
        if(path_in->type() == WSFoundation::ConfigV1::PathInput::FILE){
            auto file_path = QFileDialog::getOpenFileName(this, path_in->configTitle(), path_in->path());
            if(QFileInfo(file_path).exists()){
                input->setText(file_path);
                path_in->resetPath(file_path);
            }
        }
        else {
            auto dir_path = QFileDialog::getExistingDirectory(this, path_in->configTitle(), path_in->path());
            if(QFileInfo(dir_path).exists()){
                input->setText(dir_path);
                path_in->resetPath(dir_path);
            }
        }
    });


    input->setText(path_in->path());
}
