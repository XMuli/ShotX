#include "trayiconwindow.h"

#include <QMenu>
#include <QDebug>
#include <QKeyEvent>
#include "screenshots.h"

TrayIconWindow::TrayIconWindow()
{
    init();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &TrayIconWindow::onActiveTray);
    connect(m_actMainWindow, &QAction::triggered, this, &TrayIconWindow::onActMainWindow);
}

TrayIconWindow::~TrayIconWindow()
{
    m_trayIcon->deleteLater();  // 后面仔细看下与 delete 的差异
}

void TrayIconWindow::init()
{
    m_screenShot = nullptr;
    m_menu = new QMenu();
    m_actMainWindow = new QAction(tr("偏好设置")); // 显示主窗口
    m_actExit = new QAction(tr("退出"));           // 退出
    m_menu->addAction(m_actMainWindow);
    m_menu->addSeparator();
    m_menu->addAction(m_actExit);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/icons/logo.svg"));
    m_trayIcon->setToolTip(tr("TrayIconWindow"));
    m_trayIcon->setContextMenu(m_menu);


    // 此两行测试代码
    move(0, 0);
    setMouseTracking(true);

    m_trayIcon->show();
}

void TrayIconWindow::onActiveTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:    // 单击系统托盘
    case QSystemTrayIcon::DoubleClick:   {// 双击（在macOS上，仅在未设置上下文菜单的情况下才会发出双击，因为菜单是在鼠标按下时打开的）
        qDebug()<<"---单击或者双击---";
        m_screenShot = ScreenShots::instances();
        m_screenShot->show();
        break;
    }
    case QSystemTrayIcon::Context: {    // 请求系统托盘上下文菜单
        qDebug()<<"---请求上下文---";
        m_menu->show();
        break;
    }
    default:
        break;
    }
}

void TrayIconWindow::onActMainWindow()
{
    qDebug()<<"---配置---";
    m_mainWindow = new MainWindow();
    m_mainWindow->show();
}

void TrayIconWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape : {
        qDebug()<<"---Qt::Key_Escape---";
        if (m_screenShot != nullptr) {
            m_screenShot->close();
            m_screenShot = nullptr;
        }
        break;
    }
    case Qt::Key_F2: {
        qDebug()<<"---Qt::Key_F2---";
        break;
    }
    default:
        break;
    }

    //    QWidget::keyPressEvent(event);
}
