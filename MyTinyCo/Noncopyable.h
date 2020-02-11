//
// Created by Jialu  Hu on 2020-01-23.
//

#ifndef MYSNETCO_NONCOPYABLE_H
#define MYSNETCO_NONCOPYABLE_H
namespace SiNet{
    class Noncopyable {
    public:
        Noncopyable() = default;
        ~Noncopyable() = default;

    private:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
    };
}
#endif //MYSNETCO_NONCOPYABLE_H
