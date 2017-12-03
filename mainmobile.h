#pragma once

#include <QMainWindow>

#include <memory>

namespace Ui {
  class MainMobile;
  class MainWidget;
  class ExpansionsWidget;
  class FactionsWidget;
}

class MainMobile : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainMobile(QWidget *parent = 0);
  ~MainMobile();

public slots:
  void onExpansionsClicked();
  void onFactionsClicked();
  void onBackClicked();
private:
  std::unique_ptr<Ui::MainMobile> ui_;
  std::unique_ptr<Ui::MainWidget> ui_main_;
  std::unique_ptr<Ui::FactionsWidget> ui_factions_;
  std::unique_ptr<Ui::ExpansionsWidget> ui_expansions_;
};
