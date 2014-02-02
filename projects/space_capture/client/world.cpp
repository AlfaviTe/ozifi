#include <QDebug>

#include "world.h"

TWorld::TWorld()
    : Scale(1.0)
    , OffsetX(0)
    , OffsetY(0)
{
}

void TWorld::UpdateWorld(Space::TWorld world) {
    Space::TWorld::operator =(world);
    IdToPlayer.clear();
    for (size_t i = 0; i < players_size(); ++i) {
        IdToPlayer[players(i).id()] = mutable_players(i);
    }
    emit OnWorldUpdated();
}

void TWorld::UpdateSelection(QPointF from, QPointF to) {
    Selection = {from, to};
    emit OnWorldUpdated();
}

void TWorld::RemoveSelection() {
    Selection.reset();
}

Space::TPlayer *TWorld::SelfPlayer() {
    if (IdToPlayer.find(this->selfid()) == IdToPlayer.end()) {
        qDebug() << "SelfPlayer(): player with id" << this->selfid() << "missing";
        return nullptr;
    }
    return IdToPlayer[this->selfid()];
}