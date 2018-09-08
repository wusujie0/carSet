#include "card.h"

using namespace std;

Card::Card()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    createTime = tmp;
}

Json::Value Card::toJson()
{
    Json::Value card;
    card["UID"] = UID.data();
    card["title"] = title.data();
    card["contains"] = contains.data();
    card["createTime"] = createTime.data();
    card["pictureLength"] = pictureLength;
    return card;
}
