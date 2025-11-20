#include "floatingtoolbar.h"

#include "appsettings.h"

FloatingToolbar::FloatingToolbar(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.btn_snap->setChecked(AppSettings::snapEnabled());
    ui.btn_dir->setChecked(AppSettings::animDirEnabled());
}

void FloatingToolbar::on_btn_snap_clicked(bool checked) {
    AppSettings::snapEnabled = checked;
}

void FloatingToolbar::on_btn_dir_clicked(bool checked) {
    AppSettings::animDirEnabled = checked;
}
