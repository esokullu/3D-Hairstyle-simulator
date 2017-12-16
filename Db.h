#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QString>
#include <QList>
#include "models/Category.h"
#include "models/Hairstyle.h"

class Db
{
private:
    QSqlDatabase db;
    void applySqlFile(const QString filename);
public:
    Db(const QString databaseName);
    Category* getRootCategory();
    std::vector<Category*> getSubcategoriesByCategoryId(int categoryId);
    std::vector<Hairstyle*> getHairstylesByCategoryIds(QList<int> categoryIds);
};

#endif // DB_H
