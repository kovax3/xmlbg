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
#include "dialogedit.h"
#include  "ui_dialogedit.h"
#include <QtGui>
dialogedit::dialogedit(QWidget *parent)
    : QDialog(parent), ui(new Ui::dialogedit)
{
    ui->setupUi(this);
ui->btnAddFolder->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
ui->btnAddFile->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
}
void dialogedit::updateBtn(bool edited)
{
   ui->btnAddFolder->setHidden(edited);
   ui->btnAddFile->setVisible(edited);
}

void dialogedit::chargeCurent(QString xmlfile,bool edited)
{
    qApp->processEvents();
    QString filename;
    QString duration;
    QFile file(xmlfile);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"xmlfile",xmlfile+tr("  no exist"));
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
                        if (xml.name() == "duration"){
                            duration=xml.readElementText();

                        }else if (xml.name() == "file"){

                            filename= xml.readElementText();
                            if(edited==false){
                                 filename=newFileName(filename,xmlfile);
                            }



                        }else{
                            xml.skipCurrentElement();
                        }
                    }
                    QListWidgetItem *item = new QListWidgetItem;

                    item->setText(filename);
                    item->setCheckState(Qt::Checked);
                    qApp->processEvents();
                    ui->listWidget->addItem(item);
                }else{
                    xml.skipCurrentElement();
                }


            }
        }
    }
    ui->spinBoxDuration->setValue((duration.toFloat())/60);

    QApplication::restoreOverrideCursor();
}
QString  dialogedit::newFileName(QString oldName,QString newName)
{

    QFileInfo fi(oldName);
    QString imageName=fi.fileName();

    QFileInfo fio(newName);
    QString xmloldName=fio.fileName();
    QString imagePath=newName.replace(xmloldName,"");
    if(QFile::exists(imagePath+imageName)){
        return imagePath+imageName;
    }else{
        return oldName;
    }


}

void dialogedit::on_btnAddFolder_clicked()
{

    QString homeDir=QDir::homePath () ;

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    homeDir,
                                                    QFileDialog::ShowDirsOnly
                                                    |QFileDialog::DontResolveSymlinks);
    QFile file;
    if(!QDir(dir).exists())return;
themeTitle=QDir(dir).dirName();

    ui->listWidget->clear();
     QString subfile;

     QDir dirs(dir);
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     foreach ( subfile, dirs.entryList(QDir::Files)){

         file.setFileName(dir+"/"+subfile);
         QFileInfo fi(file);
         QString ext = fi.completeSuffix();
         if (ext.toLower()=="png"||ext.toLower()=="jpg"){
             QListWidgetItem *item = new QListWidgetItem;

             item->setText(file.fileName());


             item->setCheckState(Qt::Checked);
             qApp->processEvents();
             ui->listWidget->addItem(item);

         }
     }
      QApplication::restoreOverrideCursor();
}

void dialogedit::on_buttonBox_accepted()
{
     int count=ui->listWidget->count();
     if (count<=1)return;
     if(creatList()==true){
         creatThamb();
         creatXml();
     }

}
bool dialogedit::creatList()
{

   int count=ui->listWidget->count();
   for (int i=0 ;i<count ;i++)
   {
        if (ui->listWidget->item(i)->checkState()==Qt::Checked){
            list.append(ui->listWidget->item(i)->text());
        }
   }
   return true;
}
void dialogedit::creatThamb()
{
    QPainter p;
   int count=list.count();


   if (count==0)return;
  QString p1=list.at(0);

  QString p2=list.at(count-1);

   QPixmap pix1(p1);
   QPixmap pix2(p2);

  int i=0;
   if(count>0){
      i=count/2;
   }
  QPixmap  pix3(list.at(i));
   QPixmap   mypixmap(180,128);
   mypixmap.fill(Qt::transparent);
   p.begin(&mypixmap);
   p.drawPixmap(0,0,160,108,pix1);

   p.drawPixmap(10,10,160,108,pix2);
   p.drawPixmap(20,20,160,108,pix3);
   p.end();
  mypixmap.save(QDir::homePath()+"/.xmlbg/default.png","PNG");
}

void dialogedit::creatXml()
{


    QString duration=QString::number(ui->spinBoxDuration->value()*60);
    QString transition=QString::number(ui->spinBoxTransition->value());
    QXmlStreamWriter stream;
    QFile filx(QDir::homePath()+"/.xmlbg/default.xml");
    if (!filx.open(QFile::WriteOnly | QFile::Text)) {
        QApplication::restoreOverrideCursor();
        return ;
    }
    stream.setDevice(&filx);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("background");
     stream.setCodec(QTextCodec::codecForName("UTF-8"));
     int count2=list.count();
     for (int i=0 ;i<count2 ;i++)
     {

             QString fileName1=list.at(i);
             QString fileName2;
             if (i==count2-1){
                 fileName2=list.at(0);
             }else{
                 fileName2=list.at(i+1);
             }
             //duratino
             stream.writeStartElement("static");//static
             stream.writeTextElement("duration", duration);
             stream.writeTextElement("file", fileName1);
             stream.writeEndElement(); // static
             //transition
             stream.writeStartElement("transition");//static
             stream.writeTextElement("duration", transition);
             stream.writeTextElement("from", fileName1);
             stream.writeTextElement("to", fileName2);
             stream.writeEndElement(); // static

     }
     stream.writeEndElement(); // background
     stream.writeEndDocument();
     filx.close();


}

void dialogedit::on_listWidget_itemSelectionChanged()
{
     QString fileName=ui->listWidget->currentItem()->text();
    // QPixmap pix1(fileName);
//   pix1.scaled(128,128,Qt::KeepAspectRatio);
//     QPainter p;
  // QPixmap   mypixmap(128,128);
  //    mypixmap.fill(Qt::transparent);
//      p.begin(&mypixmap);
//      p.drawPixmap(0,0,128,128,pix1);
//
//
//      p.end();



   ui->labelPrivew->setPixmap(QPixmap(fileName));
}

void dialogedit::on_btnAddFile_clicked()
{

      QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                                QString(), tr("Images (*.png *.jpg);;jpg (*.jpg);;png (*.png)"));
      if (!fn.isEmpty())
      {

          QFileInfo fi(fn);
          QString ext = fi.completeSuffix();
          if (ext.toLower()=="png"||ext.toLower()=="jpg"){
              QListWidgetItem *item = new QListWidgetItem;

              item->setText(fn);


              item->setCheckState(Qt::Checked);

              ui->listWidget->addItem(item);


          }


      }
}
