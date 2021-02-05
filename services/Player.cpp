//
// Created by Parti on 2021/2/5.
//

#include <drogon/drogon.h>
#include <services/Player.h>

#include <utility>

using namespace tech::services;

Player::Player(
        const std::string &email,
        std::string roomID,
        std::string roomPassword
) : _roomID(move(roomID)), _password(move(roomPassword)) {
    Mapper<Techmino::User> userMapper(drogon::app().getDbClient());
    _user = userMapper.findOne(Criteria(Techmino::User::Cols::_email, CompareOperator::EQ, email));
}

void Player::subscribe(drogon::SubscriberID subscriberID) {
    _subID = subscriberID;
}

Techmino::User Player::getUser() {
    return _user;
}

std::string Player::getRoomID() const {
    return _roomID;
}

[[maybe_unused]] std::string Player::getPassword() const {
    return _password;
}

drogon::SubscriberID Player::getSubscriberID() const {
    return _subID;
}