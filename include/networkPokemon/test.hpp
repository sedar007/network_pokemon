#pragma once
/**
 * @file Client.hpp
 * @brief Définition de la classe Client.
 */

namespace pokemon {

    class Test {
    public:

        Test();

        inline std::string getName(){return name;};
        inline void setName(const std::string &name){this->name = name;};

        int getAge();
        void setAge(int age);
        std::string getLocalIp();

    private:

        std::string name;
        int age;
    };
}
