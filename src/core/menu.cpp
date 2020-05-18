#include "menu.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QPushButton>
#include <QTimer>
#include <algorithm>
#include <cmath>
#include <memory>

#include "core/menugraph.h"
#include "core/statemachine.h"
#include "data/loader.h"
#include "data/objectsstorage.h"
#include "graphics/attackresultwindow.h"
#include "graphics/buttonitem.h"
#include "graphics/fullplanetinfo.h"
#include "graphics/imageitem.h"
#include "graphics/planetgraphics.h"
#include "graphics/planetinfographics.h"
#include "graphics/shopplanetinfo.h"
#include "graphics/shopwidget.h"
#include "graphics/unitwidget.h"
#include "mainwindow.h"
#include "objects/building.h"
#include "objects/planet.h"
#include "objects/player.h"
#include "objects/unit.h"
#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "util/typeoffset.h"

MainMenu::MainMenu() {
  this->Draw();
  connect(btn_exit_, SIGNAL(clicked()), Controller::window, SLOT(Exit()));
  connect(btn_new_game_, SIGNAL(clicked()), this, SLOT(btnNewGameClicked()));
}

MainMenu::~MainMenu() {
  Controller::scene->removeItem(txt_total_space_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(btn_new_game_);

  delete txt_total_space_;
  delete btn_exit_;
  delete btn_new_game_;
}

void MainMenu::SetZValue() {
  btn_exit_->setZValue(ZValues::kMainMenu);
  txt_total_space_->setZValue(ZValues::kMainMenu);
  btn_new_game_->setZValue(ZValues::kMainMenu);
}

void MainMenu::Draw() {
  GameView* view = Controller::view;
  QMatrix matrix = view->matrix();
  view->setMatrix(
      QMatrix(1, matrix.m12(), matrix.m21(), 1, matrix.dx(), matrix.dy()));

  btn_exit_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_exit_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kExitButton));

  btn_new_game_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_new_game_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kNewGameButton));

  txt_total_space_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kBackground),
      static_cast<int>(kWidth + 10), static_cast<int>(kHeight + 10));

  SetZValue();

  Controller::scene->addItem(txt_total_space_);
  Controller::scene->addItem(btn_exit_);
  Controller::scene->addItem(btn_new_game_);

  btn_new_game_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 - kHeight / 30)));
  btn_exit_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 12)));

  txt_total_space_->setPos(Controller::view->sceneRect().center() / 2);
}

void MainMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  Controller::SetMainMenu(nullptr);

  if (menu == Controller::MenuType::kGame) {
    Controller::SetGameMenu(new GameMenu());
    Controller::SetMenuType(Controller::MenuType::kGame);
  }
}

void MainMenu::btnNewGameClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

PauseMenu::PauseMenu() {
  this->Draw();
  connect(btn_back_, SIGNAL(clicked()), this, SLOT(btnBackClicked()));
  connect(btn_exit_, SIGNAL(clicked()), this, SLOT(btnExitClicked()));
}

PauseMenu::~PauseMenu() {
  Controller::scene->removeItem(btn_back_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(background_rect_);

  delete btn_back_;
  delete btn_exit_;
  delete background_rect_;
}

void PauseMenu::SetZValue() {
  background_rect_->setZValue(ZValues::kPauseMenu);
  btn_back_->setZValue(ZValues::kPauseMenu);
  btn_exit_->setZValue(ZValues::kPauseMenu);
}

void PauseMenu::Draw() {
  GameView* view = Controller::view;
  QPointF center =
      view->mapToScene(QPoint(view->rect().width(), view->rect().height()) / 2);
  double coef = view->matrix().m11();
  QRectF rect = view->sceneRect();

  rect.setX(center.x() - view->rect().width() / coef);
  rect.setY(center.y() - view->rect().height() / coef);
  rect.setSize(rect.size() * 4);

  background_rect_ = new QGraphicsRectItem();
  background_rect_->setScale(1 / coef);
  background_rect_->setRect(rect);
  background_rect_->setOpacity(0.7);
  background_rect_->setBrush(Qt::black);

  btn_back_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_back_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kBackToGameButton));
  btn_exit_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_exit_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kToMenuButton));

  SetZValue();

  Controller::scene->addItem(background_rect_);
  Controller::scene->addItem(btn_back_);
  Controller::scene->addItem(btn_exit_);

  btn_back_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 - kHeight / 30)));
  btn_exit_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 12)));
}

void PauseMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kGame) {
    Controller::SetPauseMenu(nullptr);
    Controller::SetMenuType(Controller::MenuType::kGame);
    Controller::GetGameMenu()->Show();
  }

  if (menu == Controller::MenuType::kMain) {
    Controller::SetPauseMenu(nullptr);
    Controller::SetGameMenu(nullptr);
    Controller::SetMainMenu(new MainMenu());
    Controller::SetMenuType(Controller::MenuType::kMain);
  }
}

void PauseMenu::btnBackClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

void PauseMenu::btnExitClicked() {
  Controller::SwitchMenu(Controller::MenuType::kMain);
}

PlanetMenu::PlanetMenu() {
  if (Controller::GetActivePlanet() == nullptr ||
      Controller::GetActivePlanet()->GetOwner() !=
          Controller::scene->GetPlayer()) {
    // TODO убрать граф перехода по кнопке (getNextMenu)
    btn1_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn1_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kAttackButton));
    button_to_menu_[btn1_] = Controller::MenuType::kAttack;
    btn2_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn2_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kInfoButton));
    button_to_menu_[btn2_] = Controller::MenuType::kGame;
    btn3_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn3_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kSimpleButton));

    button_to_menu_[btn3_] = Controller::MenuType::kGame;
    connect(btn1_, SIGNAL(clicked()), this, SLOT(btnAttackClicked()));
    connect(btn2_, SIGNAL(clicked()), this, SLOT(btnInfoClicked()));
    connect(btn3_, SIGNAL(clicked()), this, SLOT(btnDefaultClicked()));
  } else {
    btn1_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn1_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kMoveButton));
    button_to_menu_[btn1_] = Controller::MenuType::kGame;
    btn2_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn2_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kInfoButton));
    button_to_menu_[btn2_] = Controller::MenuType::kGame;
    btn3_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn3_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kShopButton));
    button_to_menu_[btn3_] = Controller::MenuType::kGame;
    connect(btn1_, SIGNAL(clicked()), this, SLOT(btnMoveClicked()));
    connect(btn2_, SIGNAL(clicked()), this, SLOT(btnInfoClicked()));
    connect(btn3_, SIGNAL(clicked()), this, SLOT(btnShopClicked()));
  }
  this->Draw();
  Controller::scene->UpdatePlanetsGraph();
}

PlanetMenu::~PlanetMenu() {
  Controller::scene->removeItem(btn1_);
  Controller::scene->removeItem(btn2_);
  Controller::scene->removeItem(btn3_);
  delete btn1_;
  delete btn2_;
  delete btn3_;
}

void PlanetMenu::SetZValue() {
  btn1_->setZValue(ZValues::kPlanetMenu);
  btn2_->setZValue(ZValues::kPlanetMenu);
  btn3_->setZValue(ZValues::kPlanetMenu);
}

void PlanetMenu::Draw() {
  SetZValue();
  Controller::scene->addItem(btn1_);
  Controller::scene->addItem(btn2_);
  Controller::scene->addItem(btn3_);

  // TODO определить зависимость множителя от радиуса планеты
  Planet* p = Controller::GetActivePlanet();

  radius_ = -p->GetRadius() * 1.3;
  QPointF vec1(0, radius_);
  QPointF vec2(vec1 * QTransform().rotate(60));
  QPointF vec3(vec1 * QTransform().rotate(-60));

  btn1_->setPos(p->GetCoordinates() * 2 + vec1);
  btn2_->setPos(p->GetCoordinates() * 2 + vec2);
  btn3_->setPos(p->GetCoordinates() * 2 + vec3);

  btn1_->setY(btn1_->y() - radius_ / 48);
  btn2_->setX(btn2_->x() + radius_ / 48);
  btn3_->setX(btn3_->x() - radius_ / 48);
}

Controller::MenuType PlanetMenu::GetNextMenu(ButtonItem* button) const {
  if (button_to_menu_.find(button) != button_to_menu_.end()) {
    return button_to_menu_.at(button);
  }
  return Controller::MenuType::kPlanet;
}

void PlanetMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  switch (menu) {
    case Controller::MenuType::kGame: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetMenuType(Controller::MenuType::kGame);
      Controller::GetGameMenu()->Show();
      break;
    }
    case Controller::MenuType::kAttack: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetAttackMenu(new AttackMenu());
      Controller::SetMenuType(Controller::MenuType::kAttack);
      break;
    }
    case Controller::MenuType::kMove: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetMoveMenu(new MoveMenu());
      Controller::SetMenuType(Controller::MenuType::kMove);
      break;
    }
    case Controller::MenuType::kPlanetInfo: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetPlanetInfoMenu(new PlanetInfoMenu());
      Controller::SetMenuType(Controller::MenuType::kPlanetInfo);
      break;
    }
    case Controller::MenuType::kShop: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetShopMenu(new ShopMenu());
      Controller::SetMenuType(Controller::MenuType::kShop);
      break;
    }
    default: {
      break;
    }
  }
}

void PlanetMenu::btnInfoClicked() {
  Controller::SwitchMenu(Controller::MenuType::kPlanetInfo);
}

void PlanetMenu::btnDefaultClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

void PlanetMenu::btnAttackClicked() {
  Controller::SwitchMenu(Controller::MenuType::kAttack);
}

void PlanetMenu::btnMoveClicked() {
  Controller::SwitchMenu(Controller::MenuType::kMove);
}
void PlanetMenu::btnShopClicked() {
  Controller::SwitchMenu(Controller::MenuType::kShop);
}

UnitMenu::UnitMenu() { this->Draw(); }

UnitMenu::~UnitMenu() {}

void UnitMenu::Draw() {}

void UnitMenu::SwitchTo(Controller::MenuType) {}

//-------------------------------- Shop Menu -------------------------
ShopMenu::ShopMenu() {
  double scale = Controller::view->matrix().m11();
  // создание виджетов магазина и меню инфы
  std::map<BuildingType, int32_t> buildings_number;
  std::map<BuildingType, ShopPlanetInfo*> building_info_ptr;
  std::map<UnitType, int32_t> units_number;
  std::map<UnitType, ShopPlanetInfo*> unit_info_ptr;

  for (const BuildingType& building :
       Controller::GetActivePlanet()->GetBuildings()) {
    ++buildings_number[building];
  }
  for (const auto& building : buildings_number) {
    ShopPlanetInfo* building_info = new ShopPlanetInfo(
        kInfoWidth, kInfoHeight,
        ObjectsStorage::GetBuildingCaption(building.first), building.second,
        Loader::GetBuildingImage(building.first));
    info_buildings_.push_back(building_info);
    building_info_ptr[building.first] = building_info;
  }

  for (const UnitType& unit : Controller::GetActivePlanet()->GetUnits()) {
    ++units_number[unit];
  }
  for (const UnitType& unit : Controller::GetActivePlanet()->GetTiredUnits()) {
    ++units_number[unit];
  }
  for (const auto& unit : units_number) {
    ShopPlanetInfo* unit_info = new ShopPlanetInfo(
        kInfoWidth, kInfoHeight, ObjectsStorage::GetUnitCaption(unit.first),
        unit.second, Loader::GetUnitImage(unit.first));
    info_units_.push_back(unit_info);
    unit_info_ptr[unit.first] = unit_info;
  }

  for (const UnitType& unit :
       Controller::GetActivePlanet()->GetAvailableUnits()) {
    auto unit_data_ptr = ObjectsStorage::GetUnit(unit);
    if (units_number.find(unit) == units_number.end()) {
      ShopPlanetInfo* unit_info = new ShopPlanetInfo(
          kInfoWidth, kInfoHeight, unit_data_ptr->GetCaption(), 0,
          Loader::GetUnitImage(unit));
      info_units_.push_back(unit_info);
      unit_info_ptr[unit] = unit_info;
    }

    ShopWidget* unit_widget =
        new ShopWidget(kWidgetWidth, kWidgetHeight, ShopItemType::kUnit,
                       unit_data_ptr->GetCaption(), unit_data_ptr->GetCost(),
                       unit_info_ptr[unit]);
    shop_units_.push_back(unit_widget);
  }
  for (const BuildingType& building :
       Controller::GetActivePlanet()->GetAvailableBuildings()) {
    auto buildings_data_ptr = ObjectsStorage::GetBuilding(building);
    // Картинка посторйки, которая будет находиться слева
    if (buildings_number.find(building) == buildings_number.end()) {
      ShopPlanetInfo* building_info = new ShopPlanetInfo(
          kInfoWidth, kInfoHeight, buildings_data_ptr->GetCaption(), 0,
          Loader::GetBuildingImage(building));
      info_buildings_.push_back(building_info);
      building_info_ptr[building] = building_info;
    }
    // виджет постройки, которую можно купить
    ShopWidget* building_widget = new ShopWidget(
        kWidgetWidth, kWidgetHeight, ShopItemType::kBuilding,
        buildings_data_ptr->GetCaption(), buildings_data_ptr->GetCost(),
        building_info_ptr[building]);
    shop_buildings_.push_back(building_widget);
  }

  std::sort(info_units_.begin(), info_units_.end(),
            [](ShopPlanetInfo* l, ShopPlanetInfo* r) {
              return l->GetQuant() > r->GetQuant();
            });
  std::sort(info_buildings_.begin(), info_buildings_.end(),
            [](ShopPlanetInfo* l, ShopPlanetInfo* r) {
              return l->GetQuant() > r->GetQuant();
            });

  // создание всего прочего
  border_line_ = new QGraphicsLineItem();
  text_ = new QGraphicsSimpleTextItem();
  exit_bnt_ = new ButtonItem(static_cast<int32_t>(kExitBtnSize / scale),
                             static_cast<int32_t>(kExitBtnSize / scale), false);
  exit_bnt_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kEscapeButton));

  units_btn_ = new ButtonItem(static_cast<int32_t>(kBtnWidth / scale),
                              static_cast<int32_t>(kBtnHeight / scale), false);
  units_btn_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kUnitsButton));

  buildings_btn_ = new ButtonItem(static_cast<int32_t>(kBtnWidth / scale),
                                  static_cast<int32_t>(kBtnHeight / scale), false);
  buildings_btn_->SetPixmap(
      Loader::GetButtonImage(ButtonsEnum::kBuildingsButton));

  connect(exit_bnt_, SIGNAL(clicked()), this, SLOT(Close()));
  connect(units_btn_, SIGNAL(clicked()), this, SLOT(ChangeShop()));
  connect(buildings_btn_, SIGNAL(clicked()), this, SLOT(ChangeShop()));

  this->Draw();
}

ShopMenu::~ShopMenu() {
  Controller::scene->removeItem(background_image_);
  Controller::scene->removeItem(border_line_);
  Controller::scene->removeItem(exit_bnt_);
  Controller::scene->removeItem(units_btn_);
  Controller::scene->removeItem(buildings_btn_);
  Controller::scene->removeItem(text_);

  delete exit_bnt_;
  delete text_;
  delete units_btn_;
  delete buildings_btn_;
  delete background_image_;
  delete border_line_;

  for (auto el : shop_units_) {
    el->deleteLater();
  }
  for (auto el : shop_buildings_) {
    el->deleteLater();
  }
  for (auto el : info_units_) {
    el->deleteLater();
  }
  for (auto el : info_buildings_) {
    el->deleteLater();
  }

  shop_scrolling_view_->deleteLater();
  shop_scene_scroll_->deleteLater();
  info_scrolling_view_->deleteLater();
  info_scene_scroll_->deleteLater();
}

void ShopMenu::Draw() {
  double scale = Controller::view->matrix().m11();

  QSizeF scale_size(Controller::scene->GetWidth(), Controller::scene->GetHeight());
  scale_size *= kShopSizeCoefficient / scale;
  QSizeF real_size(kWidth, kHeight);
  real_size *= kShopSizeCoefficient;

  QPointF top_left_cor(Controller::GetActivePlanet()->GetCoordinates() -
                       QPointF(scale_size.width() / 2, scale_size.height() / 2));
  QPointF top_right_cor = top_left_cor + QPointF(scale_size.width(), 0);

  QRectF background(top_left_cor, scale_size);

  background_image_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kMenuBackground), background);

  border_line_->setPen(QPen(Qt::green));
  border_line_->setLine(top_left_cor.x() + scale_size.width() * kBorderCoefficient,
                        top_left_cor.y(),
                        top_left_cor.x() + scale_size.width() * kBorderCoefficient,
                        top_left_cor.y() + scale_size.height());

  // создание и размещения магазина
  // то окно, в котором можно купить
  shop_scene_scroll_ = new QGraphicsScene;
  QBrush* ibrush = Loader::GetBrush();
  shop_scene_scroll_->setBackgroundBrush(*ibrush);

  shop_scrolling_view_ =
      new ScrollingView(shop_scene_scroll_, Controller::window);
  shop_scrolling_view_->setStyleSheet("border: 0px");

  int32_t x_offset = static_cast<int32_t>(
      real_size.width() * (1 - kBorderCoefficient) * pow(kWidgetWidthCoef, 2));
  int32_t y_offset =
      static_cast<int32_t>(real_size.height() * pow(kWidgetHeightCoef, 2));

  QSizeF shop_size(real_size.width() * (1 - kBorderCoefficient) - 2 * x_offset,
                     real_size.height() - 2 * y_offset);
  QPointF shop_pos(
      kWidth * (1 - kShopSizeCoefficient) / 2 + real_size.width() * kBorderCoefficient,
      kHeight * (1 - kShopSizeCoefficient) / 2);
  shop_pos += QPointF(x_offset, y_offset);

  shop_scrolling_view_->setGeometry(static_cast<int32_t>(shop_pos.x()),
                                    static_cast<int32_t>(shop_pos.y()),
                                    static_cast<int32_t>(shop_size.width()),
                                    static_cast<int32_t>(shop_size.height()));
  int32_t lines = shop_buildings_.size() / kWidthCount;
  lines += shop_buildings_.size() % kWidthCount == 0 ? 1 : 2;
  shop_scrolling_view_->setSceneRect(0, 0, shop_size.width(),
      lines * (kWidgetHeight + y_offset) -
          y_offset);

  // добавление тайлов в магазин
  QPointF start_pos(kWidgetWidth / 2, kWidgetHeight / 2);
  for (auto i = 0; i < shop_units_.size(); ++i) {
    shop_units_[i]->SetScene(shop_scene_scroll_);
    shop_units_[i]->setPos(
        start_pos + QPointF(kWidgetWidth + x_offset, 0) * (i % kWidthCount) +
        QPointF(0, kWidgetHeight + y_offset) * (i / kWidthCount));
    shop_scene_scroll_->addItem(shop_units_[i]);
    shop_units_[i]->WidgetHide();
  }
  for (auto i = 0; i < shop_buildings_.size(); ++i) {
    shop_buildings_[i]->SetScene(shop_scene_scroll_);
    shop_buildings_[i]->setPos(
        start_pos + QPointF(kWidgetWidth + x_offset, 0) * (i % kWidthCount) +
        QPointF(0, kWidgetHeight + y_offset) * (i / kWidthCount));
    shop_scene_scroll_->addItem(shop_buildings_[i]);
  }

  // создание зоны с инфой
  info_scene_scroll_ = new QGraphicsScene();
  info_scene_scroll_->setBackgroundBrush(*ibrush);
  info_scrolling_view_ =
      new ScrollingView(info_scene_scroll_, Controller::window);
  info_scrolling_view_->setStyleSheet("border: 0px");

  QPointF info_pos(kWidth * (1 - kShopSizeCoefficient) / 2,
                   kHeight * (1 - kShopSizeCoefficient) / 2);
  info_pos +=
      QPointF(real_size.width() * kBorderCoefficient * (1 - kShopSizeCoefficient) / 2,
              real_size.height() * (1 - kShopSizeCoefficient) / 2);
  info_scrolling_view_->setGeometry(
      static_cast<int32_t>(info_pos.x()), static_cast<int32_t>(info_pos.y()),
      static_cast<int32_t>(real_size.width() * kBorderCoefficient *
                           kShopSizeCoefficient),
      static_cast<int32_t>(real_size.height() * kShopSizeCoefficient));
  int32_t visible_widgets = 0;

  // добавление виджетов в меню инфы
  for (auto i = 0; i < info_buildings_.size(); ++i) {
    info_buildings_[i]->setPos(
        QPointF(kInfoWidth / 2, kInfoHeight / 2 + kInfoHeight * i));
    info_scene_scroll_->addItem(info_buildings_[i]);
    visible_widgets += info_buildings_[i]->GetQuant() != 0 ? 1 : 0;
  }
  for (auto i = 0; i < info_units_.size(); ++i) {
    info_units_[i]->setPos(
        QPointF(kInfoWidth / 2, kInfoHeight / 2 + kInfoHeight * i));
    info_scene_scroll_->addItem(info_units_[i]);
    info_units_[i]->hide();
  }
  visible_widgets = std::max(kInfoCount, visible_widgets);
  info_scrolling_view_->setSceneRect(
      0, 0, real_size.width() * kBorderCoefficient * kShopSizeCoefficient,
      visible_widgets * kInfoHeight);

  text_->setText("Player info");
  text_->setBrush(Qt::green);
  text_->setPos(top_left_cor);

  exit_bnt_->setPos(top_right_cor + QPointF(-kExitBtnSize, kExitBtnSize) / scale / 2);
  units_btn_->setPos(top_left_cor + QPoint(-kBtnWidth, kBtnHeight) / scale / 2);
  // + 20 для небольшого отступа между вкладками
  buildings_btn_->setPos(top_left_cor +
                         QPointF(-kBtnWidth, 3 * kBtnHeight + 20) / scale / 2);

  QTimer::singleShot(1, this, SLOT(Show()));
}

void ShopMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  Controller::SetShopMenu(nullptr);
  Controller::SetPlanetMenu(new PlanetMenu());
  Controller::SetMenuType(Controller::MenuType::kPlanet);
}

void ShopMenu::Show() {
  SetZValue();
  shop_scrolling_view_->show();
  info_scrolling_view_->show();
  Controller::scene->addItem(background_image_);
  Controller::scene->addItem(border_line_);
  Controller::scene->addItem(text_);
  Controller::scene->addItem(exit_bnt_);
  Controller::scene->addItem(units_btn_);
  Controller::scene->addItem(buildings_btn_);
}

void ShopMenu::Close() { SwitchTo(Controller::MenuType::kPlanet); }

void ShopMenu::ChangeShop() {
  ButtonItem* sender = dynamic_cast<ButtonItem*>(QObject::sender());
  if ((sender == units_btn_ && current_state_ == kUnits) ||
      (sender == buildings_btn_ && current_state_ == kBuildings)) {
    return;
  }

  // Это просто копи-паст из Draw(). Не вижу смысла ещё такое большое кол-во
  // констант выносить
  QSizeF size(kWidth, kHeight);
  size *= kShopSizeCoefficient;
  int32_t x_offset = static_cast<int32_t>(
      size.width() * (1 - kBorderCoefficient) * pow(kWidgetWidthCoef, 2));
  int32_t y_offset =
      static_cast<int32_t>(size.height() * pow(kWidgetHeightCoef, 2));
  QSizeF shop_size(size.width() * (1 - kBorderCoefficient) - 2 * x_offset,
                   size.height() - 2 * y_offset);

  if (current_state_ == kUnits) {
    SwitchState(kBuildings);
    for (const auto& unit_widget : shop_units_) {
      unit_widget->WidgetHide();
    }
    for (const auto& build_widget : shop_buildings_) {
      build_widget->WidgetShow();
    }

    int32_t lines = shop_buildings_.size() / kWidthCount;
    lines += shop_buildings_.size() % kWidthCount == 0 ? 1 : 2;
    shop_scrolling_view_->setSceneRect(
        0, 0, shop_size.width(), lines * (kWidgetHeight + y_offset) - y_offset);

    int32_t visible_widgets = 0;
    for (const auto& unit_info : info_units_) {
      unit_info->hide();
    }
    for (const auto& building_info : info_buildings_) {
      if (building_info->GetQuant() == 0) {
        break;
      }
      ++visible_widgets;
      building_info->show();
    }
    visible_widgets = std::max(kInfoCount, visible_widgets);
    info_scrolling_view_->setSceneRect(
        0, 0, kInfoWidth, visible_widgets * kInfoHeight);
  } else {
    SwitchState(kUnits);
    for (const auto& build_widget : shop_buildings_) {
      build_widget->WidgetHide();
    }
    for (const auto& unit_widget : shop_units_) {
      unit_widget->WidgetShow();
    }

    int32_t lines = shop_units_.size() / kWidthCount;
    lines += shop_units_.size() % kWidthCount == 0 ? 1 : 2;
    shop_scrolling_view_->setSceneRect(
        0, 0, shop_size.width(), lines * (kWidgetHeight + y_offset) - y_offset);

    int32_t visible_widgets = 0;
    for (const auto& building_info : info_buildings_) {
      building_info->hide();
    }
    for (const auto& unit_info : info_units_) {
      if (unit_info->GetQuant() == 0) {
        break;
      }
      ++visible_widgets;
      unit_info->show();
    }
    visible_widgets = std::max(kInfoCount, visible_widgets);
    info_scrolling_view_->setSceneRect(
         0, 0, kInfoWidth, visible_widgets * kInfoHeight);
  }
}

void ShopMenu::SetZValue() {
  background_image_->setZValue(ZValues::kShopMenu);
  border_line_->setZValue(ZValues::kShopMenu);
  text_->setZValue(ZValues::kShopMenu);
  exit_bnt_->setZValue(ZValues::kShopMenu);
  units_btn_->setZValue(ZValues::kShopMenu);
  buildings_btn_->setZValue(ZValues::kShopMenu);
}

void ShopMenu::SwitchState(ShopState state) { current_state_ = state; }

void ShopMenu::MakePurchase(ShopItemType type, Resources cost,
                            QString item_name) {
  Controller::GetActivePlanet()->GetOwner()->SubResources(cost);
  if (type == ShopItemType::kUnit) {
    Controller::GetActivePlanet()->AddUnit(
        ObjectsStorage::GetUnitType(item_name));
  } else {
    Controller::GetActivePlanet()->AddBuilding(
        ObjectsStorage::GetBuildingType(item_name));
  }
  UpdateInfo();
}

void ShopMenu::UpdateInfo() {
  int32_t visible_widgets = 1;
  if (current_state_ == ShopState::kBuildings) {
    std::sort(info_buildings_.begin(), info_buildings_.end(),
              [](ShopPlanetInfo* l, ShopPlanetInfo* r) {
                return l->GetQuant() > r->GetQuant();
              });
    for (auto i = 0; i < info_buildings_.size(); ++i) {
      info_buildings_[i]->setPos(
          QPointF(kInfoWidth / 2, kInfoHeight / 2 + kInfoHeight * i));
      info_buildings_[i]->update();
      if (info_buildings_[i]->GetQuant() != 0) { ++visible_widgets;}
    }
    visible_widgets = std::max(kInfoCount, visible_widgets);
    info_scrolling_view_->setSceneRect(
        0, 0, kInfoWidth, visible_widgets * kInfoHeight);
    return;
  }

  std::sort(info_units_.begin(), info_units_.end(),
            [](ShopPlanetInfo* l, ShopPlanetInfo* r) {
              return l->GetQuant() > r->GetQuant();
            });
  for (auto i = 0; i < info_units_.size(); ++i) {
    info_units_[i]->setPos(
        QPointF(kInfoWidth / 2, kInfoHeight / 2 + kInfoHeight * i));
    info_units_[i]->update();
    if (info_units_[i]->GetQuant() != 0) { ++visible_widgets;}
  }
  visible_widgets = std::max(kInfoCount, visible_widgets);
  info_scrolling_view_->setSceneRect(
      0, 0, kInfoWidth, visible_widgets * kInfoHeight);
}
// ----------------------------- Game Menu -------------------------------
GameMenu::GameMenu() {
  this->StartGame();
  this->Draw();
  connect(btn_next_, SIGNAL(clicked()), Controller::scene, SLOT(Next()));
}

GameMenu::~GameMenu() { Controller::scene->Destroy(); }

void GameMenu::SetZValue() { btn_next_->setZValue(ZValues::kGameMenu); }

void GameMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  Hide();

  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }

  if (menu == Controller::MenuType::kPause) {
    if (Controller::view->EventHandler()->GetMotionType() !=
        EventHandler::View::MotionType::kNoMotion) {
      Show();
      return;
    }
    Controller::SetPauseMenu(new PauseMenu());
    Controller::SetMenuType(Controller::MenuType::kPause);
  }
}

void GameMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn_next_ = new ButtonItem(width / 10, height / 15);
  btn_next_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kNextTurnButton));

  SetZValue();
  btn_next_->setPos(Controller::view->mapToScene(
      QPoint(width, height) - QPoint(width / 8, height / 8)));
  Controller::scene->addItem(btn_next_);
}

void GameMenu::ReDraw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn_next_->setPos(Controller::view->mapToScene(
      QPoint(width, height) - QPoint(width / 8, height / 8)));
}

void GameMenu::StartGame() {
  Controller::view->SetNewGameSettings();
  Controller::scene->NewGame();
}

void GameMenu::Hide() { btn_next_->hide(); }

void GameMenu::Show() {
  btn_next_->show();
  ReDraw();
}

UnitsInteractionMenu::UnitsInteractionMenu() {
  PlanetGraphics* planet_graphics =
      dynamic_cast<PlanetGraphics*>(Controller::scene->itemAt(
          Controller::GetActivePlanet()->GetCoordinates() * 2, QTransform()));
  const double kScale = Controller::view->matrix().m11();
  if (planet_graphics != nullptr) {
    int32_t planet_info_width = static_cast<int32_t>(
        kWidth * kSizeCoefficient / kScale - kWidth / 3 / kScale +
        kWidth * (1 - kSizeCoefficient) / 2);
    int32_t planet_info_height = static_cast<int32_t>(
        (kHeight * (1 - 2 * kScrollPosition) - kHeight / 10) / kScale);

    planet_info_ = new PlanetInfoGraphics(
        planet_graphics->GetImage(), planet_info_width, planet_info_height);
  }

  interaction_button_ = new ButtonItem(kButtonWidth, kButtonHeight);
  interaction_button_->SetPixmap(
      Loader::GetButtonImage(ButtonsEnum::kBeautifulAttackButton));
  cancel_button_ = new ButtonItem(kButtonWidth, kButtonHeight);
  cancel_button_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kCancelButton));

  std::map<Planet*, QVector<UnitType>> nearest_units =
      Controller::scene->GetNearestUnits(Controller::scene->GetPlayer());
  for (const auto& planet_to_units : nearest_units) {
    for (const auto& unit : planet_to_units.second) {
      unit_widgets_.push_back(std::make_shared<UnitWidget>(
          this, planet_to_units.first, unit, kUnitCellWidth, kUnitCellHeight));
    }
  }
  connect(cancel_button_, SIGNAL(clicked()), this, SLOT(Close()));
  connect(interaction_button_, SIGNAL(clicked()), this, SLOT(Interact()));
  Draw();
}

UnitsInteractionMenu::~UnitsInteractionMenu() { Destroy(); }

void UnitsInteractionMenu::SetZValue() {
  background_image_->setZValue(ZValues::kInteractionMenu);
  interaction_button_->setZValue(ZValues::kInteractionMenu);
  cancel_button_->setZValue(ZValues::kInteractionMenu);
  planet_info_->setZValue(ZValues::kInteractionMenu);
}

void UnitsInteractionMenu::Draw() {
  QPointF coordinates = Controller::GetActivePlanet()->GetCoordinates();
  QSize size(Controller::scene->GetWidth(), Controller::scene->GetHeight());
  size *= kSizeCoefficient / Controller::view->matrix().m11();

  QRectF background_rect(
      2 * (coordinates - QPointF(size.width(), size.height()) / 4), size);
  background_image_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kMenuBackground), background_rect);

  scroll_scene_ = new QGraphicsScene();

  QBrush* ibrush = Loader::GetBrush();
  scroll_scene_->setBackgroundBrush(*ibrush);

  scroll_view_ = new ScrollingView(scroll_scene_, Controller::window);
  // Выставление положение и размер относительно экрана,
  // т.е. левый верхний угол - (0, 0)
  scroll_view_->setGeometry(
      static_cast<int32_t>(kScrollPosition * kWidth),
      static_cast<int32_t>(kScrollPosition * kHeight), kUnitCellWidth + 5,
      std::min(kUnitCellHeight * unit_widgets_.size(),
               static_cast<int32_t>(kHeight * (1 - 2 * kScrollPosition) + 1)));
  // Область внутри, которая скроллится, выставляется в зависимости от размеров
  // и количества виджетов
  scroll_view_->setSceneRect(0, 0, kUnitCellWidth + 5,
                             kUnitCellHeight * unit_widgets_.size() + 1);
  scroll_view_->setStyleSheet("border: 0px");
  int32_t y = 0;
  for (const auto& unit : unit_widgets_) {
    unit.get()->setPos(0, y);
    scroll_scene_->addItem(unit.get());
    y += kUnitCellHeight / 2;
  }

  const int32_t distance_between = static_cast<int32_t>(
      (background_rect.width() * Controller::view->matrix().m11() -
       ((kScrollPosition - (1 - kSizeCoefficient) / 2) * kWidth +
        kUnitCellWidth + 2 * kButtonWidth)) /
      3);
  const int32_t attack_x =
      static_cast<int32_t>(kWidth * kScrollPosition + distance_between +
                           kUnitCellWidth + kButtonWidth / 2);
  const int32_t attack_y = static_cast<int32_t>(
      kScrollPosition * kHeight + kHeight * (1 - 2 * kScrollPosition) -
      kButtonHeight + kButtonHeight / 2);

  interaction_button_->setPos(Controller::view->mapToScene(attack_x, attack_y));
  interaction_button_->SetEnabled(false);
  cancel_button_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(attack_x + kButtonWidth + distance_between),
      attack_y));

  planet_info_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(kWidth / 2 - kWidth / 6),
      static_cast<int32_t>(kScrollPosition * kHeight)));

  // !!!ATTENTION!!!
  // Все добавления на сцену делать в Show(), иначе фризит
  QTimer::singleShot(1, this, SLOT(Show()));
}

void UnitsInteractionMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  if (current_state_ == State::kResult) {
    CloseResult();
  }
  Switch(menu);
}

void UnitsInteractionMenu::ChooseUnit(UnitWidget* unit) {
  if (chosen_units_.size() == 0) {
    interaction_button_->SetEnabled(true);
  }
  for (const auto& another_unit : unit_widgets_) {
    double another_y = another_unit.get()->y();
    if (another_y < unit->y() && another_y >= last_chosen_y_) {
      another_unit.get()->setY(another_y + kUnitCellHeight / 2);
      another_unit.get()->update();
    }
  }
  unit->setY(last_chosen_y_);
  last_chosen_y_ += kUnitCellHeight / 2;
  chosen_units_.push_back(unit);
}

void UnitsInteractionMenu::RemoveUnit(UnitWidget* unit) {
  for (const auto& another_unit : unit_widgets_) {
    double another_y = another_unit.get()->y();
    if (another_y > unit->y() && another_y < last_chosen_y_) {
      another_unit.get()->setY(another_y - kUnitCellHeight / 2);
      another_unit.get()->update();
    }
  }
  last_chosen_y_ -= kUnitCellHeight / 2;
  unit->setY(last_chosen_y_);
  chosen_units_.removeOne(unit);
  if (chosen_units_.size() == 0) {
    interaction_button_->SetEnabled(false);
  }
}

void UnitsInteractionMenu::ShowAttackResult(
    const std::map<UnitType, int32_t>& units_to_quantity, const QString& result,
    const QString& caption) {
  attack_result_ = new AttackResultWindow(units_to_quantity, result, caption,
                                          kResultWidth, kResultHeight);
  attack_result_->setPos(Controller::view->mapToScene(kWidth / 4, kHeight / 4));
  attack_result_->setZValue(ZValues::kInteractionMenu);

  result_button_ =
      new ButtonItem(kButtonWidth, kButtonHeight - kButtonHeight / 3, true);
  result_button_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kExitButton));
  result_button_->setPos(Controller::view->mapToScene(
      static_cast<int32_t>(kWidth / 2),
      static_cast<int32_t>(kHeight / 4 +
                           kResultHeight * Controller::view->matrix().m11() -
                           kHeight / 15)));

  result_button_->setZValue(ZValues::kInteractionMenu);
  connect(result_button_, SIGNAL(clicked()), this, SLOT(Close()));

  Hide();

  Controller::scene->addItem(attack_result_);
  Controller::scene->addItem(result_button_);
}

void UnitsInteractionMenu::Hide() {
  scroll_view_->hide();
  background_image_->hide();
  interaction_button_->hide();
  cancel_button_->hide();
  planet_info_->hide();
}

void UnitsInteractionMenu::Show() {
  SetZValue();
  scroll_view_->show();
  Controller::scene->addItem(background_image_);
  Controller::scene->addItem(planet_info_);
  Controller::scene->addItem(interaction_button_);
  Controller::scene->addItem(cancel_button_);
}

void UnitsInteractionMenu::Destroy() {
  scroll_scene_->deleteLater();
  scroll_view_->deleteLater();
  Controller::scene->removeItem(cancel_button_);
  Controller::scene->removeItem(interaction_button_);
  Controller::scene->removeItem(planet_info_);
  Controller::scene->removeItem(background_image_);

  delete cancel_button_;
  delete interaction_button_;
  delete planet_info_;
  delete background_image_;
}

void UnitsInteractionMenu::Close() { SwitchTo(Controller::MenuType::kPlanet); }

void UnitsInteractionMenu::CloseResult() {
  Controller::scene->removeItem(attack_result_);
  Controller::scene->removeItem(result_button_);
}

AttackMenu::AttackMenu() : UnitsInteractionMenu() {
  // TODO
  //   interaction_button_->SetPixmap()
}

void AttackMenu::Interact() {
  current_state_ = State::kResult;
  std::map<Planet*, QVector<UnitType>> planets_to_units;
  for (const auto& unit_widget : chosen_units_) {
    planets_to_units[unit_widget->GetPlanet()].push_back(
        unit_widget->GetUnit());
  }

  std::map<UnitType, int32_t> units_to_quantity;
  for (const auto& planet_to_unit : planets_to_units) {
    for (UnitType unit : planet_to_unit.second) {
      if (units_to_quantity.find(unit) == units_to_quantity.end()) {
        units_to_quantity[unit] = 1;
        continue;
      }
      units_to_quantity[unit]++;
    }
  }

  std::map<Planet*, QVector<UnitType>> all_nearest_units;
  for (const auto& planet_to_unit : planets_to_units) {
    all_nearest_units[planet_to_unit.first] = planet_to_unit.first->GetUnits();
  }
  bool is_win = Controller::GetActivePlanet()->TakeAttack(planets_to_units);
  if (is_win) {
    for (UnitType unit : Controller::GetActivePlanet()->GetTiredUnits()) {
      units_to_quantity[unit]--;
      if (units_to_quantity[unit] == 0) {
        units_to_quantity.erase(unit);
      }
    }
    ShowAttackResult(units_to_quantity, "Winner", "They fought to the last");
  } else {
    for (const auto& planet_to_unit : planets_to_units) {
      for (UnitType unit : planet_to_unit.first->GetTiredUnits()) {
        all_nearest_units[planet_to_unit.first].removeOne(unit);
      }
      for (UnitType unit : planet_to_unit.first->GetUnits()) {
        all_nearest_units[planet_to_unit.first].removeOne(unit);
      }
    }
    std::map<UnitType, int32_t> units_to_quantity;
    for (const auto& planet_to_unit : all_nearest_units) {
      for (UnitType unit : planet_to_unit.second) {
        if (units_to_quantity.find(unit) == units_to_quantity.end()) {
          units_to_quantity[unit] = 1;
          continue;
        }
        units_to_quantity[unit]++;
      }
    }
    ShowAttackResult(units_to_quantity, "Loser", "We won't forget them");
  }
}

void AttackMenu::Switch(Controller::MenuType menu) {
  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetAttackMenu(nullptr);
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }
}

MoveMenu::MoveMenu() : UnitsInteractionMenu() {
  // TODO
  // interaction_button_->SetPixmap();
}

void MoveMenu::Interact() {
  std::map<Planet*, QVector<UnitType>> planets_to_units;
  for (UnitWidget* unit : chosen_units_) {
    planets_to_units[unit->GetPlanet()].push_back(unit->GetUnit());
  }
  Controller::GetActivePlanet()->MoveUnits(planets_to_units);
  Close();
}

void MoveMenu::Switch(Controller::MenuType menu) {
  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetMoveMenu(nullptr);
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }
}

PlanetInfoMenu::PlanetInfoMenu() {
  if (Controller::GetActivePlanet()->GetOwner() ==
      Controller::scene->GetPlayer()) {
    upgrade_button_ = new ButtonItem(kButtonWidth, kButtonHeight, true);
    connect(upgrade_button_, SIGNAL(clicked()), this, SLOT(Upgrade()));
  }

  exit_button_ = new ButtonItem(kButtonWidth, kButtonHeight, true);
  connect(exit_button_, SIGNAL(clicked()), this, SLOT(Exit()));

  Draw();
}

PlanetInfoMenu::~PlanetInfoMenu() { Destroy(); }

void PlanetInfoMenu::SetZValue() {
  background_->setZValue(ZValues::kPlanetInfo);
  if (upgrade_button_ != nullptr) {
    upgrade_button_->setZValue(ZValues::kPlanetInfo);
  }
  exit_button_->setZValue(ZValues::kPlanetInfo);
  planet_info_->setZValue(ZValues::kPlanetInfo);
}

void PlanetInfoMenu::Draw() {
  const double kScale = Controller::view->matrix().m11();
  QPointF coordinates = Controller::GetActivePlanet()->GetCoordinates();
  QSize size(Controller::scene->GetWidth(), Controller::scene->GetHeight());
  size *= kSizeCoefficient / kScale;

  QRectF background_rect(
      2 * (coordinates - QPointF(size.width(), size.height()) / 4), size);
  background_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kMenuBackground), background_rect);

  Controller::scene->addItem(background_);

  const double kLeftTopCornerCoeffient = (1 - kSizeCoefficient) / 2;
  const int32_t upgrade_x = static_cast<int32_t>(
      kLeftTopCornerCoeffient * kWidth + background_rect.width() * kScale / 6);
  const int32_t button_y = static_cast<int32_t>(
      2 * kLeftTopCornerCoeffient * kHeight +
      background_rect.height() * kScale - kButtonHeight - kHeight / 30);

  if (upgrade_button_ != nullptr) {
    upgrade_button_->setPos(Controller::view->mapToScene(upgrade_x, button_y));
    upgrade_button_->SetPixmap(
        Loader::GetButtonImage(ButtonsEnum::kUpgradActiveButton));
    Controller::scene->addItem(upgrade_button_);
    Resources upgrade = Controller::GetActivePlanet()->GetUpgradeCost();
    if (!(Controller::GetActivePlanet()->GetOwner()->GetTools() >
              upgrade.GetTools() &&
          Controller::GetActivePlanet()->GetOwner()->GetBatteries() >
              upgrade.GetBatteries())) {
      upgrade_button_->SetEnabled(false);
      upgrade_button_->SetPixmap(
          Loader::GetButtonImage(ButtonsEnum::kUpgradeUnactiveButton));
    }
  }

  const int32_t exit_x = static_cast<int32_t>(
      upgrade_x + background_rect.width() * kScale * 2 / 3);
  exit_button_->setPos(Controller::view->mapToScene(exit_x, button_y));
  exit_button_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kCancelButton));
  Controller::scene->addItem(exit_button_);

  planet_info_ = new FullPlanetInfo(
      static_cast<int32_t>(background_rect.width() * kScale),
      static_cast<int32_t>(background_rect.height() * kScale - kHeight / 30 -
                           kButtonHeight),
      Controller::GetActivePlanet());
  planet_info_->setPos(Controller::view->mapToScene(
      (upgrade_x + exit_x) / 2,
      static_cast<int32_t>((button_y + kLeftTopCornerCoeffient * kHeight) / 2 -
                           kHeight / 20)));

  Controller::scene->addItem(planet_info_);
  SetZValue();
}

void PlanetInfoMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kPlanet) {
    Controller::SetPlanetInfoMenu(nullptr);
    Controller::SetPlanetMenu(new PlanetMenu());
    Controller::SetMenuType(Controller::MenuType::kPlanet);
  }
}

void PlanetInfoMenu::Destroy() {
  if (upgrade_button_ != nullptr) {
    Controller::scene->removeItem(upgrade_button_);
  }
  Controller::scene->removeItem(exit_button_);
  Controller::scene->removeItem(planet_info_);
  Controller::scene->removeItem(background_);
  delete exit_button_;
  delete planet_info_;
  delete background_;
}

void PlanetInfoMenu::Upgrade() {
  Controller::GetActivePlanet()->Upgrade();
  planet_info_->SetLevel(Controller::GetActivePlanet()->GetLevel());
  if (Controller::scene->GetPlayer()->GetResources() <
      Controller::GetActivePlanet()->GetUpgradeCost()) {
    upgrade_button_->SetEnabled(false);
    upgrade_button_->SetPixmap(
        Loader::GetButtonImage(ButtonsEnum::kUpgradeUnactiveButton));
    upgrade_button_->update();
  }
}

void PlanetInfoMenu::Exit() { SwitchTo(Controller::MenuType::kPlanet); }
