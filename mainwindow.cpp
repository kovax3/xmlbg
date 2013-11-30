/****************************************************************************
**
**  xmlbg Copyright (C) 2011 yahia abouzakaria <yahiaui@gmail.com>
**    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
**    This is free software, and you are welcome to redistribute it
**    under certain conditions; type `show c' for details.
**
**The hypothetical commands `show w' and `show c' should show the appropriate
**parts of the General Public License.  Of course, your program's commands
**might be different; for a GUI interface, you would use an "about box".
**
**  You should also get your employer (if you work as a programmer) or school,
**if any, to sign a "copyright disclaimer" for the program, if necessary.
**For more information on this, and how to apply and follow the GNU GPL, see
**<http://www.gnu.org/licenses/>.
**
**  The GNU General Public License does not permit incorporating your program
**into proprietary programs.  If your program is a subroutine library, you
**may consider it more useful to permit linking proprietary applications with
**the library.  If this is what you want to do, use the GNU Lesser General
**Public License instead of this License.  But first, please read
**<http://www.gnu.org/philosophy/why-not-lgpl.html>.

** If you have questions regarding the use of this file, please contact
** AbouZakaria (yahiaui@gmail.com)
** $xmlbg_END_LICENSE$
***************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "dialogedit.h"
#include <QListWidgetItem>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QTranslator>
#include <QXmlStreamReader>
#include <QTextDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QPainter>
#include <QColorDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonAplique->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonRemove->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->pushButtonEdit->setIcon(style()->standardIcon(QStyle::SP_DialogResetButton));
    ui->btnSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    listImageType << "stretched"<< "wallpaper"<< "zoom"<< "centered"<< "scaled"<< "spanned";
    listColorType << "solid" << "horizontal-gradient" << "vertical-gradient";
    setPixColor(ui->btnColor1,Qt::black);
    setPixColor(ui->btnColor2,Qt::black);
    gColor1="#000000";
    gColor1="#000000";
    ui->btnColor2->setVisible(false);
    ui->labelColor2->setVisible(false);
    //ui->listWidget->setPalette(QColor(QPalette::ToolTipBase));
    QString dir=QDir::homePath()+"/.xmlbg/thumbnails";
    QDir dirs(dir);
    QFile file;
    QString subfile;
    foreach ( subfile, dirs.entryList(QDir::Files)){

        file.setFileName(dir+"/"+subfile);
        QFileInfo fi(file);
        QString ext = fi.completeSuffix();

        if (ext.toLower()=="png"){
            QString base = fi.completeBaseName();
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(file.fileName()));
            item->setText(base);
            ui->listWidget->addItem(item);
        }

    }
    QPixmap pix(QDir::homePath()+"/.xmlbg/default.png");
    ui->label->setPixmap(pix);
    curPath=QDir::homePath()+"/.xmlbg/default.xml";
    ui->groupBox->setTitle("curent");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{

    if(QLocale::system().language()==QLocale::Arabic){
       this->setLayoutDirection(Qt::RightToLeft);
         qApp->setLayoutDirection(Qt::RightToLeft);
    }

    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);

        break;
    default:
        break;
    }
}

void MainWindow::on_listWidget_currentRowChanged(int )
{

}

void MainWindow::on_pushButton_clicked()
{
    dialogedit *dlg=new dialogedit(this);
 dlg->updateBtn(false);
    if ( dlg->exec() == QDialog::Accepted){
        QPixmap pix(QDir::homePath()+"/.xmlbg/default.png");
        ui->label->setPixmap(pix);
         ui->groupBox->setTitle(dlg->themeTitle);
         themename="";
    }
}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    QString title=ui->listWidget->currentItem()->text();
     ui->groupBox->setTitle(title);
    QPixmap pix(ui->listWidget->currentItem()->icon().pixmap(180,128));
   
    
    ui->label->setPixmap(pix);
    curPath=QDir::homePath()+"/.xmlbg/xmlfiles/"+title+".xml";
themename=title;
}

void MainWindow::on_btnSave_clicked()
{
    QPixmap pix(QDir::homePath()+"/.xmlbg/default.png");

    QString path=QDir::homePath()+"/.xmlbg/thumbnails";
    QString path2=QDir::homePath()+"/.xmlbg/xmlfiles";
    QInputDialog *dff=new QInputDialog(this);
    dff->setOkButtonText(trUtf8("ok"));
    // dff->setLayoutDirection(Qt::RightToLeft);
    dff->setCancelButtonText(trUtf8("cancel"));
    dff->setWindowTitle(trUtf8("rename"));
    dff->setTextValue( ui->groupBox->title());
    if (dff->exec()==QInputDialog::Accepted ){
        if (dff->textValue().isEmpty()){
            return;
        }
        QString themetitle=dff->textValue().replace(" ","_");
        if(QFile::exists(path + "/" + themetitle+".png")){
            QFile::remove(path + "/" + themetitle+".png");
        }
        if(QFile::exists(path2 + "/" + themetitle+".xml")){
            QFile::remove(path2 + "/" + themetitle+".xml");
        }
        if(pix.save(path + "/" +themetitle+".png","PNG")==true){

            QFile::copy(QDir::homePath()+"/.xmlbg/default.xml",path2 + "/" + themetitle+".xml");

            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QIcon(pix));
            item->setText(themetitle);
            ui->listWidget->addItem(item);

        }
    }
}

void MainWindow::on_pushButtonEdit_clicked()
{

    if (!QFile(curPath).exists()){
        QMessageBox::information(this,"error",curPath);
        return;
    }
    // QMessageBox::information(this,"",curPath);
    dialogedit *dlg=new dialogedit(this);
    dlg->updateBtn(true);
    dlg->chargeCurent(curPath,true);
    if ( dlg->exec() == QDialog::Accepted){

        QPixmap pix(QDir::homePath()+"/.xmlbg/default.png");
        ui->label->setPixmap(pix);
    }
}

void MainWindow::on_pushButtonRemove_clicked()

{

     QListWidgetItem *item = ui->listWidget->currentItem();
      if(!item)return;
    int i=ui->listWidget->currentRow();

    QString title=item->text();

    int reponse = QMessageBox::warning(this,"", trUtf8("Do you want to remove : ") +item->text(),QMessageBox::Yes | QMessageBox::No);
     if (reponse == QMessageBox::No){
        return;
     }

  ui->listWidget->item(i)->setSelected(false);
     QString path=QDir::homePath()+"/.xmlbg/thumbnails";
     QString path2=QDir::homePath()+"/.xmlbg/xmlfiles";

     ui->listWidget->takeItem(i);
      QFile::remove(path + "/" + title+".png");
      QFile::remove(path2 + "/" + title+".xml");

}

void MainWindow::on_pushButtonAplique_clicked()
{

  //  if (!QFile::exists(curPath))return;
    int indexImg=ui->comboBoxType->currentIndex();
    int indexColor=ui->comboBoxColor->currentIndex();
    QString bgrType=listImageType.at(indexImg);
    QString colorType=listColorType.at(indexColor);
    QProcess prosses;
    if(ui->checkBoxColor->isChecked()){
//gnome2
         prosses.execute("gconftool-2 --set /desktop/gnome/background/draw_background --type  boolean false");// يستخدم  لون
  //gnome3
         prosses.execute("gsettings set org.gnome.desktop.background draw-background  false");// يستخدم  لون
         prosses.execute("gsettings set org.gnome.desktop.background picture-uri ''");// يستخدم  لون

    }else{
  //gnome2
        prosses.execute("gconftool-2 --set /desktop/gnome/background/draw_background --type  boolean true");//يستخدم خلفية
        prosses.execute("gconftool-2 --set /desktop/gnome/background/picture_filename --type string \""+curPath + "\"");//مسار الصورة
        prosses.execute("gconftool-2 --set /desktop/gnome/background/picture_options --type string "+bgrType);//التحكم في الخلفية
  //gnome3
        prosses.execute("gsettings set org.gnome.desktop.background draw-background true");//يستخدم خلفية
        prosses.execute("gsettings set org.gnome.desktop.background picture-uri 'file://"+curPath + "'");//مسار الصورة
        prosses.execute("gsettings set org.gnome.desktop.background picture-options '"+bgrType+"'");//التحكم في الخلفية


    }
//gnome2
    prosses.execute("gconftool-2 --set /desktop/gnome/background/color_shading_type --type string "+colorType);//نوع اللون
    prosses.execute("gconftool-2 --set /desktop/gnome/background/primary_color --type string "+gColor1);//اللون الاول
    prosses.execute("gconftool-2 --set /desktop/gnome/background/secondary_color --type string "+gColor2);//اللون الثاني
    //gnome3
    colorType=colorType.replace("-gradient",QString::null);
    prosses.execute("gsettings set org.gnome.desktop.background color-shading-type '"+colorType+"'");//نوع اللون
        prosses.execute("gsettings set org.gnome.desktop.background primary-color '"+gColor1+"'");//اللون الاول
        prosses.execute("gsettings set org.gnome.desktop.background secondary-color '"+gColor2+"'");//اللون الثاني



}

void MainWindow::on_actionExite_triggered()
{
    qApp->quit();
}



void MainWindow::on_actionArchive_triggered()
{
    if(themename.isEmpty()){
         QMessageBox::information(this,"error",tr("select theme" )  );
         return;
    }
    QProcess prosses;
    QString filename;
    QString homePath=QDir::homePath();
    QDir dir(homePath+"/.xmlbg");
    QString th=homePath+"/.xmlbg/thumbnails/"+themename+".png";
    QString newxml=homePath+"/.xmlbg/"+themename+"/"+themename+".xml";

    dir.mkdir(themename);
    qApp->processEvents();

    prosses.execute("cp \""+th+"\" "+"\""+ homePath+"/.xmlbg/"+themename+"\"");

    qApp->processEvents();

    QFile file(curPath);

    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"xmlfile",curPath);
        return;
    }
    QByteArray data = file.readAll();
    QString    str = QString::fromUtf8(data);
    file.close();

    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"xmlfile",curPath);
        return;
    }

    QXmlStreamReader xml;
    xml.setDevice(&file);

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    if (xml.readNextStartElement()) {

        if (xml.name() == "background" ){
            Q_ASSERT(xml.isStartElement() && xml.name() == "background");
            while (xml.readNextStartElement()) {

                if (xml.name() == "static"){
                    while (xml.readNextStartElement()) {
                        if (xml.name() == "file"){
                            filename=xml.readElementText();

                        }else{
                            xml.skipCurrentElement();
                        }
                    }
                    QFileInfo fi(filename);
                    QString name = fi.fileName();
                    if (QFile::exists(filename)){
                        str.replace(filename,"/usr/share/backgrounds/"+themename+"/"+name);
                        prosses.execute("cp \""+filename+"\" "+homePath+"/.xmlbg/"+themename);
                    }

                    qApp->processEvents();

                }else{
                    xml.skipCurrentElement();
                }
            }
        }
    }


    if (creatNewXml(str,newxml)==true){
    dir.setCurrent(homePath+"/.xmlbg/");


    prosses.execute("tar -cvzf \""+homePath+"/"+themename+".tar.gz\" "+"\""+themename+"\"");

    QMessageBox::information(this,"",tr("file created\n")+homePath+"/"+themename+".tar.gz ");
}

    QApplication::restoreOverrideCursor();
     prosses.execute("rm -r \""+themename+ "\"");

}
bool  MainWindow::creatNewXml(QString str,QString newxml)
{
    QFile filex(newxml);

    if (!filex.open(QFile::WriteOnly))
        return false;
    filex.setPermissions( QFile::WriteUser | QFile::ReadUser);
    QTextStream out(&filex);

    out << str;
    return true;
}

void MainWindow::on_actionImportXml_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("xml wallpaper (*.xml)"));
    if (!fn.isEmpty())
    {
        dialogedit *dlg=new dialogedit(this);
        dlg->updateBtn(true);
        dlg->chargeCurent(fn,false);
        if ( dlg->exec() == QDialog::Accepted){

            QPixmap pix(QDir::homePath()+"/.xmlbg/default.png");
            ui->label->setPixmap(pix);

            QFileInfo fio(fn);
            QString xmloldName=fio.fileName();
            QString themePath=fn.replace(xmloldName,"");
            ui->groupBox->setTitle(QDir(themePath).dirName());

        }
    }
}


void MainWindow::setPixColor(QPushButton *btn,QColor color)
{

    QIcon icon1;
    QPixmap pixColor(30,16);
     QPainter p;
     p.begin(&pixColor);
     p.fillRect(0, 0, 30, 16, color);
      p.end();
    icon1.addPixmap(pixColor);
    btn->setIcon(icon1);
}

void MainWindow::on_btnColor1_clicked()
{
    QColor couleur = QColorDialog::getColor(gColor1, this,trUtf8("Select Color"),QColorDialog::DontUseNativeDialog);
    if( couleur.isValid()){

       setPixColor(ui->btnColor1,couleur);
       gColor1=couleur.name();
    }
}

void MainWindow::on_btnColor2_clicked()
{
    QColor couleur = QColorDialog::getColor(gColor2, this,trUtf8("Select Color"),QColorDialog::DontUseNativeDialog);
    if( couleur.isValid()){

       setPixColor(ui->btnColor2,couleur);
       gColor2=couleur.name();
    }
}

void MainWindow::on_comboBoxColor_currentIndexChanged(int index)
{
    if (index==0){
        ui->btnColor2->setVisible(false);
         ui->labelColor2->setVisible(false);
    }else{
         ui->btnColor2->setVisible(true);
            ui->labelColor2->setVisible(true);
    }

}
void MainWindow::on_actionAbout_triggered()
{

QString txt=trUtf8("Copyright : AbouZakaria")+"\n"+trUtf8("version : 0.1 ")
            +"\n"+trUtf8("License : GNU General Public License version 3.0");

    QMessageBox::about(this,tr("About")+"  "+qApp->applicationName(),txt);
}
void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}
