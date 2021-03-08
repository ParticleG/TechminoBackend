//
// Created by Parti on 2021/2/5.
//

#include <drogon/drogon.h>
#include ".unfinished/structures/Player.h"

#include <utility>

using namespace tech::structures;
using namespace drogon_model;
using namespace drogon;
using namespace std;

Player::Player(
        const string &email,
        string config,
        string roomID,
        string roomPassword
) : _config(move(config)), _roomID(move(roomID)), _password(move(roomPassword)) {
    Mapper<Techmino::Info> userMapper(app().getDbClient());
    _user = userMapper.findOne(Criteria(Techmino::Info::Cols::_email, CompareOperator::EQ, email));
}

void Player::subscribe(SubscriberID subscriberID) {
    _subID = subscriberID;
}

Techmino::Info Player::getUser() {
    return _user;
}

std::string Player::getInfo() {
    return _user.getValueOfUsername() + "," +
           to_string(_user.getValueOfId()) + "," +
           to_string(_subID) + "," +
           _config + "," +
           to_string(_isReady) + ";";
}

string Player::getConfig() const {
    return _config;
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

void Player::setReadyState(const bool &isReady) {
    _isReady = isReady;
}

bool Player::getReadyState() const {
    return _isReady;
}

void Player::setAliveState(const bool &isAlive) {
    _isAlive = isAlive;
}

bool Player::getAliveState() const {
    return _isAlive;
}

void Player::setConfig(string config) {
    _config = move(config);
}

void Player::setGroup(const unsigned int &group) {
    _group = group;
}

unsigned int Player::getGroup() const {
    return _group;
}
