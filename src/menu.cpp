#include "menu.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QSizePolicy>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QScreen>

#include "scene/gamescene.h"
#include "scene/gameview.h"
#include "graphics/imageitem.h"
#include "data/loader.h"
#include "mainwindow.h"
#include "objects/planet.h"
#include "core/statemachine.h"
#include "data/objectsstorage.h"
#include "util/utility.h"

MainMenu::MainMenu() {
  connect(this, SIGNAL(btnExitClick()), StateMachine::window, SLOT(Exit()));
  connect(this, SIGNAL(btnNewGameClick()), StateMachine::window,
          SLOT(StartGame()));

  this->Draw();
}

MainMenu::~MainMenu() {
  StateMachine::scene->removeItem(txt_total_space_);
  StateMachine::scene->removeItem(btn_exit_);
  StateMachine::scene->removeItem(btn_new_game_);
}

void MainMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  GameView* view = StateMachine::view;

  btn_exit_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kExitButton),
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));
  btn_new_game_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kNewGameButton),
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));
  txt_total_space_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kBackground),
                    static_cast<int>(width / view->matrix().m11()),
                    static_cast<int>(height / view->matrix().m11()));

  StateMachine::scene->addItem(txt_total_space_);
  StateMachine::scene->addItem(btn_exit_);
  StateMachine::scene->addItem(btn_new_game_);

  QPointF cp = StateMachine::view->sceneRect().center() / 2;

  btn_new_game_->setPos(cp - QPoint(0, height / 49) / view->matrix().m11());
  btn_exit_->setPos(cp + QPoint(0, height / 28) / view->matrix().m11());
  txt_total_space_->setPos(cp);
}

PauseMenu::PauseMenu() {
  connect(this, SIGNAL(btnBackClick()), StateMachine::window,
          SLOT(RemovePauseMenu()));
  connect(this, SIGNAL(btnExitClick()), StateMachine::window,
          SLOT(DrawMainMenu()));
  this->Draw();
}

PauseMenu::~PauseMenu() {
  StateMachine::scene->removeItem(btn_back_);
  StateMachine::scene->removeItem(btn_exit_);
  StateMachine::scene->removeItem(background_rect_);
}

void PauseMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  GameView* view = StateMachine::view;
  QPointF center =
      view->mapToScene(QPoint(view->rect().width(), view->rect().height()) / 2);

  QRectF rect = view->sceneRect();

  rect.setX(center.x() - view->rect().width() / view->matrix().m11());
  rect.setY(center.y() - view->rect().height() / view->matrix().m11());
  rect.setSize((rect.size() * 4) / view->matrix().m11());

  background_rect_ = new QGraphicsRectItem();
  background_rect_->setRect(rect);
  background_rect_->setOpacity(0.7);
  background_rect_->setBrush(QColor(Qt::black));

  btn_back_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kBackToGameButton),
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));
  btn_exit_ =
      new ImageItem(Loader::GetButtonImage(ButtonsEnum::kToMenuButton),
                    static_cast<int>(width / (5 * view->matrix().m11())),
                    static_cast<int>(height / (12 * view->matrix().m11())));

  StateMachine::scene->addItem(background_rect_);
  StateMachine::scene->addItem(btn_back_);
  StateMachine::scene->addItem(btn_exit_);

  btn_back_->setPos(view->sceneRect().center() / 2 -
                    QPoint(0, static_cast<int>(height / 14)) /
                        view->matrix().m11());
  btn_exit_->setPos(btn_back_->pos() +
                    QPoint(0, static_cast<int>(height / 18)) /
                        view->matrix().m11());

}

PlanetMenu::PlanetMenu() {
  connect(this, SIGNAL(btn1Click()), StateMachine::window,
          SLOT(DrawUnitMenu()));
  connect(this, SIGNAL(btn2Click()), StateMachine::window,
          SLOT(RemovePlanetMenu()));
  connect(this, SIGNAL(btn3Click()), StateMachine::window,
          SLOT(RemovePlanetMenu()));

  this->Draw();
}

PlanetMenu::~PlanetMenu() {
  StateMachine::scene->removeItem(btn1_);
  StateMachine::scene->removeItem(btn2_);
  StateMachine::scene->removeItem(btn3_);
}

void PlanetMenu::Draw() {
  int32_t width = qApp->screens()[0]->size().width();
  int32_t height = qApp->screens()[0]->size().height();

  btn1_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                        width / 12, height / 15);
  btn2_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                        width / 12, height / 15);
  btn3_ = new ImageItem(Loader::GetButtonImage(ButtonsEnum::kSimpleButton),
                        width / 12, height / 15);

  StateMachine::scene->addItem(btn1_);
  StateMachine::scene->addItem(btn2_);
  StateMachine::scene->addItem(btn3_);

  // TODO определить зависимость множителя от радиуса планеты
  Planet* p = StateMachine::GetActivePlanet();
  radius_ = -(p->GetRadius() * 1.3 / 2);
  QPointF vec1(0, radius_);
  QPointF vec2(vec1 * QTransform().rotate(60));
  QPointF vec3(vec1 * QTransform().rotate(-60));

  btn1_->setPos(p->GetCoordinates() + vec1);
  btn2_->setPos(p->GetCoordinates() + vec2);
  btn3_->setPos(p->GetCoordinates() + vec3);

  btn1_->setY(btn1_->y() - radius_ / 48);
  btn2_->setX(btn2_->x() + radius_ / 48);
  btn3_->setX(btn3_->x() - radius_ / 48);
}

void PlanetMenu::Hide() {
  btn1_->hide();
  btn2_->hide();
  btn3_->hide();
}

void PlanetMenu::Show() {
  btn1_->show();
  btn2_->show();
  btn3_->show();
}

ShopItem::ShopItem() {}
ShopItem::ShopItem(QPointF position, QSizeF size, QString name)
    : size_(size), item_name_(name) {
  setPos(position);

  name_label_rect_ =
      QRectF(2 * pos().x(), 2 * pos().y(), size_.width(), size_.height() * 0.1);
  picture_rect_ = QRectF(2 * pos().x(), 2 * pos().y() + 0.1 * size_.height(),
                         size_.width(), size_.height() * 0.7);
  price_label_rect_ =
      QRectF(2 * pos().x(), 2 * pos().y() + 0.8 * size_.height(), size_.width(),
             size_.height() * 0.1);
  btn_rect_ = QRectF(2 * pos().x(), 2 * pos().y() + 0.9 * size_.height(),
                     size_.width(), size_.height() * 0.1);

  buy_btn_ = new QPushButton();
  buy_btn_->setText("Buy");
  buy_btn_->setGeometry(static_cast<int>(btn_rect_.x()),
                        static_cast<int>(btn_rect_.y()),
                        static_cast<int>(btn_rect_.width()),
                        static_cast<int>(btn_rect_.height()));

  price_label_ =
      new QLabel("Batteries: " + QString::number(item_cost_.GetBatteries()) +
                 " | Tools: " + QString::number(item_cost_.GetTools()));
  price_label_->setGeometry(static_cast<int>(price_label_rect_.x()),
                            static_cast<int>(price_label_rect_.y()),
                            static_cast<int>(price_label_rect_.width()),
                            static_cast<int>(price_label_rect_.height()));

  name_label_ = new QLabel(name);
  name_label_->setGeometry(static_cast<int>(name_label_rect_.x()),
                           static_cast<int>(name_label_rect_.y()),
                           static_cast<int>(name_label_rect_.width()),
                           static_cast<int>(name_label_rect_.height()));
}

ShopType ShopItem::GetType() const {
    return type_;
}

QRectF ShopItem::boundingRect() const { return QRectF(pos(), size_); }
void ShopItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rect = boundingRect();
    painter->setBrush(Qt::gray);
    painter->drawRect(rect);

    if (proxy_btn_ == nullptr) {
        proxy_btn_ = StateMachine::scene->addWidget(buy_btn_);
        proxy_name_label_ = StateMachine::scene->addWidget(name_label_);
        proxy_price_label_ = StateMachine::scene->addWidget(price_label_);
    }
}

void ShopItem::Remove() {
    delete proxy_btn_;
    delete proxy_name_label_;
    delete proxy_price_label_;
}
int ShopItem::type() const {
    return Type;
}

void ShopItem::BuySomething() {
  if (GetType() == ShopType::kUnit) {
    StateMachine::GetActivePlanet()->AddUnit(
        ObjectsStorage::GetUnitType(item_name_));

  } else {
      StateMachine::GetActivePlanet()->Build(ObjectsStorage::GetBuildingType(item_name_));
  }
}

UnitMenu::UnitMenu() {
    width_ = StateMachine::view->sceneRect().width() * 0.9;
    height_ = StateMachine::view->sceneRect().height() * 0.9;

    position_ = StateMachine::GetActivePlanet()->GetCoordinates() - QPointF(width_ / 4, height_ / 4);

    QRectF back_rect(2 * position_, QSizeF(width_, height_));
    backgroung_ = new QGraphicsRectItem();
    backgroung_->setRect(back_rect);
    backgroung_->setPen(QColor(Qt::black));
    backgroung_->setBrush(QColor(Qt::blue));

    // Хз что делать с этими рандомными константами
    qreal hor_shift = width_ / 25;
    qreal ver_shift = height_ / 20;

    int hor_count_items = 4;
    int ver_count_items = 2;
    QSizeF size((width_ - (hor_count_items + 1) * hor_shift) / hor_count_items,
                (height_ - (ver_count_items + 1) * ver_shift) /
                    ver_count_items);
    QPointF hor_step((size.width() + hor_shift) / 2, 0);
    QPointF ver_step(0, (size.height() + ver_shift) / 2);

    QPointF start = position_ + QPointF(hor_shift, ver_shift) / 2;

    for (int8_t i = 0; i < ver_count_items; ++i) {
        for (int8_t j = 0; j < hor_count_items; ++j) {
            ShopItem* new_item = new ShopItem(start + i * ver_step + j * hor_step, size, "Yo!");
            items_.push_back(new_item);
        }
    }

    Draw();
}

UnitMenu::~UnitMenu() {
  StateMachine::scene->removeItem(backgroung_);
  for (const auto& item : items_) {
    StateMachine::scene->removeItem(item);
    item->Remove();
  }
  delete backgroung_;
}

void UnitMenu::Draw() {
  StateMachine::scene->addItem(backgroung_);
  for (const auto& item : items_) {
    StateMachine::scene->addItem(item);
  };
}
