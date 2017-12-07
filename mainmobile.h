#pragma once

#include "utils.h"

#include <QMainWindow>

#include <memory>

namespace Ui {
  class MainMobile;
  class MainWidget;
}

class MainMobile : public QMainWindow
{
  Q_OBJECT
  enum class ActiveScreen {
    Main = 0,
    Expansions,
    Factions,
    Results
  };
public:
  explicit MainMobile(QWidget *parent = 0);
  ~MainMobile();

public slots:
  void onExpansionsClicked();
  void onFactionsClicked();
  void onBackClicked();
  void onRandomizeClicked();
private:
  void ChangeCurrent(ActiveScreen new_current);
  std::unique_ptr<Ui::MainMobile> ui_;
  std::unique_ptr<Ui::MainWidget> ui_main_;
  std::array<bool, kExpansionsNumber> selected_expansions_;
  std::array<bool, kFactionsNumber> selected_factions_;

  ActiveScreen current_screen_ = ActiveScreen::Main;
  size_t players_number_ = 2;
};
