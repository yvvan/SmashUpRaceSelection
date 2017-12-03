#include "mainmobile.h"
#include "ui_mainmobile.h"
#include "ui_mainwidget.h"
#include "ui_expansionswidget.h"
#include "ui_factionswidget.h"

MainMobile::MainMobile(QWidget *parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainMobile>()) {
  ui_->setupUi(this);

  ui_main_ = std::make_unique<Ui::MainWidget>();
  ui_factions_ = std::make_unique<Ui::FactionsWidget>();
  ui_expansions_ = std::make_unique<Ui::ExpansionsWidget>();
  onBackClicked();
}

static void ClearLayout(QBoxLayout* layout) {
  auto* item = layout->takeAt(0);
  while (item) {
    QBoxLayout* childLayout = static_cast<QBoxLayout*>(item->layout());
    if (childLayout) {
      ClearLayout(childLayout);
      delete childLayout;
      item = layout->takeAt(0);
      continue;
    }
    delete item->widget();
    delete item;
    item = layout->takeAt(0);
  }
}

static QPushButton* AddBackButton(QVBoxLayout* layout) {
  QHBoxLayout* horizontal_layout = new QHBoxLayout;
  layout->addLayout(horizontal_layout);
  QPushButton* back_button = new QPushButton("Back");
  horizontal_layout->addWidget(back_button);
  horizontal_layout->addStretch();
  return back_button;
}

void MainMobile::onFactionsClicked() {
  ClearLayout(ui_->verticalLayout);
  QWidget* widget = new QWidget;
  ui_factions_->setupUi(widget);
  auto* back_button = AddBackButton(ui_->verticalLayout);
  ui_->verticalLayout->addWidget(widget);
  QObject::connect(back_button, &QPushButton::clicked, this,
                   &MainMobile::onBackClicked);
}

void MainMobile::onExpansionsClicked() {
  ClearLayout(ui_->verticalLayout);
  QWidget* widget = new QWidget;
  ui_expansions_->setupUi(widget);
  auto* back_button = AddBackButton(ui_->verticalLayout);
  ui_->verticalLayout->addWidget(widget);
  QObject::connect(back_button, &QPushButton::clicked, this,
                   &MainMobile::onBackClicked);
}

void MainMobile::onBackClicked() {
  ClearLayout(ui_->verticalLayout);
  QWidget* widget = new QWidget;
  ui_main_->setupUi(widget);
  ui_->verticalLayout->addWidget(widget);
  QObject::connect(ui_main_->expansionsButton, &QPushButton::clicked, this,
                   &MainMobile::onExpansionsClicked);
  QObject::connect(ui_main_->factionsButton, &QPushButton::clicked, this,
                   &MainMobile::onFactionsClicked);
}

MainMobile::~MainMobile() = default;
