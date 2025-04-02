#include "classes.h"



void film::info()
{
    std::cout << "#=====================================================================#" << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Genre: " << genres << std::endl;
    std::cout << "User rating: " << urating << std::endl;
    std::cout << "#=====================================================================#" << std::endl;
    std::cout << description << std::endl;;
};

int film::show_reviews()
{
    sqlite3* db = db_open();
    int fid = dbfilm_get_id(db, name);
    sqlite3_stmt* stmt = dbget_review_id(db, fid);

    while (sqlite3_column_int(stmt, 0) != NULL)
    {
        std::stringstream ss;
        std::string rt, unn;
        ss << sqlite3_column_text(stmt, 1);
        rt = ss.str();
        ss.str(std::string());
        ss << sqlite3_column_text(stmt, 3);
        unn = ss.str();
        ss.str(std::string());

        review p = review(rt, unn, sqlite3_column_int(stmt, 2));
        reviews.push_back(p);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (reviews.size() > 0)
    {
        for (int i = 0; i < reviews.size(); i++)
        {
            std::cout << i + 1 << ") " << reviews[i];

        }
        return 1;
    }
    else
    {
        std::cout << "No reviews" << std::endl;
        return 0;
    }
};

void account::add_review(review creview, std::shared_ptr<film> cfilm)
{
    int id, i = 0;
    sqlite3* db = db_open();
    id = dbfilm_get_id(db, cfilm->get_name().c_str());
    dbadd_review(db, id, creview.get_text().c_str(), creview.get_grade(), creview.get_un());

    sqlite3_stmt* stmt = dbget_review_id(db, id);
    sqlite3_step(stmt);
    while (sqlite3_column_int(stmt, 0) != NULL)
    {
        i++;
        sqlite3_step(stmt);
    }

    cfilm->set_urating((cfilm->get_urating() + creview.get_grade()) / (i + 1));
    dbchange_film_rating(db, id, cfilm->get_urating());
    sqlite3_close(db);
};

void admin::add_review(review creview, std::shared_ptr<film> cfilm)
{
    int id, i = 0;
    sqlite3* db = db_open();
    id = dbfilm_get_id(db, cfilm->get_name().c_str());
    dbadd_review(db, id, creview.get_text().c_str(), creview.get_grade(), creview.get_un()+" (admin) ");

    sqlite3_stmt* stmt = dbget_review_id(db, id);
    sqlite3_step(stmt);
    while (sqlite3_column_int(stmt, 0) != NULL)
    {
        i++;
        sqlite3_step(stmt);
    }

    cfilm->set_urating((cfilm->get_urating() + creview.get_grade()) / (i + 1));
    dbchange_film_rating(db, id, cfilm->get_urating());
    sqlite3_close(db);
};

admin operator+ (admin const& a, film const& f)
{
    std::string  cg = f.get_genres();

    sqlite3* db = db_open();
    dbadd_film(db, f.get_name(), f.get_description(), f.get_urating(), cg);
    sqlite3_close(db);

    return a;

};

void account::list_all()
{
    int select, i = 0;
    std::stringstream ss;
    std::vector <std::string> fvec;

    sqlite3* db = db_open();
    sqlite3_stmt* stmt;
    try
    {
        stmt = dbget_allfilms(db);
    }
    catch (...)
    {
        std::cout << "No films in the database" << std::endl;
        exit(1);
    }

    while (sqlite3_column_text(stmt, 0) != 0)
    {
        std::string st;
        std::cout << (i + 1) << ": " << sqlite3_column_text(stmt, 1) << std::endl;
        i++;
        ss << sqlite3_column_text(stmt, 1);

        st = ss.str();
        ss.str(std::string());
        fvec.push_back(st);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);

    while (true)
    {
        while (true)
        {
            std::cout << "Type film number to get on it's page: ";
            std::cin >> select;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (select > fvec.size())
        {
            std::cout << "Bad input, try again" << std::endl;
        }
        else
        {
            break;
        }
    }

    std::string name, descr, g;
    stmt = dbget_film_name(db, fvec[select - 1]);
    std::cout << sqlite3_column_int(stmt, 0);
    ss << sqlite3_column_text(stmt, 1);


    name = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 2);
    descr = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 4);
    g = ss.str();
    ss.str(std::string());


    std::shared_ptr<film> cfilm(new film(name, descr, sqlite3_column_int(stmt, 3), g));


    sqlite3_close(db);
    sqlite3_finalize(stmt);
    cfilm->info();
    std::string t;
    while (true)
    {
        while (true)
        {
            std::cout << "Type 'R' to load user reviews or 'B' to go to the main menu or 'G' to add new review" << std::endl;
            std::cin >> t;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (t == "R")
        {
            cfilm->show_reviews();
            break;

        }
        else if (t == "B")
        {

            break;
        }
        else if (t == "G")
        {
            std::string rtext;
            int rgrade;
            while (true)
            {
                std::cout << "Input review text: " << std::endl;
                std::cin.ignore(1);
                std::getline(std::cin, rtext);
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cout << "Bad input, try again" << std::endl;
                    std::cin.ignore(1);

                }
                else
                {
                    break;
                }
            }
            while (true)
            {

                while (true)
                {
                    std::cout << "Input review grade: ";
                    std::cin >> rgrade;
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;
                        std::cin.ignore(1);

                    }
                    else
                    {
                        break;
                    }
                }

                if ((rgrade > 10) || (rgrade < 0) || (typeid(rgrade) != typeid(int)))
                {
                    std::cout << "Wrong grade format" << std::endl;
                }
                else
                {
                    break;
                }
            }
            add_review(review(rtext, get_name(), rgrade), cfilm);
            break;
        }
        else
        {
            std::cout << "Wrong input, try again" << std::endl;
        }

    };

};

void account::find()
{
    std::string fname, descr, g;
    std::stringstream ss;

    while (true)
    {

        std::cout << "Type film's name: ";
        std::cin.ignore(1);
        std::getline(std::cin, fname);



        if (std::cin.fail())
        {
            std::cin.clear();
            std::cout << "Bad input, try again" << std::endl;
            std::cin.ignore(1);

        }
        else
        {
            break;
        }
    }

    sqlite3* db = db_open();
    sqlite3_stmt* stmt;
    try
    {
        stmt = dbget_film_name(db, fname);
    }
    catch (...)
    {
        std::cout << "Wrong film name";
        exit(1);
    }


    ss << sqlite3_column_text(stmt, 1);
    name = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 2);
    descr = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 4);
    g = ss.str();
    ss.str(std::string());



    std::shared_ptr<film> ffilm(new film(name, descr, sqlite3_column_int(stmt, 3), g));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    ffilm->info();

    std::string t;
    while (true)
    {
        while (true)
        {
            std::cout << "Type 'R' to load user reviews or 'B' to go to the main menu or 'G' to add new review" << std::endl;
            std::cin >> t;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (t == "R")
        {
            ffilm->show_reviews();
            break;
        }
        else if (t == "B")
        {

            break;
        }
        else if (t == "G")
        {
            std::string rtext;
            int rgrade;
            while (true)
            {
                std::cout << "Input review text: " << std::endl;
                std::cin.ignore(1);
                std::getline(std::cin, rtext);
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cout << "Bad input, try again" << std::endl;
                    std::cin.ignore(1);

                }
                else
                {
                    break;
                }
            }
            while (true)
            {

                while (true)
                {
                    std::cout << "Input review grade: ";
                    std::cin >> rgrade;
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;
                        std::cin.ignore(1);

                    }
                    else
                    {
                        break;
                    }
                }

                if ((rgrade > 10) || (rgrade < 0) || (typeid(rgrade) != typeid(int)))
                {
                    std::cout << "Wrong grade format" << std::endl;
                }
                else
                {
                    break;
                }
            }
            add_review(review(rtext, get_name(), rgrade), ffilm);

            break;
        }
        else
        {
            std::cout << "Wrong input, try again" << std::endl;
        }

    }

};

void account::find_genre()
{
    std::string genre_name;
    std::vector <int> idvec;
    int select, i = 0;
    std::stringstream ss;

    std::cout << "Type genre name: ";
    std::cin >> genre_name;

    sqlite3_stmt* stmt;
    sqlite3* db = db_open();
    try
    {
        stmt = dbget_film_genre(db, genre_name);
    }

    catch (...)
    {
        std::cout << "Wrong genre name";
        exit(1);
    }


    std::cout << sqlite3_column_int(stmt, 0) << std::endl;
    while (sqlite3_column_text(stmt, 1) != NULL)
    {
        std::cout << (i + 1) << ": " << sqlite3_column_text(stmt, 1) << std::endl;
        idvec.push_back(sqlite3_column_int(stmt, 0));
        i++;
        sqlite3_step(stmt);
    }

    while (true)
    {
        while (true)
        {
            std::cout << "Type film number to get on it's page: ";
            std::cin >> select;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (select > idvec.size())
        {
            std::cout << "Bad input, try again" << std::endl;
        }
        else
        {
            break;
        }
    }

    std::string name, descr, g;

    stmt = dbget_film_number(db, idvec[select - 1]);
    ss << sqlite3_column_text(stmt, 1);
    name = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 2);
    descr = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 4);
    g = ss.str();
    ss.str(std::string());


    std::shared_ptr<film> cfilm(new film(name, descr, sqlite3_column_int(stmt, 3), g));

    sqlite3_close(db);
    sqlite3_finalize(stmt);
    cfilm->info();

    std::string t;
    while (true)
    {
        while (true)
        {
            std::cout << "Type 'R' to load user reviews or 'B' to go to the main menu or 'G' to add new review" << std::endl;
            std::cin >> t;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (t == "R")
        {
            cfilm->show_reviews();
            break;
        }
        else if (t == "B")
        {
            break;
        }
        else if (t == "G")
        {
            std::string rtext;
            int rgrade;
            while (true)
            {
                std::cout << "Input review text: " << std::endl;
                std::cin.ignore(1);
                std::getline(std::cin, rtext);
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cout << "Bad input, try again" << std::endl;
                    std::cin.ignore(1);

                }
                else
                {
                    break;
                }
            }
            while (true)
            {

                while (true)
                {
                    std::cout << "Input review grade: ";
                    std::cin >> rgrade;
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;
                        std::cin.ignore(1);

                    }
                    else
                    {
                        break;
                    }
                }

                if ((rgrade > 10) || (rgrade < 0) || (typeid(rgrade) != typeid(int)))
                {
                    std::cout << "Wrong grade format" << std::endl;
                }
                else
                {
                    break;
                }
            }
            add_review(review(rtext, get_name(), rgrade), cfilm);
            break;
        }
        else
        {
            std::cout << "Wrong input, try again" << std::endl;
        }

    }

};

void user::type_all()
{
    int select, i = 0;
    std::stringstream ss;
    std::vector <std::string> fvec;

    sqlite3* db = db_open();
    sqlite3_stmt* stmt;
    try
    {
        stmt = dbget_allfilms(db);
    }
    catch (...)
    {
        std::cout << "No films in the database" << std::endl;
        exit(1);
    }

    while (sqlite3_column_text(stmt, 0) != 0)
    {
        std::string st;
        std::cout << (i + 1) << ": " << sqlite3_column_text(stmt, 1) << std::endl;
        i++;
        ss << sqlite3_column_text(stmt, 1);
        st = ss.str();
        ss.str(std::string());
        fvec.push_back(st);
        sqlite3_step(stmt);
    }

    while (true)
    {
        while (true)
        {
            std::cout << "Type film number to get on it's page: ";
            std::cin >> select;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }
        if (select > fvec.size())
        {
            std::cout << "Bad input, try again" << std::endl;
        }
        else
        {
            break;
        }
    }

    std::string name, descr, g;

    stmt = dbget_film_name(db, fvec[select - 1]);
    ss << sqlite3_column_text(stmt, 1);
    name = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 2);
    descr = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 4);
    g = ss.str();
    ss.str(std::string());

    std::shared_ptr<film> cfilm(new film(name, descr, sqlite3_column_int(stmt, 3), g));


    sqlite3_close(db);
    sqlite3_finalize(stmt);
    cfilm->info();
    std::string t;
    while (true)
    {
        while (true)
        {
            std::cout << "Type 'R' to load user reviews or 'B' to go to the main menu or 'G' to add review" << std::endl;
            std::cin >> t;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (t == "R")
        {
            if (cfilm->show_reviews() != 0)
            {


                while (true)
                {
                    std::cout << "Type 'G' to grade a review or 'B' to go to the main menu" << std::endl;
                    std::cin >> t;
                    if (t == "G")
                    {
                        int rkn, gg;
                        while (true)
                        {
                            std::cout << "Type number of review you wish to grade: ";
                            std::cin >> rkn;
                            if (std::cin.fail())
                            {
                                std::cin.clear();
                                std::cout << "Bad input, try again" << std::endl;
                                std::cin.ignore(1);

                            }
                            else
                            {
                                break;
                            }
                        }

                        while (true)
                        {
                            while (true)
                            {
                                std::cout << "Enter the grade: ";
                                std::cin >> gg;
                                if (std::cin.fail())
                                {
                                    std::cin.clear();
                                    std::cout << "Bad input, try again" << std::endl;
                                    std::cin.ignore(1);

                                }
                                else
                                {
                                    break;
                                }

                            }
                            if ((gg <= 5) && (gg >= 0))
                            {
                                grade_review(rkn, cfilm, gg);
                                break;
                            }
                            else
                            {
                                std::cout << "Grade should be an integer number between 0 and 5" << std::endl;
                            }
                        }


                    }
                    else if (t == "B")
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Wrong input, try again" << std::endl;
                    }
                }
                break;
            }
            else
            {
                break;
            }
        }


        else if (t == "B")
        {

            break;
        }
        else if (t == "G")
        {
            std::string rtext;
            int rgrade;
            while (true)
            {
                std::cout << "Input review text: " << std::endl;
                std::cin.ignore(1);
                std::getline(std::cin, rtext);
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cout << "Bad input, try again" << std::endl;
                    std::cin.ignore(1);

                }
                else
                {
                    break;
                }
            }
            while (true)
            {

                while (true)
                {
                    std::cout << "Input review grade: ";
                    std::cin >> rgrade;
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;
                        std::cin.ignore(1);

                    }
                    else
                    {
                        break;
                    }
                }

                if ((rgrade > 10) || (rgrade < 0) || (typeid(rgrade) != typeid(int)))
                {
                    std::cout << "Wrong grade format" << std::endl;
                }
                else
                {
                    break;
                }
            }
            add_review(review(rtext, this->name, rgrade), cfilm);
            break;
        }
        else
        {
            std::cout << "Wrong input, try again" << std::endl;
        }

    }

};

void user::find()
{
    std::string fname, descr, g, name;
    std::stringstream ss;

    while (true)
    {

        std::cout << "Type film's name: ";
        std::cin.ignore(1);
        std::getline(std::cin, fname);



        if (std::cin.fail())
        {
            std::cin.clear();
            std::cout << "Bad input, try again" << std::endl;
            std::cin.ignore(1);

        }
        else
        {
            break;
        }
    }

    sqlite3* db = db_open();
    sqlite3_stmt* stmt;
    try
    {
        stmt = dbget_film_name(db, fname);
    }
    catch (...)
    {
        std::cout << "Wrong film name";
        exit(1);
    }


    ss << sqlite3_column_text(stmt, 1);
    name = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 2);
    descr = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 4);
    g = ss.str();
    ss.str(std::string());


    std::shared_ptr<film> ffilm(new film(name, descr, sqlite3_column_int(stmt, 3), g));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    ffilm->info();

    std::string t;
    while (true)
    {
        while (true)
        {
            std::cout << "Type 'R' to load user reviews or 'B' to go to the main menu or 'G' to add new review" << std::endl;
            std::cin >> t;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (t == "R")
        {
            if (ffilm->show_reviews() != 0)
            {


                while (true)
                {
                    std::cout << "Type 'G' to grade a review or 'B' to go to the main menu" << std::endl;
                    std::cin >> t;
                    if (t == "G")
                    {
                        int rkn, gg;
                        while (true)
                        {
                            std::cout << "Type number of review you wish to grade: ";
                            std::cin >> rkn;
                            if (std::cin.fail())
                            {
                                std::cin.clear();
                                std::cout << "Bad input, try again" << std::endl;
                                std::cin.ignore(1);

                            }
                            else
                            {
                                break;
                            }
                        }

                        while (true)
                        {
                            while (true)
                            {
                                std::cout << "Enter the grade: ";
                                std::cin >> gg;
                                if (std::cin.fail())
                                {
                                    std::cin.clear();
                                    std::cout << "Bad input, try again" << std::endl;
                                    std::cin.ignore(1);

                                }
                                else
                                {
                                    break;
                                }

                            }
                            if ((gg <= 5) && (gg >= 0))
                            {
                                grade_review(rkn, ffilm, gg);
                                break;
                            }
                            else
                            {
                                std::cout << "Grade should be an integer number between 0 and 5" << std::endl;
                            }
                        }

                    }
                    else if (t == "B")
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Wrong input, try again" << std::endl;
                    }
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (t == "B")
        {

            break;
        }
        else if (t == "G")
        {
            std::string rtext;
            int rgrade;
            while (true)
            {
                std::cout << "Input review text: " << std::endl;

                std::cin.ignore(1);
                std::getline(std::cin, rtext);
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cout << "Bad input, try again" << std::endl;
                    std::cin.ignore(1);

                }
                else
                {
                    break;
                }
            }
            while (true)
            {

                while (true)
                {
                    std::cout << "Input review grade: ";
                    std::cin >> rgrade;
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;
                        std::cin.ignore(1);

                    }
                    else
                    {
                        break;
                    }
                }

                if ((rgrade > 10) || (rgrade < 0) || (typeid(rgrade) != typeid(int)))
                {
                    std::cout << "Wrong grade format" << std::endl;
                }
                else
                {
                    break;
                }
            }

            add_review(review(rtext, this->name, rgrade), ffilm);

            break;
        }
        else
        {
            std::cout << "Wrong input, try again" << std::endl;
        }

    }

};

void user::find_genre()
{
    std::string genre_name;
    std::vector <int> idvec;
    int select, i = 0;
    std::stringstream ss;

    std::cout << "Type genre name: ";
    std::cin >> genre_name;

    sqlite3_stmt* stmt;
    sqlite3* db = db_open();
    try
    {
        stmt = dbget_film_genre(db, genre_name);
    }

    catch (...)
    {
        std::cout << "Wrong genre name";
        exit(1);
    }


    std::cout << sqlite3_column_int(stmt, 0) << std::endl;
    while (sqlite3_column_text(stmt, 1) != NULL)
    {
        std::cout << (i + 1) << ": " << sqlite3_column_text(stmt, 1) << std::endl;
        idvec.push_back(sqlite3_column_int(stmt, 0));
        i++;
        sqlite3_step(stmt);
    }

    while (true)
    {
        while (true)
        {
            std::cout << "Type film number to get on it's page: ";
            std::cin >> select;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (select > idvec.size())
        {
            std::cout << "Bad input, try again" << std::endl;
        }
        else
        {
            break;
        }
    }

    std::string name, descr, g;
    stmt = dbget_film_number(db, idvec[select - 1]);
    ss << sqlite3_column_text(stmt, 1);
    name = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 2);
    descr = ss.str();
    ss.str(std::string());
    ss << sqlite3_column_text(stmt, 4);
    g = ss.str();
    ss.str(std::string());

    std::shared_ptr<film> cfilm(new film(name, descr, sqlite3_column_int(stmt, 3), g));
    sqlite3_close(db);
    sqlite3_finalize(stmt);
    cfilm->info();

    std::string t;
    while (true)
    {
        while (true)
        {
            std::cout << "Type 'R' to load user reviews or 'B' to go to the main menu or 'G' to add new review" << std::endl;
            std::cin >> t;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cout << "Bad input, try again" << std::endl;
                std::cin.ignore(1);

            }
            else
            {
                break;
            }
        }

        if (t == "R")
        {

            if (cfilm->show_reviews() != 0)
            {


                while (true)
                {
                    std::cout << "Type 'G' to grade a review or 'B' to go to the main menu" << std::endl;
                    std::cin >> t;
                    if (t == "G")
                    {
                        int rkn, gg;
                        while (true)
                        {
                            std::cout << "Type number of review you wish to grade: ";
                            std::cin >> rkn;
                            if (std::cin.fail())
                            {
                                std::cin.clear();
                                std::cout << "Bad input, try again" << std::endl;
                                std::cin.ignore(1);

                            }
                            else
                            {
                                break;
                            }
                        }

                        while (true)
                        {
                            while (true)
                            {
                                std::cout << "Enter the grade: ";
                                std::cin >> gg;
                                if (std::cin.fail())
                                {
                                    std::cin.clear();
                                    std::cout << "Bad input, try again" << std::endl;
                                    std::cin.ignore(1);

                                }
                                else
                                {
                                    break;
                                }

                            }
                            if ((gg <= 5) && (gg >= 0))
                            {
                                grade_review(rkn, cfilm, gg);
                                break;
                            }
                            else
                            {
                                std::cout << "Grade should be an integer number between 0 and 5" << std::endl;
                            }
                        }

                    }
                    else if (t == "B")
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Wrong input, try again" << std::endl;
                    }
                }
                break;
            }
            else
            {
                break;
            }
        }
        else if (t == "B")
        {
            break;
        }
        else if (t == "G")
        {
            std::string rtext;
            int rgrade;
            while (true)
            {
                std::cout << "Input review text: " << std::endl;
                std::cin.ignore(1);
                std::getline(std::cin, rtext);
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cout << "Bad input, try again" << std::endl;
                    std::cin.ignore(1);

                }
                else
                {
                    break;
                }
            }
            while (true)
            {

                while (true)
                {
                    std::cout << "Input review grade: ";
                    std::cin >> rgrade;
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;
                        std::cin.ignore(1);

                    }
                    else
                    {
                        break;
                    }
                }

                if ((rgrade > 10) || (rgrade < 0) || (typeid(rgrade) != typeid(int)))
                {
                    std::cout << "Wrong grade format" << std::endl;
                }
                else
                {
                    break;
                }
            }
            add_review(review(rtext, this->name, rgrade), cfilm);
            break;
        }
        else
        {
            std::cout << "Wrong input, try again" << std::endl;
        }

    }

};

void user::grade_review(int num, std::shared_ptr<film> cfilm, int grade)
{
    sqlite3* db = db_open();
    sqlite3_stmt* stmt;
    try
    {
        stmt = dbget_user_name(db, cfilm->get_reviews()[num - 1].get_un());
    }
    catch (...)
    {
        std::cout << "Wrong review number";
        exit(1);
    }
    grade += sqlite3_column_int(stmt, 2);
    sqlite3_finalize(stmt);

    dbchange_user_rating(db, cfilm->get_reviews()[num - 1].get_un(), grade);

    sqlite3_close(db);

};

void admin::add_film(film cfilm)
{
    std::string  cg = cfilm.get_genres();

    sqlite3* db = db_open();
    dbadd_film(db, cfilm.get_name(), cfilm.get_description(), cfilm.get_urating(), cg);
    sqlite3_close(db);

};

void admin::delete_film(std::string name)
{
    sqlite3* db = db_open();
    db_delete_film(db, name);
    std::cout << "Film was sucsesfully deleted or was not present" << std::endl;
    sqlite3_close(db);
};

