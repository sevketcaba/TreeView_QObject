#ifndef SAMPLE_H
#define SAMPLE_H

#include <QObject>

class Animal : public QObject
{
    Q_OBJECT
public:
    explicit Animal(QObject *parent = nullptr) :
        QObject(parent)
    {

    }
    virtual ~Animal() override {}
};

class Dog : public Animal
{
    Q_OBJECT
public:
    explicit Dog(QObject *parent = nullptr) :
        Animal(parent)
    {

    }
    virtual ~Dog() override {}
};

class Cat : public Animal
{
    Q_OBJECT
public:
    explicit Cat(QObject *parent = nullptr) :
        Animal(parent)
    {

    }
    virtual ~Cat() override {}
};

class Sample : public QObject
{
    Q_OBJECT
public:
    explicit Sample(QObject *parent = nullptr);
    virtual ~Sample() override;

    static QObject *createRandom(QObject *parent);
    void populate(QObject *parent, int childCount, int depth, int maxDepth);
};

#endif // SAMPLE_H
