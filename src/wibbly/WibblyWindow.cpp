#include <QButtonGroup>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QStatusBar>
#include <QTimeEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <VSScript.h>

#include "WibblyWindow.h"
#include "WobblyException.h"


WibblyWindow::WibblyWindow()
    : QMainWindow()
{
    createUI();
}


void WibblyWindow::createUI() {
    setWindowTitle(QStringLiteral("Wibbly Metrics Collector v%1").arg(PACKAGE_VERSION));

    statusBar()->setSizeGripEnabled(true);

    createMainWindow();
    createVideoOutputWindow();
    createCropWindow();
    createVFMWindow();
    createTrimWindow();
    createInterlacedFadesWindow();
}


void WibblyWindow::createMenus() {
    QMenuBar *bar = menuBar();

    menu_menu = bar->addMenu("&Menu");

    QAction *quit_action = new QAction("&Quit", this);
    quit_action->setShortcut(QKeySequence("Ctrl+Q"));

    connect(quit_action, &QAction::triggered, this, &WibblyWindow::close);

    menu_menu->addSeparator();
    menu_menu->addAction(quit_action);
}


void WibblyWindow::createMainWindow() {
    createMenus();


    main_jobs_list = new ListWidget;
    main_jobs_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    main_jobs_list->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QPushButton *main_add_jobs_button = new QPushButton("Add jobs");
    QPushButton *main_remove_jobs_button = new QPushButton("Remove jobs");
    QPushButton *main_copy_jobs_button = new QPushButton("Copy jobs");
    QPushButton *main_move_jobs_up_button = new QPushButton("Move up");
    QPushButton *main_move_jobs_down_button = new QPushButton("Move down");

    QLineEdit *main_destination_edit = new QLineEdit;

    QPushButton *main_choose_button = new QPushButton("Choose");

    std::map<int, QString> steps = {
        { StepTrim, "Trim" },
        { StepCrop, "Crop" },
        { StepFieldMatch, "Field matching" },
        { StepInterlacedFades, "Interlaced fades" },
        { StepDecimation, "Decimation" },
        { StepSceneChanges, "Scene changes" },
    };

    QButtonGroup *main_steps_buttons = new QButtonGroup(this);
    main_steps_buttons->setExclusive(false);

    for (auto it = steps.cbegin(); it != steps.cend(); it++) {
        main_steps_buttons->addButton(new QCheckBox(it->second), it->first);
        main_steps_buttons->button(it->first)->setChecked(true);
    }

    main_progress_bar = new QProgressBar;

    QPushButton *main_engage_button = new QPushButton("Engage");
    QPushButton *main_cancel_button = new QPushButton("Cancel");


    // connect


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(main_jobs_list, 1);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(new QLabel("Destination:"));
    hbox->addWidget(main_destination_edit);
    hbox->addWidget(main_choose_button);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout;

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(main_add_jobs_button);
    vbox2->addWidget(main_remove_jobs_button);
    vbox2->addWidget(main_copy_jobs_button);
    vbox2->addWidget(main_move_jobs_up_button);
    vbox2->addWidget(main_move_jobs_down_button);
    vbox2->addStretch(1);
    hbox->addLayout(vbox2);

    vbox2 = new QVBoxLayout;
    for (auto it = steps.cbegin(); it != steps.cend(); it++) {
        vbox2->addWidget(main_steps_buttons->button(it->first));
    }
    vbox2->addStretch(1);

    hbox->addLayout(vbox2);

    vbox->addLayout(hbox);

    hbox = new QHBoxLayout;
    hbox->addWidget(main_progress_bar);
    hbox->addWidget(main_engage_button);
    hbox->addWidget(main_cancel_button);

    vbox->addLayout(hbox);


    QWidget *main_widget = new QWidget;
    main_widget->setLayout(vbox);

    setCentralWidget(main_widget);
}


void WibblyWindow::createVideoOutputWindow() {
    video_frame_label = new QLabel;
    video_frame_label->setMinimumSize(720, 480);

    QScrollArea *video_frame_scroll = new QScrollArea;
    video_frame_scroll->setFrameShape(QFrame::NoFrame);
    video_frame_scroll->setFocusPolicy(Qt::NoFocus);
    video_frame_scroll->setAlignment(Qt::AlignCenter);
    video_frame_scroll->setWidgetResizable(true);
    video_frame_scroll->setWidget(video_frame_label);

    QSpinBox *video_frame_spin = new QSpinBox;
    QTimeEdit *video_time_edit = new QTimeEdit;
    video_time_edit->setDisplayFormat(QStringLiteral("hh:mm:ss.zzz"));

    video_frame_slider = new QSlider(Qt::Horizontal);
    video_frame_slider->setTracking(false);
    video_frame_slider->setFocusPolicy(Qt::NoFocus);


    // connect


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(video_frame_scroll);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(video_frame_spin);
    hbox->addWidget(video_time_edit);
    hbox->addWidget(video_frame_slider);
    vbox->addLayout(hbox);


    QWidget *video_widget = new QWidget;
    video_widget->setLayout(vbox);


    video_dock = new DockWidget("Video output", this);
    video_dock->resize(720, 480);
    video_dock->setObjectName("video output window");
    video_dock->setVisible(true);
    video_dock->setFloating(true);
    video_dock->setWidget(video_widget);
    addDockWidget(Qt::RightDockWidgetArea, video_dock);
    QList<QAction *> actions = menu_menu->actions();
    menu_menu->insertAction(actions[actions.size() - 2], video_dock->toggleViewAction());
    connect(video_dock, &DockWidget::visibilityChanged, video_dock, &DockWidget::setEnabled);
}


void WibblyWindow::createCropWindow() {
    const char *crop_prefixes[4] = {
        "Left: ",
        "Top: ",
        "Right: ",
        "Bottom: "
    };

    QSpinBox *crop_spin[4];
    for (int i = 0; i < 4; i++) {
        crop_spin[i] = new QSpinBox;
        crop_spin[i]->setRange(0, 99999);
        crop_spin[i]->setPrefix(crop_prefixes[i]);
        crop_spin[i]->setSuffix(QStringLiteral(" px"));
    }


    // connect


    QVBoxLayout *vbox = new QVBoxLayout;
    for (int i = 0; i < 4; i++)
        vbox->addWidget(crop_spin[i]);
    vbox->addStretch(1);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addLayout(vbox);
    hbox->addStretch(1);


    QWidget *crop_widget = new QWidget;
    crop_widget->setLayout(hbox);


    crop_dock = new DockWidget("Crop", this);
    crop_dock->setObjectName("crop window");
    crop_dock->setVisible(false);
    crop_dock->setFloating(true);
    crop_dock->setWidget(crop_widget);
    addDockWidget(Qt::RightDockWidgetArea, crop_dock);
    QList<QAction *> actions = menu_menu->actions();
    menu_menu->insertAction(actions[actions.size() - 2], crop_dock->toggleViewAction());
    connect(crop_dock, &DockWidget::visibilityChanged, crop_dock, &DockWidget::setEnabled);
}


void WibblyWindow::createVFMWindow() {
    struct VFMSpin {
        QSpinBox *spinbox;
        QString prefix;
        int minimum;
        int maximum;
    };

    std::vector<VFMSpin> vfm_spins = {
        { nullptr, "order: ",        INT_MIN,          1 },
        { nullptr, "cthresh: ",           -1,        255 },
        { nullptr, "mi: ",           INT_MIN,    INT_MAX },
        { nullptr, "blockx: ",             4,        512 },
        { nullptr, "blocky: ",             4,        512 },
        { nullptr, "y0: ",           INT_MIN,    INT_MAX },
        { nullptr, "y1: ",           INT_MIN,    INT_MAX },
        { nullptr, "micmatch: ",     INT_MIN,          2 }
    };

    for (size_t i = 0; i < vfm_spins.size(); i++) {
        vfm_spins[i].spinbox = new QSpinBox;
        vfm_spins[i].spinbox->setPrefix(vfm_spins[i].prefix);
        if (vfm_spins[i].minimum != INT_MIN)
            vfm_spins[i].spinbox->setMinimum(vfm_spins[i].minimum);
        if (vfm_spins[i].maximum != INT_MAX)
            vfm_spins[i].spinbox->setMaximum(vfm_spins[i].maximum);
    }

    QDoubleSpinBox *vfm_scthresh_spin = new QDoubleSpinBox;
    vfm_scthresh_spin->setMaximum(100);

    QCheckBox *vfm_mchroma_check = new QCheckBox("mchroma");
    QCheckBox *vfm_chroma_check = new QCheckBox("chroma");


    // connect


    QVBoxLayout *vbox = new QVBoxLayout;
    for (size_t i = 0; i < vfm_spins.size(); i++)
        vbox->addWidget(vfm_spins[i].spinbox);
    vbox->addWidget(vfm_scthresh_spin);
    vbox->addWidget(vfm_mchroma_check);
    vbox->addWidget(vfm_chroma_check);
    vbox->addStretch(1);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addLayout(vbox);
    hbox->addStretch(1);


    QWidget *vfm_widget = new QWidget;
    vfm_widget->setLayout(hbox);


    vfm_dock = new DockWidget("VFM", this);
    vfm_dock->setObjectName("vfm window");
    vfm_dock->setVisible(false);
    vfm_dock->setFloating(true);
    vfm_dock->setWidget(vfm_widget);
    addDockWidget(Qt::RightDockWidgetArea, vfm_dock);
    QList<QAction *> actions = menu_menu->actions();
    menu_menu->insertAction(actions[actions.size() - 2], vfm_dock->toggleViewAction());
    connect(vfm_dock, &DockWidget::visibilityChanged, vfm_dock, &DockWidget::setEnabled);
}


void WibblyWindow::createTrimWindow() {
    trim_ranges_list = new ListWidget;
    trim_ranges_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    trim_ranges_list->setSelectionMode(QAbstractItemView::ExtendedSelection);

    trim_label = new QLabel;

    QPushButton *trim_start_button = new QPushButton("Start trim");
    QPushButton *trim_end_button = new QPushButton("End trim");
    QPushButton *trim_add_button = new QPushButton("Add trim");


    // connect


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(trim_ranges_list);
    vbox->addWidget(trim_label);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(trim_add_button);
    hbox->addWidget(trim_start_button);
    hbox->addWidget(trim_end_button);
    hbox->addStretch(1);
    vbox->addLayout(hbox);


    QWidget *trim_widget = new QWidget;
    trim_widget->setLayout(vbox);


    trim_dock = new DockWidget("Trim", this);
    trim_dock->setObjectName("trim window");
    trim_dock->setVisible(false);
    trim_dock->setFloating(true);
    trim_dock->setWidget(trim_widget);
    addDockWidget(Qt::RightDockWidgetArea, trim_dock);
    QList<QAction *> actions = menu_menu->actions();
    menu_menu->insertAction(actions[actions.size() - 2], trim_dock->toggleViewAction());
    connect(trim_dock, &DockWidget::visibilityChanged, trim_dock, &DockWidget::setEnabled);
}


void WibblyWindow::createInterlacedFadesWindow() {
    fades_threshold_spin = new QDoubleSpinBox;
    fades_threshold_spin->setPrefix(QStringLiteral("Threshold: "));


    // connect


    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(fades_threshold_spin);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addStretch(1);


    QWidget *fades_widget = new QWidget;
    fades_widget->setLayout(vbox);


    fades_dock = new DockWidget("Interlaced fades", this);
    fades_dock->setObjectName("interlaced fades window");
    fades_dock->setVisible(false);
    fades_dock->setFloating(true);
    fades_dock->setWidget(fades_widget);
    addDockWidget(Qt::RightDockWidgetArea, fades_dock);
    QList<QAction *> actions = menu_menu->actions();
    menu_menu->insertAction(actions[actions.size() - 2], fades_dock->toggleViewAction());
    connect(fades_dock, &DockWidget::visibilityChanged, fades_dock, &DockWidget::setEnabled);
}
