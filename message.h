#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
class Message {
    public:
    Message(std::string text, std::string color);
    std::string text;
    std::string color;

};
#endif
