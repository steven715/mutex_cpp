#if !defined(OBJECT_H)
#define OBJECT_H

#include <string>

class Object
{
private:
    std::string id;
    double value;
    double number;

public:
    Object(const std::string& id, double value, double number)
    {
        this->id = id;
        this->value = value;
        this->number = number;
    }
    ~Object()
    {
    }

    std::string GetID() const { return id; }
    double GetValue() const { return value; }
    double GetNumber() const { return number; }
};


#endif // OBJECT_H
