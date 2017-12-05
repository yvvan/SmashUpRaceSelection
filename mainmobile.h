#pragma once

#include <QMainWindow>
#include <QListWidget>

#include <memory>
#include <set>

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
private:
  void ChangeCurrent(ActiveScreen new_current);
  void UncheckFactions(QListWidget* factionsList);
  void UncheckExpansions(QListWidget* expansionsList);
  std::unique_ptr<Ui::MainMobile> ui_;
  std::unique_ptr<Ui::MainWidget> ui_main_;
  std::set<QString> selected_expansions_;
  std::set<QString> selected_factions_;

  ActiveScreen current_screen_ = ActiveScreen::Main;
};
