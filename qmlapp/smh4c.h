#ifndef SMH4C_H
#define SMH4C_H

#include <QtCore>
#include <QColor>

struct SMH4C
{
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    static const int DEFAULT_HEADER_HEIGHT;
    static const int DEFAULT_HEADER_FONT_SIZE;

    static const QString DEFAULT_FONT_FAMILY;
    static const int     DEFAULT_FONT_SIZE;

    static const QString NAVBAR_FONT_FAMILY;
    static const int     NAVBAR_FONT_SIZE;

    static const QString STATUS_FONT_FAMILY;
    static const int     STATUS_FONT_SIZE;

    static const QString MENU_FONT_FAMILY;
    static const int     MENU_FONT_SIZE;

    static const QString MENUHEAD_FONT_FAMILY;
    static const int     MENUHEAD_FONT_SIZE;

    static const QString SMALLEST_FONT_FAMILY;
    static const int     SMALLEST_FONT_SIZE;

    static const QString  MIDDLE_FONT_FAMILY;
    static const int  MIDDLE_FONT_SIZE;


    static const int SCENE_SPACE_RIGHT;
    static const int SCENEWIDTH;
    static const int SCENEHEIGHT;

    static const QColor M_COLOR;
    static const QColor I_COLOR;
    static const QColor V_COLOR;

    static const QColor LIST_TEXT_COLOR;
    static const QColor SELECTION_BACKGROUND_COLOR;
    static const QColor SELECTION_COLOR;
    static const QColor LIST_DARK_COLOR;
    static const QColor LIST_LIGHT_COLOR;
    static const QColor LIST_BACKGROUND_COLOR;

    static const int STATUSBAR_HEIGHT;

    static const double DINAMO_WIDTH;
    static const double DINAMO_HEIGHT;

    static const double DINAMO_HEIGHT_MAX;
    static const double X_PADDING_FIELD_WIDTH_K;

    static const int INX_MENU_WIDTH;
};

#endif // SMH4C_H
