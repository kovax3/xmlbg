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
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QLocale>
#include <QTranslator>
#include <QDir>
/****************************************************************************
**
** Copyright AbouZakaria.
** All rights reserved.
** Contact: AbouZakaria (yahia67@live.com)
**
** $xmlbg_BEGIN_LICENSE:GPL$
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** AbouZakaria (yahia67@live.com)
** $xmlbg_END_LICENSE$
***************************************************************************/
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("obgdesktop");
    a.setOrganizationName("Abou zakaria");
    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/xmlbg48.png")), QIcon::Normal, QIcon::Off);
    a.setWindowIcon(icon);
    QDir dir;
    QString h=dir.homePath();

    if (!dir.exists(h+"/.xmlbg")) //التاكد من وجود مجلد المكتبة
    {
        dir.mkdir( h+"/.xmlbg");
    }
    if (!dir.exists(h+"/.xmlbg/thumbnails")) //التاكد من وجود مجلد البياات
    {
        dir.mkdir( h+"/.xmlbg/thumbnails");
    }
    if (!dir.exists(h+"/.xmlbg/xmlfiles")) //التاكد من وجود مجلد البياات
    {
        dir.mkdir( h+"/.xmlbg/xmlfiles");
    }

    QString locale = QLocale::system().name();
  //  QMessageBox::information(0,"",locale);
//! [2]
    QTranslator translator;
//! [2] //! [3]
    translator.load(QString(":/xmlbg_") + locale);
    a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
