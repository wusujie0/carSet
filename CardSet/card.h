#ifndef CARD_H
#define CARD_H

#include<iostream>
#include "./json/json.h"

class Card
{
public:
    Card();
    Json::Value toJson();
    std::string UID;
    std::string title;
    std::string contains;
    std::string createTime;
    std::string pic;
    int pictureLength;
};

#endif // CARD_H
