#ifndef __RHWIDGET_H__
#define __RHWIDGET_H__

#include "listwidget.h"
#include "ui_rhwidget.h"

class ListWidget;

class RequestHystoryWidget : public QDialog, private Ui::RequestHystoryWidget
{
Q_OBJECT

public:
    /** \brief Creates all elements of the window and connects signals with slots.
     * Restores size and position from settings.
     *  \param[in] parent  parent widget */
    RequestHystoryWidget(ListWidget *parent);

    bool contains(const QString &itemName);

    void insertItem(const QString &itemName,
                    const QString& selectedItems,
                    const QString& relationType,
                    const QString& outputFormat,
                    int level);

    void saveRequests();

private:
    QSettings*    settings;           ///< Settings
    ListWidget*   listWidget;
    QString       pathToRequestsFile;

    void closeEvent(QCloseEvent* event);

private slots:

    void restoreRequest();

    void deleteItem();
};

#endif // __RHWIDGET_H__
