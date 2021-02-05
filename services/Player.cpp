//
// Created by Parti on 2021/2/5.
//

#include <drogon/drogon.h>
#include <services/Player.h>

using namespace tech::services;
using namespace drogon_model;
using namespace drogon;
using namespace std;

Player::Player(
        const string &email,
        string roomID,
        string roomPassword
) : _roomID(move(roomID)), _password(move(roomPassword)) {
    Mapper<Techmino::User> userMapper(app().getDbClient());
    _user = userMapper.findOne(Criteria(Techmino::User::Cols::_email, CompareOperator::EQ, email));
}

void Player::subscribe(SubscriberID subscriberID) {
    _subID = subscriberID;
}

Techmino::User Player::getUser() {
    return _user;
}

string Player::getRoomID() const {
    return _roomID;
}

[[maybe_unused]] string Player::getPassword() const {
    return _password;
}

SubscriberID Player::getSubscriberID() const {
    return _subID;
}