#ifndef CLASSES_H
#define CLASSES_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include "sqlite3.h"
#include "db_functions.h"
#include <memory>


    class review {
    private:
        std::string text;
        std::string user;
        int grade;

    public:
        review() {};
        review(std::string text, std::string user, int grade) : text(text), user(user), grade(grade) {};

        friend std::ostream& operator << (std::ostream& s, const review& p) {  s << "Name: " << p.user << "             Grade: " << p.grade << "\n" << p.text << std::endl; return s;}

        std::string get_text() { return(text); };
        void set_text(std::string text) { this->text = text; };

        std::string get_un() { return(user); };
        void set_un(std::string user) { this->user = user; };

        int get_grade() { return grade; };
        void set_grade(int grade) { this->grade = grade; };
    };

    class film
    {
    private:
        std::string name, description;
        int urating;
        std::string genres;
        std::vector <review> reviews;

    public:
        film(std::string name, std::string description, std::string genres) : name(name), description(description), genres(genres) { urating = 0; };
        film(std::string name, std::string description, int urating, std::string genres) : name(name), description(description), urating(urating), genres(genres) {};
        film() {};

        std::string get_name() { return(name); };
        std::string get_name() const { return(name); };
        void set_name(std::string name) { this->name = name; };

        std::string get_description() { return(description); };
        std::string get_description() const { return(description); };
        void set_description(std::string description) { this->description = description; };

        int get_urating() { return urating; };
        int get_urating() const { return urating; };
        void set_urating(int urating) { this->urating = urating; };

        std::string get_genres() { return genres; };
        std::string get_genres() const { return genres; };
        void set_genres(std::string genres) { this->genres = genres; };

        std::vector <review> get_reviews() { return reviews; };
        std::vector <review> get_reviews() const { return reviews; };
        void set_reviews(std::vector <review> reviews) { this->reviews = reviews; };

        void info();

        int show_reviews();
    };

    class account
    {
    private:
        std::string name, password;

    public:
        virtual ~account() {};

        virtual std::string get_name() { return name; };

        virtual void add_review(review creview, std::shared_ptr<film> cfilm);

        virtual void list_all();

        virtual void find();

        virtual void find_genre();
    };

    class user : virtual public account
    {
    private:
        int userscore;
        std::string name, password;
    public:
        user() {};
        user(std::string name, std::string password) : name(name), password(password) { userscore = 0; };

        std::string get_name() { return name; };

        virtual void type_all();

        virtual void find();

        virtual void find_genre();

        void grade_review(int num, std::shared_ptr<film> cfilm, int grade);
    };

    class admin : virtual public account
    {
    private:
        std::string name, password;

    public:
        admin() {};
        admin(std::string name, std::string password) : name(name), password(password) {};

        friend admin operator+ (admin const& a, film const& f);

        std::string get_name() { return name; };

        void add_film(film cfilm);

        void delete_film(std::string name);

        virtual void add_review(review creview, std::shared_ptr<film> cfilm);
    };


#endif