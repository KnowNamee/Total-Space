#include "menu.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMediaPlayer>
#include <QPushButton>
#include <QTimer>
#include <algorithm>
#include <cmath>
#include <memory>

#include "core/keyhandler.h"
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
#include "graphics/statusbar.h"
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
  connect(btn_settings_, SIGNAL(clicked()), this, SLOT(btnSettingsClicked()));
}

MainMenu::~MainMenu() {
  Controller::scene->removeItem(txt_total_space_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(btn_new_game_);
  Controller::scene->removeItem(btn_settings_);

  delete txt_total_space_;
  delete btn_exit_;
  delete btn_new_game_;
  delete btn_settings_;
}

void MainMenu::SetZValue() {
  btn_exit_->setZValue(ZValues::kMainMenu);
  txt_total_space_->setZValue(ZValues::kMainMenu);
  btn_new_game_->setZValue(ZValues::kMainMenu);
  btn_settings_->setZValue(ZValues::kMainMenu);
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

  btn_settings_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);

  txt_total_space_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kBackground),
      static_cast<int>(kWidth + 10), static_cast<int>(kHeight + 10));

  SetZValue();

  Controller::scene->addItem(txt_total_space_);
  Controller::scene->addItem(btn_exit_);
  Controller::scene->addItem(btn_new_game_);
  Controller::scene->addItem(btn_settings_);

  btn_new_game_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 - kHeight / 30)));
  btn_exit_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 12)));
  btn_settings_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 6)));

  txt_total_space_->setPos(Controller::view->sceneRect().center() / 2);
}

void MainMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kGame) {
    Controller::SetMainMenu(nullptr);
    Controller::SetGameMenu(new GameMenu());
  }

  if (menu == Controller::MenuType::kSettings) {
    Hide();
    Controller::SetSettingsMenu(new SettingsMenu());
    Controller::GetSettingsMenu()->SetPrevMenu(Controller::MenuType::kMain);
  }

  Controller::SetMenuType(menu);
}

void MainMenu::Hide() {
  txt_total_space_->hide();
  btn_exit_->hide();
  btn_new_game_->hide();
  btn_settings_->hide();
}

void MainMenu::Show() {
  txt_total_space_->show();
  btn_exit_->show();
  btn_new_game_->show();
  btn_settings_->show();
}

void MainMenu::btnNewGameClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

void MainMenu::btnSettingsClicked() {
  Controller::SwitchMenu(Controller::MenuType::kSettings);
}

PauseMenu::PauseMenu() {
  this->Draw();
  connect(btn_back_, SIGNAL(clicked()), this, SLOT(btnBackClicked()));
  connect(btn_exit_, SIGNAL(clicked()), this, SLOT(btnExitClicked()));
  connect(btn_settings_, SIGNAL(clicked()), this, SLOT(btnSettingsClicked()));

  Controller::MenuType type = Controller::MenuType::kPause;
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
}

PauseMenu::~PauseMenu() {
  Controller::scene->removeItem(btn_back_);
  Controller::scene->removeItem(btn_exit_);
  Controller::scene->removeItem(btn_settings_);
  Controller::scene->removeItem(background_rect_);

  delete btn_back_;
  delete btn_exit_;
  delete background_rect_;
  delete btn_settings_;
}

void PauseMenu::SetZValue() {
  background_rect_->setZValue(ZValues::kPauseMenu);
  btn_back_->setZValue(ZValues::kPauseMenu);
  btn_exit_->setZValue(ZValues::kPauseMenu);
  btn_settings_->setZValue(ZValues::kPauseMenu);
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
  btn_settings_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);

  SetZValue();

  Controller::scene->addItem(background_rect_);
  Controller::scene->addItem(btn_back_);
  Controller::scene->addItem(btn_exit_);
  Controller::scene->addItem(btn_settings_);

  btn_back_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 - kHeight / 30)));
  btn_exit_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 12)));
  btn_settings_->setPos(Controller::view->mapToScene(
      QPoint(kWidth / 2, kHeight / 2 + kHeight / 6)));
}

void PauseMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  if (menu == Controller::MenuType::kGame) {
    Controller::SetPauseMenu(nullptr);
    Controller::GetGameMenu()->Show();
  } else if (menu == Controller::MenuType::kMain) {
    Controller::SetPauseMenu(nullptr);
    Controller::SetGameMenu(nullptr);
    Controller::SetMainMenu(new MainMenu());
  } else if (menu == Controller::MenuType::kSettings) {
    Hide();
    Controller::SetSettingsMenu(new SettingsMenu());
    Controller::GetSettingsMenu()->SetPrevMenu(Controller::MenuType::kPause);
  }

  Controller::SetMenuType(menu);
}

void PauseMenu::Hide() {
  btn_back_->hide();
  btn_exit_->hide();
  background_rect_->hide();
  btn_settings_->hide();
}

void PauseMenu::Show() {
  btn_back_->show();
  btn_exit_->show();
  background_rect_->show();
  btn_settings_->show();
}

void PauseMenu::btnBackClicked() {
  Controller::SwitchMenu(Controller::MenuType::kGame);
}

void PauseMenu::btnExitClicked() {
  Controller::SwitchMenu(Controller::MenuType::kMain);
}

void PauseMenu::btnSettingsClicked() {
  Controller::SwitchMenu(Controller::MenuType::kSettings);
}

void PauseMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kGame);
  }
}

PlanetMenu::PlanetMenu() {
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Controller::MenuType type = Controller::MenuType::kPlanet;
  Qt::Key key_btn1 = key_handler->Get(type, Qt::Key_W).key;
  Qt::Key key_btn2 = key_handler->Get(type, Qt::Key_D).key;
  Qt::Key key_btn3 = key_handler->Get(type, Qt::Key_A).key;
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;

  if (Controller::GetActivePlanet() == nullptr ||
      Controller::GetActivePlanet()->GetOwner() !=
          Controller::scene->GetPlayer()) {
    btn1_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn2_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn3_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);

    btn1_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kAttackButton));
    btn2_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kInfoButton));
    btn3_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kSimpleButton));

    shortcuts_[key_btn1] =
        std::make_shared<QShortcut>(key_btn1, Controller::window);
    shortcuts_[key_btn3] =
        std::make_shared<QShortcut>(key_btn3, Controller::window);
    shortcuts_[key_btn2] =
        std::make_shared<QShortcut>(key_btn2, Controller::window);

    connect(shortcuts_[key_btn1].get(), SIGNAL(activated()), this,
            SLOT(keyAttackReleased()));
    connect(shortcuts_[key_btn2].get(), SIGNAL(activated()), this,
            SLOT(keyInfoReleased()));
    connect(shortcuts_[key_btn3].get(), SIGNAL(activated()), this,
            SLOT(keyEscapeReleased()));
    btn3_->hide();
    connect(btn1_, SIGNAL(clicked()), this, SLOT(btnAttackClicked()));
    connect(btn2_, SIGNAL(clicked()), this, SLOT(btnInfoClicked()));
    connect(btn3_, SIGNAL(clicked()), this, SLOT(btnDefaultClicked()));
  } else {
    btn1_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);
    btn2_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);

    btn3_ =
        new ButtonItem(kPlanetMenuButtonWidth, kPlanetMenuButtonHeight, false);

    btn1_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kMoveButton));
    btn2_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kInfoButton));
    btn3_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kShopButton));

    shortcuts_[key_btn1] =
        std::make_shared<QShortcut>(key_btn1, Controller::window);
    shortcuts_[key_btn3] =
        std::make_shared<QShortcut>(key_btn3, Controller::window);
    shortcuts_[key_btn2] =
        std::make_shared<QShortcut>(key_btn2, Controller::window);

    connect(shortcuts_[key_btn1].get(), SIGNAL(activated()), this,
            SLOT(keyMoveReleased()));
    connect(shortcuts_[key_btn2].get(), SIGNAL(activated()), this,
            SLOT(keyInfoReleased()));
    connect(shortcuts_[key_btn3].get(), SIGNAL(activated()), this,
            SLOT(keyShopReleased()));
    connect(btn1_, SIGNAL(clicked()), this, SLOT(btnMoveClicked()));
    connect(btn2_, SIGNAL(clicked()), this, SLOT(btnInfoClicked()));
    connect(btn3_, SIGNAL(clicked()), this, SLOT(btnShopClicked()));
  }
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
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

void PlanetMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }

  switch (menu) {
    case Controller::MenuType::kGame: {
      Controller::SetPlanetMenu(nullptr);
      Controller::GetGameMenu()->Show();
      break;
    }
    case Controller::MenuType::kAttack: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetAttackMenu(new AttackMenu());
      break;
    }
    case Controller::MenuType::kMove: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetMoveMenu(new MoveMenu());
      break;
    }
    case Controller::MenuType::kPlanetInfo: {
      Controller::SetPlanetMenu(nullptr);
      Controller::SetPlanetInfoMenu(new PlanetInfoMenu());
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

  Controller::SetMenuType(menu);
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

void PlanetMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kGame);
  }
}

void PlanetMenu::keyInfoReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kPlanetInfo);
  }
}

void PlanetMenu::keyAttackReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kAttack);
  }
}

void PlanetMenu::keyMoveReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kMove);
  }
}

void PlanetMenu::keyShopReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kShop);
  }
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
  BuildingType building = Controller::GetActivePlanet()->GetCurrentBuilding();
  if (building != BuildingType::kNoBuilding) {
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

  buildings_btn_ =
      new ButtonItem(static_cast<int32_t>(kBtnWidth / scale),
                     static_cast<int32_t>(kBtnHeight / scale), false);
  buildings_btn_->SetPixmap(
      Loader::GetButtonImage(ButtonsEnum::kBuildingsButton));

  connect(exit_bnt_, SIGNAL(clicked()), this, SLOT(Close()));
  connect(units_btn_, SIGNAL(clicked()), this, SLOT(ChangeShop()));
  connect(buildings_btn_, SIGNAL(clicked()), this, SLOT(ChangeShop()));

  Controller::MenuType type = Controller::MenuType::kShop;
  Qt::Key key_esc = Controller::GetKeyHandler()->Get(type, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);

  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));

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

  QSizeF scale_size(Controller::scene->GetWidth(),
                    Controller::scene->GetHeight());
  scale_size *= kShopSizeCoefficient / scale;
  QSizeF real_size(kWidth, kHeight);
  real_size *= kShopSizeCoefficient;

  QPointF top_left_cor(
      Controller::GetActivePlanet()->GetCoordinates() -
      QPointF(scale_size.width() / 2, scale_size.height() / 2));
  QPointF top_right_cor = top_left_cor + QPointF(scale_size.width(), 0);

  QRectF background(top_left_cor, scale_size);

  background_image_ = new ImageItem(
      Loader::GetButtonImage(ButtonsEnum::kMenuBackground), background);

  border_line_->setPen(QPen(Qt::green));
  border_line_->setLine(
      top_left_cor.x() + scale_size.width() * kBorderCoefficient,
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
  QPointF shop_pos(kWidth * (1 - kShopSizeCoefficient) / 2 +
                       real_size.width() * kBorderCoefficient,
                   kHeight * (1 - kShopSizeCoefficient) / 2);

  shop_pos += QPointF(x_offset, y_offset);

  shop_scrolling_view_->setGeometry(static_cast<int32_t>(shop_pos.x()),
                                    static_cast<int32_t>(shop_pos.y()),
                                    static_cast<int32_t>(shop_size.width()),
                                    static_cast<int32_t>(shop_size.height()));
  int32_t lines = shop_buildings_.size() / kWidthCount;
  lines += shop_buildings_.size() % kWidthCount == 0 ? 1 : 2;
  shop_scrolling_view_->setSceneRect(
      0, 0, shop_size.width(), lines * (kWidgetHeight + y_offset) - y_offset);

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
  info_pos += QPointF(
      real_size.width() * kBorderCoefficient * (1 - kShopSizeCoefficient) / 2,
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

  exit_bnt_->setPos(top_right_cor +
                    QPointF(-kExitBtnSize, kExitBtnSize) / scale / 2);
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

void ShopMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kPlanet);
  }
}

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
    info_scrolling_view_->setSceneRect(0, 0, kInfoWidth,
                                       visible_widgets * kInfoHeight);
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
    info_scrolling_view_->setSceneRect(0, 0, kInfoWidth,
                                       visible_widgets * kInfoHeight);
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

void ShopMenu::MakePurchase(ShopItemType type, QString item_name) {
  if (type == ShopItemType::kUnit) {
    Controller::GetActivePlanet()->BuyUnit(
        ObjectsStorage::GetUnitType(item_name));
  } else {
    Controller::GetActivePlanet()->BuyBuilding(
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
      if (info_buildings_[i]->GetQuant() != 0) {
        ++visible_widgets;
      }
    }
    visible_widgets = std::max(kInfoCount, visible_widgets);
    info_scrolling_view_->setSceneRect(0, 0, kInfoWidth,
                                       visible_widgets * kInfoHeight);
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
    if (info_units_[i]->GetQuant() != 0) {
      ++visible_widgets;
    }
  }
  visible_widgets = std::max(kInfoCount, visible_widgets);
  info_scrolling_view_->setSceneRect(0, 0, kInfoWidth,
                                     visible_widgets * kInfoHeight);
}
// ----------------------------- Game Menu -------------------------------
GameMenu::GameMenu() {
  Controller::SetMenuType(Controller::MenuType::kLoad);

  this->StartGame();
  this->Draw();

  Controller::SetMenuType(Controller::MenuType::kGame);

  Controller::MenuType type = Controller::MenuType::kGame;
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;
  Qt::Key key_next_turn = key_handler->Get(type, Qt::Key_N).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);
  shortcuts_[key_next_turn] =
      std::make_shared<QShortcut>(key_next_turn, Controller::window);

  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
  connect(shortcuts_[key_next_turn].get(), SIGNAL(activated()), this,
          SLOT(keyNextReleased()));
  connect(btn_next_, SIGNAL(clicked()), Controller::scene, SLOT(Next()));
}

GameMenu::~GameMenu() { Controller::scene->Destroy(); }

void GameMenu::SetZValue() {
  btn_next_->setZValue(ZValues::kGameMenu);
  status_bar_->setZValue(ZValues::kGameMenu);
}

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
    QCoreApplication::processEvents();
    Controller::SetMenuType(Controller::MenuType::kPause);
  }

  if (menu == Controller::MenuType::kMain) {
    Controller::SetGameMenu(nullptr);
    Controller::SetMainMenu(new MainMenu());
    Controller::SetMenuType(Controller::MenuType::kMain);
  }
}

void GameMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn_next_ = new ButtonItem(width / 10, height / 15);
  btn_next_->SetPixmap(Loader::GetButtonImage(ButtonsEnum::kNextTurnButton));

  status_bar_ =
      new StatusBar(width / kStatusWidthCoef, height / kStatusHeightCoef);

  SetZValue();
  btn_next_->setPos(Controller::view->mapToScene(
      QPoint(width, height) - QPoint(width / 8, height / 8)));
  status_bar_->setPos(Controller::view->mapToScene(QPoint(
      width - width / kStatusWidthCoef / 2, height / kStatusHeightCoef / 2)));

  Controller::scene->addItem(btn_next_);
  Controller::scene->addItem(status_bar_);
}

void GameMenu::ReDraw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn_next_->setPos(Controller::view->mapToScene(
      QPoint(width, height) - QPoint(width / 8, height / 8)));
  status_bar_->setPos(Controller::view->mapToScene(QPoint(
      width - width / kStatusWidthCoef / 2, height / kStatusHeightCoef / 2)));
}

void GameMenu::UpdateStatusBar() { status_bar_->update(); }

void GameMenu::ShowWinMessage() {
  int32_t w = qApp->screens()[0]->size().width();
  int32_t h = qApp->screens()[0]->size().height();

  result_msg_ = new ButtonItem(w, h);
  result_msg_->setZValue(ZValues::kResultMessage);
  Controller::scene->addItem(result_msg_);
  Controller::view->DisableMotion();

  connect(result_msg_, SIGNAL(clicked()), this, SLOT(GameOver()));
}

void GameMenu::ShowLoseMessage() {
  int32_t w = qApp->screens()[0]->size().width();
  int32_t h = qApp->screens()[0]->size().height();

  result_msg_ = new ButtonItem(w, h);
  result_msg_->setZValue(ZValues::kResultMessage);
  Controller::scene->addItem(result_msg_);
  Controller::view->DisableMotion();

  connect(result_msg_, SIGNAL(clicked()), this, SLOT(GameOver()));
}

void GameMenu::StartGame() {
  Controller::view->SetNewGameSettings();
  Controller::scene->NewGame();
}

void GameMenu::Hide() {
  btn_next_->hide();
  status_bar_->hide();
}

void GameMenu::Show() {
  btn_next_->show();
  status_bar_->show();
  ReDraw();
}

void GameMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::SwitchMenu(Controller::MenuType::kPause);
  }
}

void GameMenu::keyNextReleased() {
  if (Controller::view->IsInMotion() ||
      Controller::GetCurrentState() != Controller::MenuType::kGame) {
    return;
  }
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    Controller::scene->Next();
  }
}

void GameMenu::GameOver() {
  Controller::view->EnableMotion();
  SwitchTo(Controller::MenuType::kMain);
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
  if (Controller::GetActivePlanet()->GetOwner() ==
      Controller::scene->GetPlayer()) {
    interaction_button_->SetPixmap(
        Loader::GetButtonImage(ButtonsEnum::kBeautifulMoveButton));
  } else {
    interaction_button_->SetPixmap(
        Loader::GetButtonImage(ButtonsEnum::kBeautifulAttackButton));
  }

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
  // Область внутри, которая скроллится, выставляется в зависимости от
  // размеров и количества виджетов
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
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc =
      key_handler->Get(Controller::MenuType::kAttack, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
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

void AttackMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::MenuType::kPlanet);
  }
}

MoveMenu::MoveMenu() : UnitsInteractionMenu() {
  // TODO
  // interaction_button_->SetPixmap();
  KeyHandler* key_handler = Controller::GetKeyHandler();
  Qt::Key key_esc =
      key_handler->Get(Controller::MenuType::kMove, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
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

void MoveMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::MenuType::kPlanet);
  }
}

PlanetInfoMenu::PlanetInfoMenu() {
  if (Controller::GetActivePlanet()->GetOwner() ==
      Controller::scene->GetPlayer()) {
    upgrade_button_ = new ButtonItem(kButtonWidth, kButtonHeight, true);
    connect(upgrade_button_, SIGNAL(clicked()), this, SLOT(Upgrade()));
  }

  exit_button_ = new ButtonItem(kButtonWidth, kButtonHeight, true);

  KeyHandler* key_handler = Controller::GetKeyHandler();
  Controller::MenuType type = Controller::MenuType::kMove;
  Qt::Key key_esc = key_handler->Get(type, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);

  connect(exit_button_, SIGNAL(clicked()), this, SLOT(Exit()));
  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapePressed()));

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
  }

  Controller::SetMenuType(Controller::MenuType::kPlanet);
}

void PlanetInfoMenu::Destroy() {
  if (upgrade_button_ != nullptr) {
    Controller::scene->removeItem(upgrade_button_);
    delete upgrade_button_;
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

void PlanetInfoMenu::keyEscapePressed() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::MenuType::kPlanet);
  }
}

SettingsMenu::SettingsMenu() {
  Draw();
  SetZValue();
  DrawData();

  connect(btn_back_, SIGNAL(clicked()), this, SLOT(btnBackClicked()));

  Controller::MenuType type = Controller::MenuType::kSettings;
  Qt::Key key_esc = Controller::GetKeyHandler()->Get(type, Qt::Key_Escape).key;
  shortcuts_[key_esc] =
      std::make_shared<QShortcut>(key_esc, Controller::window);

  connect(shortcuts_[key_esc].get(), SIGNAL(activated()), this,
          SLOT(keyEscapeReleased()));
}

SettingsMenu::~SettingsMenu() {
  delete (settings_);
  delete (background_rect_);
  delete (btn_back_);
  delete (keypad_);
}

void SettingsMenu::SwitchTo(Controller::MenuType menu) {
  if (!Controller::Graph()->HasConnection(Controller::GetMenuType(), menu)) {
    return;
  }
  Controller::SetMenuType(menu);

  if (menu == Controller::MenuType::kPause) {
    Controller::GetPauseMenu()->Show();
    Controller::SetSettingsMenu(nullptr);
  } else if (menu == Controller::MenuType::kMain) {
    Controller::GetMainMenu()->Show();
    Controller::SetSettingsMenu(nullptr);
  }
}

void SettingsMenu::Draw() {
  int h = qApp->screens()[0]->size().height();
  int w = qApp->screens()[0]->size().width();

  cur_x = w / 20;
  cur_y = h / 9;

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

  settings_ = new QGraphicsTextItem("Settings");
  settings_->setDefaultTextColor(Qt::white);
  settings_->setFont(QFont("Arial", 48));
  settings_->setPos(Controller::view->mapToScene(
      (w - settings_->boundingRect().width()) / 2, h / 30));
  settings_->setScale(1 / coef);

  keypad_ = new QGraphicsTextItem("Keypad");
  keypad_->setDefaultTextColor(Qt::white);
  keypad_->setFont(QFont("Arial", 36));
  keypad_->setPos(Controller::view->mapToScene(cur_x, cur_y));
  keypad_->setScale(1 / coef);

  btn_back_ = new ButtonItem(kGeneralButtonWidth, kGeneralButtonHeight);
  btn_back_->setPos(
      Controller::view->mapToScene(QPoint(w, h) - QPoint(w / 8, h / 15)));

  Controller::scene->addItem(keypad_);
  Controller::scene->addItem(btn_back_);
  Controller::scene->addItem(settings_);
  Controller::scene->addItem(background_rect_);
}

void SettingsMenu::SetZValue() {
  background_rect_->setZValue(ZValues::kSettingsMenuDown);
  settings_->setZValue(ZValues::kSettingsMenuUp);
  btn_back_->setZValue(ZValues::kSettingsMenuUp);
  keypad_->setZValue(ZValues::kSettingsMenuUp);
}

void SettingsMenu::SetActiveKeyField(KeyField* field) { active_field_ = field; }

KeyField* SettingsMenu::GetActiveKeyField() { return active_field_; }

Controller::MenuType SettingsMenu::GetPrevMenu() { return prev_menu_; }

void SettingsMenu::SetPrevMenu(Controller::MenuType menu) { prev_menu_ = menu; }

void SettingsMenu::keyEscapeReleased() {
  QShortcut* sc = dynamic_cast<QShortcut*>(QObject::sender());
  if (sc && sc->objectName() == "From KeyReleaseEvent") {
    sc->setObjectName("");
    SwitchTo(Controller::GetSettingsMenu()->GetPrevMenu());
  }
}

void SettingsMenu::btnBackClicked() { Controller::SwitchMenu(prev_menu_); }

bool SettingsMenu::AddSection(const QString& name, int x, int y) {
  if (name != kStrNoSection) {
    sections_.push_back(std::make_shared<Section>(name, x, y));
    return true;
  }
  return false;
}

void SettingsMenu::DrawData() {
  int offset_section_x = keypad_->boundingRect().width() / 4;
  for (const std::pair<Controller::MenuType,
                       std::map<Qt::Key, KeyHandler::Key>>& p1 :
       Controller::GetKeyHandler()->GetData()) {
    cur_y += keypad_->boundingRect().height() * 1.25;
    bool is_added =
        AddSection(GetSectionName(p1.first), cur_x + offset_section_x, cur_y);
    cur_y -= (keypad_->boundingRect().height() * 1.25) * (!is_added);
    if (is_added) {
      for (const std::pair<Qt::Key, KeyHandler::Key>& p2 : p1.second) {
        sections_.back()->AddData(p2.second, p1.first);
      }
      cur_y = sections_.back()->GetY();
    }
  }
}

QString SettingsMenu::GetSectionName(Controller::MenuType type) {
  switch (type) {
    case Controller::MenuType::kGame:
      return "Game";
    case Controller::MenuType::kPlanet:
      return "Planet";
    default:
      return kStrNoSection;
  }
}

QShortcut* Menu::GetShortcut(int key) {
  return (shortcuts_[key] == nullptr ? nullptr : shortcuts_[key].get());
}

Section::Section() {}

Section::Section(const QString& name, int x, int y) : cur_x_(x), cur_y_(y) {
  name_ = new QGraphicsTextItem(name);
  name_->setDefaultTextColor(Qt::white);
  name_->setFont(QFont("Arial", 28));
  name_->setPos(Controller::view->mapToScene(cur_x_, cur_y_));
  name_->setScale(1 / Controller::view->matrix().m11());
  name_->setZValue(ZValues::kSettingsMenuUp);

  Controller::scene->addItem(name_);
}

Section::~Section() {
  delete (name_);
  for (auto x : actions_) {
    delete (x);
  }
  for (auto x : actions_keys_) {
    delete (x);
  }
}

void Section::AddData(const KeyHandler::Key& data, Controller::MenuType type) {
  if (!data.can_modify) {
    return;
  }
  int w = qApp->screens()[0]->size().width();

  cur_y_ += name_->boundingRect().height() * 1.2;
  QGraphicsTextItem* text = new QGraphicsTextItem(data.description);
  text->setDefaultTextColor(Qt::white);
  text->setFont(QFont("Arial", 24));
  text->setPos(Controller::view->mapToScene(
      cur_x_ + name_->boundingRect().width() / 4, cur_y_));
  text->setScale(1 / Controller::view->matrix().m11());
  text->setZValue(ZValues::kSettingsMenuUp);
  actions_.push_back(text);

  KeyField* ledit = new KeyField(data.key, type);
  ledit->setFont(QFont("Arial", 23));
  ledit->setReadOnly(true);
  ledit->setStyleSheet(
      "background-color: transparent;"
      "border: 2px solid gray;"
      "border-color: white;"
      "padding-left: 10;"
      "color: white;");
  ledit->setCursor(Controller::view->cursor());
  actions_keys_.push_back(ledit);

  QGraphicsProxyWidget* widget = Controller::scene->addWidget(ledit);
  widget->setPos(Controller::view->mapToScene(cur_x_ + w / 3, cur_y_));
  widget->setScale(1 / Controller::view->matrix().m11());
  widget->setZValue(ZValues::kSettingsMenuUp);
  widget->setFlag(QGraphicsItem::ItemIsSelectable);

  connect(ledit, SIGNAL(clicked()), this, SLOT(btnChangeKeyClicked()));
  Controller::scene->addItem(text);
}

int Section::GetY() { return cur_y_; }

void Section::btnChangeKeyClicked() {
  KeyField* ledit = dynamic_cast<KeyField*>(QObject::sender());
  if (ledit) {
    if (!Controller::view->IsKeyListenerEnabled()) {
      ledit->setText("");
      Controller::view->EnableKeyReleaseListener();
      Controller::GetSettingsMenu()->SetActiveKeyField(ledit);
    }
  }
}
