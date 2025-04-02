
#include "classes.h"



account* login()
{

    std::string un, pass;

    while (true)
    {
        std::cout << "Enter login: ";
        std::cin >> un;
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
        std::cout << "Enter password: ";
        std::cin >> pass;
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
    sqlite3_stmt* stmt = dbget_user(db, un, pass);
    sqlite3_close(db);

    if ((sqlite3_column_text(stmt, 0) != NULL) && (sqlite3_column_text(stmt, 1) != NULL))
    {
        if (sqlite3_column_int(stmt, 3) == 0)
        {

            sqlite3_finalize(stmt);
            return new user(un, pass);
        }

        else
        {
            sqlite3_finalize(stmt);
            return new admin(un, pass);
        }
    }

    else
    {
        std::cout << "Login or password is not valid";
        sqlite3_finalize(stmt);
        exit(1);
    }





}

account* create()
{
    std::string un, pass, pcode;
    int choise;
    sqlite3* db = db_open();
    sqlite3_stmt* stmt = nullptr;
    while (true)
    {
        std::cout << "1. Create usual account\n2. Create admin account" << std::endl;
        std::cin >> choise;
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
    switch (choise)
    {
    case 1:
        while (true)
        {
            std::cout << "Enter new login: ";
            std::cin >> un;
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
            std::cout << "Enter new password: ";
            std::cin >> pass;
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

        try
        {
            stmt = dbget_user_name(db, un);
        }
        catch (...)
        {
            sqlite3_finalize(stmt);
            dbadd_user(db, un, pass, 0, 0);
            sqlite3_close(db);
            std::cout << "Account sucsessfully created" << std::endl;
            return new  user(un, pass);;
            break;
        }

        std::cout << "Username already taken" << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);


    case 2:
        while (true)
        {
            std::cout << "Enter administrator passcode: ";
            std::cin >> pcode;
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

        if (pcode == "a")
        {
            std::cout << "Enter new login: ";
            std::cin >> un;
            std::cout << "Enter new password: ";
            std::cin >> pass;

            try
            {
                stmt = dbget_user_name(db, un);
            }
            catch (...)
            {
                dbadd_user(db, un, pass, 0, 1);
                sqlite3_close(db);
                std::cout << "Account sucsessfully created" << std::endl;
                return new  admin(un, pass);;
                break;
            }

            std::cout << "Username already taken" << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            exit(1);


        }
        else
        {
            std::cout << "Wrong administrator passcode value";
            sqlite3_close(db);
            exit(1);
        }
        break;
    default:
        std::cout << "ERROR\n";
        sqlite3_close(db);
        exit(1);
        break;
    }

}

std::shared_ptr<film> findfilm(std::string rfname)
{
    std::stringstream ss;
    std::string rfdescription, rfgenres;
    sqlite3* db = db_open();
    sqlite3_stmt* stmt = dbget_film_name(db, rfname);
    ss << sqlite3_column_text(stmt, 1);
    rfname = ss.str();
    ss.str(std::string());

    ss << sqlite3_column_text(stmt, 2);
    rfdescription = ss.str();
    ss.str(std::string());

    ss << sqlite3_column_text(stmt, 4);
    rfgenres = ss.str();
    ss.str(std::string());

    int t = sqlite3_column_int(stmt, 3);
    std::shared_ptr<film> p(new film(rfname, rfdescription, t, rfgenres));
    sqlite3_close(db);
    sqlite3_finalize(stmt);
    return(p);
}

int main()
{
    int choise, rgrade, rfgrade;
    std::string rtext, rfname, rfgenres, rfdescription;
    std::stringstream ss;
    account* current;
    std::shared_ptr<film> pf;




    std::cout << "1. Login\n2. Create user\n";
    std::cin >> choise;
    switch (choise)
    {
    case 1:
        current = login();
        std::cout << "Sucsesfully logged in";
        break;
    case 2:
        current = create();
        std::cout << "Sucsesfully logged in";
        break;
    default:
        std::cout << "ERROR\n";

        exit(1);
        break;
    }


    user* ucurrent = dynamic_cast<user*>(current);
    admin* acurrent = dynamic_cast<admin*>(current);
    if (ucurrent != nullptr)
    {
        delete acurrent;
        while (true)
        {
            int choise;
            while (true)
            {
                std::cout << "\n1. See list of all movies\n2. Find movie by name\n3. See list of all movies by genre\n4. Finish the programm\n5. Add review\n";
                std::cin >> choise;
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

            switch (choise)
            {
            case 1:
                ucurrent->type_all();
                break;
            case 2:
                ucurrent->find();
                break;
            case 3:
                ucurrent->find_genre();
                break;
            case 4:
                std::cout << "Exit";
                delete ucurrent;
                exit(0);
                break;
            case 5:
                while (true)
                {
                    std::cout << "Input film name: ";
                    std::cin >> rfname;
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
                pf = findfilm(rfname);

                while (true)
                {
                    std::cout << "Input review text: ";
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

                current->add_review(review(rtext, ucurrent->get_name(), rgrade), pf);

                break;
            default:
                std::cout << "ERROR\n";
                delete ucurrent;
                exit(1);
                break;
            }
        }
    }
    else
    {
        delete ucurrent;
        std::string name, descr, l, dname;
        std::string genres;
        while (true)
        {
            int choise;
            while (true)
            {
                std::cout << "\n1. See list of all movies\n2. Find movie by name\n3. See list of all movies by genre\n4. Finish the programm\n5. Add film\n6. Delete film\n7. Add review\n";
                std::cin >> choise;
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

            switch (choise)
            {
            case 1:
                acurrent->list_all();
                break;
            case 2:
                acurrent->find();
                break;
            case 3:
                acurrent->find_genre();
                break;
            case 4:
                std::cout << "Exit";
                exit(0);
                break;
            case 5:

                while (true)
                {
                    std::cout << "Type film's name: ";
                    std::cin.ignore(1);
                    std::getline(std::cin, name);
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
                    std::cout << "Type film's description: ";
                    std::getline(std::cin, descr);
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
                    std::cout << "Type film's genre: ";
                    std::getline(std::cin, genres);
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

                acurrent->add_film(film(name, descr, genres));
                std::cout << "Film added";
                break;
            case 6:
                while (true)
                {
                    std::cout << "Input name of the film you want to delete: ";

                    std::cin.ignore(1);
                    std::getline(std::cin, dname);
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;


                    }
                    else
                    {
                        break;
                    }
                }
                acurrent->delete_film(dname);
                break;
            case 7:
                while (true)
                {
                    std::cout << "Input film name: ";
                    std::cin.ignore(1);
                    std::getline(std::cin, rfname);
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

                try
                {
                    pf = findfilm(rfname);
                }
                catch (...)
                {
                    std::cout << "Film is not present";
                    delete acurrent;
                    exit(1);
                }

                while (true)
                {
                    std::cout << "Input review text: ";
                    std::cin.ignore(1);
                    std::getline(std::cin, rtext);
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cout << "Bad input, try again" << std::endl;
                        std::cin.ignore(rtext.size());

                    }
                    else
                    {
                        break;
                    }
                }

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

                current->add_review(review(rtext, acurrent->get_name(), rgrade), pf);

                break;
            default:
                std::cout << "ERROR\n";
                delete acurrent;
                exit(1);
                break;
            }
        }
    }

}

