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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
QString curPath;
QString themename;
QList<QString> listImageType;
QList<QString> listColorType;
QString gColor1;
QString gColor2;
private slots:
void on_actionAboutQt_triggered();
void on_comboBoxColor_currentIndexChanged(int index);
void on_btnColor2_clicked();
void on_btnColor1_clicked();
void setPixColor(QPushButton *btn,QColor color);
void on_actionAbout_triggered();
void on_actionImportXml_triggered();
bool creatNewXml(QString str,QString newxml);
    void on_actionArchive_triggered();
    void on_actionExite_triggered();
    void on_pushButtonAplique_clicked();
    void on_pushButtonRemove_clicked();
    void on_pushButtonEdit_clicked();
    void on_btnSave_clicked();
    void on_listWidget_itemSelectionChanged();
    void on_pushButton_clicked();
    void on_listWidget_currentRowChanged(int );
};

#endif // MAINWINDOW_H
